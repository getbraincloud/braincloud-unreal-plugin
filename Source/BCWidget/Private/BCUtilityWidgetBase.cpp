// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "BCUtilityWidgetBase.h"
#include "BCChildAppRowWidget.h"
#include "BCEditorCredentialsStore.h"
#include "BCWidgetPrivatePCH.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Styling/CoreStyle.h"
#include "HAL/PlatformProcess.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/EditableTextBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HAL/PlatformProcess.h"
#include "Widgets/Notifications/SNotificationList.h"

namespace
{
    const TCHAR* GDispatcherSuffix = TEXT("/dispatcherv2");

    void SetPanelText(UWidget* Widget, const FString& Value)
    {
        const FText Text = FText::FromString(Value);
        if (UTextBlock* Block = Cast<UTextBlock>(Widget)) Block->SetText(Text);
        else if (UEditableText* Edit = Cast<UEditableText>(Widget)) Edit->SetText(Text);
        else if (UEditableTextBox* Box = Cast<UEditableTextBox>(Widget)) Box->SetText(Text);
        else if (UMultiLineEditableText* Multi = Cast<UMultiLineEditableText>(Widget)) Multi->SetText(Text);
        else if (UMultiLineEditableTextBox* MultiBox = Cast<UMultiLineEditableTextBox>(Widget)) MultiBox->SetText(Text);
    }

    bool IsEditableText(const UWidget* Widget)
    {
        return Cast<UEditableText>(Widget) || Cast<UEditableTextBox>(Widget) ||
            Cast<UMultiLineEditableText>(Widget) || Cast<UMultiLineEditableTextBox>(Widget);
    }

    FString GetPanelText(UWidget* Widget)
    {
        if (UTextBlock* Block = Cast<UTextBlock>(Widget)) return Block->GetText().ToString();
        if (UEditableText* Edit = Cast<UEditableText>(Widget)) return Edit->GetText().ToString();
        if (UEditableTextBox* Box = Cast<UEditableTextBox>(Widget)) return Box->GetText().ToString();
        if (UMultiLineEditableText* Multi = Cast<UMultiLineEditableText>(Widget)) return Multi->GetText().ToString();
        if (UMultiLineEditableTextBox* MultiBox = Cast<UMultiLineEditableTextBox>(Widget)) return MultiBox->GetText().ToString();
        return FString();
    }

    void SetPanelPassword(UWidget* Widget, bool bIsPassword)
    {
        if (UEditableText* Edit = Cast<UEditableText>(Widget)) Edit->SetIsPassword(bIsPassword);
        else if (UEditableTextBox* Box = Cast<UEditableTextBox>(Widget)) Box->SetIsPassword(bIsPassword);
    }

    void SetPanelShown(UWidget* Widget, bool bShown)
    {
        if (Widget)
        {
            Widget->SetVisibility(bShown ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        }
    }

    UTextBlock* FindButtonLabel(UButton* Button)
    {
        if (!Button) return nullptr;
        UWidget* Content = Button->GetContent();
        if (UTextBlock* Block = Cast<UTextBlock>(Content)) return Block;
        if (UPanelWidget* Panel = Cast<UPanelWidget>(Content))
        {
            for (int32 i = 0; i < Panel->GetChildrenCount(); ++i)
            {
                if (UTextBlock* Child = Cast<UTextBlock>(Panel->GetChildAt(i))) return Child;
            }
        }
        return nullptr;
    }

    FString StripDispatcher(FString Url)
    {
        Url.TrimStartAndEndInline();
        Url.RemoveFromEnd(TEXT("/"));
        Url.RemoveFromEnd(GDispatcherSuffix);
        Url.RemoveFromEnd(TEXT("/"));
        if (!Url.IsEmpty() && !Url.Contains(TEXT("://")))
        {
            Url = TEXT("https://") + Url;
        }
        return Url;
    }

    FString ParseAppSecret(const TSharedPtr<FJsonObject>& Json)
    {
        FString AppSecret;
        const TSharedPtr<FJsonObject>* AppObj = nullptr;
        if (Json.IsValid() && Json->TryGetObjectField(TEXT("app"), AppObj) && AppObj)
        {
            (*AppObj)->TryGetStringField(TEXT("appSecret"), AppSecret);
        }
        return AppSecret;
    }

    const TCHAR* GCreateAppOption = TEXT("+ Create New App");

    struct FBCPlatform
    {
        const TCHAR* Id;
        const TCHAR* Name;
        bool bDefault;
    };

    const FBCPlatform GPlatforms[] = {
        { TEXT("AMAZON"), TEXT("Amazon"), false },
        { TEXT("APPLE_TV_OS"), TEXT("Apple tvOS"), false },
        { TEXT("VISION_OS"), TEXT("Apple visionOS"), false },
        { TEXT("WATCH_OS"), TEXT("Apple watchOS"), false },
        { TEXT("BB"), TEXT("BlackBerry"), false },
        { TEXT("FB"), TEXT("Facebook"), false },
        { TEXT("ANG"), TEXT("Google Android"), false },
        { TEXT("IOS"), TEXT("iOS"), false },
        { TEXT("LINUX"), TEXT("Linux"), true },
        { TEXT("MAC"), TEXT("Mac OS X"), true },
        { TEXT("NINTENDO"), TEXT("Nintendo"), false },
        { TEXT("OCULUS"), TEXT("Oculus"), false },
        { TEXT("PS4"), TEXT("Playstation"), false },
        { TEXT("PS3"), TEXT("Playstation 3"), false },
        { TEXT("PS_VITA"), TEXT("Playstation Vita"), false },
        { TEXT("ROKU"), TEXT("Roku"), false },
        { TEXT("TIZEN"), TEXT("Tizen"), false },
        { TEXT("WEB"), TEXT("Web"), false },
        { TEXT("WII"), TEXT("Wii"), false },
        { TEXT("WINDOWS"), TEXT("Windows"), true },
        { TEXT("WINP"), TEXT("Windows Phone"), false },
        { TEXT("XBOX_ONE"), TEXT("Xbox"), false },
        { TEXT("XBOX_360"), TEXT("Xbox 360"), false },
        { TEXT("UNKNOWN"), TEXT("Unknown"), false }
    };

    FString NormalizeWidgetName(const FString& Name)
    {
        return Name.Replace(TEXT(" "), TEXT("")).ToLower();
    }

    TArray<UWidget*> FindAllByNameOrLabel(UWidgetTree* Tree, const TCHAR* Name)
    {
        TArray<UWidget*> Found;
        if (!Tree)
        {
            return Found;
        }
        const FString Wanted = NormalizeWidgetName(Name);
        Tree->ForEachWidget([&](UWidget* Widget)
        {
            if (!Widget)
            {
                return;
            }
            bool bMatch = NormalizeWidgetName(Widget->GetName()) == Wanted;
#if WITH_EDITOR
            bMatch = bMatch || NormalizeWidgetName(Widget->GetDisplayLabel()) == Wanted;
#endif
            if (bMatch)
            {
                Found.Add(Widget);
            }
        });
        if (Found.Num() > 1)
        {
            TArray<FString> Names;
            for (const UWidget* Widget : Found)
            {
                Names.Add(Widget->GetName());
            }
            UE_LOG(LogBCWidget, Warning, TEXT("[Panel] several widgets match %s: %s"), Name, *FString::Join(Names, TEXT(", ")));
        }
        return Found;
    }

    UWidget* FindByNameOrLabel(UWidgetTree* Tree, const TCHAR* Name)
    {
        if (!Tree)
        {
            return nullptr;
        }
        if (UWidget* ByName = Tree->FindWidget(FName(Name)))
        {
            return ByName;
        }
        const FString Wanted = NormalizeWidgetName(Name);
        UWidget* Found = nullptr;
        Tree->ForEachWidget([&](UWidget* Widget)
        {
            if (Found || !Widget)
            {
                return;
            }
            if (NormalizeWidgetName(Widget->GetName()) == Wanted)
            {
                Found = Widget;
            }
#if WITH_EDITOR
            else if (NormalizeWidgetName(Widget->GetDisplayLabel()) == Wanted)
            {
                Found = Widget;
            }
#endif
        });
        return Found;
    }

    template <typename T>
    T* FindPanelWidget(UWidgetTree* Tree, const TCHAR* Name, TArray<FString>* Missing = nullptr)
    {
        UWidget* Found = FindByNameOrLabel(Tree, Name);
        if (!Found && Missing)
        {
            Missing->Add(Name);
        }
        T* Typed = Cast<T>(Found);
        if (Found && !Typed)
        {
            UE_LOG(LogBCWidget, Warning, TEXT("[Panel] widget '%s' is a %s, expected %s - not bound"),
                Name, *Found->GetClass()->GetName(), *T::StaticClass()->GetName());
        }
        return Typed;
    }

    void ParseAppExtras(FBCPortalApp& App, const TSharedPtr<FJsonObject>& Obj)
    {
        Obj->TryGetStringField(TEXT("parentAppId"), App.ParentAppId);
    }

    void ParseAppExtras(FBCPortalTemplateApp&, const TSharedPtr<FJsonObject>&)
    {
    }

    template <typename TAppStruct>
    TArray<TAppStruct> ParseAppArray(const TSharedPtr<FJsonObject>& Json)
    {
        TArray<TAppStruct> Result;
        if (!Json.IsValid())
        {
            return Result;
        }

        const TArray<TSharedPtr<FJsonValue>>* Apps = nullptr;
        if (Json->TryGetArrayField(TEXT("apps"), Apps) && Apps)
        {
            for (const TSharedPtr<FJsonValue>& AppValue : *Apps)
            {
                const TSharedPtr<FJsonObject>* AppObj;
                if (AppValue.IsValid() && AppValue->TryGetObject(AppObj))
                {
                    TAppStruct App;
                    (*AppObj)->TryGetStringField(TEXT("appId"), App.AppId);
                    (*AppObj)->TryGetStringField(TEXT("appName"), App.AppName);
                    ParseAppExtras(App, *AppObj);
                    Result.Add(App);
                }
            }
        }
        return Result;
    }
}

UBCUtilityWidgetBase::UBCUtilityWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    OAuthClient = MakeShared<BCPortalOAuthClient>();
    BuilderApiClient = MakeShared<BCBuilderApiClient>();
}

