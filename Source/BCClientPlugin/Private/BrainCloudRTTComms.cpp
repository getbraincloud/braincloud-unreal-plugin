// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "BrainCloudRTTComms.h"
#include "ConvertUtilities.h"
#include "BCClientPluginPrivatePCH.h"
#include "Serialization/JsonTypes.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "JsonUtil.h"
#include "BCBlueprintRTTCallProxyBase.h"
#include "BCRTTProxy.h"
#include "IServerCallback.h"
#include "ServerCall.h"
#include "ServiceName.h"
#include "ServiceOperation.h"
#include "BrainCloudWrapper.h"
#include "BrainCloudClient.h"
#include "BCFileUploader.h"
#include "ReasonCodes.h"
#include "HttpCodes.h"

#include "BCRTTCommsProxy.h"
#include "WinWebSocketBase.h"
#include <iostream>
#include "Runtime/Launch/Resources/Version.h"

#define MAX_PAYLOAD_RTT (64 * 1024) // [dsl] This used to be set to 10MB, failed on mac SNDBUF too big for the TCP socket.
#define INITIAL_HEARTBEAT_TIME 10
#define HEARTBEAT_IDLE_DELAY 2

BrainCloudRTTComms::BrainCloudRTTComms(BrainCloudClient *client) 
: m_client(client)
, m_appCallback(nullptr)
, m_appCallbackBP(nullptr)
, m_connectedSocket(nullptr)
, m_commsPtr(nullptr)
, m_cxId(TEXT(""))
, m_eventServer(TEXT(""))
, m_rttHeaders(nullptr)
, m_endpoint(nullptr)
, m_heartBeatSecs(INITIAL_HEARTBEAT_TIME)
, m_heartBeatIdleSecs(HEARTBEAT_IDLE_DELAY)
, m_timeSinceLastRequest(0)
, m_lastNowMS(FPlatformTime::Seconds())
, m_rttConnectionStatus(BCRTTConnectionStatus::DISCONNECTED)
, m_websocketStatus(BCWebsocketStatus::NONE)
{
}

BrainCloudRTTComms::~BrainCloudRTTComms()
{
	disconnect();
	deregisterAllRTTCallbacks();
}

void BrainCloudRTTComms::enableRTT(BCRTTConnectionType in_connectionType, IServerCallback *callback)
{
	m_disconnectedWithReason = false;
	
	if(isRTTEnabled() || m_rttConnectionStatus == BCRTTConnectionStatus::CONNECTING)
	{
		return; 
	}
	else
	{
		m_connectionType = in_connectionType;
		m_appCallback = callback;
		m_client->getRTTService()->requestClientConnection(this);
		m_rttConnectionStatus = BCRTTConnectionStatus::CONNECTING;
	}
}

void BrainCloudRTTComms::enableRTT(BCRTTConnectionType in_connectionType, UBCRTTProxy *callback)
{
	if(isRTTEnabled() || m_rttConnectionStatus == BCRTTConnectionStatus::CONNECTING)
	{
		return;
	}
	else
	{
		m_connectionType = in_connectionType;
		m_appCallbackBP = callback;
		m_client->getRTTService()->requestClientConnection(this);
		m_rttConnectionStatus = BCRTTConnectionStatus::CONNECTING;
	}
}

void BrainCloudRTTComms::disableRTT()
{
	if(m_rttConnectionStatus == BCRTTConnectionStatus::DISCONNECTED || m_rttConnectionStatus == BCRTTConnectionStatus::DISCONNECTING)
	{
		return;
	}
	else
	{
		processRegisteredListeners(ServiceName::RTTRegistration.getValue().ToLower(), "disconnect", UBrainCloudWrapper::buildErrorJson(403, ReasonCodes::RTT_CLIENT_ERROR, "DisableRTT Called"));
	}
}

bool BrainCloudRTTComms::isRTTEnabled()
{
	return m_rttConnectionStatus == BCRTTConnectionStatus::CONNECTED;
}

