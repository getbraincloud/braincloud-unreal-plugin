// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#include "ServiceOperation.h"
#include "BCClientPluginPrivatePCH.h"

ServiceOperation::ServiceOperation(const FString &operation)
{
	_value = operation;
}

const ServiceOperation ServiceOperation::None = ServiceOperation(TEXT("NONE"));

const ServiceOperation ServiceOperation::Authenticate = ServiceOperation(TEXT("AUTHENTICATE"));
const ServiceOperation ServiceOperation::Attach = ServiceOperation(TEXT("ATTACH"));
const ServiceOperation ServiceOperation::Merge = ServiceOperation(TEXT("MERGE"));
const ServiceOperation ServiceOperation::Detach = ServiceOperation(TEXT("DETACH"));
const ServiceOperation ServiceOperation::GetServerVersion = ServiceOperation(TEXT("GET_SERVER_VERSION"));
const ServiceOperation ServiceOperation::ResetEmailPassword = ServiceOperation(TEXT("RESET_EMAIL_PASSWORD"));
const ServiceOperation ServiceOperation::ResetEmailPasswordAdvanced = ServiceOperation(TEXT("RESET_EMAIL_PASSWORD_ADVANCED"));
const ServiceOperation ServiceOperation::ResetEmailPasswordWithExpiry = ServiceOperation(TEXT("RESET_EMAIL_PASSWORD_WITH_EXPIRY"));
const ServiceOperation ServiceOperation::ResetEmailPasswordAdvancedWithExpiry = ServiceOperation(TEXT("RESET_EMAIL_PASSWORD_ADVANCED_WITH_EXPIRY"));
const ServiceOperation ServiceOperation::ResetUniversalIdPassword = ServiceOperation(TEXT("RESET_UNIVERSAL_ID_PASSWORD"));
const ServiceOperation ServiceOperation::ResetUniversalIdPasswordAdvanced = ServiceOperation(TEXT("RESET_UNIVERSAL_ID_PASSWORD_ADVANCED"));
const ServiceOperation ServiceOperation::ResetUniversalIdPasswordWithExpiry = ServiceOperation(TEXT("RESET_UNIVERSAL_ID_PASSWORD_WITH_EXPIRY"));
const ServiceOperation ServiceOperation::ResetUniversalIdPasswordAdvancedWithExpiry = ServiceOperation(TEXT("RESET_UNiVERSAL_ID_PASSWORD_ADVANCED_WITH_EXPIRY"));
const ServiceOperation ServiceOperation::SwitchToChildProfile = ServiceOperation(TEXT("SWITCH_TO_CHILD_PROFILE"));
const ServiceOperation ServiceOperation::SwitchToParentProfile = ServiceOperation(TEXT("SWITCH_TO_PARENT_PROFILE"));
const ServiceOperation ServiceOperation::GetChildProfiles = ServiceOperation(TEXT("GET_CHILD_PROFILES"));
const ServiceOperation ServiceOperation::GetIdentities = ServiceOperation(TEXT("GET_IDENTITIES"));
const ServiceOperation ServiceOperation::GetIdentityStatus = ServiceOperation(TEXT("GET_IDENTITY_STATUS"));
const ServiceOperation ServiceOperation::GetExpiredIdentities = ServiceOperation(TEXT("GET_EXPIRED_IDENTITIES"));
const ServiceOperation ServiceOperation::RefreshIdentity = ServiceOperation(TEXT("REFRESH_IDENTITY"));
const ServiceOperation ServiceOperation::ChangeEmailIdentity = ServiceOperation(TEXT("CHANGE_EMAIL_IDENTITY"));
const ServiceOperation ServiceOperation::AttachParentWithIdentity = ServiceOperation(TEXT("ATTACH_PARENT_WITH_IDENTITY"));
const ServiceOperation ServiceOperation::DetachParent = ServiceOperation(TEXT("DETACH_PARENT"));
const ServiceOperation ServiceOperation::AttachPeerProfile = ServiceOperation(TEXT("ATTACH_PEER_PROFILE"));
const ServiceOperation ServiceOperation::DetachPeer = ServiceOperation(TEXT("DETACH_PEER"));
const ServiceOperation ServiceOperation::GetPeerProfiles = ServiceOperation(TEXT("GET_PEER_PROFILES"));
const ServiceOperation ServiceOperation::AttachNonLoginUniversalId = ServiceOperation(TEXT("ATTACH_NONLOGIN_UNIVERSAL"));
const ServiceOperation ServiceOperation::UpdateUniversalIdLogin = ServiceOperation(TEXT("UPDATE_UNIVERSAL_LOGIN"));

const ServiceOperation ServiceOperation::Create = ServiceOperation(TEXT("CREATE"));
const ServiceOperation ServiceOperation::CreateWithIndexedId = ServiceOperation(TEXT("CREATE_WITH_INDEXED_ID"));
const ServiceOperation ServiceOperation::Reset = ServiceOperation(TEXT("RESET"));
const ServiceOperation ServiceOperation::Read = ServiceOperation(TEXT("READ"));
const ServiceOperation ServiceOperation::ReadSingleton = ServiceOperation(TEXT("READ_SINGLETON"));
const ServiceOperation ServiceOperation::ReadByType = ServiceOperation(TEXT("READ_BY_TYPE"));
const ServiceOperation ServiceOperation::Verify = ServiceOperation(TEXT("VERIFY"));
const ServiceOperation ServiceOperation::ReadShared = ServiceOperation(TEXT("READ_SHARED"));
const ServiceOperation ServiceOperation::ReadSharedEntity = ServiceOperation(TEXT("READ_SHARED_ENTITY"));

// push notification
const ServiceOperation ServiceOperation::Deregister = ServiceOperation(TEXT("DEREGISTER"));
const ServiceOperation ServiceOperation::DeregisterAll = ServiceOperation(TEXT("DEREGISTER_ALL"));
const ServiceOperation ServiceOperation::Register = ServiceOperation(TEXT("REGISTER"));
const ServiceOperation ServiceOperation::SendSimple = ServiceOperation(TEXT("SEND_SIMPLE"));
const ServiceOperation ServiceOperation::SendRich = ServiceOperation(TEXT("SEND_RICH"));
const ServiceOperation ServiceOperation::SendRaw = ServiceOperation(TEXT("SEND_RAW"));
const ServiceOperation ServiceOperation::SendRawBatch = ServiceOperation(TEXT("SEND_RAW_BATCH"));
const ServiceOperation ServiceOperation::SendRawToGroup = ServiceOperation(TEXT("SEND_RAW_TO_GROUP"));
const ServiceOperation ServiceOperation::SendTemplatedToGroup = ServiceOperation(TEXT("SEND_TEMPLATED_TO_GROUP"));
const ServiceOperation ServiceOperation::SendNormalizedToGroup = ServiceOperation(TEXT("SEND_NORMALIZED_TO_GROUP"));
const ServiceOperation ServiceOperation::SendNormalized = ServiceOperation(TEXT("SEND_NORMALIZED"));
const ServiceOperation ServiceOperation::SendNormalizedBatch = ServiceOperation(TEXT("SEND_NORMALIZED_BATCH"));
const ServiceOperation ServiceOperation::ScheduleNormalizedNotifcation = ServiceOperation(TEXT("SCHEDULE_NORMALIZED_NOTIFICATION"));
const ServiceOperation ServiceOperation::ScheduledRichNotifcation = ServiceOperation(TEXT("SCHEDULE_RICH_NOTIFICATION"));
const ServiceOperation ServiceOperation::ScheduledRawNotifcation = ServiceOperation(TEXT("SCHEDULE_RAW_NOTIFICATION"));