bool UBCUtilityWidgetBase::IsOAuthAvailable() const
{
    return BCPortalOAuthClient::IsOAuthSupported();
}

void UBCUtilityWidgetBase::StartLogin(const FString& ServerUrl, const FString& ClientId)
{
    CurrentServerUrl = ServerUrl;
    OAuthClient->StartLogin(ServerUrl, ClientId,
        FBCLoginSuccessDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleLoginSuccess),
        FBCLoginFailureDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleLoginFailure));
}

void UBCUtilityWidgetBase::CancelLogin()
{
    OAuthClient->CancelLogin();
}

void UBCUtilityWidgetBase::Logout()
{
    BCEditorCredentialsStore::Clear();
    CurrentAdminEmail.Empty();
    CurrentTeamId.Empty();
    CurrentApiKey.Empty();
}

bool UBCUtilityWidgetBase::TryRestoreRememberedAccount(const FString& ServerUrl, FString& OutAdminEmail, FString& OutTeamId)
{
    const FBCEditorCredentials Credentials = BCEditorCredentialsStore::Load();
    if (!Credentials.IsValid())
    {
        return false;
    }

    CurrentServerUrl = ServerUrl;
    CurrentAdminEmail = Credentials.AdminEmail;
    CurrentTeamId = Credentials.TeamId;
    CurrentApiKey = Credentials.ApiKey;
    BuilderApiClient->Configure(ServerUrl, CurrentAdminEmail, CurrentApiKey);

    OutAdminEmail = CurrentAdminEmail;
    OutTeamId = CurrentTeamId;
    return true;
}

void UBCUtilityWidgetBase::SwitchTeam(const FString& TeamId)
{
    PendingSwitchTeamId = TeamId;
    OAuthClient->RefreshApiKeyForTeam(TeamId,
        FBCApiKeyRefreshSuccessDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleApiKeyRefreshed),
        FBCLoginFailureDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleApiKeyRefreshFailed));
}

void UBCUtilityWidgetBase::HandleApiKeyRefreshed(const FString& NewApiKey)
{
    CurrentApiKey = NewApiKey;
    BuilderApiClient->Configure(CurrentServerUrl, CurrentAdminEmail, NewApiKey);

    FBCEditorCredentials Credentials = BCEditorCredentialsStore::Load();
    Credentials.AdminEmail = CurrentAdminEmail;
    Credentials.TeamId = PendingSwitchTeamId;
    Credentials.ApiKey = NewApiKey;
    BCEditorCredentialsStore::Save(Credentials);

    FetchApps(PendingSwitchTeamId);
}

void UBCUtilityWidgetBase::HandleApiKeyRefreshFailed(const FString& ErrorMessage)
{
    PopulateTeams(CachedTeams, CurrentTeamId);
    ReportRequestFailure(ErrorMessage);
}

void UBCUtilityWidgetBase::ReportRequestFailure(const FString& ErrorMessage)
{
    Notify(ErrorMessage, true);
    OnRequestFailed(ErrorMessage);
}

void UBCUtilityWidgetBase::FetchTeams()
{
    BuilderApiClient->GetTeams(FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleTeamsResponse));
}

void UBCUtilityWidgetBase::FetchApps(const FString& TeamId)
{
    CurrentTeamId = TeamId;
    BuilderApiClient->GetApps(TeamId, FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleAppsResponse));
}

void UBCUtilityWidgetBase::FetchAppSecret(const FString& TeamId, const FString& AppId)
{
    PendingAppSecretAppId = AppId;
    BuilderApiClient->GetAppSecret(TeamId, AppId, FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleAppSecretResponse));
}

void UBCUtilityWidgetBase::FetchTemplateApps()
{
    BuilderApiClient->GetTemplateApps(FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleTemplateAppsResponse));
}

void UBCUtilityWidgetBase::CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
    const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled)
{
    CurrentTeamId = TeamId;
    BuilderApiClient->CreateApp(TeamId, AppName, TemplateAppId, SupportedPlatforms, bGamificationEnabled,
        FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleCreateAppResponse));
}

void UBCUtilityWidgetBase::HandleLoginSuccess(const FBCLoginResult& Result)
{
    CurrentAdminEmail = Result.Email;
    CurrentTeamId = Result.CurrentTeamId;
    CurrentApiKey = Result.ApiKey;

    FBCEditorCredentials Credentials;
    Credentials.AdminEmail = Result.Email;
    Credentials.TeamId = Result.CurrentTeamId;
    Credentials.ApiKey = Result.ApiKey;
    Credentials.AccessToken = Result.AccessToken;
    Credentials.AccessTokenExpiresAt = Result.AccessTokenExpiresAt;
    BCEditorCredentialsStore::Save(Credentials);

    BuilderApiClient->Configure(CurrentServerUrl, Result.Email, Result.ApiKey);

    SetLoginInFlight(false);

    FString StoredBase = StripDispatcher(GetConfiguredServerUrl());
    if (StoredBase.IsEmpty())
    {
        StoredBase = StripDispatcher(DefaultServerUrl);
    }
    if (!StoredBase.Equals(CurrentServerUrl, ESearchCase::IgnoreCase))
    {
        SelectedAppId.Empty();
        SelectedAppName.Empty();
        ResetChildApps();
    }

    PopulateTeams(Result.Teams, Result.CurrentTeamId);
    ShowPickerView();
    FetchApps(Result.CurrentTeamId);

    OnLoginSucceeded(Result.Email);
    OnTeamsReceived(Result.Teams);
}

void UBCUtilityWidgetBase::HandleLoginFailure(const FString& ErrorMessage)
{
    SetLoginInFlight(false);
    Notify(ErrorMessage, true);
    OnLoginFailed(ErrorMessage);
}

void UBCUtilityWidgetBase::HandleResumeFailure(const FString& ErrorMessage)
{
    UE_LOG(LogBCWidget, Log, TEXT("[OAuth] session resume failed (%s), starting browser login"), *ErrorMessage);
    FBCEditorCredentials Credentials = BCEditorCredentialsStore::Load();
    Credentials.AccessToken.Empty();
    Credentials.AccessTokenExpiresAt = 0;
    BCEditorCredentialsStore::Save(Credentials);
    StartLogin(CurrentServerUrl, OAuthClientId);
}