BCRTTConnectionStatus BrainCloudRTTComms::getConnectionStatus()
{
	return m_rttConnectionStatus;
}

void BrainCloudRTTComms::RunCallbacks()
{

	if (isRTTEnabled())
	{
		// check to see if we need to send an RTT heartbeat to keep the connection alive
		float nowMS = FPlatformTime::Seconds();

		// the heart beat
		m_timeSinceLastRequest += (nowMS - m_lastNowMS);
		m_lastNowMS = nowMS;
        if (m_heartBeatSent && m_timeSinceLastRequest >= m_heartBeatIdleSecs)
        {
            if(!m_heartBeatRecv){
                UE_LOG(LogBrainCloudComms, Log, TEXT("RTT: lost heartbeat %f idle"), m_heartBeatIdleSecs);
                processRegisteredListeners(ServiceName::RTTRegistration.getValue().ToLower(), "disconnect", UBrainCloudWrapper::buildErrorJson(403, ReasonCodes::RS_CLIENT_ERROR,"Could not connect at this time"));
            }
            m_heartBeatSent = false;
        }
        if (m_timeSinceLastRequest >= m_heartBeatSecs)
        {
            m_timeSinceLastRequest = 0;
            m_heartBeatSent = true;
            m_heartBeatRecv = false;
            send(buildHeartbeatRequest(), false);
		}
	}

	if(m_disconnectedWithReason)
	{
		disconnect();
	}
}

// add blueprints
void BrainCloudRTTComms::registerRTTCallback(ServiceName in_serviceName, UBCBlueprintRTTCallProxyBase *callback)
{
	callback->AddToRoot();
	m_registeredRTTBluePrintCallbacks.Emplace(in_serviceName.getValue().ToLower(), callback);
}

// regular c++ overtyped, does nothing memory wise
void BrainCloudRTTComms::registerRTTCallback(ServiceName in_serviceName, IRTTCallback *callback)
{
	m_registeredRTTCallbacks.Emplace(in_serviceName.getValue().ToLower(), callback);
}

void BrainCloudRTTComms::deregisterRTTCallback(ServiceName in_serviceName)
{
	FString serviceName = in_serviceName.getValue().ToLower();
	if (m_registeredRTTBluePrintCallbacks.Contains(serviceName))
	{
		m_registeredRTTBluePrintCallbacks[serviceName]->RemoveFromRoot();
		m_registeredRTTBluePrintCallbacks.Remove(serviceName);
	}
	else if (m_registeredRTTCallbacks.Contains(serviceName))
	{
		m_registeredRTTCallbacks.Remove(serviceName);
	}
}

void BrainCloudRTTComms::deregisterAllRTTCallbacks()
{
	UBCBlueprintRTTCallProxyBase *pObject;
	for (auto iterator = m_registeredRTTBluePrintCallbacks.CreateIterator(); iterator; ++iterator)
	{
		pObject = iterator.Value();
		if (pObject->IsValidLowLevel())
		{
			pObject->RemoveFromRoot();
			pObject->ConditionalBeginDestroy();
		}
	}

	m_registeredRTTCallbacks.Empty();
	m_registeredRTTBluePrintCallbacks.Empty();
}

void BrainCloudRTTComms::setRTTHeartBeatSeconds(int32 in_value)
{
	m_heartBeatSecs = in_value;
    if(m_heartBeatIdleSecs > m_heartBeatSecs) m_heartBeatIdleSecs = m_heartBeatSecs;
}

void BrainCloudRTTComms::connectWebSocket()
{
	if (!isRTTEnabled())
	{
		startReceivingWebSocket();
	}
}

