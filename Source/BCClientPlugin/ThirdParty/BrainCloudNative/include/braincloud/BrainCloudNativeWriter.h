#pragma once

#include "BrainCloudNative.h"

#include <string>
#include <utility>
#include <vector>

namespace braincloud {

/**
 * An app's id, name and secret, for writing a configuration.
 */
struct AppCredentials {
    std::string appId;
    std::string appName;
    std::string secret;
};

/**
 * An app's id and name, as read back from a configuration.
 */
struct AppName {
    std::string appId;
    std::string appName;
};

/**
 * A brainCloud portal session.
 */
struct PortalSession {
    /** The session's access token. */
    std::string accessToken;
    /** The account's email. */
    std::string email;
    /** The selected team id. */
    std::string teamId;
    /** The selected team's API key. */
    std::string apiKey;
    /** When the access token expires, in Unix seconds. */
    int64_t expiresAtUnix = 0;
};

/**
 * Writes app configurations and stores the editor's portal session.
 *
 * For editor tools only. Do not link this library into a game.
 */
class BCNATIVE_API ConfigWriter {
public:
    /**
     * Writes an app configuration, replacing the file. An app with an empty secret keeps
     * the secret already stored for its app id.
     *
     * @param path The file to write
     * @param app The app
     * @param childApps The app's child apps
     * @param serverUrl The server url
     * @param appVersion The app version
     * @return True if the file was written
     */
    static bool WriteConfig(const std::string& path, const AppCredentials& app,
                            const std::vector<AppCredentials>& childApps,
                            const std::string& serverUrl, const std::string& appVersion);

    /**
     * Updates the server url and app version, keeping the stored apps.
     *
     * @param path The file to update
     * @param serverUrl The server url
     * @param appVersion The app version
     * @return True if the file was updated
     */
    static bool WriteSettings(const std::string& path, const std::string& serverUrl,
                              const std::string& appVersion);

    /**
     * Reads the app and child app ids and names from a configuration.
     *
     * @param path The file to read
     * @param appOut The app
     * @param childAppsOut The child apps
     * @return True if the file was read
     */
    static bool ReadAppNames(const std::string& path, AppName& appOut,
                             std::vector<AppName>& childAppsOut);

    /**
     * Reads the secret of the app or one of its child apps, for an editor to show on request.
     *
     * @param path The file to read
     * @param appId The app id or a child app id
     * @param secretOut The app's secret
     * @return True if the app is in the configuration
     */
    static bool ReadSecret(const std::string& path, const std::string& appId, std::string& secretOut);

    /**
     * Stores plain text app credentials in a configuration.
     *
     * @param legacyIniPath The plain text credentials file
     * @param path The configuration file to write
     * @param serverUrl The server url
     * @param appVersion The app version
     * @param scrubPlaintext True to remove the app secret from the plain text file
     * @return True if the configuration was written
     */
    static bool ImportFromPlaintext(const std::string& legacyIniPath, const std::string& path,
                                    const std::string& serverUrl, const std::string& appVersion,
                                    bool scrubPlaintext);

    /**
     * Writes a brainCloud portal session to a file.
     *
     * @param path The file to write
     * @param session The session
     * @return True if the file was written
     */
    static bool SaveSession(const std::string& path, const PortalSession& session);

    /**
     * Reads a brainCloud portal session from a file.
     *
     * @param path The file to read
     * @param sessionOut The session
     * @return True if the file holds a session
     */
    static bool ResolveSession(const std::string& path, PortalSession& sessionOut);

    /**
     * Returns true if the session in a file has an access token that has not expired.
     * A session saved without an expiry is not active.
     *
     * @param path The file to read
     */
    static bool IsSessionActive(const std::string& path);

