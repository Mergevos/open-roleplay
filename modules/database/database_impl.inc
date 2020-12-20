#include <YSI_Coding\y_hooks>

static
    MySQL: s_Db_Handle;

// connection establishing

hook OnGameModeInit()
{
    new 
        MySQLOpt:options = mysql_init_options(), 
        hostname[MAX_HOSTNAME],
        username[MAX_MYSQL_USER],
        password[MAX_MYSQL_PASSWORD],
        database[MAX_MYSQL_DATABASE];
        
    #if defined MYSQL_LOGGING
        Log_ToggleHandle(DEFAULT_MYSQL_LOG_HANDLE, true);
    #endif
    if (Env_Has(DEFAULT_MYSQL_HOST_ENV_KEY)) {
        Env_Get(DEFAULT_MYSQL_HOST_ENV_KEY, hostname, sizeof hostname);
    } else {
        Log_Error(DEFAULT_MYSQL_LOG_HANDLE, "There's no host key.", Log_Field_s("Wanted key", DEFAULT_MYSQL_HOST_ENV_KEY));
    } if (Env_Has(DEFAULT_MYSQL_USER_ENV_KEY)) {
        Env_Get(DEFAULT_MYSQL_USER_ENV_KEY, username, sizeof username);
    } else {
        Log_Error(DEFAULT_MYSQL_LOG_HANDLE, "There's no user key.", Log_Field_s("Wanted key", DEFAULT_MYSQL_USER_ENV_KEY));
    } if (Env_Has(DEFAULT_MYSQL_PASSWORD_ENV_KEY)) {
        Env_Get(DEFAULT_MYSQL_PASSWORD_ENV_KEY, password, sizeof password);
    } else {
        Log_Error(DEFAULT_MYSQL_LOG_HANDLE, "There's no password key.", Log_Field_s("Wanted key", DEFAULT_MYSQL_PASSWORD_ENV_KEY));
    } if (Env_Has(DEFAULT_MYSQL_DATABASE_ENV_KEY)) {
        Env_Get(DEFAULT_MYSQL_DATABASE_ENV_KEY, database, sizeof database);
    } else {
        Log_Error(DEFAULT_MYSQL_LOG_HANDLE, "There's no user key.", Log_Field_s("Wanted key", DEFAULT_MYSQL_DATABASE_ENV_KEY));
    }

    Log_Debug(DEFAULT_MYSQL_LOG_HANDLE, "MySQL options initialization...");
    mysql_set_option(options, AUTO_RECONNECT, DEFAULT_MYSQL_AUTO_RECCONECT);
    mysql_set_option(options, MULTI_STATEMENTS, DEFAULT_MYSQL_MULTI_STATEMENTS);
    mysql_set_option(options, POOL_SIZE, DEFAULT_MYSQL_POOL_SIZE);
    Log_Debug(DEFAULT_MYSQL_LOG_HANDLE, "Options initialized. Connecting...", Log_Field_i("Auto Recconect", DEFAULT_MYSQL_AUTO_RECCONECT), Log_Field_i("Multi Statements", DEFAULT_MYSQL_MULTI_STATEMENTS), Log_Field_i("Pool size", DEFAULT_MYSQL_MULTI_STATEMENTS));

    s_Db_Handle = mysql_connect(hostname, username, password, database, options);
    if (mysql_errno(s_Db_Handle) != 0) {
        Log_Error(DEFAULT_MYSQL_LOG_HANDLE, "Invalid connection, connection failed.");
        return 0;
    }
    Log_Info(DEFAULT_MYSQL_LOG_HANDLE, "MySQL connection established.");
    return 1;
}

// -- required

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
    return 1;
}

/**
 * <summary> Returns database's handle</summary>
 * <returns> Database habdke</returns>
 */

stock MySQL: Database_ReturnHandle()
{
    return s_Db_Handle;
}