void BrainCloudRTTComms::disconnect()
{
	if (!isRTTEnabled()) return;

	m_rttConnectionStatus = BCRTTConnectionStatus::DISCONNECTING;
	// clear everything
	if (m_connectedSocket != nullptr && m_commsPtr != nullptr)
	{
		m_commsPtr->RemoveFromRoot();
		m_connectedSocket->ResetCallbacks();
		m_connectedSocket->Close();
		m_connectedSocket->RemoveFromRoot();
	}

	if (m_commsPtr)
		m_commsPtr->ConditionalBeginDestroy();
	m_commsPtr = nullptr;

	if (m_connectedSocket)
		m_connectedSocket->ConditionalBeginDestroy();
	m_connectedSocket = nullptr;

	m_cxId = TEXT("");
	m_eventServer = TEXT("");
	m_disconnectedWithReason = false;
	m_heartBeatSent = false;
	m_heartBeatRecv = true;
	m_timeSinceLastRequest = 0;
	m_rttConnectionStatus = BCRTTConnectionStatus::DISCONNECTED;

	m_appCallback = nullptr;

	if (m_appCallbackBP != nullptr)
	{
		// allow it to be removed, if no longer referenced
        m_appCallbackBP->RemoveFromRoot();
        m_appCallbackBP->ConditionalBeginDestroy();
	}
}

FString BrainCloudRTTComms::buildConnectionRequest()
{
	TSharedRef<FJsonObject> sysJson = MakeShareable(new FJsonObject());
	sysJson->SetStringField("platform", m_client->getReleasePlatform());
	sysJson->SetStringField("protocol", "ws");

	TSharedRef<FJsonObject> jsonData = MakeShareable(new FJsonObject());
	jsonData->SetStringField("appId", m_client->getAppId());
	jsonData->SetStringField("sessionId", m_client->getSessionId());
	jsonData->SetStringField("profileId", m_client->getProfileId());
	jsonData->SetObjectField("system", sysJson);
	jsonData->SetObjectField("auth", m_rttHeaders);

	TSharedRef<FJsonObject> json = MakeShareable(new FJsonObject());
	json->SetStringField("service", ServiceName::RTT.getValue());
	json->SetStringField("operation", "CONNECT");
	json->SetObjectField("data", jsonData);

	return JsonUtil::jsonValueToString(json);
}

FString BrainCloudRTTComms::buildHeartbeatRequest()
{
	TSharedRef<FJsonObject> json = MakeShareable(new FJsonObject());
	json->SetStringField("service", ServiceName::RTT.getValue());
	json->SetStringField("operation", "HEARTBEAT");
	json->SetObjectField("data", nullptr);

	return JsonUtil::jsonValueToString(json);
}

void BrainCloudRTTComms::send(const FString &in_message, bool in_allowLogging/* = true*/)
{
	// early return
	if (m_connectedSocket == nullptr)
	{
		return;
	}

	m_connectedSocket->SendText(in_message);
	if (in_allowLogging && isLoggingEnabled())
		UE_LOG(LogBrainCloudComms, Log, TEXT("RTT SEND:  %s"), *in_message);

}