    /**
     * Removes the session from a file.
     *
     * @param path The file to update
     * @return True if the session was removed
     */
    static bool ClearSession(const std::string& path);
};

/**
 * An HTTP request for the caller to send.
 */
struct PortalRequest {
    /** GET or POST. */
    std::string method;
    std::string url;
    /** Header names and values. */
    std::vector<std::pair<std::string, std::string>> headers;
    std::string body;
};

/**
 * A team of the logged in brainCloud portal account.
 */
struct PortalTeam {
    std::string teamId;
    std::string teamName;
    bool apiEnabled = false;
};

/**
 * A logged in brainCloud portal account.
 */
struct PortalAccount {
    std::string email;
    std::string adminId;
    /** The current team's Builder API key. */
    std::string apiKey;
    std::string currentTeamId;
    std::string accessToken;
    /** When the access token expires, in Unix seconds. */
    int64_t accessTokenExpiresAt = 0;
    std::vector<PortalTeam> teams;
};

/**
 * Logs in to the brainCloud portal through the browser.
 *
 * The caller opens the URL returned by Begin, serves RedirectPath on localhost at
 * kRedirectPort, passes the redirect's query parameters to HandleRedirect, then sends each
 * request from NextRequest and passes its response to HandleResponse until IsComplete.
 *
 * For editor tools only. Do not link this library into a game.
 */
class BCNATIVE_API PortalAuth {
public:
    /** The localhost port the browser is redirected to. */
    static const int kRedirectPort = 45678;

    PortalAuth();
    ~PortalAuth();

    PortalAuth(const PortalAuth&) = delete;
    PortalAuth& operator=(const PortalAuth&) = delete;

    /**
     * Returns the path the browser is redirected to.
     */
    static const char* RedirectPath();

    /**
     * Starts a login through the browser.
     *
     * @param serverUrl The portal url
     * @param clientId The OAuth client id
     * @return The url to open in the browser, or empty if the login could not start
     */
    std::string Begin(const std::string& serverUrl, const std::string& clientId);

    /**
     * Continues a login with a saved access token, without the browser.
     *
     * @param serverUrl The portal url
     * @param accessToken The saved access token
     * @param expiresAtUnix When the access token expires, in Unix seconds
     * @param preferredTeamId The team to select if the account is still in it, or empty
     */
    void Resume(const std::string& serverUrl, const std::string& accessToken, int64_t expiresAtUnix,
                const std::string& preferredTeamId);

    /**
     * Handles the browser's redirect.
     *
     * @param code The redirect's code parameter
     * @param state The redirect's state parameter
     * @param errorOut Receives the reason when the redirect is rejected
     * @return True if the login can continue
     */
    bool HandleRedirect(const std::string& code, const std::string& state, std::string& errorOut);

    /**
     * Returns the next request to send.
     *
     * @param requestOut Receives the request
     * @return False if no request is waiting
     */
    bool NextRequest(PortalRequest& requestOut) const;

    /**
     * Handles the response to the request from NextRequest.
     *
     * @param statusCode The HTTP status code, or 0 if the request could not be sent
     * @param body The response body
     * @param errorOut Receives the reason when the login fails
     * @return True if the login can continue
     */
    bool HandleResponse(int statusCode, const std::string& body, std::string& errorOut);

    /**
     * Returns true once the login has finished.
     */
    bool IsComplete() const;

    /**
     * Returns the account. Complete once IsComplete returns true; the access token is set
     * as soon as it is received.
     */
    const PortalAccount& Account() const;

    /**
     * Returns the request that creates a Builder API key for a team.
     *
     * @param serverUrl The portal url
     * @param accessToken The access token
     * @param teamId The team
     */
    static PortalRequest ApiKeyRequest(const std::string& serverUrl, const std::string& accessToken,
                                       const std::string& teamId);

    /**
     * Reads the Builder API key from the response to ApiKeyRequest.
     *
     * @param statusCode The HTTP status code, or 0 if the request could not be sent
     * @param body The response body
     * @param apiKeyOut Receives the API key
     * @param errorOut Receives the reason when there is no API key
     * @return True if the response holds an API key
     */
    static bool ParseApiKey(int statusCode, const std::string& body, std::string& apiKeyOut,
                            std::string& errorOut);

private:
    struct Impl;
    Impl* _impl;
};

}