void UBCUtilityWidgetBase::HandleTeamsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        ReportRequestFailure(ErrorMessage);
        return;
    }

    TArray<FBCPortalTeam> Teams;
    if (Json.IsValid())
    {
        const TArray<TSharedPtr<FJsonValue>>* TeamValues = nullptr;
        if (Json->TryGetArrayField(TEXT("teams"), TeamValues) && TeamValues)
        {
            for (const TSharedPtr<FJsonValue>& TeamValue : *TeamValues)
            {
                const TSharedPtr<FJsonObject>* TeamObj;
                if (TeamValue.IsValid() && TeamValue->TryGetObject(TeamObj))
                {
                    FBCPortalTeam Team;
                    (*TeamObj)->TryGetStringField(TEXT("teamId"), Team.TeamId);
                    (*TeamObj)->TryGetStringField(TEXT("teamName"), Team.TeamName);
                    (*TeamObj)->TryGetBoolField(TEXT("apiEnabled"), Team.bApiEnabled);
                    Teams.Add(Team);
                }
            }
        }
    }
    PopulateTeams(Teams, CurrentTeamId);
    OnTeamsReceived(Teams);
}

void UBCUtilityWidgetBase::HandleAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        ReportRequestFailure(ErrorMessage);
        return;
    }
    TArray<FBCPortalApp> ReceivedApps = ParseAppArray<FBCPortalApp>(Json);
    ReceivedApps.Sort([](const FBCPortalApp& A, const FBCPortalApp& B) { return A.AppName < B.AppName; });
    bAppsLoaded = true;
    PopulateApps(ReceivedApps);
    OnAppsReceived(ReceivedApps);
}

void UBCUtilityWidgetBase::HandleAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        ReportRequestFailure(ErrorMessage);
        return;
    }

    const FString AppSecret = ParseAppSecret(Json);
    if (AppSecret.IsEmpty())
    {
        Notify(TEXT("The portal did not return a secret for this app."), true);
    }
    else if (PendingAppSecretAppId == SelectedAppId)
    {
        SetPanelText(AppIdTextW, SelectedAppId);
        SetPanelText(AppNameTextW, SelectedAppName);
        SetPanelText(AppSecretTextW, AppSecret);
        Notify(FString::Printf(TEXT("Loaded %s - press SAVE to use it in this project."), *SelectedAppName), false);
    }
    OnAppSecretReceived(PendingAppSecretAppId, AppSecret);
}

void UBCUtilityWidgetBase::HandleTemplateAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        ReportRequestFailure(ErrorMessage);
        return;
    }
    const TArray<FBCPortalTemplateApp> Templates = ParseAppArray<FBCPortalTemplateApp>(Json);
    PopulateTemplates(Templates);
    OnTemplateAppsReceived(Templates);
}

void UBCUtilityWidgetBase::HandleCreateAppResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    if (!bSuccess)
    {
        SetCreateInFlight(false);
        ReportRequestFailure(ErrorMessage);
        return;
    }

    FString AppId, AppName;
    if (Json.IsValid())
    {
        const TSharedPtr<FJsonObject>* AppObj = nullptr;
        if (Json->TryGetObjectField(TEXT("app"), AppObj) && AppObj)
        {
            (*AppObj)->TryGetStringField(TEXT("appId"), AppId);
            (*AppObj)->TryGetStringField(TEXT("appName"), AppName);
        }
    }

    if (AppId.IsEmpty())
    {
        SetCreateInFlight(false);
        ReportRequestFailure(TEXT("The portal did not return an app id for the new app."));
        return;
    }

    PendingCreateAppId = AppId;
    PendingCreateAppName = AppName;

    BuilderApiClient->GetAppSecret(CurrentTeamId, AppId,
        FBCApiResponseDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleCreateAppSecretResponse));
}

void UBCUtilityWidgetBase::HandleCreateAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
{
    SetCreateInFlight(false);
    const FString AppSecret = bSuccess ? ParseAppSecret(Json) : FString();
    if (AppSecret.IsEmpty())
    {
        Notify(FString::Printf(TEXT("App %s was created, but its secret could not be loaded. Refresh the app list and select it."), *PendingCreateAppId), true);
    }
    else
    {
        FinishCreatedApp(PendingCreateAppId, PendingCreateAppName, AppSecret);
    }
    OnAppCreated(PendingCreateAppId, PendingCreateAppName, AppSecret);
}

void UBCUtilityWidgetBase::RefreshCredentialState()
{
    CredentialState = FBCSecureStore::DetectState();

    if (CredentialState == EBCCredentialState::Legacy)
    {
        FString Error;
        if (FBCSecureStore::MigrateLegacy(Error))
        {
            CredentialState = FBCSecureStore::DetectState();
            OnCredentialsMigrated(GetConfiguredAppId());
        }
        else
        {
            UE_LOG(LogBCWidget, Warning, TEXT("[Credentials] migration failed: %s"), *Error);
        }
    }

    OnCredentialStateChanged(CredentialState, GetConfiguredAppId(), GetConfiguredAppName());
}

FString UBCUtilityWidgetBase::GetConfiguredAppId() const
{
    return FBCSecureStore::ResolveAppId();
}

FString UBCUtilityWidgetBase::GetConfiguredAppName() const
{
    return FBCSecureStore::ResolveAppName();
}

FString UBCUtilityWidgetBase::GetConfiguredServerUrl() const
{
    return FBCSecureStore::ResolveServerUrl();
}

bool UBCUtilityWidgetBase::RevealStoredCredentials(FString& OutAppId, FString& OutAppSecret)
{
    FBCStoredCredentials Credentials;
    if (!FBCSecureStore::Resolve(Credentials))
    {
        OutAppId.Empty();
        OutAppSecret.Empty();
        return false;
    }
    OutAppId = Credentials.AppId;
    OutAppSecret = Credentials.AppSecret;
    return true;
}

bool UBCUtilityWidgetBase::MigrateLegacyCredentials(FString& OutError)
{
    const bool bMigrated = FBCSecureStore::MigrateLegacy(OutError);
    if (bMigrated)
    {
        CredentialState = FBCSecureStore::DetectState();
        OnCredentialsMigrated(GetConfiguredAppId());
        OnCredentialStateChanged(CredentialState, GetConfiguredAppId(), GetConfiguredAppName());
    }
    return bMigrated;
}

bool UBCUtilityWidgetBase::ApplySelectedApp(const FString& AppId, const FString& AppName,
    const FString& AppSecret, const FString& ServerUrl, FString& OutError)
{
    const FString EffectiveUrl = ServerUrl.IsEmpty() ? FBCSecureStore::ResolveServerUrl() : ServerUrl;

    if (!FBCSecureStore::Store(AppId, AppSecret, AppName, EffectiveUrl, OutError))
    {
        return false;
    }

    CredentialState = FBCSecureStore::DetectState();
    OnCredentialsSaved(AppId, AppName);
    OnCredentialStateChanged(CredentialState, AppId, AppName);
    return true;
}

void UBCUtilityWidgetBase::ClearStoredCredentials()
{
    FBCSecureStore::Clear();
    CredentialState = FBCSecureStore::DetectState();
    OnCredentialStateChanged(CredentialState, FString(), FString());
}

bool UBCUtilityWidgetBase::HasLiveOAuthSession() const
{
    return OAuthClient.IsValid() && OAuthClient->HasAccessToken();
}

void UBCUtilityWidgetBase::BeginChangeApp(const FString& ServerUrl)
{
    if (HasLiveOAuthSession())
    {
        FetchTeams();
        return;
    }

    FString RestoredEmail;
    FString RestoredTeamId;
    if (TryRestoreRememberedAccount(ServerUrl, RestoredEmail, RestoredTeamId))
    {
        OnLoginSucceeded(RestoredEmail);
        FetchApps(RestoredTeamId);
        return;
    }

    OnNeedsLogin();
}

void UBCUtilityWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    if (IsDesignTime())
    {
        return;
    }

    BindPanelWidgets();
    TryResumeSession();
    RefreshCredentialState();
    LoadFieldsFromStore();
    ShowAccountView();
}

void UBCUtilityWidgetBase::NativeDestruct()
{
    if (bLoginInFlight)
    {
        CancelLogin();
        bLoginInFlight = false;
    }
    Super::NativeDestruct();
}

#define BC_BIND_CLICK(Button, Handler) \
    if (Button) { Button->OnClicked.RemoveAll(this); Button->OnClicked.AddDynamic(this, Handler); }

void UBCUtilityWidgetBase::BindPanelWidgets()
{
    TArray<FString> Missing;
    LoginButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("LoginButton"), &Missing);
    ChangeAppButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("ChangeAppButton"), &Missing);
    TeamRefreshButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("TeamRefreshButton"), &Missing);
    AppRefreshButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("AppRefreshButton"), &Missing);
    SaveButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("SaveButton"), &Missing);
    AppIdClearButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("AppIdClearButton"), &Missing);
    AppSecretClearButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("AppSecretClearButton"), &Missing);
    AppSecretShowToggleButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("AppSecretShowToggleButton"), &Missing);
    TeamSelectBoxW = FindPanelWidget<UComboBoxString>(WidgetTree, TEXT("TeamSelectBox"), &Missing);
    AppSelectBoxW = FindPanelWidget<UComboBoxString>(WidgetTree, TEXT("AppSelectBox"), &Missing);
    UseDefaultServerCheckboxW = FindPanelWidget<UCheckBox>(WidgetTree, TEXT("UseDefaultServerCheckbox"), &Missing);
    AccountSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("AccountNotAuthenticatedSection"), &Missing);
    SelectAppSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("SelectAppSection"), &Missing);
    ServerUrlSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ServerUrlSection"), &Missing);
    HelpTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("HelpText"), &Missing);
    AccountDescriptionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("accountEmail"), &Missing);
    AppIdTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("AppIdText"), &Missing);
    AppNameTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("AppNameText"), &Missing);
    AppSecretTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("AppSecretText"), &Missing);
    AppSecretToggleTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("AppSecretToggleButtonText"), &Missing);
    ServerUrlTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ServerUrl"), &Missing);
    ParentAppCheckboxW = FindPanelWidget<UCheckBox>(WidgetTree, TEXT("ParentAppCheckbox"), &Missing);
    ParentAppCheckboxSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ParentAppCheckboxSection"));
    ChildAppsCredentialsSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ChildAppsCredentialsSection"), &Missing);
    ChildAppsTextW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ChildAppsText"), &Missing);
    DebugLoggingCheckboxW = FindPanelWidget<UCheckBox>(WidgetTree, TEXT("EnableDebugLoggingCheckbox"), &Missing);
    ChildAppsSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("ChildAppsSection"), &Missing);
    ChildAppRowsContainerW = FindPanelWidget<UPanelWidget>(WidgetTree, TEXT("ChildAppRowsContainer"), &Missing);
    AddChildAppButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("AddChildAppButton"), &Missing);
    CreateAppSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("CreateAppSection"), &Missing);
    NewAppNameW = FindPanelWidget<UWidget>(WidgetTree, TEXT("NewAppName"), &Missing);
    CreateFromTemplateCheckboxW = FindPanelWidget<UCheckBox>(WidgetTree, TEXT("CreateFromTemplateCheckbox"), &Missing);
    TemplateAppSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("TemplateAppSection"), &Missing);
    TemplateAppSelectBoxW = FindPanelWidget<UComboBoxString>(WidgetTree, TEXT("TemplateAppSelectBox"), &Missing);
    PlatformsSectionW = FindPanelWidget<UWidget>(WidgetTree, TEXT("PlatformsSection"), &Missing);
    PlatformsContainerW = FindPanelWidget<UPanelWidget>(WidgetTree, TEXT("PlatformsContainer"), &Missing);
    CreateAppButtonW = FindPanelWidget<UButton>(WidgetTree, TEXT("CreateAppButton"), &Missing);
    if (Missing.Num() > 0)
    {
        UE_LOG(LogBCWidget, Warning, TEXT("[Panel] widgets not found: %s"), *FString::Join(Missing, TEXT(", ")));
    }
    if (!IsEditableText(ServerUrlTextW))
    {
        ServerUrlTextW = nullptr;
    }
    if (!ChildAppRowClass)
    {
        ChildAppRowClass = LoadClass<UBCChildAppRowWidget>(nullptr, TEXT("/BCClient/EditorUtility/BCChildAppRow.BCChildAppRow_C"));
    }
    PlatformCheckboxes.Reset();
    PlatformIds.Reset();

    BC_BIND_CLICK(LoginButtonW, &UBCUtilityWidgetBase::HandleLoginClicked);
    BC_BIND_CLICK(ChangeAppButtonW, &UBCUtilityWidgetBase::HandleChangeAppClicked);
    BC_BIND_CLICK(TeamRefreshButtonW, &UBCUtilityWidgetBase::HandleTeamRefreshClicked);
    BC_BIND_CLICK(AppRefreshButtonW, &UBCUtilityWidgetBase::HandleAppRefreshClicked);
    for (UWidget* Match : FindAllByNameOrLabel(WidgetTree, TEXT("SaveButton")))
    {
        UButton* SaveMatch = Cast<UButton>(Match);
        BC_BIND_CLICK(SaveMatch, &UBCUtilityWidgetBase::HandleSaveClicked);
    }
    BC_BIND_CLICK(AppIdClearButtonW, &UBCUtilityWidgetBase::HandleAppIdClearClicked);
    BC_BIND_CLICK(AppSecretClearButtonW, &UBCUtilityWidgetBase::HandleAppSecretClearClicked);
    BC_BIND_CLICK(AppSecretShowToggleButtonW, &UBCUtilityWidgetBase::HandleSecretToggleClicked);
    BC_BIND_CLICK(AddChildAppButtonW, &UBCUtilityWidgetBase::HandleAddChildAppClicked);
    BC_BIND_CLICK(CreateAppButtonW, &UBCUtilityWidgetBase::HandleCreateAppClicked);

    UButton* PortalLink = FindPanelWidget<UButton>(WidgetTree, TEXT("PortalButtonLink"));
    UButton* ApiReferenceLink = FindPanelWidget<UButton>(WidgetTree, TEXT("ApiReferenceButtonLink"));
    UButton* DocsLink = FindPanelWidget<UButton>(WidgetTree, TEXT("DocsButtonLink"));
    UButton* UnrealSdkLink = FindPanelWidget<UButton>(WidgetTree, TEXT("UnrealSDKButtonLink"));
    BC_BIND_CLICK(PortalLink, &UBCUtilityWidgetBase::HandlePortalLinkClicked);
    BC_BIND_CLICK(ApiReferenceLink, &UBCUtilityWidgetBase::HandleApiReferenceLinkClicked);
    BC_BIND_CLICK(DocsLink, &UBCUtilityWidgetBase::HandleDocsLinkClicked);
    BC_BIND_CLICK(UnrealSdkLink, &UBCUtilityWidgetBase::HandleUnrealSdkLinkClicked);

    if (TeamSelectBoxW)
    {
        TeamSelectBoxW->OnSelectionChanged.RemoveAll(this);
        TeamSelectBoxW->OnSelectionChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleTeamSelected);
    }
    if (AppSelectBoxW)
    {
        AppSelectBoxW->OnSelectionChanged.RemoveAll(this);
        AppSelectBoxW->OnSelectionChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleAppSelected);
    }
    if (DebugLoggingCheckboxW)
    {
        DebugLoggingCheckboxW->OnCheckStateChanged.RemoveAll(this);
        DebugLoggingCheckboxW->OnCheckStateChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleDebugLoggingChanged);
    }
    if (CreateFromTemplateCheckboxW)
    {
        CreateFromTemplateCheckboxW->OnCheckStateChanged.RemoveAll(this);
        CreateFromTemplateCheckboxW->OnCheckStateChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleCreateFromTemplateChanged);
    }
    if (ParentAppCheckboxW)
    {
        ParentAppCheckboxW->OnCheckStateChanged.RemoveAll(this);
        ParentAppCheckboxW->OnCheckStateChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleParentAppChanged);
    }
    if (UseDefaultServerCheckboxW)
    {
        UseDefaultServerCheckboxW->OnCheckStateChanged.RemoveAll(this);
        UseDefaultServerCheckboxW->OnCheckStateChanged.AddDynamic(this, &UBCUtilityWidgetBase::HandleUseDefaultServerChanged);
    }

    if (UTextBlock* Label = FindButtonLabel(LoginButtonW)) LoginLabel = Label->GetText();
    if (UTextBlock* Label = FindButtonLabel(ChangeAppButtonW)) ChangeAppLabel = Label->GetText();
}

