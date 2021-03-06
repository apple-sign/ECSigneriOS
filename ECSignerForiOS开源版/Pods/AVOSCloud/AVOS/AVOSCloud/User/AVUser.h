// AVUser.h
// Copyright 2013 AVOS, Inc. All rights reserved.

#import <Foundation/Foundation.h>
#import "AVConstants.h"
#import "AVObject.h"
#import "AVSubclassing.h"
#import "AVDynamicObject.h"

@class AVRole;
@class AVQuery;
@class AVUserShortMessageRequestOptions;

NS_ASSUME_NONNULL_BEGIN

typedef NSString * const LeanCloudSocialPlatform NS_TYPED_EXTENSIBLE_ENUM;
extern LeanCloudSocialPlatform LeanCloudSocialPlatformWeiBo;
extern LeanCloudSocialPlatform LeanCloudSocialPlatformQQ;
extern LeanCloudSocialPlatform LeanCloudSocialPlatformWeiXin;

@interface AVUserAuthDataLoginOption : NSObject

/**
 Third platform.
 */
@property (nonatomic, strong, nullable) LeanCloudSocialPlatform platform;

/**
 UnionId from the third platform.
 */
@property (nonatomic, strong, nullable) NSString *unionId;

/**
 Set true to generate a platform-unionId signature.
 if a AVUser instance has a platform-unionId signature, then the platform and the unionId will be the highest priority in auth data matching.
 @Note must cooperate with platform & unionId.
 */
@property (nonatomic, assign) BOOL isMainAccount;

/**
 Set true to check whether already exists a AVUser instance with the auth data.
 if not exists, return an error.
 */
@property (nonatomic, assign) BOOL failOnNotExist;

@end

/*!
A LeanCloud Framework User Object that is a local representation of a user persisted to the LeanCloud. This class
 is a subclass of a AVObject, and retains the same functionality of a AVObject, but also extends it with various
 user specific methods, like authentication, signing up, and validation uniqueness.
 
 Many APIs responsible for linking a AVUser with Facebook or Twitter have been deprecated in favor of dedicated
 utilities for each social network. See AVFacebookUtils and AVTwitterUtils for more information.
 */


@interface AVUser : AVObject<AVSubclassing>

/** @name Accessing the Current User */

/*!
 Gets the currently logged in user from disk and returns an instance of it.
 @return a AVUser that is the currently logged in user. If there is none, returns nil.
 */
+ (instancetype _Nullable)currentUser;

/*!
 * change the current login user manually.
 *  @param newUser ?????? AVUser ??????
 *  @param save ??????????????? newUser ?????????????????????????????? newUser==nil && save==YES???????????????????????????
 * Note: ??????????????????????????????????????????
 */
+ (void)changeCurrentUser:(AVUser * _Nullable)newUser save:(BOOL)save;

/// The session token for the AVUser. This is set by the server upon successful authentication.
@property (nonatomic, copy, nullable) NSString *sessionToken;

/// Whether the AVUser was just created from a request. This is only set after a Facebook or Twitter login.
@property (nonatomic, assign, readonly) BOOL isNew;

/*!
 Whether the user is an authenticated object with the given sessionToken.
 */
- (void)isAuthenticatedWithSessionToken:(NSString *)sessionToken callback:(AVBooleanResultBlock)callback;

/** @name Creating a New User */

/*!
 Creates a new AVUser object.
 @return a new AVUser object.
 */
+ (instancetype)user;

/*!
 Enables automatic creation of anonymous users.  After calling this method, [AVUser currentUser] will always have a value.
 The user will only be created on the server once the user has been saved, or once an object with a relation to that user or
 an ACL that refers to the user has been saved.
 
 Note: saveEventually will not work if an item being saved has a relation to an automatic user that has never been saved.
 */
+ (void)enableAutomaticUser;

/// The username for the AVUser.
@property (nonatomic, copy, nullable) NSString *username;

/** 
 The password for the AVUser. This will not be filled in from the server with
 the password. It is only meant to be set.
 */
@property (nonatomic, copy, nullable) NSString *password;

/**
 *  Email of the user. If enable "Enable Email Verification" option in the console, when register a user, will send a verification email to the user. Otherwise, only save the email to the server.
 */
@property (nonatomic, copy, nullable) NSString *email;

/**
 *  Mobile phone number of the user. Can be set when registering. If enable the "Enable Mobile Phone Number Verification" option in the console, when register a user, will send an sms message to the phone. Otherwise, only save the mobile phone number to the server.
 */
@property (nonatomic, copy, nullable) NSString *mobilePhoneNumber;