void BrainCloudRTTComms::processRegisteredListeners(const FString &in_service, const FString &in_operation, const FString &in_jsonMessage)
{
	//app out of focus error check
	if(m_websocketStatus == BCWebsocketStatus::CLOSED)
	{
		// if the websocket has been closed by this point, the RTT connection will have been closed and the user will have to re-enable rtt
		// error callback!
		if (m_appCallback != nullptr)
		{
			m_appCallback->serverError(ServiceName::RTTRegistration, ServiceOperation::Connect, 400, -1, in_jsonMessage);
		}
		else if (m_appCallbackBP != nullptr)
		{
			m_appCallbackBP->serverError(ServiceName::RTTRegistration, ServiceOperation::Connect, 400, -1, in_jsonMessage);
		}
		m_disconnectedWithReason = true;
		return;
	}

	// does this go to one of our registered service listeners?
	if (m_registeredRTTBluePrintCallbacks.Contains(in_service))
	{
		m_registeredRTTBluePrintCallbacks[in_service]->rttCallback(in_jsonMessage);
	}
	else if (m_registeredRTTCallbacks.Contains(in_service))
	{
		m_registeredRTTCallbacks[in_service]->rttCallback(in_jsonMessage);
	}
	// are we actually connected? only pump this back, when the server says we've connected
	else if (in_operation == TEXT("connect"))
	{
		m_rttConnectionStatus = BCRTTConnectionStatus::CONNECTED;
		m_lastNowMS = FPlatformTime::Seconds();

		// success callback!
		// server callback rtt connected with data!
		if (m_appCallback != nullptr)
		{
			m_appCallback->serverCallback(ServiceName::RTTRegistration, ServiceOperation::Connect, in_jsonMessage);
		}
		else if (m_appCallbackBP != nullptr)
		{
			m_appCallbackBP->serverCallback(ServiceName::RTTRegistration, ServiceOperation::Connect, in_jsonMessage);
		}
	}
	else if (in_operation == TEXT("error") || in_operation == TEXT("disconnect"))
	{
		// error callback!
		if (m_appCallback != nullptr)
		{
			m_appCallback->serverError(ServiceName::RTTRegistration, ServiceOperation::Connect, 400, -1, in_jsonMessage);
		}
		else if (m_appCallbackBP != nullptr)
		{
			m_appCallbackBP->serverError(ServiceName::RTTRegistration, ServiceOperation::Connect, 400, -1, in_jsonMessage);
		}

		if (in_operation == TEXT("disconnect"))
		{
			// this may remove the callback
			m_disconnectedWithReason = true;
		}
	}
}

void BrainCloudRTTComms::startReceivingWebSocket()
{
	bool sslEnabled = m_endpoint->GetBoolField(TEXT("ssl"));

	FString url = (sslEnabled ? TEXT("wss://") : TEXT("ws://"));
	url += m_endpoint->GetStringField(TEXT("host"));
	url += ":";
	url += FString::Printf(TEXT("%d"), m_endpoint->GetIntegerField(TEXT("port")));
	url += getUrlQueryParameters();

	UE_LOG(LogBrainCloudComms, Log, TEXT("Setting up web socket with url %s "), *url);

	setupWebSocket(url);
}

void BrainCloudRTTComms::setupWebSocket(const FString &in_url)
{
	m_timeSinceLastRequest = 0;
	
	// lazy load
	if (m_commsPtr == nullptr)
	{
		m_commsPtr = NewObject<UBCRTTCommsProxy>();
		m_commsPtr->AddToRoot();

		m_commsPtr->SetRTTComms(this);
	}

	// lazy load
	if (m_connectedSocket == nullptr)
	{
		m_connectedSocket = NewObject<UWinWebSocketBase>();
		m_connectedSocket->AddToRoot();

		m_connectedSocket->OnConnectError.AddDynamic(m_commsPtr, &UBCRTTCommsProxy::WebSocket_OnError);
		m_connectedSocket->OnClosed.AddDynamic(m_commsPtr, &UBCRTTCommsProxy::WebSocket_OnClose);
		m_connectedSocket->OnConnectComplete.AddDynamic(m_commsPtr, &UBCRTTCommsProxy::Websocket_OnOpen);
		m_connectedSocket->OnReceiveData.AddDynamic(m_commsPtr, &UBCRTTCommsProxy::WebSocket_OnMessage);

		m_connectedSocket->SetupSocket(in_url, m_client);
		m_connectedSocket->Connect();
	}
}

void BrainCloudRTTComms::webSocket_OnClose()
{
	if (isLoggingEnabled())
	{
		UE_LOG(LogBrainCloudComms, Log, TEXT("Connection closed"));
		
		if (m_disconnectedWithReason == true)
		{
			FString response;
			TSharedRef<TJsonWriter<>> disconnectJson = TJsonWriterFactory<>::Create(&response);
			UE_LOG(LogBrainCloudComms, Log, TEXT("RTT: Disconnect "), *response);
		}
	}
	
	m_websocketStatus = BCWebsocketStatus::CLOSED;
	processRegisteredListeners(ServiceName::RTTRegistration.getValue().ToLower(), "error", UBrainCloudWrapper::buildErrorJson(403, ReasonCodes::RS_CLIENT_ERROR,"Could not connect at this time"));
}