#undef BC_BIND_CLICK

void UBCUtilityWidgetBase::TryResumeSession()
{
    FString Email, TeamId;
    if (TryRestoreRememberedAccount(GetPortalUrl(), Email, TeamId))
    {
        UE_LOG(LogBCWidget, Log, TEXT("[Panel] remembered portal account %s"), *Email);
    }
}

void UBCUtilityWidgetBase::LoadFieldsFromStore()
{
    FString AppId, AppSecret;
    if (!RevealStoredCredentials(AppId, AppSecret))
    {
        AppId = GetConfiguredAppId();
    }
    const FString AppName = GetConfiguredAppName();

    SetPanelText(AppIdTextW, AppId);
    SetPanelText(AppSecretTextW, AppSecret);
    SetPanelPassword(AppSecretTextW, !bSecretVisible);
    SetPanelText(AppNameTextW, AppName.IsEmpty() ? (AppId.IsEmpty() ? TEXT("No app selected") : TEXT("(unnamed)")) : *AppName);
    SelectedAppId = AppId;
    SelectedAppName = AppName;

    FBCStoredCredentials Stored;
    FBCSecureStore::Resolve(Stored);
    ClearChildRows();
    for (const FBCStoredChildApp& Child : Stored.ChildApps)
    {
        AddChildRow(Child);
    }
    if (ParentAppCheckboxW)
    {
        ParentAppCheckboxW->SetIsChecked(Stored.ChildApps.Num() > 0);
    }
    UpdateChildVisibility();

    if (DebugLoggingCheckboxW)
    {
        DebugLoggingCheckboxW->SetIsChecked(FBCSecureStore::ResolveDebugLogging());
    }

    bCreateOptionSelected = false;
    UpdateCreateAppView();

    const FString StoredBase = StripDispatcher(GetConfiguredServerUrl());
    const bool bDefault = StoredBase.IsEmpty() || StoredBase == StripDispatcher(DefaultServerUrl);
    if (UseDefaultServerCheckboxW)
    {
        UseDefaultServerCheckboxW->SetIsChecked(bDefault);
    }
    SetPanelText(ServerUrlTextW, bDefault ? DefaultServerUrl : StoredBase);
    SetPanelShown(ServerUrlSectionW, !bDefault);
}

void UBCUtilityWidgetBase::ShowAccountView()
{
    const bool bConfigured = CredentialState == EBCCredentialState::Secure || CredentialState == EBCCredentialState::Legacy;
    const bool bAuthenticated = HasAuthenticatedSession();
    const bool bShowChangeApp = bConfigured || bAuthenticated;

    SetPanelShown(AccountSectionW, true);
    SetPanelShown(SelectAppSectionW, false);
    SetPanelShown(LoginButtonW, !bShowChangeApp);
    SetPanelShown(ChangeAppButtonW, bShowChangeApp);

    FString Help;
    if (bConfigured)
    {
        Help = TEXT("This project's app credentials (below) are already configured and in use. Change the app only if you want to switch to a different one.");
    }
    else if (bAuthenticated)
    {
        Help = FString::Printf(TEXT("Signed in as %s. Choose CHANGE APP to pick an app for this project."), *CurrentAdminEmail);
    }
    else if (CredentialState == EBCCredentialState::Invalid)
    {
        Help = TEXT("The stored credentials can't be read. They were copied from another project or edited. Log in to pick the app again.");
    }
    else
    {
        Help = TEXT("No app credentials are set. Log in to brainCloud to pick an app for this project.");
    }
    SetPanelText(HelpTextW, Help);
}

void UBCUtilityWidgetBase::ShowPickerView()
{
    SetPanelShown(AccountSectionW, false);
    SetPanelShown(SelectAppSectionW, true);
    SetPanelShown(LoginButtonW, false);
    SetPanelShown(ChangeAppButtonW, false);
    SetPanelText(AccountDescriptionW, CurrentAdminEmail);
}

void UBCUtilityWidgetBase::SetLoginInFlight(bool bInFlight)
{
    bLoginInFlight = bInFlight;
    const FText Cancel = FText::FromString(TEXT("CANCEL LOGIN"));
    if (UTextBlock* Label = FindButtonLabel(LoginButtonW)) Label->SetText(bInFlight ? Cancel : LoginLabel);
    if (UTextBlock* Label = FindButtonLabel(ChangeAppButtonW)) Label->SetText(bInFlight ? Cancel : ChangeAppLabel);

    if (bInFlight)
    {
        SetPanelText(HelpTextW, TEXT("Finish signing in in your browser. Press the button again to cancel."));
    }
    else
    {
        ShowAccountView();
    }
}

void UBCUtilityWidgetBase::BeginPortalFlow()
{
    if (!IsOAuthAvailable())
    {
        Notify(TEXT("Portal login requires Unreal Engine 4.24 or later."), true);
        return;
    }

    CurrentServerUrl = GetPortalUrl();
    UE_LOG(LogBCWidget, Log, TEXT("[OAuth] portal %s"), *CurrentServerUrl);
    SetLoginInFlight(true);

    const FBCEditorCredentials Saved = BCEditorCredentialsStore::Load();
    if (Saved.HasLiveAccessToken())
    {
        CurrentAdminEmail = Saved.AdminEmail;
        OAuthClient->ResumeLogin(CurrentServerUrl, Saved.AccessToken, Saved.AccessTokenExpiresAt, Saved.TeamId,
            FBCLoginSuccessDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleLoginSuccess),
            FBCLoginFailureDelegate::CreateUObject(this, &UBCUtilityWidgetBase::HandleResumeFailure));
        return;
    }

    StartLogin(CurrentServerUrl, OAuthClientId);
}

void UBCUtilityWidgetBase::Notify(const FString& Message, bool bError) const
{
    if (bError)
    {
        UE_LOG(LogBCWidget, Warning, TEXT("[Panel] %s"), *Message);
    }
    else
    {
        UE_LOG(LogBCWidget, Log, TEXT("[Panel] %s"), *Message);
    }

    FNotificationInfo Info(FText::FromString(Message));
    Info.ExpireDuration = bError ? 6.0f : 3.0f;
    TSharedPtr<SNotificationItem> Item = FSlateNotificationManager::Get().AddNotification(Info);
    if (Item.IsValid())
    {
        Item->SetCompletionState(bError ? SNotificationItem::CS_Fail : SNotificationItem::CS_Success);
    }
}

FString UBCUtilityWidgetBase::GetPortalUrl() const
{
    if (UseDefaultServerCheckboxW && !UseDefaultServerCheckboxW->IsChecked())
    {
        const FString Custom = StripDispatcher(ServerUrlTextW ? GetPanelText(ServerUrlTextW) : GetConfiguredServerUrl());
        if (!Custom.IsEmpty())
        {
            return Custom;
        }
    }
    return StripDispatcher(DefaultServerUrl);
}

FString UBCUtilityWidgetBase::GetRuntimeServerUrl() const
{
    return GetPortalUrl() + GDispatcherSuffix;
}