/**
 *  Mobile phone number verification flag. Read-only. if calling verifyMobilePhone:withBlock: succeeds, the server will set this value YES.
 */
@property (nonatomic, assign, readonly) BOOL mobilePhoneVerified;

/**
 *  ????????????????????????
 *  ???????????????????????????????????????????????????????????????, ???????????????????????????????????????.
 *  
 *  @warning ???????????????,????????????????????????1??????????????????1???!
 *
 *  @param email ????????????
 *  @param block ????????????
 */
+(void)requestEmailVerify:(NSString*)email withBlock:(AVBooleanResultBlock)block;

/*!
 *  ????????????????????????
 *  ?????????????????????????????????????????????6??????????????????????????????10??????????????????
 *  
 *  @warning ???????????????????????????????????? 5 ??????????????????????????????????????????????????????????????????
 *
 *  @param phoneNumber 11???????????????
 *  @param block ????????????
 */
+(void)requestMobilePhoneVerify:(NSString *)phoneNumber withBlock:(AVBooleanResultBlock)block;

/**
 Request a verification code for a phone number.

 @param phoneNumber The phone number that will be verified later.
 @param options     The short message request options.
 @param callback    The callback of request.
 */
+ (void)requestVerificationCodeForPhoneNumber:(NSString *)phoneNumber
                                      options:(nullable AVUserShortMessageRequestOptions *)options
                                     callback:(AVBooleanResultBlock)callback;

/*!
 *  ?????????????????????
 *  ??????????????????????????????????????????
 *  @param code 6??????????????????
 *  @param block ????????????
 */
+(void)verifyMobilePhone:(NSString *)code withBlock:(AVBooleanResultBlock)block;

/*!
 Get roles which current user belongs to.

 @param error The error of request, or nil if request did succeed.

 @return An array of roles, or nil if some error occured.
 */
- (nullable NSArray<AVRole *> *)getRoles:(NSError **)error;

/*!
 An alias of `-[AVUser getRolesAndThrowsWithError:]` methods that supports Swift exception.
 @seealso `-[AVUser getRolesAndThrowsWithError:]`
 */
- (nullable NSArray<AVRole *> *)getRolesAndThrowsWithError:(NSError **)error;

/*!
 Asynchronously get roles which current user belongs to.

 @param block The callback for request.
 */
- (void)getRolesInBackgroundWithBlock:(void (^)(NSArray<AVRole *> * _Nullable objects, NSError * _Nullable error))block;

/*!
 Signs up the user. Make sure that password and username are set. This will also enforce that the username isn't already taken.
 @param error Error object to set on error. 
 @return whether the sign up was successful.
 */
- (BOOL)signUp:(NSError **)error;

/*!
 An alias of `-[AVUser signUp:]` methods that supports Swift exception.
 @seealso `-[AVUser signUp:]`
 */
- (BOOL)signUpAndThrowsWithError:(NSError **)error;

/*!
 Signs up the user asynchronously. Make sure that password and username are set. This will also enforce that the username isn't already taken.
 @param block The block to execute. The block should have the following argument signature: (BOOL succeeded, NSError *error) 
 */
- (void)signUpInBackgroundWithBlock:(AVBooleanResultBlock)block;

/*!
 ????????????????????????????????? 3.1.6 ???????????????????????????????????????????????????????????????????????????????????????????????????
 @param oldPassword ?????????
 @param newPassword ?????????
 @param block ???????????????????????????????????? (id object, NSError *error)
 @warning ?????????????????????????????????????????????????????????????????????????????????
 */
- (void)updatePassword:(NSString *)oldPassword newPassword:(NSString *)newPassword block:(AVIdResultBlock)block;

/*!
 Refresh user session token asynchronously.

 @param block The callback of request.
 */
- (void)refreshSessionTokenWithBlock:(AVBooleanResultBlock)block;

/*!
 Makes a request to login a user with specified credentials. Returns an
 instance of the successfully logged in AVUser. This will also cache the user 
 locally so that calls to userFromCurrentUser will use the latest logged in user.
 @param username The username of the user.
 @param password The password of the user.
 @param error The error object to set on error.
 @return an instance of the AVUser on success. If login failed for either wrong password or wrong username, returns nil.
 */
+ (nullable instancetype)logInWithUsername:(NSString *)username
                                  password:(NSString *)password
                                     error:(NSError **)error;

/*!
 Makes an asynchronous request to log in a user with specified credentials.
 Returns an instance of the successfully logged in AVUser. This will also cache 
 the user locally so that calls to userFromCurrentUser will use the latest logged in user. 
 @param username The username of the user.
 @param password The password of the user.
 @param block The block to execute. The block should have the following argument signature: (AVUser *user, NSError *error) 
 */