void BrainCloudRTTComms::websocket_OnOpen()
{
	// first time connecting? send the server connection call
	m_websocketStatus = BCWebsocketStatus::OPEN;
	send(buildConnectionRequest());
}

void BrainCloudRTTComms::webSocket_OnMessage(TArray<uint8> in_data)
{
	m_websocketStatus = BCWebsocketStatus::MESSAGE;
	FString parsedMessage = ConvertUtilities::BCBytesToString(in_data);
	onRecv(parsedMessage);
}

void BrainCloudRTTComms::webSocket_OnError(const FString &in_message)
{
	if (isLoggingEnabled())
		UE_LOG(LogBrainCloudComms, Log, TEXT("Error: %s"), *in_message);

	m_websocketStatus = BCWebsocketStatus::SOCKETERROR;
	processRegisteredListeners(ServiceName::RTTRegistration.getValue().ToLower(), "disconnect", UBrainCloudWrapper::buildErrorJson(403, ReasonCodes::RS_CLIENT_ERROR, in_message));
}

void BrainCloudRTTComms::onRecv(FString in_message)
{
	// deserialize and push broadcast to the correct m_registeredRTTCallbacks
	TSharedPtr<FJsonObject> jsonData = JsonUtil::jsonStringToValue(in_message);

	if (!jsonData.IsValid())
	{
		if (isLoggingEnabled())
			UE_LOG(LogBrainCloudComms, Log, TEXT("Failed to parse incoming JSON message: %s"), *in_message);
		return;
	}

	FString service = jsonData->HasField(TEXT("service")) ? jsonData->GetStringField(TEXT("service")) : "";
	FString operation = jsonData->HasField(TEXT("operation")) ? jsonData->GetStringField(TEXT("operation")) : "";
	TSharedPtr<FJsonObject> innerData = nullptr;
	bool bIsInnerDataValid = jsonData->HasTypedField<EJson::Object>(TEXT("data"));
	if (bIsInnerDataValid)
		innerData = jsonData->GetObjectField(TEXT("data"));

    if(operation == "HEARTBEAT"){
        m_heartBeatRecv = true;
    }
    
	if (operation != "HEARTBEAT" && isLoggingEnabled())
		UE_LOG(LogBrainCloudComms, Log, TEXT("RTT RECV:: %s"), *in_message);

	if (operation == "CONNECT")
	{
		int32 heartBeat = INITIAL_HEARTBEAT_TIME;
		if (bIsInnerDataValid && innerData->HasField(TEXT("heartbeatSeconds")))
		{
			heartBeat = innerData->GetIntegerField(TEXT("heartbeatSeconds"));
		}
		else if (bIsInnerDataValid && innerData->HasField(TEXT("wsHeartbeatSecs")))
		{
			heartBeat = innerData->GetIntegerField(TEXT("wsHeartbeatSecs"));
		}
		setRTTHeartBeatSeconds(heartBeat);
	}
	else if (operation == "DISCONNECT")
	{
		m_disconnectedWithReason = true;
		if (ensureAlways(innerData != nullptr)) {
			m_disconnectJson->SetStringField("reason", innerData->GetStringField(TEXT("reason")));
			m_disconnectJson->SetNumberField("reasonCode", innerData->GetNumberField(TEXT("reasonCode")));
			m_disconnectJson->SetStringField("severity", "ERROR");
		}
	}

	if (bIsInnerDataValid)
	{
		if (innerData->HasField(TEXT("cxId")))
		{
			m_cxId = innerData->GetStringField(TEXT("cxId"));
		}

		if (innerData->HasField(TEXT("evs")))
		{
			m_eventServer = innerData->GetStringField(TEXT("evs"));
		}
	}

	processRegisteredListeners(service.ToLower(), operation.ToLower(), in_message);
}