void UBCUtilityWidgetBase::PopulateTeams(const TArray<FBCPortalTeam>& InTeams, const FString& SelectTeamId)
{
    CachedTeams = InTeams;
    if (!TeamSelectBoxW)
    {
        return;
    }

    TeamSelectBoxW->ClearOptions();
    int32 SelectIndex = INDEX_NONE;
    for (int32 i = 0; i < CachedTeams.Num(); ++i)
    {
        TeamSelectBoxW->AddOption(CachedTeams[i].TeamName.IsEmpty() ? CachedTeams[i].TeamId : CachedTeams[i].TeamName);
        if (CachedTeams[i].TeamId == SelectTeamId)
        {
            SelectIndex = i;
        }
    }
    if (SelectIndex != INDEX_NONE)
    {
        TeamSelectBoxW->SetSelectedIndex(SelectIndex);
    }
}

void UBCUtilityWidgetBase::PopulateApps(const TArray<FBCPortalApp>& InApps)
{
    CachedApps = InApps;
    if (!AppSelectBoxW)
    {
        RefreshChildRows();
        return;
    }

    AppSelectBoxW->ClearOptions();
    AppSelectBoxW->ClearSelection();
    bAppListHasCreateOption = bAppsLoaded && CreateAppSectionW != nullptr;
    if (bAppListHasCreateOption)
    {
        AppSelectBoxW->AddOption(GCreateAppOption);
    }
    const int32 Offset = bAppListHasCreateOption ? 1 : 0;

    int32 SelectIndex = INDEX_NONE;
    const FString PreferredAppId = SelectedAppId.IsEmpty() ? GetConfiguredAppId() : SelectedAppId;
    for (int32 i = 0; i < CachedApps.Num(); ++i)
    {
        AppSelectBoxW->AddOption(FString::Printf(TEXT("%s (%s)"), *CachedApps[i].AppName, *CachedApps[i].AppId));
        if (CachedApps[i].AppId == PreferredAppId)
        {
            SelectIndex = i + Offset;
        }
    }
    if (bCreateOptionSelected && bAppListHasCreateOption)
    {
        AppSelectBoxW->SetSelectedIndex(0);
    }
    else if (SelectIndex != INDEX_NONE)
    {
        AppSelectBoxW->SetSelectedIndex(SelectIndex);
    }
    RefreshChildRows();
}

void UBCUtilityWidgetBase::HandleLoginClicked()
{
    if (bLoginInFlight)
    {
        CancelLogin();
        SetLoginInFlight(false);
        return;
    }
    BeginPortalFlow();
}

void UBCUtilityWidgetBase::HandleChangeAppClicked()
{
    if (bLoginInFlight)
    {
        CancelLogin();
        SetLoginInFlight(false);
        return;
    }

    if (HasLiveOAuthSession() && CachedTeams.Num() > 0 && HasBuilderApiKey())
    {
        PopulateTeams(CachedTeams, CurrentTeamId);
        ShowPickerView();
        FetchApps(CurrentTeamId);
        return;
    }

    BeginPortalFlow();
}

bool UBCUtilityWidgetBase::HasAuthenticatedSession() const
{
    return HasLiveOAuthSession() || BCEditorCredentialsStore::Load().HasLiveAccessToken();
}

void UBCUtilityWidgetBase::HandleTeamRefreshClicked()
{
    if (!bLoginInFlight)
    {
        BeginPortalFlow();
    }
}

void UBCUtilityWidgetBase::HandleAppRefreshClicked()
{
    if (!CurrentTeamId.IsEmpty() && HasBuilderApiKey())
    {
        FetchApps(CurrentTeamId);
    }
}

void UBCUtilityWidgetBase::HandleSaveClicked()
{
    const FString AppId = GetPanelText(AppIdTextW).TrimStartAndEnd();
    const FString AppSecret = GetPanelText(AppSecretTextW).TrimStartAndEnd();

    FString AppName;
    if (AppId == SelectedAppId)
    {
        AppName = SelectedAppName;
    }
    else if (AppId == GetConfiguredAppId())
    {
        AppName = GetConfiguredAppName();
    }

    const bool bSaveChildren = IsParentAppChecked();
    TArray<FBCStoredChildApp> Children;
    if (bSaveChildren)
    {
        for (const UBCChildAppRowWidget* Row : ChildRows)
        {
            const FBCStoredChildApp& Child = Row->GetChild();
            if (Child.AppId.IsEmpty())
            {
                Notify(TEXT("Pick an app in every child row, remove the empty rows, or untick Parent App."), true);
                return;
            }
            if (Child.AppSecret.IsEmpty())
            {
                Notify(FString::Printf(TEXT("Still loading the secret for child app %s - try again in a moment."), *Child.AppId), true);
                return;
            }
            if (Child.AppId == AppId || Children.ContainsByPredicate([&Child](const FBCStoredChildApp& C) { return C.AppId == Child.AppId; }))
            {
                Notify(FString::Printf(TEXT("Child app %s is the parent or listed twice."), *Child.AppId), true);
                return;
            }
            Children.Add(Child);
        }
        if (Children.Num() == 0)
        {
            Notify(TEXT("Pick at least one child app, or untick Parent App."), true);
            return;
        }
    }

    FString Error;
    if (!ApplySelectedApp(AppId, AppName, AppSecret, GetRuntimeServerUrl(), Error))
    {
        Notify(Error, true);
        return;
    }

    if (bSaveChildren)
    {
        if (!FBCSecureStore::StoreChildren(Children, Error))
        {
            Notify(Error, true);
            return;
        }
    }
    else
    {
        FBCSecureStore::ClearChildren();
    }

    Notify(FString::Printf(TEXT("Saved brainCloud app %s."), *(AppName.IsEmpty() ? AppId : AppName)), false);
    LoadFieldsFromStore();
    ShowAccountView();
}

void UBCUtilityWidgetBase::HandleAppIdClearClicked()
{
    SetPanelText(AppIdTextW, FString());
}

void UBCUtilityWidgetBase::HandleAppSecretClearClicked()
{
    SetPanelText(AppSecretTextW, FString());
}

void UBCUtilityWidgetBase::HandleSecretToggleClicked()
{
    bSecretVisible = !bSecretVisible;
    SetPanelPassword(AppSecretTextW, !bSecretVisible);
    SetPanelText(AppSecretToggleTextW, bSecretVisible ? TEXT("Hide") : TEXT("Show"));
}

void UBCUtilityWidgetBase::HandlePortalLinkClicked() { FPlatformProcess::LaunchURL(*GetPortalWebsiteUrl(), nullptr, nullptr); }

FString UBCUtilityWidgetBase::GetPortalWebsiteUrl() const
{
    const FString ServerUrl = GetPortalUrl();
    const int32 HostStart = ServerUrl.Find(TEXT("://"));
    if (HostStart == INDEX_NONE)
    {
        return ServerUrl;
    }
    const FString Scheme = ServerUrl.Left(HostStart + 3);
    const FString Host = ServerUrl.RightChop(HostStart + 3);
    if (Host.StartsWith(TEXT("api."), ESearchCase::IgnoreCase))
    {
        return Scheme + TEXT("portal.") + Host.RightChop(4);
    }
    return ServerUrl;
}
void UBCUtilityWidgetBase::HandleApiReferenceLinkClicked() { FPlatformProcess::LaunchURL(*ApiReferenceLinkUrl, nullptr, nullptr); }
void UBCUtilityWidgetBase::HandleDocsLinkClicked() { FPlatformProcess::LaunchURL(*DocsLinkUrl, nullptr, nullptr); }
void UBCUtilityWidgetBase::HandleUnrealSdkLinkClicked() { FPlatformProcess::LaunchURL(*UnrealSdkLinkUrl, nullptr, nullptr); }

void UBCUtilityWidgetBase::HandleTeamSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !TeamSelectBoxW)
    {
        return;
    }
    const int32 Index = TeamSelectBoxW->GetSelectedIndex();
    if (!CachedTeams.IsValidIndex(Index) || CachedTeams[Index].TeamId == CurrentTeamId)
    {
        return;
    }
    bAppsLoaded = false;
    ResetChildApps();
    PopulateApps(TArray<FBCPortalApp>());
    SwitchTeam(CachedTeams[Index].TeamId);
}

