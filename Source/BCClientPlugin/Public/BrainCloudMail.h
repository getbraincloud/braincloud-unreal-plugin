// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

class BrainCloudClient;
class IServerCallback;

class BCCLIENTPLUGIN_API BrainCloudMail
{
public:
	BrainCloudMail(BrainCloudClient *client);

	/**
	 * Sends a simple text email to the specified player
	 *
	 * Service Name - mail
	 * Service Operation - SEND_BASIC_EMAIL
	 *
	 * @param in_profileId The user to send the email to
	 * @param in_subject The email subject
	 * @param in_body The email body
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendBasicEmail(const FString &profileId, const FString &subject, const FString &body, IServerCallback *callback = nullptr);

	/**
	 * Sends an advanced email to the specified player
	 *
	 * Service Name - mail
	 * Service Operation - SEND_ADVANCED_EMAIL
	 *
	 * @param in_profileId The user to send the email to
	 * @param in_jsonServiceParams Parameters to send to the email service. See the documentation for
	 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendAdvancedEmail(const FString &profileId, const FString &jsonServiceParams, IServerCallback *callback = nullptr);

	/**
	 * Sends an advanced email to the specified email address
	 *
	 * Service Name - mail
	 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESS
	 *
	 * @param in_emailAddress The address to send the email to
	 * @param in_jsonServiceParams Parameters to send to the email service. See the documentation for
	 *	a full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendAdvancedEmailByAddress(const FString &emailAddress, const FString &jsonServiceParams, IServerCallback *callback = nullptr);

	/**
	 * Sends an advanced email to the specified email addresses.
	 *
	 * Service Name - Mail
	 * Service Operation - SEND_ADVANCED_EMAIL_BY_ADDRESSES
	 *
	 * @param in_emailAddress The list of addresses to send the email to
	 * @param in_serviceParams Set of parameters dependant on the mail service configured
	 * @param in_callback The method to be invoked when the server response is received
	 */
	void sendAdvancedEmailByAddresses(const TArray<FString> &emailAddresses, const FString &jsonServiceParams, IServerCallback *callback = nullptr);

private:
	BrainCloudClient *_client = nullptr;
};