const ServiceOperation ServiceOperation::Trigger = ServiceOperation(TEXT("TRIGGER"));
const ServiceOperation ServiceOperation::TriggerMultiple = ServiceOperation(TEXT("TRIGGER_MULTIPLE"));
const ServiceOperation ServiceOperation::TriggerGrantExperience = ServiceOperation(TEXT("TRIGGER_GRANT_EXPERIENCE"));

const ServiceOperation ServiceOperation::FullReset = ServiceOperation(TEXT("FULL_PLAYER_RESET"));
const ServiceOperation ServiceOperation::DataReset = ServiceOperation(TEXT("GAME_DATA_RESET"));
const ServiceOperation ServiceOperation::SubmitCrashReport = ServiceOperation(TEXT("SEND_CRASH_REPORT"));

const ServiceOperation ServiceOperation::Update = ServiceOperation(TEXT("UPDATE"));
const ServiceOperation ServiceOperation::UpdatePartial = ServiceOperation(TEXT("UPDATE_PARTIAL"));
const ServiceOperation ServiceOperation::UpdateShared = ServiceOperation(TEXT("UPDATE_SHARED"));
const ServiceOperation ServiceOperation::UpdateSingleton = ServiceOperation(TEXT("UPDATE_SINGLETON"));
const ServiceOperation ServiceOperation::UpdateAcl = ServiceOperation(TEXT("UPDATE_ACL"));
const ServiceOperation ServiceOperation::UpdateTimeToLive = ServiceOperation(TEXT("UPDATE_TIME_TO_LIVE"));
const ServiceOperation ServiceOperation::Delete = ServiceOperation(TEXT("DELETE"));
const ServiceOperation ServiceOperation::DeleteSingleton = ServiceOperation(TEXT("DELETE_SINGLETON"));
const ServiceOperation ServiceOperation::UpdateSummary = ServiceOperation(TEXT("UPDATE_SUMMARY"));
const ServiceOperation ServiceOperation::UpdateSetMinimum = ServiceOperation(TEXT("UPDATE_SET_MINIMUM"));
const ServiceOperation ServiceOperation::UpdateIncrementToMaximum = ServiceOperation(TEXT("UPDATE_INCREMENT_TO_MAXIMUM"));
const ServiceOperation ServiceOperation::ProcessStatistics = ServiceOperation(TEXT("PROCESS_STATISTICS"));
const ServiceOperation ServiceOperation::UpdateEntityIndexedId = ServiceOperation(TEXT("UPDATE_INDEXED_ID"));
const ServiceOperation ServiceOperation::UpdateEntityOwnerAndAcl = ServiceOperation(TEXT("UPDATE_ENTITY_OWNER_AND_ACL"));
const ServiceOperation ServiceOperation::MakeSystemEntity = ServiceOperation(TEXT("MAKE_SYSTEM_ENTITY"));

const ServiceOperation ServiceOperation::GetScheduledCloudSripts = ServiceOperation(TEXT("GET_SCHEDULED_CLOUD_SCRIPTS"));
const ServiceOperation ServiceOperation::GetRunningOrQueuedCloudScripts = ServiceOperation(TEXT("GET_RUNNING_OR_QUEUED_CLOUD_SCRIPTS"));