void UBCUtilityWidgetBase::HandleAppSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || !AppSelectBoxW)
    {
        return;
    }
    const int32 Index = AppSelectBoxW->GetSelectedIndex();
    if (bAppListHasCreateOption && Index == 0)
    {
        bCreateOptionSelected = true;
        UpdateCreateAppView();
        return;
    }

    const int32 AppIndex = Index - (bAppListHasCreateOption ? 1 : 0);
    if (!CachedApps.IsValidIndex(AppIndex))
    {
        return;
    }
    bCreateOptionSelected = false;
    SelectedAppId = CachedApps[AppIndex].AppId;
    SelectedAppName = CachedApps[AppIndex].AppName;
    FetchAppSecret(CurrentTeamId, SelectedAppId);
    LoadChildrenOf(SelectedAppId);
    UpdateCreateAppView();
}

void UBCUtilityWidgetBase::HandleUseDefaultServerChanged(bool bIsChecked)
{
    SetPanelShown(ServerUrlSectionW, !bIsChecked);
}

TArray<FString> UBCUtilityWidgetBase::GetConfiguredChildAppIds() const
{
    return FBCSecureStore::ResolveChildAppIds();
}

bool UBCUtilityWidgetBase::IsParentApp() const
{
    if (bAppsLoaded)
    {
        return GetChildAppsOf(SelectedAppId).Num() > 0;
    }
    return ChildRows.Num() > 0;
}

bool UBCUtilityWidgetBase::IsParentAppChecked() const
{
    return ParentAppCheckboxW && ParentAppCheckboxW->IsChecked();
}

void UBCUtilityWidgetBase::UpdateChildVisibility()
{
    const bool bShowParentOption = !bCreateOptionSelected && IsParentApp();
    const bool bParent = bShowParentOption && IsParentAppChecked();
    if (ParentAppCheckboxSectionW)
    {
        SetPanelShown(ParentAppCheckboxSectionW, bShowParentOption);
    }
    else
    {
        SetPanelShown(ParentAppCheckboxW, bShowParentOption);
    }
    if (ChildAppsSectionW)
    {
        SetPanelShown(ChildAppsSectionW, bParent);
    }
    else
    {
        SetPanelShown(ChildAppRowsContainerW, bParent);
        SetPanelShown(AddChildAppButtonW, bParent);
    }

    if (bParent && ChildRows.Num() == 0)
    {
        const TArray<FBCPortalApp> Children = GetChildAppsOf(SelectedAppId);
        if (bAppsLoaded && Children.Num() > 0)
        {
            for (const FBCPortalApp& App : Children)
            {
                FBCStoredChildApp Child;
                Child.AppId = App.AppId;
                Child.AppName = App.AppName;
                FetchChildSecret(AddChildRow(Child));
            }
        }
        else
        {
            AddChildRow(FBCStoredChildApp());
        }
    }
    UpdateChildAppsDisplay();
}

void UBCUtilityWidgetBase::UpdateChildAppsDisplay()
{
    TArray<FString> Lines;
    if (IsParentAppChecked())
    {
        for (const UBCChildAppRowWidget* Row : ChildRows)
        {
            const FBCStoredChildApp& Child = Row->GetChild();
            if (!Child.AppId.IsEmpty())
            {
                Lines.Add(Child.AppName.IsEmpty() ? Child.AppId : FString::Printf(TEXT("%s (%s)"), *Child.AppName, *Child.AppId));
            }
        }
    }
    SetPanelText(ChildAppsTextW, FString::Join(Lines, TEXT("\n")));
    SetPanelShown(ChildAppsCredentialsSectionW, Lines.Num() > 0);
}

UBCChildAppRowWidget* UBCUtilityWidgetBase::AddChildRow(const FBCStoredChildApp& Child)
{
    if (!ChildAppRowClass || !ChildAppRowsContainerW)
    {
        UE_LOG(LogBCWidget, Warning, TEXT("[Panel] child app rows need ChildAppRowsContainer and a ChildAppRowClass (/BCClient/EditorUtility/BCChildAppRow)"));
        return nullptr;
    }

    UBCChildAppRowWidget* Row = CreateWidget<UBCChildAppRowWidget>(this, ChildAppRowClass);
    if (!Row)
    {
        return nullptr;
    }
    Row->Setup(this, Child);
    ChildAppRowsContainerW->AddChild(Row);
    ChildRows.Add(Row);
    RefreshChildRows();
    return Row;
}

void UBCUtilityWidgetBase::ClearChildRows()
{
    for (UBCChildAppRowWidget* Row : ChildRows)
    {
        if (Row)
        {
            Row->RemoveFromParent();
        }
    }
    ChildRows.Reset();
}

void UBCUtilityWidgetBase::ResetChildApps()
{
    ClearChildRows();
    if (ParentAppCheckboxW)
    {
        ParentAppCheckboxW->SetIsChecked(false);
    }
    UpdateChildVisibility();
}

void UBCUtilityWidgetBase::RefreshChildRows(const UBCChildAppRowWidget* Skip)
{
    const TArray<FBCPortalApp> Children = GetChildAppsOf(SelectedAppId);
    for (int32 RowIndex = 0; RowIndex < ChildRows.Num(); ++RowIndex)
    {
        UBCChildAppRowWidget* Row = ChildRows[RowIndex];
        const FString OwnId = Row->GetChild().AppId;

        TArray<FBCPortalApp> Available = Children.FilterByPredicate([&](const FBCPortalApp& App)
        {
            if (App.AppId == OwnId)
            {
                return true;
            }
            return !ChildRows.ContainsByPredicate([&App, Row](const UBCChildAppRowWidget* Other)
            {
                return Other != Row && Other->GetChild().AppId == App.AppId;
            });
        });

        if (Row != Skip)
        {
            Row->SetOptions(Available, bAppsLoaded);
        }
        Row->SetRemovable(RowIndex > 0);
    }
    UpdateChildControls();
    UpdateChildAppsDisplay();
}

TArray<FBCPortalApp> UBCUtilityWidgetBase::GetChildAppsOf(const FString& ParentAppId) const
{
    if (ParentAppId.IsEmpty())
    {
        return TArray<FBCPortalApp>();
    }
    return CachedApps.FilterByPredicate([&ParentAppId](const FBCPortalApp& App) { return App.ParentAppId == ParentAppId; });
}

void UBCUtilityWidgetBase::LoadChildrenOf(const FString& ParentAppId)
{
    ClearChildRows();
    const bool bHasChildren = GetChildAppsOf(ParentAppId).Num() > 0;
    if (ParentAppCheckboxW)
    {
        ParentAppCheckboxW->SetIsChecked(bHasChildren);
    }
    UpdateChildVisibility();
    RefreshChildRows();
}

void UBCUtilityWidgetBase::UpdateChildControls()
{
    if (!bAppsLoaded)
    {
        if (AddChildAppButtonW)
        {
            AddChildAppButtonW->SetIsEnabled(false);
        }
        return;
    }

    const TArray<FBCPortalApp> Children = GetChildAppsOf(SelectedAppId);
    if (Children.Num() == 0 && (ChildRows.Num() > 0 || IsParentAppChecked()))
    {
        if (ParentAppCheckboxW)
        {
            ParentAppCheckboxW->SetIsChecked(false);
        }
        ClearChildRows();
        UpdateChildVisibility();
    }

    int32 Unpicked = 0;
    for (const FBCPortalApp& App : Children)
    {
        if (!ChildRows.ContainsByPredicate([&App](const UBCChildAppRowWidget* Row) { return Row->GetChild().AppId == App.AppId; }))
        {
            ++Unpicked;
        }
    }
    const bool bHasEmptyRow = ChildRows.ContainsByPredicate([](const UBCChildAppRowWidget* Row) { return Row->GetChild().AppId.IsEmpty(); });
    if (AddChildAppButtonW)
    {
        AddChildAppButtonW->SetIsEnabled(Unpicked > 0 && !bHasEmptyRow);
    }
}

void UBCUtilityWidgetBase::HandleParentAppChanged(bool bIsChecked)
{
    UpdateChildVisibility();
}

void UBCUtilityWidgetBase::HandleDebugLoggingChanged(bool bIsChecked)
{
    FBCSecureStore::StoreDebugLogging(bIsChecked);
}

void UBCUtilityWidgetBase::HandleAddChildAppClicked()
{
    AddChildRow(FBCStoredChildApp());
}

