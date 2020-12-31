#include <YSI_Coding\y_hooks>

/*
    *   forwards 	*
*/

forward [MAX_PLAYER_NAME + 1]Account_ReturnName(playerid);
forward bool: account_BCryptHashCheck(const password[], const hash[]);
/*
    *   vars 	*
*/

static 
    s_AccountSQL[MAX_PLAYERS],
    s_AccountPassword[MAX_PLAYERS][MAX_PLAYER_PASSWORD],
    s_AccountEmail[MAX_PLAYERS][MAX_EMAIL_LENGTH],
    s_AccountRaceCheck[MAX_PLAYERS];

/*
    *   Getting 	*
*/

/** 
* <summary> Gets the authentication email.</summary>
* <param name="playerid">The ID of a player we're getting email from.</param>
* <returns> The email of <paramref name="playerid"/></returns>
* <seealso name="Account_SetEmail"/>
*/
stock Account_ReturnEmail(playerid) {
    return s_AccountEmail[playerid];
}

/**
* <summary>Gets the authentication name.</summary>
* <param name="playerid">The ID of a player we're getting name from.</param>
* <returns> The name <paramref name="playerid"/></returns>
* <remarks> This function uses #emit to return a name. It returns the player name which player is currently using.</remarks>
* <seealso name="SetPlayerName"/>
*/

stock Account_ReturnName(playerid)
{
    #assert MAX_PLAYER_NAME + 1 == 25
    #emit PUSH.C 25
    #emit PUSH.S 16
    #emit PUSH.S playerid
    #emit PUSH.C 12
    #emit SYSREQ.C GetPlayerName
    #emit STACK 16
    #emit RETN
}

// no password return, gets deleted after login

/*
    *   Setting 	*
*/

/**
* <summary>Sets the authentication email.</summary>
* <param name="playerid">The ID of a player we're getting name from.</param>
* <seealso name="Account_ReturnEmail"/>
*/

stock Account_SetEmail(playerid, const email[MAX_EMAIL_LENGTH]) {
    strcopy(s_AccountEmail, email);
}

/*
    *   Hooks 	*
*/

hook OnPlayerConnect(playerid, classid) {
    s_AccountRaceCheck[playerid] ++;
    TogglePlayerSpectating(playerid, true);
    account_CheckRaceCheck(playerid, s_AccountRaceCheck[playerid]);   
    
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    s_AccountRaceCheck[playerid] ++;
    return 1;
}

/*
    * Internal *
*/

static stock account_CheckRaceCheck(playerid, race_check) {
    if(race_check != s_AccountRaceCheck[playerid]) {
        return Kick(playerid);
    }
    
    await mysql_aquery_s(Database_ReturnHandle(), str_format("SELECT * FROM `accounts` WHERE `account_name` = '%e'", Account_ReturnName(playerid)));
    if(cache_num_rows()) {
        cache_get_value_name(0, "account_password", s_AccountPassword[playerid]);
        cache_get_value_name(0, "account_email", s_AccountEmail[playerid]);
        account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_LOGIN);
        return 1;
    }
    account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_REGISTER);
    return 0;
}

/*
    * Login/Register *
*/

static stock account_AuthenticationDialogs(playerid, type) {
    switch(type) {
        case AUTHENTICATION_DIALOG_REGISTER: {
            new 
                dialog_response[e_DIALOG_RESPONSE_INFO];
            await_arr(dialog_response) ShowPlayerAsyncDialogStr(playerid, DIALOG_STYLE_PASSWORD, str_new("Register"), str_new("Input your password"), str_new("Ok"), str_new("Exit"));
            if(dialog_response[E_DIALOG_RESPONSE_Response]) {
                if(!(MIN_PLAYER_PASSWORD <= strlen(dialog_response[E_DIALOG_RESPONSE_InputText]) < MAX_PLAYER_PASSWORD)) {
                    return account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_REGISTER);
                }
                account_BCryptHashPassword(dialog_response[E_DIALOG_RESPONSE_InputText], s_AccountPassword[playerid]);
                account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_EMAIL);
                return 1;
            }
            Kick(playerid);

        } 
        case AUTHENTICATION_DIALOG_LOGIN: {
            new 
                dialog_response[e_DIALOG_RESPONSE_INFO];
            await_arr(dialog_response) ShowPlayerAsyncDialogStr(playerid, DIALOG_STYLE_PASSWORD, str_new("Login"), str_new("Input your password"), str_new("Ok"), str_new("Exit"));
            if(dialog_response[E_DIALOG_RESPONSE_Response]) {
                if(account_BCryptHashCheck(dialog_response[E_DIALOG_RESPONSE_InputText], s_AccountPassword[playerid])) {
                    s_AccountPassword[playerid][0] = EOS;
                    TogglePlayerSpectating(playerid, false);
                    SetSpawnInfo(playerid, NO_TEAM, 3, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
                    SpawnPlayer(playerid);
                    return 1;
                }
                static 
                    login_attempts;
                if(login_attempts == 2) {
                    return Kick(playerid);
                }
                login_attempts ++;
                account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_LOGIN);
            }
        }
        case AUTHENTICATION_DIALOG_EMAIL: {
            new 
                dialog_response[e_DIALOG_RESPONSE_INFO];
            await_arr(dialog_response) ShowPlayerAsyncDialogStr(playerid, DIALOG_STYLE_INPUT, str_new("Email"), str_new("Input your email"), str_new("Ok"), str_new("Exit"));
            if(dialog_response[E_DIALOG_RESPONSE_Response]) {
                if(!(MIN_EMAIL_LENGTH < strlen(dialog_response[E_DIALOG_RESPONSE_InputText]) < MAX_EMAIL_LENGTH)) {
                    return account_AuthenticationDialogs(playerid, AUTHENTICATION_DIALOG_EMAIL);
                }
                strcopy(s_AccountEmail[playerid], dialog_response[E_DIALOG_RESPONSE_InputText]);
                await mysql_aquery_s(Database_ReturnHandle(), str_format("INSERT INTO `accounts` (`account_name`, `account_password`, `account_email`) VALUES ('%e', '%e', '%e')", Account_ReturnName(playerid), s_AccountPassword[playerid], s_AccountEmail[playerid]));
                s_AccountSQL[playerid] = cache_insert_id();
                s_AccountPassword[playerid][0] = EOS;

                TogglePlayerSpectating(playerid, false);
                SetSpawnInfo(playerid, NO_TEAM, 3, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
                SpawnPlayer(playerid);
            }
        }
    }
    return 0;
}

static stock account_BCryptHashPassword(const password[], dest[61]) {
    new 
        bcrypt_info[e_BCRYPT_INFO];
    
    await_arr(bcrypt_info) bcrypt_ahash(password, BCRYPT_COST);
    strcopy(dest, bcrypt_info[E_BCRYPT_Hash]);
}

static stock bool: account_BCryptHashCheck(const password[], const hash[]) {
    new 
        bcrypt_info[e_BCRYPT_INFO];
    
    await_arr(bcrypt_info) bcrypt_acheck(password, hash);
    return bcrypt_info[E_BCRYPT_Equal];
}