+ (void)logInWithUsernameInBackground:(NSString *)username
                             password:(NSString *)password
                                block:(AVUserResultBlock)block;

/**
 Login by email and password.

 @param email The email string.
 @param password The password string.
 @param block callback.
 */
+ (void)loginWithEmail:(NSString *)email password:(NSString *)password block:(AVUserResultBlock)block;

//phoneNumber + password
/*!
 *  ?????????????????????????????????
 *  @param phoneNumber 11???????????????
 *  @param password ??????
 *  @param error ?????????????????????????????????
 */
+ (nullable instancetype)logInWithMobilePhoneNumber:(NSString *)phoneNumber
                                           password:(NSString *)password
                                              error:(NSError **)error;
/*!
 *  ?????????????????????????????????
 *  @param phoneNumber 11???????????????
 *  @param password ??????
 *  @param block ????????????
 */
+ (void)logInWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                      password:(NSString *)password
                                         block:(AVUserResultBlock)block;
//phoneNumber + smsCode

/*!
 *  ?????????????????????
 *  ?????????????????????????????????????????????6??????????????????????????????10??????????????????
 *  @param phoneNumber 11???????????????
 *  @param block ????????????
 */
+(void)requestLoginSmsCode:(NSString *)phoneNumber withBlock:(AVBooleanResultBlock)block;

/**
 Request a login code for a phone number.

 @param phoneNumber The phone number of an user who will login later.
 @param options     The short message request options.
 @param callback    The callback of request.
 */
+ (void)requestLoginCodeForPhoneNumber:(NSString *)phoneNumber
                               options:(nullable AVUserShortMessageRequestOptions *)options
                              callback:(AVBooleanResultBlock)callback;

/*!
 *  ????????????????????????????????????
 *  @param phoneNumber 11???????????????
 *  @param code 6????????????
 *  @param error ?????????????????????????????????
 */
+ (nullable instancetype)logInWithMobilePhoneNumber:(NSString *)phoneNumber
                                            smsCode:(NSString *)code
                                              error:(NSError **)error;

/*!
 *  ????????????????????????????????????
 *  @param phoneNumber 11???????????????
 *  @param code 6????????????
 *  @param block ????????????
 */
+ (void)logInWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                       smsCode:(NSString *)code
                                         block:(AVUserResultBlock)block;


/*!
 *  ?????????????????????????????????????????????
 *  ???????????????????????????????????????????????? [AVOSCloud requestSmsCodeWithPhoneNumber:callback:] ???????????????
 *  @param phoneNumber 11???????????????
 *  @param code 6????????????
 *  @param error ?????????????????????????????????
 */
+ (nullable instancetype)signUpOrLoginWithMobilePhoneNumber:(NSString *)phoneNumber
                                                    smsCode:(NSString *)code
                                                      error:(NSError **)error;

/*!
 *  ?????????????????????????????????????????????
 *  ???????????????????????????????????????????????? [AVOSCloud requestSmsCodeWithPhoneNumber:callback:] ???????????????
 *  @param phoneNumber 11???????????????
 *  @param code 6????????????
 *  @param block ????????????
 */
+ (void)signUpOrLoginWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                               smsCode:(NSString *)code
                                                 block:(AVUserResultBlock)block;

/**
 Use mobile phone number & SMS code & password to sign up or login.

 @param phoneNumber Phone number.
 @param smsCode SMS code.
 @param password Password.
 @param block Result callback.
 */
+ (void)signUpOrLoginWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                               smsCode:(NSString *)smsCode
                                              password:(NSString *)password
                                                 block:(AVUserResultBlock)block;


/** @name Logging Out */

/*!
 Logs out the currently logged in user on disk.
 */
+ (void)logOut;

/** @name Requesting a Password Reset */


/*!
 Send a password reset request for a specified email and sets an error object. If a user
 account exists with that email, an email will be sent to that address with instructions 
 on how to reset their password.
 @param email Email of the account to send a reset password request.
 @param error Error object to set on error.
 @return true if the reset email request is successful. False if no account was found for the email address.
 */
+ (BOOL)requestPasswordResetForEmail:(NSString *)email
                               error:(NSError **)error;

/*!
 Send a password reset request asynchronously for a specified email.
 If a user account exists with that email, an email will be sent to that address with instructions
 on how to reset their password.
 @param email Email of the account to send a reset password request.
 @param block The block to execute. The block should have the following argument signature: (BOOL succeeded, NSError *error) 
 */