void UBCUtilityWidgetBase::OnChildRowSelected(UBCChildAppRowWidget* Row)
{
    RefreshChildRows(Row);
    FetchChildSecret(Row);
}

void UBCUtilityWidgetBase::FetchChildSecret(UBCChildAppRowWidget* Row)
{
    if (!Row || Row->GetChild().AppId.IsEmpty())
    {
        return;
    }

    const FString AppId = Row->GetChild().AppId;
    const FString AppName = Row->GetChild().AppName;
    TWeakObjectPtr<UBCChildAppRowWidget> WeakRow(Row);
    BuilderApiClient->GetAppSecret(CurrentTeamId, AppId, FBCApiResponseDelegate::CreateWeakLambda(this,
        [this, WeakRow, AppId, AppName](bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage)
        {
            if (!bSuccess)
            {
                ReportRequestFailure(ErrorMessage);
                return;
            }
            const FString Secret = ParseAppSecret(Json);
            if (Secret.IsEmpty())
            {
                Notify(FString::Printf(TEXT("The portal did not return a secret for child app %s."), *AppId), true);
                return;
            }
            if (WeakRow.IsValid())
            {
                WeakRow->SetSecret(AppId, Secret);
            }
        }));
}

void UBCUtilityWidgetBase::UpdateCreateAppView()
{
    SetPanelShown(CreateAppSectionW, bCreateOptionSelected);
    if (bCreateOptionSelected)
    {
        BuildPlatformCheckboxes();
        const bool bTemplate = CreateFromTemplateCheckboxW && CreateFromTemplateCheckboxW->IsChecked();
        SetPanelShown(TemplateAppSectionW, bTemplate);
        SetPanelShown(PlatformsSectionW, !bTemplate);
        if (bTemplate && CachedTemplates.Num() == 0 && HasBuilderApiKey())
        {
            FetchTemplateApps();
        }
    }
    UpdateChildVisibility();
}

void UBCUtilityWidgetBase::BuildPlatformCheckboxes()
{
    if (!PlatformsContainerW || PlatformCheckboxes.Num() > 0)
    {
        return;
    }

    const int32 Columns = 3;
    UUniformGridPanel* Grid = Cast<UUniformGridPanel>(PlatformsContainerW);
    PlatformsContainerW->ClearChildren();
    if (!Grid)
    {
        Grid = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass());
        PlatformsContainerW->AddChild(Grid);
    }
    Grid->SetSlotPadding(FMargin(4.0f, 3.0f));

    int32 Index = 0;
    for (const FBCPlatform& Platform : GPlatforms)
    {
        UHorizontalBox* Content = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
        USpacer* Gap = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
        Gap->SetSize(FVector2D(6.0f, 1.0f));
        Content->AddChildToHorizontalBox(Gap);

        UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
        Label->SetText(FText::FromString(Platform.Name));
        Label->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 10));
        if (UHorizontalBoxSlot* LabelSlot = Content->AddChildToHorizontalBox(Label))
        {
            LabelSlot->SetVerticalAlignment(VAlign_Center);
        }

        UCheckBox* Box = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass());
        Box->AddChild(Content);
        Box->SetIsChecked(Platform.bDefault);
        if (UUniformGridSlot* Cell = Grid->AddChildToUniformGrid(Box, Index / Columns, Index % Columns))
        {
            Cell->SetHorizontalAlignment(HAlign_Left);
            Cell->SetVerticalAlignment(VAlign_Center);
        }
        PlatformCheckboxes.Add(Box);
        PlatformIds.Add(Platform.Id);
        ++Index;
    }
}

void UBCUtilityWidgetBase::PopulateTemplates(const TArray<FBCPortalTemplateApp>& InTemplates)
{
    CachedTemplates = InTemplates;
    CachedTemplates.Sort([](const FBCPortalTemplateApp& A, const FBCPortalTemplateApp& B) { return A.AppName < B.AppName; });
    if (!TemplateAppSelectBoxW)
    {
        return;
    }
    TemplateAppSelectBoxW->ClearOptions();
    for (const FBCPortalTemplateApp& Template : CachedTemplates)
    {
        TemplateAppSelectBoxW->AddOption(Template.AppName);
    }
    if (CachedTemplates.Num() > 0)
    {
        TemplateAppSelectBoxW->SetSelectedIndex(0);
    }
}

void UBCUtilityWidgetBase::HandleCreateFromTemplateChanged(bool bIsChecked)
{
    UpdateCreateAppView();
}

void UBCUtilityWidgetBase::SetCreateInFlight(bool bInFlight)
{
    bCreateInFlight = bInFlight;
    if (CreateAppButtonW)
    {
        CreateAppButtonW->SetIsEnabled(!bInFlight);
    }
}

void UBCUtilityWidgetBase::HandleCreateAppClicked()
{
    if (bCreateInFlight)
    {
        return;
    }
    if (CurrentTeamId.IsEmpty() || !HasBuilderApiKey())
    {
        Notify(TEXT("Log in and pick a team before creating an app."), true);
        return;
    }

    const FString AppName = GetPanelText(NewAppNameW).TrimStartAndEnd();
    if (AppName.IsEmpty())
    {
        Notify(TEXT("Enter a name for the new app."), true);
        return;
    }

    FString TemplateAppId;
    TArray<FString> Platforms;
    if (CreateFromTemplateCheckboxW && CreateFromTemplateCheckboxW->IsChecked())
    {
        const int32 TemplateIndex = TemplateAppSelectBoxW ? TemplateAppSelectBoxW->GetSelectedIndex() : INDEX_NONE;
        if (!CachedTemplates.IsValidIndex(TemplateIndex))
        {
            Notify(TEXT("Select a template app."), true);
            return;
        }
        TemplateAppId = CachedTemplates[TemplateIndex].AppId;
    }
    else
    {
        for (int32 i = 0; i < PlatformCheckboxes.Num(); ++i)
        {
            if (PlatformCheckboxes[i] && PlatformCheckboxes[i]->IsChecked())
            {
                Platforms.Add(PlatformIds[i]);
            }
        }
        if (Platforms.Num() == 0)
        {
            Notify(TEXT("Select at least one platform."), true);
            return;
        }
    }

    SetCreateInFlight(true);
    CreateApp(CurrentTeamId, AppName, TemplateAppId, Platforms, true);
}

void UBCUtilityWidgetBase::FinishCreatedApp(const FString& AppId, const FString& AppName, const FString& AppSecret)
{
    FBCPortalApp NewApp;
    NewApp.AppId = AppId;
    NewApp.AppName = AppName;
    CachedApps.Add(NewApp);
    CachedApps.Sort([](const FBCPortalApp& A, const FBCPortalApp& B) { return A.AppName < B.AppName; });

    bCreateOptionSelected = false;
    SelectedAppId = AppId;
    SelectedAppName = AppName;
    ClearChildRows();
    if (ParentAppCheckboxW)
    {
        ParentAppCheckboxW->SetIsChecked(false);
    }
    PopulateApps(CachedApps);
    UpdateCreateAppView();
    SetPanelText(NewAppNameW, FString());

    SetPanelText(AppIdTextW, AppId);
    SetPanelText(AppNameTextW, AppName);
    SetPanelText(AppSecretTextW, AppSecret);

    FString Error;
    if (!ApplySelectedApp(AppId, AppName, AppSecret, GetRuntimeServerUrl(), Error))
    {
        Notify(Error, true);
        return;
    }
    FBCSecureStore::ClearChildren();
    Notify(FString::Printf(TEXT("Created and saved brainCloud app %s."), *AppName), false);
}

void UBCUtilityWidgetBase::OnChildRowRefresh(UBCChildAppRowWidget* Row)
{
    if (CurrentTeamId.IsEmpty() || !HasBuilderApiKey())
    {
        Notify(TEXT("Log in and pick a team to load its apps."), true);
        return;
    }
    FetchApps(CurrentTeamId);
}

void UBCUtilityWidgetBase::OnChildRowRemove(UBCChildAppRowWidget* Row)
{
    if (ChildRows.Num() <= 1)
    {
        Row->Clear();
    }
    else
    {
        Row->RemoveFromParent();
        ChildRows.Remove(Row);
    }
    RefreshChildRows();
}
