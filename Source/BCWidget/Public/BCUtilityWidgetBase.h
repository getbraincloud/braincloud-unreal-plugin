// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BCPortalTypes.h"
#include "BCPortalOAuthClient.h"
#include "BCBuilderApiClient.h"
#include "BCSecureStore.h"
#include "Components/SlateWrapperTypes.h"
#include "BCUtilityWidgetBase.generated.h"

class UButton;
class UCheckBox;
class UComboBoxString;
class UWidget;
class UPanelWidget;
class UBCChildAppRowWidget;

/**
 * Base class for the brainCloud editor panel (Tools > brainCloud).
 */
UCLASS(Blueprintable)
class UBCUtilityWidgetBase : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    UBCUtilityWidgetBase(const FObjectInitializer& ObjectInitializer);

    /**
     * The OAuth client id used to log in to the brainCloud portal.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    FString OAuthClientId = TEXT("unreal_plugin");

    /**
     * The widget used for each child app row. Defaults to /BCClient/EditorUtility/BCChildAppRow.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    TSubclassOf<UBCChildAppRowWidget> ChildAppRowClass;

    void OnChildRowSelected(UBCChildAppRowWidget* Row);
    void OnChildRowRefresh(UBCChildAppRowWidget* Row);
    void OnChildRowRemove(UBCChildAppRowWidget* Row);

    /**
     * The default brainCloud server url.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    FString DefaultServerUrl = TEXT("https://api.braincloudservers.com");

    /**
     * The url opened by the API Reference link.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    FString ApiReferenceLinkUrl = TEXT("https://docs.braincloudservers.com/api/");

    /**
     * The url opened by the Docs link.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    FString DocsLinkUrl = TEXT("https://docs.braincloudservers.com/");

    /**
     * The url opened by the Unreal Engine SDK link.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BrainCloud|Panel")
    FString UnrealSdkLinkUrl = TEXT("https://github.com/getbraincloud/braincloud-unreal-plugin-src");

    /**
     * Returns true if logging in to the brainCloud portal is supported by this engine version.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    bool IsOAuthAvailable() const;

    /**
     * Opens the browser to log in to the brainCloud portal.
     *
     * @param ServerUrl The url to the brainCloud server
     * @param ClientId The OAuth client id
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void StartLogin(const FString& ServerUrl, const FString& ClientId);

    /**
     * Cancels a login in progress.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void CancelLogin();

    /**
     * Logs out of the brainCloud portal.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void Logout();

    /**
     * Restores the last portal account used in this project.
     *
     * @param ServerUrl The url to the brainCloud server
     * @param OutAdminEmail The account's email
     * @param OutTeamId The account's selected team id
     * @return True if an account was restored
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    bool TryRestoreRememberedAccount(const FString& ServerUrl, FString& OutAdminEmail, FString& OutTeamId);

    /**
     * Returns the selected team id.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    FString GetCurrentTeamId() const { return CurrentTeamId; }

    /**
     * Requests the account's teams. Results are sent to OnTeamsReceived.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchTeams();

    /**
     * Selects a team and requests its apps.
     *
     * @param TeamId The team id
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void SwitchTeam(const FString& TeamId);

    /**
     * Requests the apps of a team. Results are sent to OnAppsReceived.
     *
     * @param TeamId The team id
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchApps(const FString& TeamId);

    /**
     * Requests the secret of an app. Results are sent to OnAppSecretReceived.
     *
     * @param TeamId The team id
     * @param AppId The app's id
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchAppSecret(const FString& TeamId, const FString& AppId);

    /**
     * Requests the available template apps. Results are sent to OnTemplateAppsReceived.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void FetchTemplateApps();

    /**
     * Creates a new app in a team. Results are sent to OnAppCreated.
     *
     * @param TeamId The team id
     * @param AppName The app's name
     * @param TemplateAppId The template app id, or empty for none
     * @param SupportedPlatforms The app's supported platforms
     * @param bGamificationEnabled True to enable gamification
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void CreateApp(const FString& TeamId, const FString& AppName, const FString& TemplateAppId,
        const TArray<FString>& SupportedPlatforms, bool bGamificationEnabled);

    /**
     * Reads the stored credentials and sends their state to OnCredentialStateChanged.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud|Credentials")
    void RefreshCredentialState();

    /**
     * Returns the state of the stored credentials.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud|Credentials")
    EBCCredentialState GetCredentialState() const { return CredentialState; }

    /**
     * Returns the stored app id.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud|Credentials")
    FString GetConfiguredAppId() const;

    /**
     * Returns the stored app name.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud|Credentials")
    FString GetConfiguredAppName() const;

    /**
     * Returns the stored url to the brainCloud server.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud|Credentials")
    FString GetConfiguredServerUrl() const;

    /**
     * Returns the stored child app ids.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud|Credentials")
    TArray<FString> GetConfiguredChildAppIds() const;

    /**
     * Returns the stored app id and app secret.
     *
     * @param OutAppId The app's id
     * @param OutAppSecret The app's secret
     * @return True if credentials are stored
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud|Credentials")
    bool RevealStoredCredentials(FString& OutAppId, FString& OutAppSecret);

    /**
     * Stores credentials that are in plain text in the current format.
     *
     * @param OutError The reason the credentials could not be stored
     * @return True if the credentials were stored
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud|Credentials")
    bool MigrateLegacyCredentials(FString& OutError);

    /**
     * Stores the credentials of an app.
     *
     * @param AppId The app's id
     * @param AppName The app's name
     * @param AppSecret The app's secret
     * @param ServerUrl The url to the brainCloud server
     * @param OutError The reason the credentials could not be stored
     * @return True if the credentials were stored
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud|Credentials")
    bool ApplySelectedApp(const FString& AppId, const FString& AppName, const FString& AppSecret,
        const FString& ServerUrl, FString& OutError);

    /**
     * Removes all stored credentials.
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud|Credentials")
    void ClearStoredCredentials();

    /**
     * Shows the team and app selection, logging in first if needed.
     *
     * @param ServerUrl The url to the brainCloud server
     */
    UFUNCTION(BlueprintCallable, Category = "BrainCloud")
    void BeginChangeApp(const FString& ServerUrl);

    /**
     * Returns true if logged in to the brainCloud portal.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    bool HasLiveOAuthSession() const;

    /**
     * Returns true if apps can be requested for the selected team.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    bool HasBuilderApiKey() const { return !CurrentApiKey.IsEmpty(); }

    /**
     * Returns the email of the logged in portal account.
     */
    UFUNCTION(BlueprintPure, Category = "BrainCloud")
    FString GetCurrentAdminEmail() const { return CurrentAdminEmail; }

    /**
     * Called when the state of the stored credentials changes.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud|Credentials")
    void OnCredentialStateChanged(EBCCredentialState State, const FString& AppId, const FString& AppName);

    /**
     * Called when credentials in plain text were stored in the current format.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud|Credentials")
    void OnCredentialsMigrated(const FString& AppId);

    /**
     * Called when app credentials were stored.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud|Credentials")
    void OnCredentialsSaved(const FString& AppId, const FString& AppName);

    /**
     * Called when a login to the brainCloud portal is required.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnNeedsLogin();

    /**
     * Called when the login to the brainCloud portal succeeds.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnLoginSucceeded(const FString& Email);

    /**
     * Called when the login to the brainCloud portal fails.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnLoginFailed(const FString& ErrorMessage);

    /**
     * Called with the account's teams.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnTeamsReceived(const TArray<FBCPortalTeam>& Teams);

    /**
     * Called with the apps of a team.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppsReceived(const TArray<FBCPortalApp>& Apps);

    /**
     * Called with the secret of an app.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppSecretReceived(const FString& AppId, const FString& AppSecret);

    /**
     * Called with the available template apps.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnTemplateAppsReceived(const TArray<FBCPortalTemplateApp>& TemplateApps);

    /**
     * Called when a new app was created.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnAppCreated(const FString& AppId, const FString& AppName, const FString& AppSecret);

    /**
     * Called when a request to the brainCloud portal fails.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "BrainCloud")
    void OnRequestFailed(const FString& ErrorMessage);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindPanelWidgets();
    void LoadFieldsFromStore();
    void TryResumeSession();
    void ShowAccountView();
    void ShowPickerView();
    void SetLoginInFlight(bool bInFlight);
    void BeginPortalFlow();
    bool HasAuthenticatedSession() const;
    void Notify(const FString& Message, bool bError) const;
    FString GetPortalUrl() const;
    FString GetRuntimeServerUrl() const;
    FString GetPortalWebsiteUrl() const;
    void PopulateTeams(const TArray<FBCPortalTeam>& InTeams, const FString& SelectTeamId);
    void PopulateApps(const TArray<FBCPortalApp>& InApps);
    void UpdateChildVisibility();
    bool IsParentAppChecked() const;
    bool IsParentApp() const;
    UBCChildAppRowWidget* AddChildRow(const FBCStoredChildApp& Child);
    void ClearChildRows();
    void ResetChildApps();
    void RefreshChildRows(const UBCChildAppRowWidget* Skip = nullptr);
    TArray<FBCPortalApp> GetChildAppsOf(const FString& ParentAppId) const;
    void LoadChildrenOf(const FString& ParentAppId);
    void FetchChildSecret(UBCChildAppRowWidget* Row);
    void UpdateChildControls();
    void UpdateChildAppsDisplay();
    void UpdateCreateAppView();
    void BuildPlatformCheckboxes();
    void PopulateTemplates(const TArray<FBCPortalTemplateApp>& InTemplates);
    void FinishCreatedApp(const FString& AppId, const FString& AppName, const FString& AppSecret);
    void SetCreateInFlight(bool bInFlight);

    UFUNCTION() void HandleLoginClicked();
    UFUNCTION() void HandleChangeAppClicked();
    UFUNCTION() void HandleTeamRefreshClicked();
    UFUNCTION() void HandleAppRefreshClicked();
    UFUNCTION() void HandleSaveClicked();
    UFUNCTION() void HandleAppIdClearClicked();
    UFUNCTION() void HandleAppSecretClearClicked();
    UFUNCTION() void HandleSecretToggleClicked();
    UFUNCTION() void HandlePortalLinkClicked();
    UFUNCTION() void HandleApiReferenceLinkClicked();
    UFUNCTION() void HandleDocsLinkClicked();
    UFUNCTION() void HandleUnrealSdkLinkClicked();
    UFUNCTION() void HandleTeamSelected(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleAppSelected(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleUseDefaultServerChanged(bool bIsChecked);
    UFUNCTION() void HandleParentAppChanged(bool bIsChecked);
    UFUNCTION() void HandleAddChildAppClicked();
    UFUNCTION() void HandleDebugLoggingChanged(bool bIsChecked);
    UFUNCTION() void HandleCreateFromTemplateChanged(bool bIsChecked);
    UFUNCTION() void HandleCreateAppClicked();

    UPROPERTY(Transient) UButton* LoginButtonW = nullptr;
    UPROPERTY(Transient) UButton* ChangeAppButtonW = nullptr;
    UPROPERTY(Transient) UButton* TeamRefreshButtonW = nullptr;
    UPROPERTY(Transient) UButton* AppRefreshButtonW = nullptr;
    UPROPERTY(Transient) UButton* SaveButtonW = nullptr;
    UPROPERTY(Transient) UButton* AppIdClearButtonW = nullptr;
    UPROPERTY(Transient) UButton* AppSecretClearButtonW = nullptr;
    UPROPERTY(Transient) UButton* AppSecretShowToggleButtonW = nullptr;
    UPROPERTY(Transient) UComboBoxString* TeamSelectBoxW = nullptr;
    UPROPERTY(Transient) UComboBoxString* AppSelectBoxW = nullptr;
    UPROPERTY(Transient) UCheckBox* UseDefaultServerCheckboxW = nullptr;
    UPROPERTY(Transient) UWidget* AccountSectionW = nullptr;
    UPROPERTY(Transient) UWidget* SelectAppSectionW = nullptr;
    UPROPERTY(Transient) UWidget* ServerUrlSectionW = nullptr;
    UPROPERTY(Transient) UWidget* HelpTextW = nullptr;
    UPROPERTY(Transient) UWidget* AccountDescriptionW = nullptr;
    UPROPERTY(Transient) UWidget* AppIdTextW = nullptr;
    UPROPERTY(Transient) UWidget* AppNameTextW = nullptr;
    UPROPERTY(Transient) UWidget* AppSecretTextW = nullptr;
    UPROPERTY(Transient) UWidget* AppSecretToggleTextW = nullptr;
    UPROPERTY(Transient) UWidget* ServerUrlTextW = nullptr;
    UPROPERTY(Transient) UCheckBox* ParentAppCheckboxW = nullptr;
    UPROPERTY(Transient) UCheckBox* DebugLoggingCheckboxW = nullptr;
    UPROPERTY(Transient) UWidget* ChildAppsSectionW = nullptr;
    UPROPERTY(Transient) UPanelWidget* ChildAppRowsContainerW = nullptr;
    UPROPERTY(Transient) UButton* AddChildAppButtonW = nullptr;
    UPROPERTY(Transient) TArray<UBCChildAppRowWidget*> ChildRows;
    UPROPERTY(Transient) UWidget* ParentAppCheckboxSectionW = nullptr;
    UPROPERTY(Transient) UWidget* ChildAppsCredentialsSectionW = nullptr;
    UPROPERTY(Transient) UWidget* ChildAppsTextW = nullptr;
    UPROPERTY(Transient) UWidget* CreateAppSectionW = nullptr;
    UPROPERTY(Transient) UWidget* NewAppNameW = nullptr;
    UPROPERTY(Transient) UCheckBox* CreateFromTemplateCheckboxW = nullptr;
    UPROPERTY(Transient) UWidget* TemplateAppSectionW = nullptr;
    UPROPERTY(Transient) UComboBoxString* TemplateAppSelectBoxW = nullptr;
    UPROPERTY(Transient) UWidget* PlatformsSectionW = nullptr;
    UPROPERTY(Transient) UPanelWidget* PlatformsContainerW = nullptr;
    UPROPERTY(Transient) UButton* CreateAppButtonW = nullptr;
    UPROPERTY(Transient) TArray<UCheckBox*> PlatformCheckboxes;

    TArray<FBCPortalTeam> CachedTeams;
    TArray<FBCPortalApp> CachedApps;
    FString SelectedAppId;
    FString SelectedAppName;
    bool bAppsLoaded = false;
    bool bAppListHasCreateOption = false;
    bool bCreateOptionSelected = false;
    bool bCreateInFlight = false;
    TArray<FString> PlatformIds;
    TArray<FBCPortalTemplateApp> CachedTemplates;
    bool bLoginInFlight = false;
    bool bSecretVisible = false;
    FText LoginLabel;
    FText ChangeAppLabel;

    void HandleResumeFailure(const FString& ErrorMessage);
    void ReportRequestFailure(const FString& ErrorMessage);

    void HandleLoginSuccess(const FBCLoginResult& Result);
    void HandleLoginFailure(const FString& ErrorMessage);

    void HandleApiKeyRefreshed(const FString& NewApiKey);
    void HandleApiKeyRefreshFailed(const FString& ErrorMessage);

    void HandleTeamsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleTemplateAppsResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleCreateAppResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);
    void HandleCreateAppSecretResponse(bool bSuccess, TSharedPtr<FJsonObject> Json, FString ErrorMessage);

    TSharedPtr<BCPortalOAuthClient> OAuthClient;
    TSharedPtr<BCBuilderApiClient> BuilderApiClient;

    FString CurrentServerUrl;
    FString CurrentAdminEmail;
    FString CurrentTeamId;
    FString CurrentApiKey;

    FString PendingAppSecretAppId;
    FString PendingSwitchTeamId;
    FString PendingCreateAppId;
    FString PendingCreateAppName;

    EBCCredentialState CredentialState = EBCCredentialState::None;
};