+ (void)requestPasswordResetForEmailInBackground:(NSString *)email
                                           block:(AVBooleanResultBlock)block;

/*!
 *  ??????????????????????????????????????????????????????????????????
 *  ?????????????????????????????????????????????6??????????????????????????????10??????????????????
 *  @param phoneNumber 11???????????????
 *  @param block ????????????
 */
+(void)requestPasswordResetWithPhoneNumber:(NSString *)phoneNumber
                                     block:(AVBooleanResultBlock)block;

/**
 Request a password reset code for a phone number.

 @param phoneNumber The phone number of an user whose password will be reset.
 @param options     The short message request options.
 @param callback    The callback of request.
 */
+ (void)requestPasswordResetCodeForPhoneNumber:(NSString *)phoneNumber
                                       options:(nullable AVUserShortMessageRequestOptions *)options
                                      callback:(AVBooleanResultBlock)callback;

/*!
 *  ???????????????????????????
 *  @param code 6????????????
 *  @param password ?????????
 *  @param block ????????????
 */
+(void)resetPasswordWithSmsCode:(NSString *)code
                    newPassword:(NSString *)password
                          block:(AVBooleanResultBlock)block;

/*!
 *  ??? sessionToken ???????????????
 *  @param sessionToken sessionToken
 *  @param block        ????????????
 */
+ (void)becomeWithSessionTokenInBackground:(NSString *)sessionToken block:(AVUserResultBlock)block;
/*!
 *  ??? sessionToken ???????????????
 *  @param sessionToken sessionToken
 *  @param error        ????????????
 *  @return ?????????????????????
 */
+ (nullable instancetype)becomeWithSessionToken:(NSString *)sessionToken error:(NSError **)error;

/** @name Querying for Users */

/*!
 Creates a query for AVUser objects.
 */
+ (AVQuery *)query;

// MARK: - Auth Data

/**
 Login use auth data.

 @param authData Get from third platform, data format e.g. { "id" : "id_string", "access_token" : "access_token_string", ... ... }.
 @param platformId The key for the auth data, to identify auth data.
 @param options See AVUserAuthDataLoginOption.
 @param callback Result callback.
 */
- (void)loginWithAuthData:(NSDictionary *)authData
               platformId:(NSString *)platformId
                  options:(AVUserAuthDataLoginOption * _Nullable)options
                 callback:(void (^)(BOOL succeeded, NSError * _Nullable error))callback;

/**
 Associate auth data to the AVUser instance.
 
 @param authData Get from third platform, data format e.g. { "id" : "id_string", "access_token" : "access_token_string", ... ... }.
 @param platformId The key for the auth data, to identify auth data.
 @param options See AVUserAuthDataLoginOption.
 @param callback Result callback.
 */
- (void)associateWithAuthData:(NSDictionary *)authData
                   platformId:(NSString *)platformId
                      options:(AVUserAuthDataLoginOption * _Nullable)options
                     callback:(void (^)(BOOL succeeded, NSError * _Nullable error))callback;

/**
 Disassociate auth data from the AVUser instance.

 @param platformId The key for the auth data, to identify auth data.
 @param callback Result callback.
 */
- (void)disassociateWithPlatformId:(NSString *)platformId
                          callback:(void (^)(BOOL succeeded, NSError * _Nullable error))callback;

// MARK: - Anonymous

/**
 Login anonymously.

 @param callback Result callback.
 */
+ (void)loginAnonymouslyWithCallback:(void (^)(AVUser * _Nullable user, NSError * _Nullable error))callback;

/**
 Check whether the instance of AVUser is anonymous.

 @return Result.
 */
- (BOOL)isAnonymous;

@end

@interface AVUserShortMessageRequestOptions : AVDynamicObject

@property (nonatomic, copy, nullable) NSString *validationToken;

@end

@interface AVUser (Deprecated)

/**
 Use a SNS's auth data to login or signup.
 if the auth data already bind to a valid AVUser, then the instance of the AVUser will return in result block.
 if the auth data not bind to a exist AVUser, then a new instance of AVUser will be created and return in result block.
 
 @param authData a Dictionary with specific format.
 e.g.
 {
 "authData" : {
 'platform' : {
 'uid' : someChars,
 'access_token' : someChars,
 ... ... (other attribute)
 }
 }
 }
 @param platform if the auth data belongs to Weibo, QQ or Weixin(Wechat),
 please use `LeanCloudSocialPlatformXXX` to assign platform.
 if not above platform, use a custom string.
 @param block result callback.
 */
+ (void)loginOrSignUpWithAuthData:(NSDictionary *)authData
                         platform:(NSString *)platform
                            block:(AVUserResultBlock)block
