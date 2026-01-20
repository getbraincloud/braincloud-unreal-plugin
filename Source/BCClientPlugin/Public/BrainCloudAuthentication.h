// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once

#include "BCAuthType.h"
#include "BCAuthenticationIds.h"

class BrainCloudClient;
class IServerCallback;
class BCCLIENTPLUGIN_API BrainCloudAuthentication
{
public:
  BrainCloudAuthentication(BrainCloudClient *client);

  /**
		 * Initialize - initializes the identity service with a saved
		 * anonymous installation id and most recently used profile id
		 *
		 * @param in_anonymousId  The anonymous installation id that was generated for this device
		 * @param in_profileId The id of the profile id that was most recently used by the app (on this device)
		 */
  void initialize(const FString &profileId, const FString &anonymousId);

  /**
	* Used to create the anonymous installation id for the brainCloud profile.
	* @returns A unique Anonymous ID
	*/
  FString generateAnonymousId();

  /**
     * Used to clear the saved profile id - to use in cases when the user is
     * attempting to switch to a different game profile.
     */
  void clearSavedProfileId();

  /*
		 * Get server version.
		 */
  void getServerVersion(IServerCallback* callback);

  /**
		 * Authenticate a user anonymously with brainCloud - used for apps that don't want to bother
		 * the user to login, or for users who are sensitive to their privacy
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_forceCreate  Should a new profile be created if it does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
  void authenticateAnonymous(bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user with brainCloud using their Facebook Credentials
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_fbUserId The facebook id of the user
		 * @param in_fbAuthToken The validated token from the Facebook SDK
		 *   (that will be further validated when sent to the bC service)
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
  void authenticateFacebook(const FString &externalId, const FString &authenticationToken, bool forceCreate, IServerCallback *callback);

    /*
     * Authenticate the user with brainCloud using their FacebookLimited Credentials
     *
     * Service Name - Authenticate
     * Service Operation - Authenticate
     *
     * @param externalId The facebookLimited id of the user
     * @param authenticationToken The validated token from the Facebook SDK
     *   (that will be further validated when sent to the bC service)
     * @param forceCreate Should a new profile be created for this user if the account does not exist?
     * @param callback The method to be invoked when the server response is received
     */
  void authenticateFacebookLimited(const FString &externalId, const FString &authenticationToken, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user with brainCloud using their Oculus Credentials
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_oculusUserId The oculus id of the user
		 * @param in_oculusNonce Oculus token from the Oculus SDK
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
  void authenticateOculus(const FString &oculusId, const FString &oculusNonce, bool forceCreate, IServerCallback *callback);

  /*
     * Authenticate the user specifically for Playstation 4 with brainCloud using their psn account id and auth token
     *
     * Service Name - Authenticate
     * Service Operation - Authenticate
     *
     * @param psnAccountId The account id of the user
     * @param psnAuthToken The validated token from the Playstation SDK
     *   (that will be further validated when sent to the bC service)
     * @param forceCreate Should a new profile be created for this user if the account does not exist?
     * @param callback The method to be invoked when the server response is received
     */
  void authenticatePlaystationNetwork(const FString &psnAccountId, const FString &psnAuthToken, bool forceCreate, IServerCallback *callback);

  /*
     * Authenticate the user specifically for Playstation 5 with brainCloud using their psn account id and auth token
     *
     * Service Name - Authenticate
     * Service Operation - Authenticate
     *
     * @param psnAccountId The account id of the user
     * @param psnAuthToken The validated token from the Playstation SDK
     *   (that will be further validated when sent to the bC service)
     * @param forceCreate Should a new profile be created for this user if the account does not exist?
     * @param callback The method to be invoked when the server response is received
     */
  void authenticatePlaystation5(const FString &psnAccountId, const FString &psnAuthToken, bool forceCreate, IServerCallback *callback);


  /*
		 * Authenticate the user using their Game Center id
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_gameCenterId The player's game center id  (use the playerID property from the local GKPlayer object)
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateGameCenter(const FString &gameCenterId, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user with a custom Email and Password.  Note that the client app
		 * is responsible for collecting (and storing) the e-mail and potentially password
		 * (for convenience) in the client data.  For the greatest security,
		 * force the user to re-enter their * password at each login.
		 * (Or at least give them that option).
		 *
		 * Note that the password sent from the client to the server is protected via SSL.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_email  The e-mail address of the user
		 * @param in_password  The password of the user
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */
  void authenticateEmailPassword(const FString &email, const FString &password, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user using a userid and password (without any validation on the userid).
		 * Similar to AuthenticateEmailPassword - except that that method has additional features to
		 * allow for e-mail validation, password resets, etc.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_userId  The User Id of the user
		 * @param in_password  The password of the user
		 * @param forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateUniversal(const FString &userid, const FString &password, bool forceCreate, IServerCallback *callback);

  //UFUNCTION(BlueprintCallable, meta = (HidePin = "callback", DefaultToSelf = "callback"), Category = "BrainCloud|Authentication")
  //  void authenticateUniversal(const FString& userid, const FString& password, bool forceCreate, IServerCallback * callback = nullptr);

  /*
		 * Authenticate the user using a steam userid and session ticket (without any validation on the userid).
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_userId  String representation of 64 bit steam id
		 * @param in_sessionticket  The session ticket of the user (hex encoded)
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateSteam(const FString &userid, const FString &sessionticket, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_googleUserId  String representation of google+ userid (email)
		 * @param in_serverAuthCode  The authentication token derived via the google apis.
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateGoogle(const FString &googleUserId, const FString &serverAuthCode, bool forceCreate, IServerCallback *callback);

    /*
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_googleUserAccountEmail String representation of google+ userid (email)
		 * @param in_IdToken  The authentication token derived via the google apis.
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateGoogleOpenId(const FString &googleUserAccountEmail, const FString &IdToken, bool forceCreate, IServerCallback *callback);

    /*
		 * Authenticate the user using a google userid(email address) and google authentication token.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_appleUserId  String of the apple accounts user Id OR email
		 * @param in_identityToken  The authentication token confirming users identity
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateApple(const FString &appleUserId, const FString &identityToken, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user using a Twitter userid, authentication token, and secret from Twitter.
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_userId  String representation of Twitter userid
		 * @param in_token  The authentication token derived via the Twitter apis.
		 * @param in_secret  The secret given when attempting to link with Twitter
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateTwitter(const FString &userid, const FString &token, const FString &secret, bool forceCreate, IServerCallback *callback);

  /*
		 * Authenticate the user using a Pase userid and authentication token
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_userId String representation of Parse userid
		 * @param in_token The authentication token
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateParse(const FString &userid, const FString &token, bool forceCreate, IServerCallback *callback);

  /*
    * Authenticate the user using a handoffId and a token 
    *
    * Service Name - Authenticate
    * Service Operation - Authenticate
    *
    * @param handoffId braincloud handoff id generated from cloud script
    * @param securityToken The security token entered byt the user
    * @param callback The method to be invoked when the server response is received
    */
  void authenticateHandoff(const FString &handoffId, const FString &securityToken, bool forceCreate, IServerCallback *callback);

    /*
		 * Authenticate the user using a handoffCode
		 *
		 * Service Name - Authenticate
		 * Service Operation - Authenticate
		 *
		 * @param in_handoffCode the code we generate in cloudcode
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateSettopHandoff(const FString &handoffCode, IServerCallback *callback);
  ////////////////////////////////////////////////////////////this/////////////////////////////////////////////////////////////////////

  /**
		 * Authenticate the user via cloud code (which in turn validates the supplied credentials against an external system).
		 * This allows the developer to extend brainCloud authentication to support other backend authentication systems.
		 *
		 * Service Name - Authenticate
		 * Server Operation - Authenticate
		 *
		 * @param in_userId The user id
		 * @param in_token The user token (password etc)
		 * @param in_externalAuthName The name of the cloud script to call for external authentication
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
  void authenticateExternal(const FString &userid, const FString &token, const FString &externalAuthName, bool forceCreate, IServerCallback *callback);


  /**
   * A generic Authenticate method that translates to the same as calling a specific one, except it takes an extraJson
   * that will be passed along to pre- or post- hooks.
   *
   * Service Name - Authenticate
   * Service Operation - Authenticate
   *
   * @param authenticationType Universal, Email, Facebook, etc
   * @param ids Auth IDs structure
   * @param forceCreate Should a new profile be created for this user if the account does not exist?
   * @param extraJson Additional to piggyback along with the call, to be picked up by pre- or post- hooks. Leave empty string for no extraJson.
   * @param callback The method to be invoked when the server response is received
   */
  void authenticateAdvanced(EBCAuthType authenticationType, const FAuthenticationIds &ids, bool forceCreate, const FString &extraJson, IServerCallback * callback = NULL);

  /**
		 * Authenticate the user for Ultra.
		 *
		 * Service Name - Authenticate
		 * Server Operation - Authenticate
		 *
		 * @param in_ultraUsername it's what the user uses to log into the Ultra endpoint initially
		 * @param in_ultraIdToken The "id_token" taken from Ultra's JWT.
		 * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
		 * @param in_callback The method to be invoked when the server response is received
		 */
   void authenticateUltra(const FString &in_ultraUsername,const FString &in_ultraIdToken, bool in_forceCreate, IServerCallback * in_callback = NULL);
  
  /**
   * Authenticate the user using their Nintendo account id and an auth token
   *
   * Service Name - Authenticate
   * Service Operation - Authenticate
   *
   * @param in_accountId The user's Nintendo account id
   * @param in_authToken The user's Nintendo auth token
   * @param in_forceCreate Should a new profile be created for this user if the account does not exist?
   * @param in_callback The method to be invoked when the server response is received
   */
   void authenticateNintendo(const FString &in_accountId,const FString &in_authToken, bool in_forceCreate, IServerCallback * in_callback = NULL);

    /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - Authenticate
		 * Operation - ResetEmailPassword
		 *
		 * @param in_externalId The email address to send the reset email to.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
  void resetEmailPassword(const FString &in_email, IServerCallback *in_callback);

  /**
		 * Reset Email password with service parameters - Sends a password reset email to
		 * the specified address
		 *
		 * Service Name - Authenticate
		 * Operation - ResetEmailPasswordAdvanced
		 *
		 * @param appId the applicationId
		 * @param in_emailAddress The email address to send the reset email to.
		 * @param in_serviceParams - parameters to send to the email service. See documentation for
		 * full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
  void resetEmailPasswordAdvanced(const FString &in_emailAddress, const FString &in_serviceParams, IServerCallback *in_callback);
  
  /**
		 * Reset Email password - Sends a password reset email to the specified address
		 *
		 * Service Name - Authenticate
		 * Operation - ResetEmailPassword
		 *
		 * @param in_externalId The email address to send the reset email to.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
  void resetEmailPasswordWithExpiry(const FString &in_email, int32 in_tokenTtlInMinutes, IServerCallback *in_callback);

    /**
		 * Reset Email password with service parameters - Sends a password reset email to
		 * the specified address
		 *
		 * Service Name - Authenticate
		 * Operation - ResetEmailPasswordAdvanced
		 *
		 * @param appId the applicationId
		 * @param in_emailAddress The email address to send the reset email to.
		 * @param in_serviceParams - parameters to send to the email service. See documentation for
		 * full list. http://getbraincloud.com/apidocs/apiref/#capi-mail
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 * Note the follow error reason codes:
		 *
		 * SECURITY_ERROR (40209) - If the email address cannot be found.
		 */
  void resetEmailPasswordAdvancedWithExpiry(const FString &in_emailAddress, const FString &in_serviceParams, int32 in_tokenTtlInMinutes, IServerCallback *in_callback);

    /**
		 * Resets Universal ID password
		 *
		 * Service Name - Authenticate
		 * Operation - ResetUniversalIdPassword
		 *
		 * @param appId the applicationId
		 * @param in_universalId the universal Id in question
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */ 
  void resetUniversalIdPassword(const FString &in_universalId, IServerCallback *in_callback);

      /**
		 * Advanced Universal ID password reset using templates
		 *
		 * Service Name - Authenticate
		 * Operation - ResetUniversalIdPassword
		 *
		 * @param appId the applicationId
		 * @param in_universalId the universal Id in question
		 * @param in_serviceParams - parameters to send to the email service.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */ 
  void resetUniversalIdPasswordAdvanced(const FString &in_universalId, const FString &in_serviceParams, IServerCallback *in_callback);

    /**
		 * Resets Universal ID password
		 *
		 * Service Name - Authenticate
		 * Operation - ResetUniversalIdPassword
		 *
		 * @param appId the applicationId
		 * @param in_universalId the universal Id in question
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */ 
  void resetUniversalIdPasswordWithExpiry(const FString &in_universalId, int32 in_tokenTtlInMinutes, IServerCallback *in_callback);

      /**
		 * Advanced Universal ID password reset using templates
		 *
		 * Service Name - Authenticate
		 * Operation - ResetUniversalIdPassword
		 *
		 * @param appId the applicationId
		 * @param in_universalId the universal Id in question
		 * @param in_serviceParams - parameters to send to the email service.
		 * @param in_callback The method to be invoked when the server response is received
		 *
		 */ 
  void resetUniversalIdPasswordAdvancedWithExpiry(const FString &in_universalId, const FString &in_serviceParams, int32 in_tokenTtlInMinutes, IServerCallback *in_callback);


  const FString &getAnonymousId() const;
  const FString &getProfileId() const;
  void setAnonymousId(const FString &anonymousId);
  void setProfileId(const FString &profileId);

  bool CompressResponses = true;

private:
  BrainCloudClient *_client = nullptr;

  FString _anonymousId;
  FString _profileId;

  void authenticate(const FString &externalId, const FString &authenticationToken, EBCAuthType authenticationType, const FString &externalAuthName, bool forceCreate, const FString &in_extraJson, IServerCallback *callback);
};