const ServiceOperation ServiceOperation::GetFriendProfileInfoForExternalId = ServiceOperation(TEXT("GET_FRIEND_PROFILE_INFO_FOR_EXTERNAL_ID"));
const ServiceOperation ServiceOperation::GetProfileInfoForCredential = ServiceOperation(TEXT("GET_PROFILE_INFO_FOR_CREDENTIAL"));
const ServiceOperation ServiceOperation::GetProfileInfoForCredentialIfExists = ServiceOperation(TEXT("GET_PROFILE_INFO_FOR_CREDENTIAL_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetProfileInfoForExternalAuthId = ServiceOperation(TEXT("GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID"));
const ServiceOperation ServiceOperation::GetProfileInfoForExternalAuthIdIfExists = ServiceOperation(TEXT("GET_PROFILE_INFO_FOR_EXTERNAL_AUTH_ID_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetExternalIdForProfileId = ServiceOperation(TEXT("GET_EXTERNAL_ID_FOR_PROFILE_ID"));
const ServiceOperation ServiceOperation::ReadFriendEntity = ServiceOperation(TEXT("READ_FRIEND_ENTITY"));
const ServiceOperation ServiceOperation::ReadFriendsEntities = ServiceOperation(TEXT("READ_FRIENDS_ENTITIES"));
const ServiceOperation ServiceOperation::ReadFriendsWithApplication = ServiceOperation(TEXT("READ_FRIENDS_WITH_APPLICATION"));
const ServiceOperation ServiceOperation::ReadFriends = ServiceOperation(TEXT("READ_FRIENDS"));
const ServiceOperation ServiceOperation::ReadFriendsPlayerState = ServiceOperation(TEXT("READ_FRIEND_PLAYER_STATE"));
const ServiceOperation ServiceOperation::UpdateTimeZoneOffset = ServiceOperation(TEXT("UPDATE_TIMEZONE_OFFSET"));
const ServiceOperation ServiceOperation::UpdateLanguageCode = ServiceOperation(TEXT("UPDATE_LANGUAGE_CODE"));
const ServiceOperation ServiceOperation::FindPlayerByName = ServiceOperation(TEXT("FIND_PLAYER_BY_NAME"));
const ServiceOperation ServiceOperation::FindUsersByExactName = ServiceOperation(TEXT("FIND_USERS_BY_EXACT_NAME"));
const ServiceOperation ServiceOperation::FindUsersBySubstrName = ServiceOperation(TEXT("FIND_USERS_BY_SUBSTR_NAME"));
const ServiceOperation ServiceOperation::ListFriends = ServiceOperation(TEXT("LIST_FRIENDS"));
const ServiceOperation ServiceOperation::GetMySocialInfo = ServiceOperation(TEXT("GET_MY_SOCIAL_INFO"));
const ServiceOperation ServiceOperation::AddFriends = ServiceOperation(TEXT("ADD_FRIENDS"));
const ServiceOperation ServiceOperation::AddFriendsFromPlatform = ServiceOperation(TEXT("ADD_FRIENDS_FROM_PLATFORM"));
const ServiceOperation ServiceOperation::GetUsersOnlineStatus = ServiceOperation(TEXT("GET_USERS_ONLINE_STATUS"));
const ServiceOperation ServiceOperation::RemoveFriends = ServiceOperation(TEXT("REMOVE_FRIENDS"));
const ServiceOperation ServiceOperation::FindPlayerByUniversalId = ServiceOperation(TEXT("FIND_PLAYER_BY_UNIVERSAL_ID"));
const ServiceOperation ServiceOperation::FindUserByExactUniversalId = ServiceOperation(TEXT("FIND_USER_BY_EXACT_UNIVERSAL_ID"));
const ServiceOperation ServiceOperation::FindUsersByUniversalIdStartingWith = ServiceOperation(TEXT("FIND_USERS_BY_UNIVERSAL_ID_STARTING_WITH"));
const ServiceOperation ServiceOperation::FindUsersByNameStartingWith = ServiceOperation(TEXT("FIND_USERS_BY_NAME_STARTING_WITH"));
const ServiceOperation ServiceOperation::GetSocialLeaderboard = ServiceOperation(TEXT("GET_SOCIAL_LEADERBOARD"));
const ServiceOperation ServiceOperation::GetSocialLeaderboardIfExists = ServiceOperation(TEXT("GET_SOCIAL_LEADERBOARD_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetSocialLeaderboardByVersion = ServiceOperation(TEXT("GET_SOCIAL_LEADERBOARD_BY_VERSION"));
const ServiceOperation ServiceOperation::GetSocialLeaderboardByVersionIfExists = ServiceOperation(TEXT("GET_SOCIAL_LEADERBOARD_BY_VERSION_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetMultiSocialLeaderboard = ServiceOperation(TEXT("GET_MULTI_SOCIAL_LEADERBOARD"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboard = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardPage = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_PAGE"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardPageIfExists = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_PAGE_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardView = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_VIEW"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardViewIfExists = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_VIEW_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardVersions = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_VERSIONS"));
const ServiceOperation ServiceOperation::GetCompletedTournament = ServiceOperation(TEXT("GET_COMPLETED_TOURNAMENT"));
const ServiceOperation ServiceOperation::RewardTournament = ServiceOperation(TEXT("REWARD_TOURNAMENT"));
const ServiceOperation ServiceOperation::PostScore = ServiceOperation(TEXT("POST_SCORE"));
const ServiceOperation ServiceOperation::PostScoreDynamic = ServiceOperation(TEXT("POST_SCORE_DYNAMIC"));
const ServiceOperation ServiceOperation::GetGroupSocialLeaderboard = ServiceOperation(TEXT("GET_GROUP_SOCIAL_LEADERBOARD"));
const ServiceOperation ServiceOperation::GetGroupSocialLeaderboardByVersion = ServiceOperation(TEXT("GET_GROUP_SOCIAL_LEADERBOARD_BY_VERSION"));
const ServiceOperation ServiceOperation::GetSummaryDataForProfileId = ServiceOperation(TEXT("GET_SUMMARY_DATA_FOR_PROFILE_ID"));
const ServiceOperation ServiceOperation::GetPlayersSocialLeaderboard = ServiceOperation(TEXT("GET_PLAYERS_SOCIAL_LEADERBOARD"));
const ServiceOperation ServiceOperation::GetPlayersSocialLeaderboardIfExists = ServiceOperation(TEXT("GET_PLAYERS_SOCIAL_LEADERBOARD_IF_EXISTS"));
const ServiceOperation ServiceOperation::GetPlayersSocialLeaderboardByVersion = ServiceOperation(TEXT("GET_PLAYERS_SOCIAL_LEADERBOARD_BY_VERSION"));
const ServiceOperation ServiceOperation::GetPlayersSocialLeaderboardByVersionIfExists = ServiceOperation(TEXT("GET_PLAYERS_SOCIAL_LEADERBOARD_BY_VERSION_IF_EXISTS"));
const ServiceOperation ServiceOperation::ListAllLeaderboards = ServiceOperation(TEXT("LIST_ALL_LEADERBOARDS"));
const ServiceOperation ServiceOperation::GetGlobalLeaderboardEntryCount = ServiceOperation(TEXT("GET_GLOBAL_LEADERBOARD_ENTRY_COUNT"));
const ServiceOperation ServiceOperation::RemovePlayerScore = ServiceOperation(TEXT("REMOVE_PLAYER_SCORE"));
const ServiceOperation ServiceOperation::GetPlayerScore = ServiceOperation(TEXT("GET_PLAYER_SCORE"));
const ServiceOperation ServiceOperation::GetPlayerScores = ServiceOperation(TEXT("GET_PLAYER_SCORES"));
const ServiceOperation ServiceOperation::GetPlayerScoresFromLeaderboards = ServiceOperation(TEXT("GET_PLAYER_SCORES_FROM_LEADERBOARDS"));
const ServiceOperation ServiceOperation::PostScoreToGroupLeaderboard = ServiceOperation(TEXT("POST_GROUP_SCORE"));
const ServiceOperation ServiceOperation::PostScoreToDynamicGroupLeaderboard = ServiceOperation(TEXT("POST_GROUP_SCORE_DYNAMIC"));
const ServiceOperation ServiceOperation::PostScoreToDynamicGroupLeaderboardUsingConfig = ServiceOperation(TEXT("POST_GROUP_SCORE_DYNAMIC_USING_CONFIG"));
const ServiceOperation ServiceOperation::RemoveGroupScore = ServiceOperation(TEXT("REMOVE_GROUP_SCORE"));
const ServiceOperation ServiceOperation::GetGroupLeaderboardView = ServiceOperation(TEXT("GET_GROUP_LEADERBOARD_VIEW"));
const ServiceOperation ServiceOperation::PostScoreDynamicUsingConfig = ServiceOperation(TEXT("POST_SCORE_DYNAMIC_USING_CONFIG"));

//Global File
const ServiceOperation ServiceOperation::GetFileInfo = ServiceOperation(TEXT("GET_FILE_INFO"));
const ServiceOperation ServiceOperation::GetFileInfoSimple = ServiceOperation(TEXT("GET_FILE_INFO_SIMPLE"));
const ServiceOperation ServiceOperation::GetGlobalCDNUrl = ServiceOperation(TEXT("GET_GLOBAL_CDN_URL"));
const ServiceOperation ServiceOperation::GetGlobalFileList = ServiceOperation(TEXT("GET_GLOBAL_FILE_LIST"));

const ServiceOperation ServiceOperation::InitThirdParty = ServiceOperation(TEXT("initThirdParty"));
const ServiceOperation ServiceOperation::PostThirdPartyLeaderboardScore = ServiceOperation(TEXT("postThirdPartyLeaderboardScore"));
const ServiceOperation ServiceOperation::IncrementThirdPartyLeaderboardScore = ServiceOperation(TEXT("incrementThirdPartyLeaderboardScore"));
const ServiceOperation ServiceOperation::LaunchAchievementUI = ServiceOperation(TEXT("launchAchievementUI"));
const ServiceOperation ServiceOperation::PostThirdPartyLeaderboardAchievement = ServiceOperation(TEXT("postThirdPartyLeaderboardAchievement"));
const ServiceOperation ServiceOperation::IsThirdPartyAchievementComplete = ServiceOperation(TEXT("isThirdPartyAchievementComplete"));
const ServiceOperation ServiceOperation::ResetThirdPartyAchievements = ServiceOperation(TEXT("resetThirdPartyAchievements"));
const ServiceOperation ServiceOperation::QueryThirdPartyAchievements = ServiceOperation(TEXT("queryThirdPartyAchievements"));

const ServiceOperation ServiceOperation::GetInventory = ServiceOperation(TEXT("GET_INVENTORY"));
const ServiceOperation ServiceOperation::CashInReceipt = ServiceOperation(TEXT("OP_CASH_IN_RECEIPT"));
const ServiceOperation ServiceOperation::AwardVC = ServiceOperation(TEXT("AWARD_VC"));
const ServiceOperation ServiceOperation::ConsumeVC = ServiceOperation(TEXT("CONSUME_VC"));
const ServiceOperation ServiceOperation::GetPlayerVC = ServiceOperation(TEXT("GET_PLAYER_VC"));
const ServiceOperation ServiceOperation::ResetPlayerVC = ServiceOperation(TEXT("RESET_PLAYER_VC"));

const ServiceOperation ServiceOperation::AwardParentVC = ServiceOperation(TEXT("AWARD_PARENT_VC"));
const ServiceOperation ServiceOperation::ConsumeParentVC = ServiceOperation(TEXT("CONSUME_PARENT_VC"));
const ServiceOperation ServiceOperation::GetParentVC = ServiceOperation(TEXT("GET_PARENT_VC"));
const ServiceOperation ServiceOperation::ResetParentVC = ServiceOperation(TEXT("RESET_PARENT_VC"));

const ServiceOperation ServiceOperation::Send = ServiceOperation(TEXT("SEND"));
const ServiceOperation ServiceOperation::SendToProfiles = ServiceOperation(TEXT("SEND_EVENT_TO_PROFILES"));
const ServiceOperation ServiceOperation::UpdateEventData = ServiceOperation(TEXT("UPDATE_EVENT_DATA"));
const ServiceOperation ServiceOperation::UpdateEventDataIfExists = ServiceOperation(TEXT("UPDATE_EVENT_DATA_IF_EXISTS"));
const ServiceOperation ServiceOperation::DeleteSent = ServiceOperation(TEXT("DELETE_SENT"));
const ServiceOperation ServiceOperation::DeleteIncoming = ServiceOperation(TEXT("DELETE_INCOMING"));
const ServiceOperation ServiceOperation::DeleteIncomingEvents = ServiceOperation("DELETE_INCOMING_EVENTS");
const ServiceOperation ServiceOperation::DeleteIncomingEventsOlderThan = ServiceOperation("DELETE_INCOMING_EVENTS_OLDER_THAN");
const ServiceOperation ServiceOperation::DeleteIncomingEventsByTypeOlderThan = ServiceOperation("DELETE_INCOMING_EVENTS_BY_TYPE_OLDER_THAN");
const ServiceOperation ServiceOperation::GetEvents = ServiceOperation(TEXT("GET_EVENTS"));

const ServiceOperation ServiceOperation::UpdateIncrement = ServiceOperation(TEXT("UPDATE_INCREMENT"));
const ServiceOperation ServiceOperation::ReadNextXplevel = ServiceOperation(TEXT("READ_NEXT_XPLEVEL"));
const ServiceOperation ServiceOperation::ReadXpLevels = ServiceOperation(TEXT("READ_XP_LEVELS"));
const ServiceOperation ServiceOperation::SetXpPoints = ServiceOperation(TEXT("SET_XPPOINTS"));
const ServiceOperation ServiceOperation::ReadSubset = ServiceOperation(TEXT("READ_SUBSET"));
const ServiceOperation ServiceOperation::ReadForCategory = ServiceOperation(TEXT("READ_FOR_CATEGORY"));

const ServiceOperation ServiceOperation::Run = ServiceOperation(TEXT("RUN"));
const ServiceOperation ServiceOperation::Tweet = ServiceOperation(TEXT("TWEET"));

const ServiceOperation ServiceOperation::AwardAchievements = ServiceOperation(TEXT("AWARD_ACHIEVEMENTS"));
const ServiceOperation ServiceOperation::ReadAchievements = ServiceOperation(TEXT("READ_ACHIEVEMENTS"));
const ServiceOperation ServiceOperation::ReadAchievedAchievements = ServiceOperation(TEXT("READ_ACHIEVED_ACHIEVEMENTS"));

const ServiceOperation ServiceOperation::SetPlayerRating = ServiceOperation(TEXT("SET_PLAYER_RATING"));
const ServiceOperation ServiceOperation::ResetPlayerRating = ServiceOperation(TEXT("RESET_PLAYER_RATING"));
const ServiceOperation ServiceOperation::IncrementPlayerRating = ServiceOperation(TEXT("INCREMENT_PLAYER_RATING"));
const ServiceOperation ServiceOperation::DecrementPlayerRating = ServiceOperation(TEXT("DECREMENT_PLAYER_RATING"));
const ServiceOperation ServiceOperation::ShieldOn = ServiceOperation(TEXT("SHIELD_ON"));
const ServiceOperation ServiceOperation::ShieldOnFor = ServiceOperation(TEXT("SHIELD_ON_FOR"));
const ServiceOperation ServiceOperation::ShieldOff = ServiceOperation(TEXT("SHIELD_OFF"));
const ServiceOperation ServiceOperation::IncrementShieldOnFor = ServiceOperation(TEXT("INCREMENT_SHIELD_ON_FOR"));
const ServiceOperation ServiceOperation::GetShieldExpiry = ServiceOperation(TEXT("GET_SHIELD_EXPIRY"));
const ServiceOperation ServiceOperation::FindPlayers = ServiceOperation(TEXT("FIND_PLAYERS"));
const ServiceOperation ServiceOperation::FindPlayersUsingFilter = ServiceOperation(TEXT("FIND_PLAYERS_USING_FILTER"));

const ServiceOperation ServiceOperation::SubmitTurn = ServiceOperation(TEXT("SUBMIT_TURN"));
const ServiceOperation ServiceOperation::UpdateMatchSummary = ServiceOperation(TEXT("UPDATE_SUMMARY"));
const ServiceOperation ServiceOperation::UpdateMatchStateCurrentTurn = ServiceOperation(TEXT("UPDATE_MATCH_STATE_CURRENT_TURN"));
const ServiceOperation ServiceOperation::Abandon = ServiceOperation(TEXT("ABANDON"));
const ServiceOperation ServiceOperation::Complete = ServiceOperation(TEXT("COMPLETE"));
const ServiceOperation ServiceOperation::ReadMatch = ServiceOperation(TEXT("READ_MATCH"));
const ServiceOperation ServiceOperation::ReadMatchHistory = ServiceOperation(TEXT("READ_MATCH_HISTORY"));
const ServiceOperation ServiceOperation::FindMatches = ServiceOperation(TEXT("FIND_MATCHES"));
const ServiceOperation ServiceOperation::FindMatchesCompleted = ServiceOperation(TEXT("FIND_MATCHES_COMPLETED"));
const ServiceOperation ServiceOperation::DeleteMatch = ServiceOperation(TEXT("DELETE_MATCH"));

const ServiceOperation ServiceOperation::LastUploadStatus = ServiceOperation(TEXT("LAST_UPLOAD_STATUS"));

const ServiceOperation ServiceOperation::ReadQuests = ServiceOperation(TEXT("READ_QUESTS"));
const ServiceOperation ServiceOperation::ReadCompletedQuests = ServiceOperation(TEXT("READ_COMPLETED_QUESTS"));
const ServiceOperation ServiceOperation::ReadInProgressQuests = ServiceOperation(TEXT("READ_IN_PROGRESS_QUESTS"));
const ServiceOperation ServiceOperation::ReadNotStartedQuests = ServiceOperation(TEXT("READ_NOT_STARTED_QUESTS"));
const ServiceOperation ServiceOperation::ReadQuestsWithStatus = ServiceOperation(TEXT("READ_QUESTS_WITH_STATUS"));
const ServiceOperation ServiceOperation::ReadQuestsWithBasicPercentage = ServiceOperation(TEXT("READ_QUESTS_WITH_BASIC_PERCENTAGE"));
const ServiceOperation ServiceOperation::ReadQuestsWithComplexPercentage = ServiceOperation(TEXT("READ_QUESTS_WITH_COMPLEX_PERCENTAGE"));
const ServiceOperation ServiceOperation::ReadQuestsByCategory = ServiceOperation(TEXT("READ_QUESTS_BY_CATEGORY"));

const ServiceOperation ServiceOperation::ReadGameStatisticsByCategory = ServiceOperation(TEXT("READ_GAME_STATISTICS_BY_CATEGORY"));
const ServiceOperation ServiceOperation::ReadPlayerStatisticsByCategory = ServiceOperation(TEXT("READ_PLAYER_STATISTICS_BY_CATEGORY"));

const ServiceOperation ServiceOperation::ReadMilestones = ServiceOperation(TEXT("READ_MILESTONES"));
const ServiceOperation ServiceOperation::ReadMilestonesByCategory = ServiceOperation(TEXT("READ_MILESTONES_BY_CATEGORY"));
const ServiceOperation ServiceOperation::ResetMilestones = ServiceOperation(TEXT("RESET_MILESTONES"));

const ServiceOperation ServiceOperation::ReadCompletedMilestones = ServiceOperation(TEXT("READ_COMPLETED_MILESTONES"));
const ServiceOperation ServiceOperation::ReadInProgressMilestones = ServiceOperation(TEXT("READ_IN_PROGRESS_MILESTONES"));

const ServiceOperation ServiceOperation::Logout = ServiceOperation(TEXT("LOGOUT"));
const ServiceOperation ServiceOperation::UpdateName = ServiceOperation(TEXT("UPDATE_NAME"));

const ServiceOperation ServiceOperation::StartMatch = ServiceOperation(TEXT("START_MATCH"));
const ServiceOperation ServiceOperation::CancelMatch = ServiceOperation(TEXT("CANCEL_MATCH"));
const ServiceOperation ServiceOperation::CompleteMatch = ServiceOperation(TEXT("COMPLETE_MATCH"));
const ServiceOperation ServiceOperation::EnableMatchMaking = ServiceOperation(TEXT("ENABLE_FOR_MATCH"));
const ServiceOperation ServiceOperation::DisableMatchMaking = ServiceOperation(TEXT("DISABLE_FOR_MATCH"));

const ServiceOperation ServiceOperation::StartStream = ServiceOperation(TEXT("START_STREAM"));
const ServiceOperation ServiceOperation::ReadStream = ServiceOperation(TEXT("READ_STREAM"));
const ServiceOperation ServiceOperation::EndStream = ServiceOperation(TEXT("END_STREAM"));
const ServiceOperation ServiceOperation::DeleteStream = ServiceOperation(TEXT("DELETE_STREAM"));
const ServiceOperation ServiceOperation::AddEvent = ServiceOperation(TEXT("ADD_EVENT"));
const ServiceOperation ServiceOperation::GetStreamSummariesForInitiatingPlayer = ServiceOperation(TEXT("GET_STREAM_SUMMARIES_FOR_INITIATING_PLAYER"));
const ServiceOperation ServiceOperation::GetStreamSummariesForTargetPlayer = ServiceOperation(TEXT("GET_STREAM_SUMMARIES_FOR_TARGET_PLAYER"));
const ServiceOperation ServiceOperation::GetRecentStreamsForInitiatingPlayer = ServiceOperation(TEXT("GET_RECENT_STREAMS_FOR_INITIATING_PLAYER"));
const ServiceOperation ServiceOperation::GetRecentStreamsForTargetPlayer = ServiceOperation(TEXT("GET_RECENT_STREAMS_FOR_TARGET_PLAYER"));
const ServiceOperation ServiceOperation::ProtectStreamUntil = ServiceOperation(TEXT("PROTECT_STREAM_UNTIL"));

const ServiceOperation ServiceOperation::GetUserInfo = ServiceOperation(TEXT("GET_USER_INFO"));

const ServiceOperation ServiceOperation::StartSteamTransaction = ServiceOperation(TEXT("START_STEAM_TRANSACTION"));
const ServiceOperation ServiceOperation::FinalizeSteamTransaction = ServiceOperation(TEXT("FINALIZE_STEAM_TRANSACTION"));
const ServiceOperation ServiceOperation::VerifyMicrosoftReceipt = ServiceOperation(TEXT("VERIFY_MICROSOFT_RECEIPT"));
const ServiceOperation ServiceOperation::ConfirmGooglePlayPurchase = ServiceOperation(TEXT("CONFIRM_GOOGLEPLAY_PURCHASE"));
const ServiceOperation ServiceOperation::EligiblePromotions = ServiceOperation(TEXT("ELIGIBLE_PROMOTIONS"));

const ServiceOperation ServiceOperation::ReadSharedEntitesList = ServiceOperation(TEXT("READ_SHARED_ENTITIES_LIST"));
const ServiceOperation ServiceOperation::GetList = ServiceOperation(TEXT("GET_LIST"));
const ServiceOperation ServiceOperation::GetListByIndexedId = ServiceOperation(TEXT("GET_LIST_BY_INDEXED_ID"));
const ServiceOperation ServiceOperation::GetListCount = ServiceOperation(TEXT("GET_LIST_COUNT"));
const ServiceOperation ServiceOperation::GetPage = ServiceOperation(TEXT("GET_PAGE"));
const ServiceOperation ServiceOperation::GetPageOffset = ServiceOperation(TEXT("GET_PAGE_BY_OFFSET"));
const ServiceOperation ServiceOperation::IncrementUserEntityData = ServiceOperation(TEXT("INCREMENT_USER_ENTITY_DATA"));
const ServiceOperation ServiceOperation::IncrementSharedUserEntityData = ServiceOperation(TEXT("INCREMENT_SHARED_USER_ENTITY_DATA"));
const ServiceOperation ServiceOperation::IncrementGlobalEntityData = ServiceOperation(TEXT("INCREMENT_GLOBAL_ENTITY_DATA"));
const ServiceOperation ServiceOperation::GetRandomEntitiesMatching = ServiceOperation(TEXT("GET_RANDOM_ENTITIES_MATCHING"));
const ServiceOperation ServiceOperation::AbandonMatchWithSummaryData = ServiceOperation(TEXT("ABANDON_MATCH_WITH_SUMMARY_DATA"));
const ServiceOperation ServiceOperation::CompleteMatchWithSummaryData = ServiceOperation(TEXT("COMPLETE_MATCH_WITH_SUMMARY_DATA"));

const ServiceOperation ServiceOperation::AttachBlockchainIdentity = ServiceOperation(TEXT("ATTACH_BLOCKCHAIN_IDENTITY"));
const ServiceOperation ServiceOperation::DetachBlockchainIdentity = ServiceOperation(TEXT("DETACH_BLOCKCHAIN_IDENTITY")); 
const ServiceOperation ServiceOperation::RemoveUserItemFromBlockchain = ServiceOperation(TEXT("REMOVE_USER_ITEM_FROM_BLOCKCHAIN"));

const ServiceOperation ServiceOperation::GetAttributes = ServiceOperation(TEXT("GET_ATTRIBUTES"));
const ServiceOperation ServiceOperation::UpdateAttributes = ServiceOperation(TEXT("UPDATE_ATTRIBUTES"));
const ServiceOperation ServiceOperation::RemoveAttributes = ServiceOperation(TEXT("REMOVE_ATTRIBUTES"));
const ServiceOperation ServiceOperation::UpdatePlayerPicture = ServiceOperation(TEXT("UPDATE_PICTURE_URL"));
const ServiceOperation ServiceOperation::UpdateContactEmail = ServiceOperation(TEXT("UPDATE_CONTACT_EMAIL"));
const ServiceOperation ServiceOperation::ClearUserStatus = ServiceOperation(TEXT("CLEAR_USER_STATUS"));
const ServiceOperation ServiceOperation::ExtendUserStatus = ServiceOperation(TEXT("EXTEND_USER_STATUS"));
const ServiceOperation ServiceOperation::GetUserStatus = ServiceOperation(TEXT("GET_USER_STATUS"));
const ServiceOperation ServiceOperation::SetUserStatus = ServiceOperation(TEXT("SET_USER_STATUS"));

const ServiceOperation ServiceOperation::ReadProperties = ServiceOperation(TEXT("READ_PROPERTIES"));
const ServiceOperation ServiceOperation::ReadSelectedProperties = ServiceOperation(TEXT("READ_SELECTED_PROPERTIES"));
const ServiceOperation ServiceOperation::ReadPropertiesInCategories = ServiceOperation(TEXT("READ_PROPERTIES_IN_CATEGORIES"));

const ServiceOperation ServiceOperation::GetUpdatedFiles = ServiceOperation(TEXT("GET_UPDATED_FILES"));
const ServiceOperation ServiceOperation::GetFileList = ServiceOperation(TEXT("GET_FILE_LIST"));

const ServiceOperation ServiceOperation::ScheduleCloudScript = ServiceOperation(TEXT("SCHEDULE_CLOUD_SCRIPT"));
const ServiceOperation ServiceOperation::RunParentScript = ServiceOperation(TEXT("RUN_PARENT_SCRIPT"));
const ServiceOperation ServiceOperation::CancelScheduledScript = ServiceOperation(TEXT("CANCEL_SCHEDULED_SCRIPT"));
const ServiceOperation ServiceOperation::RunPeerScript = ServiceOperation(TEXT("RUN_PEER_SCRIPT"));
const ServiceOperation ServiceOperation::RunPeerScriptAsync = ServiceOperation(TEXT("RUN_PEER_SCRIPT_ASYNC"));

const ServiceOperation ServiceOperation::RedeemCode = ServiceOperation(TEXT("REDEEM_CODE"));
const ServiceOperation ServiceOperation::GetRedeemedCodes = ServiceOperation(TEXT("GET_REDEEMED_CODES"));

const ServiceOperation ServiceOperation::CustomPageEvent = ServiceOperation(TEXT("CUSTOM_PAGE_EVENT"));
const ServiceOperation ServiceOperation::CustomScreenEvent = ServiceOperation(TEXT("CUSTOM_SCREEN_EVENT"));
const ServiceOperation ServiceOperation::CustomTrackEvent = ServiceOperation(TEXT("CUSTOM_TRACK_EVENT"));

// profanity
const ServiceOperation ServiceOperation::ProfanityCheck = ServiceOperation(TEXT("PROFANITY_CHECK"));
const ServiceOperation ServiceOperation::ProfanityReplaceText = ServiceOperation(TEXT("PROFANITY_REPLACE_TEXT"));
const ServiceOperation ServiceOperation::ProfanityIdenitfyBadWords = ServiceOperation(TEXT("PROFANITY_IDENTIFY_BAD_WORDS"));

// file
const ServiceOperation ServiceOperation::PrepareUserUpload = ServiceOperation(TEXT("PREPARE_USER_UPLOAD"));
const ServiceOperation ServiceOperation::ListUserFiles = ServiceOperation(TEXT("LIST_USER_FILES"));
const ServiceOperation ServiceOperation::DeleteUserFiles = ServiceOperation(TEXT("DELETE_USER_FILES"));
const ServiceOperation ServiceOperation::GetCdnUrl = ServiceOperation(TEXT("GET_CDN_URL"));

// group
const ServiceOperation ServiceOperation::AcceptGroupInvitation = ServiceOperation(TEXT("ACCEPT_GROUP_INVITATION"));
const ServiceOperation ServiceOperation::AddGroupMember = ServiceOperation(TEXT("ADD_GROUP_MEMBER"));
const ServiceOperation ServiceOperation::ApproveGroupJoinRequest = ServiceOperation(TEXT("APPROVE_GROUP_JOIN_REQUEST"));
const ServiceOperation ServiceOperation::AutoJoinGroup = ServiceOperation(TEXT("AUTO_JOIN_GROUP"));
const ServiceOperation ServiceOperation::AutoJoinGroupMulti = ServiceOperation(TEXT("AUTO_JOIN_GROUP_MULTI"));
const ServiceOperation ServiceOperation::CancelGroupInvitation = ServiceOperation(TEXT("CANCEL_GROUP_INVITATION"));
const ServiceOperation ServiceOperation::CreateGroup = ServiceOperation(TEXT("CREATE_GROUP"));
const ServiceOperation ServiceOperation::CreateGroupEntity = ServiceOperation(TEXT("CREATE_GROUP_ENTITY"));
const ServiceOperation ServiceOperation::DeleteGroup = ServiceOperation(TEXT("DELETE_GROUP"));
const ServiceOperation ServiceOperation::DeleteGroupEntity = ServiceOperation(TEXT("DELETE_GROUP_ENTITY"));
const ServiceOperation ServiceOperation::DeleteGroupJoinRequest = ServiceOperation(TEXT("DELETE_GROUP_JOIN_REQUEST"));
const ServiceOperation ServiceOperation::DeleteGroupMemeber = ServiceOperation(TEXT("DELETE_MEMBER_FROM_GROUP"));
const ServiceOperation ServiceOperation::GetMyGroups = ServiceOperation(TEXT("GET_MY_GROUPS"));
const ServiceOperation ServiceOperation::IncrementGroupData = ServiceOperation(TEXT("INCREMENT_GROUP_DATA"));
const ServiceOperation ServiceOperation::IncrementGroupEntityData = ServiceOperation(TEXT("INCREMENT_GROUP_ENTITY_DATA"));
const ServiceOperation ServiceOperation::InviteGroupMember = ServiceOperation(TEXT("INVITE_GROUP_MEMBER"));
const ServiceOperation ServiceOperation::JoinGroup = ServiceOperation(TEXT("JOIN_GROUP"));
const ServiceOperation ServiceOperation::LeaveGroup = ServiceOperation(TEXT("LEAVE_GROUP"));
const ServiceOperation ServiceOperation::ListGroupsPage = ServiceOperation(TEXT("LIST_GROUPS_PAGE"));
const ServiceOperation ServiceOperation::ListGroupsPageByOffset = ServiceOperation(TEXT("LIST_GROUPS_PAGE_BY_OFFSET"));
const ServiceOperation ServiceOperation::ListGroupsWithMember = ServiceOperation(TEXT("LIST_GROUPS_WITH_MEMBER"));
const ServiceOperation ServiceOperation::ReadGroup = ServiceOperation(TEXT("READ_GROUP"));
const ServiceOperation ServiceOperation::ReadGroupData = ServiceOperation(TEXT("READ_GROUP_DATA"));
const ServiceOperation ServiceOperation::ReadGroupEntitiesPage = ServiceOperation(TEXT("READ_GROUP_ENTITIES_PAGE"));
const ServiceOperation ServiceOperation::ReadGroupEntitiesPageByOffset = ServiceOperation(TEXT("READ_GROUP_ENTITIES_PAGE_BY_OFFSET"));
const ServiceOperation ServiceOperation::ReadGroupEntity = ServiceOperation(TEXT("READ_GROUP_ENTITY"));
const ServiceOperation ServiceOperation::ReadGroupMembers = ServiceOperation(TEXT("READ_GROUP_MEMBERS"));
const ServiceOperation ServiceOperation::RejectGroupInvitation = ServiceOperation(TEXT("REJECT_GROUP_INVITATION"));
const ServiceOperation ServiceOperation::RejectGroupJoinRequest = ServiceOperation(TEXT("REJECT_GROUP_JOIN_REQUEST"));
const ServiceOperation ServiceOperation::RemoveGroupMember = ServiceOperation(TEXT("REMOVE_GROUP_MEMBER"));
const ServiceOperation ServiceOperation::UpdateGroupData = ServiceOperation(TEXT("UPDATE_GROUP_DATA"));
const ServiceOperation ServiceOperation::UpdateGroupAcl = ServiceOperation(TEXT("UPDATE_GROUP_ACL"));
const ServiceOperation ServiceOperation::UpdateGroupEntity = ServiceOperation(TEXT("UPDATE_GROUP_ENTITY_DATA"));
const ServiceOperation ServiceOperation::UpdateGroupEntityAcl = ServiceOperation(TEXT("UPDATE_GROUP_ENTITY_ACL"));
const ServiceOperation ServiceOperation::UpdateGroupMember = ServiceOperation(TEXT("UPDATE_GROUP_MEMBER"));
const ServiceOperation ServiceOperation::UpdateGroupName = ServiceOperation(TEXT("UPDATE_GROUP_NAME"));
const ServiceOperation ServiceOperation::SetGroupOpen = ServiceOperation(TEXT("SET_GROUP_OPEN"));
const ServiceOperation ServiceOperation::UpdateGroupSummaryData = ServiceOperation(TEXT("UPDATE_GROUP_SUMMARY_DATA"));
const ServiceOperation ServiceOperation::GetRandomGroupsMatching = ServiceOperation(TEXT("GET_RANDOM_GROUPS_MATCHING"));

//group file
const ServiceOperation ServiceOperation::CheckFilenameExists = ServiceOperation(TEXT("CHECK_FILENAME_EXISTS"));
const ServiceOperation ServiceOperation::CheckFullpathFilenameExists = ServiceOperation(TEXT("CHECK_FULLPATH_FILENAME_EXISTS"));
const ServiceOperation ServiceOperation::CopyGroupFile = ServiceOperation(TEXT("COPY_FILE"));
const ServiceOperation ServiceOperation::DeleteGroupFile = ServiceOperation(TEXT("DELETE_FILE"));
const ServiceOperation ServiceOperation::MoveGroupFile = ServiceOperation(TEXT("MOVE_FILE"));
const ServiceOperation ServiceOperation::MoveUserToGroupFile = ServiceOperation(TEXT("MOVE_USER_TO_GROUP_FILE"));
const ServiceOperation ServiceOperation::UpdateFileInfo = ServiceOperation(TEXT("UPDATE_FILE_INFO"));

//mail
const ServiceOperation ServiceOperation::SendBasicEmail = ServiceOperation(TEXT("SEND_BASIC_EMAIL"));
const ServiceOperation ServiceOperation::SendAdvancedEmail = ServiceOperation(TEXT("SEND_ADVANCED_EMAIL"));
const ServiceOperation ServiceOperation::SendAdvancedEmailByAddress = ServiceOperation(TEXT("SEND_ADVANCED_EMAIL_BY_ADDRESS"));
const ServiceOperation ServiceOperation::SendAdvancedEmailByAddresses = ServiceOperation(TEXT("SEND_ADVANCED_EMAIL_BY_ADDRESSES"));

//presence
const ServiceOperation ServiceOperation::ForcePush = ServiceOperation(TEXT("FORCE_PUSH"));
const ServiceOperation ServiceOperation::GetPresenceOfFriends = ServiceOperation(TEXT("GET_PRESENCE_OF_FRIENDS"));
const ServiceOperation ServiceOperation::GetPresenceOfGroup = ServiceOperation(TEXT("GET_PRESENCE_OF_GROUP"));
const ServiceOperation ServiceOperation::GetPresenceOfUsers = ServiceOperation(TEXT("GET_PRESENCE_OF_USERS"));
const ServiceOperation ServiceOperation::RegisterListenersForFriends = ServiceOperation(TEXT("REGISTER_LISTENERS_FOR_FRIENDS"));
const ServiceOperation ServiceOperation::RegisterListenersForGroup = ServiceOperation(TEXT("REGISTER_LISTENERS_FOR_GROUP"));
const ServiceOperation ServiceOperation::RegisterListenersForProfiles = ServiceOperation(TEXT("REGISTER_LISTENERS_FOR_PROFILES"));
const ServiceOperation ServiceOperation::SetVisibility = ServiceOperation(TEXT("SET_VISIBILITY"));
const ServiceOperation ServiceOperation::StopListening = ServiceOperation(TEXT("STOP_LISTENING"));
const ServiceOperation ServiceOperation::UpdateActivity = ServiceOperation(TEXT("UPDATE_ACTIVITY"));

//tournament
const ServiceOperation ServiceOperation::GetTournamentStatus = ServiceOperation(TEXT("GET_TOURNAMENT_STATUS"));
const ServiceOperation ServiceOperation::GetDivisionInfo = ServiceOperation(TEXT("GET_DIVISION_INFO"));
const ServiceOperation ServiceOperation::GetMyDivisions = ServiceOperation(TEXT("GET_MY_DIVISIONS"));
const ServiceOperation ServiceOperation::JoinTournament = ServiceOperation(TEXT("JOIN_TOURNAMENT"));
const ServiceOperation ServiceOperation::JoinDivision = ServiceOperation(TEXT("JOIN_DIVISION"));
const ServiceOperation ServiceOperation::LeaveTournament = ServiceOperation(TEXT("LEAVE_TOURNAMENT"));
const ServiceOperation ServiceOperation::LeaveDivisionInstance = ServiceOperation(TEXT("LEAVE_DIVISION_INSTANCE"));
const ServiceOperation ServiceOperation::PostTournamentScore = ServiceOperation(TEXT("POST_TOURNAMENT_SCORE"));
const ServiceOperation ServiceOperation::PostTournamentScoreWithResults = ServiceOperation(TEXT("POST_TOURNAMENT_SCORE_WITH_RESULTS"));
const ServiceOperation ServiceOperation::ViewCurrentReward = ServiceOperation(TEXT("VIEW_CURRENT_REWARD"));
const ServiceOperation ServiceOperation::ViewReward = ServiceOperation(TEXT("VIEW_REWARD"));
const ServiceOperation ServiceOperation::ClaimTournamentReward = ServiceOperation(TEXT("CLAIM_TOURNAMENT_REWARD"));

//virtual currency
const ServiceOperation ServiceOperation::GetPlayerCurrency = ServiceOperation(TEXT("GET_PLAYER_VC"));
const ServiceOperation ServiceOperation::GetParentCurrency = ServiceOperation(TEXT("GET_PARENT_VC"));
const ServiceOperation ServiceOperation::GetPeerCurrency = ServiceOperation(TEXT("GET_PEER_VC"));
const ServiceOperation ServiceOperation::AwardVirtualCurrency = ServiceOperation(TEXT("AWARD_VC"));
const ServiceOperation ServiceOperation::ConsumeVirtualCurrency = ServiceOperation(TEXT("CONSUME_VC"));

//App Store
const ServiceOperation ServiceOperation::VerifyPurchase = ServiceOperation(TEXT("VERIFY_PURCHASE"));
const ServiceOperation ServiceOperation::GetEligiblePromotions = ServiceOperation(TEXT("ELIGIBLE_PROMOTIONS"));
const ServiceOperation ServiceOperation::GetSalesInventory = ServiceOperation(TEXT("GET_INVENTORY"));
const ServiceOperation ServiceOperation::StartPurchase = ServiceOperation(TEXT("START_PURCHASE"));
const ServiceOperation ServiceOperation::FinalizePurchase = ServiceOperation(TEXT("FINALIZE_PURCHASE"));
const ServiceOperation ServiceOperation::RefreshPromotions = ServiceOperation(TEXT("REFRESH_PROMOTIONS"));

// rtt
const ServiceOperation ServiceOperation::RequestClientConnection = ServiceOperation(TEXT("REQUEST_CLIENT_CONNECTION"));
const ServiceOperation ServiceOperation::Connect = ServiceOperation(TEXT("CONNECT"));

// chat
const ServiceOperation ServiceOperation::ChannelConnect = ServiceOperation("CHANNEL_CONNECT");
const ServiceOperation ServiceOperation::ChannelDisconnect = ServiceOperation("CHANNEL_DISCONNECT");
const ServiceOperation ServiceOperation::DeleteChatMessage = ServiceOperation("DELETE_CHAT_MESSAGE");
const ServiceOperation ServiceOperation::GetChannelId = ServiceOperation("GET_CHANNEL_ID");
const ServiceOperation ServiceOperation::GetChannelInfo = ServiceOperation("GET_CHANNEL_INFO");
const ServiceOperation ServiceOperation::GetChatMessage = ServiceOperation("GET_CHAT_MESSAGE");
const ServiceOperation ServiceOperation::GetRecentChatMessages = ServiceOperation("GET_RECENT_CHAT_MESSAGES");
const ServiceOperation ServiceOperation::GetSubscribedChannels = ServiceOperation("GET_SUBSCRIBED_CHANNELS");
const ServiceOperation ServiceOperation::PostChatMessage = ServiceOperation("POST_CHAT_MESSAGE");
const ServiceOperation ServiceOperation::PostChatMessageSimple = ServiceOperation("POST_CHAT_MESSAGE_SIMPLE");
const ServiceOperation ServiceOperation::UpdateChatMessage = ServiceOperation("UPDATE_CHAT_MESSAGE");

// messaging
const ServiceOperation ServiceOperation::DeleteMessages = ServiceOperation("DELETE_MESSAGES");
const ServiceOperation ServiceOperation::GetMessageBoxes = ServiceOperation("GET_MESSAGE_BOXES");
const ServiceOperation ServiceOperation::GetMessageCounts = ServiceOperation("GET_MESSAGE_COUNTS");
const ServiceOperation ServiceOperation::GetMessages = ServiceOperation("GET_MESSAGES");
const ServiceOperation ServiceOperation::GetMessagesPage = ServiceOperation("GET_MESSAGES_PAGE");
const ServiceOperation ServiceOperation::GetMessagesPageOffset = ServiceOperation("GET_MESSAGES_PAGE_OFFSET");
const ServiceOperation ServiceOperation::MarkMessagesRead = ServiceOperation("MARK_MESSAGES_READ");
const ServiceOperation ServiceOperation::SEND_MESSAGE = ServiceOperation("SEND_MESSAGE");
const ServiceOperation ServiceOperation::SendMessageSimple = ServiceOperation("SEND_MESSAGE_SIMPLE");

// lobby
const ServiceOperation ServiceOperation::FindLobby = ServiceOperation("FIND_LOBBY");
const ServiceOperation ServiceOperation::FindLobbyWithPingData = ServiceOperation("FIND_LOBBY_WITH_PING_DATA");
const ServiceOperation ServiceOperation::CreateLobby = ServiceOperation("CREATE_LOBBY");
const ServiceOperation ServiceOperation::CreateLobbyWithPingData = ServiceOperation("CREATE_LOBBY_WITH_PING_DATA");
const ServiceOperation ServiceOperation::FindOrCreateLobby = ServiceOperation("FIND_OR_CREATE_LOBBY");
const ServiceOperation ServiceOperation::FindOrCreateLobbyWithPingData = ServiceOperation("FIND_OR_CREATE_LOBBY_WITH_PING_DATA");
const ServiceOperation ServiceOperation::GetLobbyData = ServiceOperation("GET_LOBBY_DATA");
const ServiceOperation ServiceOperation::UpdateReady = ServiceOperation("UPDATE_READY");
const ServiceOperation ServiceOperation::UpdateSettings = ServiceOperation("UPDATE_SETTINGS");
const ServiceOperation ServiceOperation::SwitchTeam = ServiceOperation("SWITCH_TEAM");
const ServiceOperation ServiceOperation::SendSignal = ServiceOperation("SEND_SIGNAL");
const ServiceOperation ServiceOperation::JoinLobby = ServiceOperation("JOIN_LOBBY");
const ServiceOperation ServiceOperation::JoinLobbyWithPingData = ServiceOperation("JOIN_LOBBY_WITH_PING_DATA");
const ServiceOperation ServiceOperation::LeaveLobby = ServiceOperation("LEAVE_LOBBY");
const ServiceOperation ServiceOperation::RemoveMember = ServiceOperation("REMOVE_MEMBER");
const ServiceOperation ServiceOperation::CancelFindRequest = ServiceOperation("CANCEL_FIND_REQUEST");
const ServiceOperation ServiceOperation::GetRegionsForLobbies = ServiceOperation("GET_REGIONS_FOR_LOBBIES");
const ServiceOperation ServiceOperation::PingData = ServiceOperation("PING_DATA");	// not an official API, but need it for proper calback
const ServiceOperation ServiceOperation::GetLobbyInstances = ServiceOperation("GET_LOBBY_INSTANCES");
const ServiceOperation ServiceOperation::GetLobbyInstancesWithPingData = ServiceOperation("GET_LOBBY_INSTANCES_WITH_PING_DATA");

// user inventory management
const ServiceOperation ServiceOperation::AwardUserItem = ServiceOperation("AWARD_USER_ITEM");
const ServiceOperation ServiceOperation::DropUserItem = ServiceOperation("DROP_USER_ITEM");
const ServiceOperation ServiceOperation::GetUserItemsPage = ServiceOperation("GET_USER_ITEMS_PAGE");
const ServiceOperation ServiceOperation::GetUserItemsPageOffset = ServiceOperation("GET_USER_ITEMS_PAGE_OFFSET");
const ServiceOperation ServiceOperation::GetUserItem = ServiceOperation("GET_USER_ITEM");
const ServiceOperation ServiceOperation::GiveUserItemTo = ServiceOperation("GIVE_USER_ITEM_TO");
const ServiceOperation ServiceOperation::PurchaseUserItem = ServiceOperation("PURCHASE_USER_ITEM");
const ServiceOperation ServiceOperation::ReceiveUserItemFrom = ServiceOperation("RECEIVE_USER_ITEM_FROM");
const ServiceOperation ServiceOperation::SellUserItem = ServiceOperation("SELL_USER_ITEM");
const ServiceOperation ServiceOperation::UpdateUserItemData = ServiceOperation("UPDATE_USER_ITEM_DATA");
const ServiceOperation ServiceOperation::UseUserItem = ServiceOperation("USE_USER_ITEM");
const ServiceOperation ServiceOperation::PublishUserItemToBlackchain = ServiceOperation("PUBLISH_USER_ITEM_TO_BLOCKCHAIN");
const ServiceOperation ServiceOperation::RefreshBlockchainUserItems = ServiceOperation("REFRESH_BLOCKCHAIN_USER_ITEMS");

//item catalog
const ServiceOperation ServiceOperation::GetCatalogItemDefinition = ServiceOperation("GET_CATALOG_ITEM_DEFINITION");
const ServiceOperation ServiceOperation::GetCatalogItemsPage = ServiceOperation("GET_CATALOG_ITEMS_PAGE");
const ServiceOperation ServiceOperation::GetCatalogItemsPageOffset = ServiceOperation("GET_CATALOG_ITEMS_PAGE_OFFSET");

//custom entities
const ServiceOperation ServiceOperation::CreateEntity = ServiceOperation("CREATE_ENTITY");
const ServiceOperation ServiceOperation::DeleteEntity = ServiceOperation("DELETE_ENTITY");
const ServiceOperation ServiceOperation::GetCount = ServiceOperation("GET_COUNT");
const ServiceOperation ServiceOperation::CustomEntityGetPage = ServiceOperation("GET_PAGE");
const ServiceOperation ServiceOperation::CustomEntityGetPageOffset = ServiceOperation("GET_PAGE_BY_OFFSET");
const ServiceOperation ServiceOperation::CustomEntityGetEntityPage = ServiceOperation("GET_ENTITY_PAGE");
const ServiceOperation ServiceOperation::CustomEntityGetEntityPageOffset = ServiceOperation("GET_ENTITY_PAGE_OFFSET");
const ServiceOperation ServiceOperation::ReadEntity = ServiceOperation("READ_ENTITY");
const ServiceOperation ServiceOperation::IncrementData = ServiceOperation("INCREMENT_DATA");
const ServiceOperation ServiceOperation::IncrementSingletonData = ServiceOperation("INCREMENT_SINGLETON_DATA");
const ServiceOperation ServiceOperation::UpdateEntity = ServiceOperation("UPDATE_ENTITY");
const ServiceOperation ServiceOperation::UpdateEntityFields = ServiceOperation("UPDATE_ENTITY_FIELDS");
const ServiceOperation ServiceOperation::UpdateEntityFieldsSharded = ServiceOperation("UPDATE_ENTITY_FIELDS_SHARDED");
const ServiceOperation ServiceOperation::DeleteEntities = ServiceOperation("DELETE_ENTITIES");
const ServiceOperation ServiceOperation::UpdateSingletonFields = ServiceOperation("UPDATE_SINGLETON_FIELDS");

const ServiceOperation ServiceOperation::GetBlockchainItems = ServiceOperation("GET_BLOCKCHAIN_ITEMS");
const ServiceOperation ServiceOperation::GetUniqs = ServiceOperation("GET_UNIQS");

bool ServiceOperation::operator==(const ServiceOperation &s) const
{
	return _value == s.getValue();
}

void ServiceOperation::operator=(const ServiceOperation &s)
{
	_value = s.getValue();
}