__deprecated_msg("deprecated, use -[loginWithAuthData:platformId:options:callback:] instead.");

/**
 Associate a SNS's auth data to a instance of AVUser.
 after associated, user can login by auth data.
 
 @param authData a Dictionary with specific format.
 e.g.
 {
 "authData" : {
 'platform' : {
 'uid' : someChars,
 'access_token' : someChars,
 ... ... (other attribute)
 }
 }
 }
 @param platform if the auth data belongs to Weibo, QQ or Weixin(Wechat),
 please use `LeanCloudSocialPlatformXXX` to assign platform.
 if not above platform, use a custom string.
 @param block result callback.
 */
- (void)associateWithAuthData:(NSDictionary *)authData
                     platform:(NSString *)platform
                        block:(AVUserResultBlock)block
__deprecated_msg("deprecated, use -[associateWithAuthData:platformId:options:callback:] instead.");

/**
 Disassociate the specified platform's auth data from a instance of AVUser.
 
 @param platform if the auth data belongs to Weibo, QQ or Weixin(Wechat),
 please use `LeanCloudSocialPlatformXXX` to assign platform.
 if not above platform, use a custom string.
 @param block result callback.
 */
- (void)disassociateWithPlatform:(NSString *)platform
                           block:(AVUserResultBlock)block
__deprecated_msg("deprecated, use -[disassociateWithPlatformId:callback:] instead.");

/*!
 Signs up the user. Make sure that password and username are set. This will also enforce that the username isn't already taken.
 @return true if the sign up was successful.
 */
- (BOOL)signUp AV_DEPRECATED("2.6.10");

/*!
 Signs up the user asynchronously. Make sure that password and username are set. This will also enforce that the username isn't already taken.
 */
- (void)signUpInBackground AV_DEPRECATED("2.6.10");

/*!
 Makes a request to login a user with specified credentials. Returns an instance
 of the successfully logged in AVUser. This will also cache the user locally so
 that calls to userFromCurrentUser will use the latest logged in user.
 @param username The username of the user.
 @param password The password of the user.
 @return an instance of the AVUser on success. If login failed for either wrong password or wrong username, returns nil.
 */
+ (nullable instancetype)logInWithUsername:(NSString *)username
                                  password:(NSString *)password  AV_DEPRECATED("2.6.10");

/*!
 Makes an asynchronous request to login a user with specified credentials.
 Returns an instance of the successfully logged in AVUser. This will also cache
 the user locally so that calls to userFromCurrentUser will use the latest logged in user.
 @param username The username of the user.
 @param password The password of the user.
 */
+ (void)logInWithUsernameInBackground:(NSString *)username
                             password:(NSString *)password AV_DEPRECATED("2.6.10");

+ (nullable instancetype)logInWithMobilePhoneNumber:(NSString *)phoneNumber
                                           password:(NSString *)password AV_DEPRECATED("2.6.10");
+ (void)logInWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                      password:(NSString *)password AV_DEPRECATED("2.6.10");

+ (nullable instancetype)logInWithMobilePhoneNumber:(NSString *)phoneNumber
                                            smsCode:(NSString *)code AV_DEPRECATED("2.6.10");
+ (void)logInWithMobilePhoneNumberInBackground:(NSString *)phoneNumber
                                       smsCode:(NSString *)code AV_DEPRECATED("2.6.10");

/*!
 Send a password reset request for a specified email. If a user account exists with that email,
 an email will be sent to that address with instructions on how to reset their password.
 @param email Email of the account to send a reset password request.
 @return true if the reset email request is successful. False if no account was found for the email address.
 */
+ (BOOL)requestPasswordResetForEmail:(NSString *)email AV_DEPRECATED("2.6.10");

/*!
 Send a password reset request asynchronously for a specified email and sets an
 error object. If a user account exists with that email, an email will be sent to
 that address with instructions on how to reset their password.
 @param email Email of the account to send a reset password request.
 */
+ (void)requestPasswordResetForEmailInBackground:(NSString *)email AV_DEPRECATED("2.6.10");

/*!
 Whether the user is an authenticated object for the device. An authenticated AVUser is one that is obtained via
 a signUp or logIn method. An authenticated object is required in order to save (with altered values) or delete it.
 @return whether the user is authenticated.
 */
- (BOOL)isAuthenticated AV_DEPRECATED("Deprecated in AVOSCloud SDK 3.7.0. Use -[AVUser isAuthenticatedWithSessionToken:callback:] instead.");

@end

NS_ASSUME_NONNULL_END