FString BrainCloudRTTComms::buildRTTRequestError(FString in_statusMessage)
{
	TSharedRef<FJsonObject> json = MakeShareable(new FJsonObject());
	
    json->SetNumberField("status", 403);
	json->SetNumberField("reason_code", ReasonCodes::RTT_CLIENT_ERROR);
	json->SetStringField("status_message", in_statusMessage);
	json->SetStringField("severity", "ERROR");

	FString response;
    TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&response);
    FJsonSerializer::Serialize(json, writer);

	return response;
}

void BrainCloudRTTComms::setEndpointFromType(TArray<TSharedPtr<FJsonValue>> in_endpoints, FString in_socketType)
{
	TSharedPtr<FJsonObject> tempObj = nullptr;
	bool isSocketType = in_socketType == TEXT("ws");
	for (int32 i = 0; i != in_endpoints.Num(); ++i)
	{
		tempObj = in_endpoints[i]->AsObject();
		if (tempObj->GetStringField(TEXT("protocol")) == in_socketType)
		{
			// try SSL by default for "ws"
			m_endpoint = isSocketType && tempObj->GetBoolField(TEXT("ssl")) ? tempObj : nullptr;

			// try the inverse
			if (!m_endpoint.IsValid())
			{
				m_endpoint = !isSocketType && !tempObj->GetBoolField(TEXT("ssl")) ? tempObj : nullptr;
			}
			break;
		}
	}
}

FString BrainCloudRTTComms::getUrlQueryParameters()
{
	FString toReturn = TEXT("/?");
	int32 count = 0;

	// clear this, since we add them within the iterator
	m_rttHeadersMap.Empty();

	// Iterate over Json Values
	for (auto currJsonValue = m_rttHeaders->Values.CreateConstIterator(); currJsonValue; ++currJsonValue)
	{
		// construct params
		if (count > 0)
			toReturn += TEXT("&");

		toReturn += (*currJsonValue).Key;
		toReturn += TEXT("=");
		TSharedPtr<FJsonValue> Value = (*currJsonValue).Value;
		toReturn += Value->AsString();

		m_rttHeadersMap.Add((*currJsonValue).Key, Value->AsString());

		++count;
	}

	return toReturn;
}

// server callback -> initial connect
void BrainCloudRTTComms::serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, const FString &in_jsonData)
{
	// read json
	//  --- data
	//      --- endpoints (endpoints for connection)
	///     --- auth (headers)
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(in_jsonData);
	TSharedPtr<FJsonObject> jsonPacket = MakeShareable(new FJsonObject());

	bool res = FJsonSerializer::Deserialize(reader, jsonPacket);
	if (res)
	{
		TSharedPtr<FJsonObject> jsonData = jsonPacket->GetObjectField(TEXT("data"));
		TArray<TSharedPtr<FJsonValue>> endpoints = jsonData->GetArrayField(TEXT("endpoints"));
		m_rttHeaders = jsonData->GetObjectField(TEXT("auth"));

		setEndpointFromType(endpoints, TEXT("ws"));
		connectWebSocket();
	}
}

void BrainCloudRTTComms::serverError(ServiceName serviceName, ServiceOperation serviceOperation, int32 statusCode, int32 reasonCode, const FString &jsonError)
{
	// server callback rtt connected with data!
	if (m_appCallback != nullptr)
	{
		m_appCallback->serverError(serviceName, serviceOperation, statusCode, reasonCode, jsonError);
	}
	else if (m_appCallbackBP != nullptr)
	{
		m_appCallbackBP->serverError(serviceName, serviceOperation, statusCode, reasonCode, jsonError);
	}

	//disconnect();
	m_disconnectedWithReason = true;
}

bool BrainCloudRTTComms::isLoggingEnabled()
{
	if (ensure(m_client != nullptr)) {
		return m_client->isLoggingEnabled();
	}

	return false;
}
