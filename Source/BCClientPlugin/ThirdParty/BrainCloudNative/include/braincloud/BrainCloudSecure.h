#pragma once

#include <cstdint>
#include <string>
#include <vector>

#if defined(_WIN32) && defined(BCSECURE_SHARED)
#if defined(BCSECURE_BUILDING)
#define BCSECURE_API __declspec(dllexport)
#else
#define BCSECURE_API __declspec(dllimport)
#endif
#elif defined(BCSECURE_SHARED)
#define BCSECURE_API __attribute__((visibility("default")))
#else
#define BCSECURE_API
#endif

namespace braincloud {

/**
 * The state of the stored app credentials.
 */
enum class CredentialState {
    /** No credentials are stored. */
    None,
    /** Credentials are stored in plain text. */
    Legacy,
    /** Credentials are stored and can be read. */
    Secure,
    /** Credentials are stored but cannot be read. */
    Invalid,
};

/**
 * Encodes an app secret.
 *
 * @param secret The app secret
 * @return The encoded values, SlotCount() of them
 */
BCSECURE_API std::vector<std::string> EncodeSecret(const std::string& secret);

/**
 * Decodes an app secret.
 *
 * @param slots The values returned by EncodeSecret
 * @return The app secret, or an empty string if the values cannot be decoded
 */
BCSECURE_API std::string DecodeSecret(const std::vector<std::string>& slots);

/**
 * Encodes a single value.
 *
 * @param value The value to encode
 * @return The encoded value
 */
BCSECURE_API std::string EncodeSimpleValue(const std::string& value);

/**
 * Decodes a single value.
 *
 * @param encoded The value returned by EncodeSimpleValue
 * @return The value, or an empty string if it cannot be decoded
 */
BCSECURE_API std::string DecodeSimpleValue(const std::string& encoded);

/**
 * Signs a request payload with an encoded app secret.
 *
 * @param slots The values returned by EncodeSecret
 * @param payload The request payload
 * @return The signature, or an empty string if the secret cannot be decoded
 */
BCSECURE_API std::string SignWithSlots(const std::vector<std::string>& slots,
                                       const std::string& payload);

/**
 * Returns the number of values EncodeSecret returns.
 */
BCSECURE_API int SlotCount();

/**
 * Reads app credentials from a file and signs requests with them.
 */
class BCSECURE_API BrainCloudSecure {
public:
    BrainCloudSecure();
    ~BrainCloudSecure();

    BrainCloudSecure(const BrainCloudSecure&) = delete;
    BrainCloudSecure& operator=(const BrainCloudSecure&) = delete;

    /**
     * Reads the app credentials from a file.
     *
     * @param path The file to read
     * @return True if the credentials were read
     */
    bool Resolve(const std::string& path);

    /**
     * Returns true if Resolve succeeded.
     */
    bool IsResolved() const;

    /**
     * Returns the app id, or an empty string if Resolve has not succeeded.
     */
    const std::string& GetAppId() const;

    /**
     * Signs a request payload with the app secret.
     *
     * @param payload The request payload
     * @return The signature, or an empty string if Resolve has not succeeded
     */
    std::string Sign(const std::vector<uint8_t>& payload) const;
    std::string Sign(const std::string& payload) const;

    /**
     * Writes app credentials to a file.
     *
     * @param path The file to write
     * @param appId The app's id
     * @param secret The app's secret
     * @return True if the file was written
     */
    static bool PrepareConfig(const std::string& path, const std::string& appId,
                              const std::string& secret);

    /**
     * Writes the app name to a file.
     *
     * @param path The file to write
     * @param appId The app's id
     * @param appName The app's name
     * @return True if the file was written
     */
    static bool SaveAppName(const std::string& path, const std::string& appId,
                            const std::string& appName);

    /**
     * Reads the app id and app name from a file.
     *
     * @param path The file to read
     * @param appIdOut The app's id
     * @param appNameOut The app's name
     * @return True if the file was read
     */
    static bool ResolveAppName(const std::string& path, std::string& appIdOut,
                               std::string& appNameOut);

    /**
     * Returns the state of the stored app credentials.
     *
     * @param securePath The credentials file, or empty to skip it
     * @param legacyIniPath The plain text credentials file, or empty to skip it
     */
    static CredentialState DetectState(const std::string& securePath,
                                       const std::string& legacyIniPath);

    /**
     * Stores plain text app credentials in the credentials file.
     *
     * @param legacyIniPath The plain text credentials file
     * @param securePath The credentials file to write
     * @param scrubPlaintext True to remove the app secret from the plain text file
     * @return True if the credentials were stored
     */
    static bool ImportFromPlaintext(const std::string& legacyIniPath,
                                    const std::string& securePath,
                                    bool scrubPlaintext);

    /**
     * Writes a brainCloud portal session to a file.
     *
     * @param path The file to write
     * @param accessToken The session's access token
     * @param email The account's email
     * @param teamId The selected team id
     * @param expiresAtUnix When the access token expires, in Unix seconds
     * @return True if the file was written
     */
    static bool SaveSession(const std::string& path, const std::string& accessToken,
                            const std::string& email, const std::string& teamId,
                            int64_t expiresAtUnix);

    /**
     * Reads a brainCloud portal session from a file.
     *
     * @param path The file to read
     * @param accessTokenOut The session's access token
     * @param emailOut The account's email
     * @param teamIdOut The selected team id
     * @param expiresAtUnixOut When the access token expires, in Unix seconds
     * @return True if the file was read
     */
    static bool ResolveSession(const std::string& path, std::string& accessTokenOut,
                               std::string& emailOut, std::string& teamIdOut,
                               int64_t& expiresAtUnixOut);

    /**
     * Returns true if the session in a file has an access token that has not expired.
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

    /**
     * Returns this library's version.
     */
    static const char* Version();

private:
    struct Impl;
    Impl* _impl;
};

}
