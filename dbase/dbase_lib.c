/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'dBase' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <unistd.h>             //  UNIX standard library.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
#include "dbase_lib.h"          //  API for all DBASE__*            PRIVATE
                                //*******************************************
#include <sqlite3.h>            //  Everything SqLite-3
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Initialize the 'store' interface.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__callback(
    void                        *   NotUsed,
    int                             argc,
    char                        **  argv,
    char                        **  azColName
    )
{
    /**
     *  @param  ndx             Index into error report.                    */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    for( ndx = 0; ndx < argc; ndx ++ )
    {
        log_write( MID_WARNING, "DBASE__callback",
                   "%s = %s\n",
                   azColName[ ndx ], argv[ ndx ] ? argv[ ndx ] : "NULL");
    }

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  DONE!
    return( 0 );
}

/****************************************************************************/
/**
 *  Open an existing SqLite database and create tables (if the tables do not
 *  already exist).
 *
 *  @param  none
 *
 *  @return dbase_rc            TRUE when the dBase already exists or was
 *                              successfully created, else FALSE
 *
 *  @note
 *
 ****************************************************************************/
int
DBASE__open(
    enum    dbase_open_flags_e      flags
    )
{
    /**
     * @param dbase_rc          Function return code                        */
    int                             dbase_rc;
    /**
     *  @param  dbase_file_p    Pointer to the directory path               */
    char                        *   dbase_file_p;
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;
    /**
     *  @param  sqlite_err_p    Pointer to an error message                 */
    char                        *   sqlite_err_p;
    /**
     *  @param  sqlite_cmd      Buffer used to build a SqLite command       */
    char                            sqlite_cmd[ DB_COMMAND_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    //  Get the LibTools directory path
    dbase_file_p = file_path_to_lib( );

    //  Append the Translations file name to the path
    strncat( dbase_file_p, "/", FILE_NAME_L );
    strncat( dbase_file_p, "otr.db", FILE_NAME_L );

    /************************************************************************
     *  Create the tools directory if it doesn't already exist
     ************************************************************************/

    //  @ToDo:  0010   When the '.LibTools' directory does not exist.


    /************************************************************************
     *  DBASE_OF_CREATE
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Delete the old file
        unlink( dbase_file_p );
    }

    /************************************************************************
     *  Open the dBase
     ************************************************************************/

    sqlite_rc = sqlite3_open( dbase_file_p, &otr_db );

        //  Was the SqLite open successful ?
    if( sqlite_rc != SQLITE_OK )
    {
        //  NO:     Log the event
        log_write( MID_WARNING, "DBASE__open",
                   "Can't open database: %s\n", sqlite3_errmsg( otr_db ) );
        sqlite3_close( otr_db );
        log_write( MID_FATAL, "DBASE__open",
                   "Shutting down........\n" );
    }
#if 0
    else
    {
        //  YES:    Log the event
        log_write( MID_INFO, "DBASE__open",
                   "Database: 'otr.db' was successfully opened\n" );
    }
#endif

    /************************************************************************
     *  Create the NETWORK table
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Clear out the command buffer
        memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

        snprintf( sqlite_cmd, sizeof( sqlite_cmd ),
                  "CREATE TABLE IF NOT EXISTS NETWORK("
                      "network_id       INTEGER   PRIMARY KEY AUTOINCREMENT,"
                      "name             TEXT      NOT NULL UNIQUE"
                  ");" );

        //  Log the database access
        log_write( MID_INFO, "DBASE__open",
                "SqLiteCmd: '%s'\n", sqlite_cmd );

        //  Execute SQL statement
        sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd, DBASE__callback, 0, &sqlite_err_p);

        if( sqlite_rc != SQLITE_OK )
        {
            log_write( MID_WARNING, "DBASE__open",
                       "SQL error: %s\n", sqlite_err_p);
            sqlite3_free( sqlite_err_p );
            log_write( MID_FATAL, "DBASE__open",
                       "'%s'\n", sqlite_cmd );
        }
    }

    /************************************************************************
     *  Create the STATION table
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Clear out the command buffer
        memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

        snprintf( sqlite_cmd, sizeof( sqlite_cmd ),
                  "CREATE TABLE IF NOT EXISTS STATION("
                      "station_id       INTEGER   PRIMARY KEY AUTOINCREMENT,"
                      "name             TEXT      NOT NULL UNIQUE"
                  ");" );

        //  Log the database access
        log_write( MID_INFO, "DBASE__open",
                "SqLiteCmd: '%s'\n", sqlite_cmd );

        //  Execute SQL statement
        sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd, DBASE__callback, 0, &sqlite_err_p);

        if( sqlite_rc != SQLITE_OK )
        {
            log_write( MID_WARNING, "DBASE__open",
                       "SQL error: %s\n", sqlite_err_p);
            sqlite3_free( sqlite_err_p );
            log_write( MID_FATAL, "DBASE__open",
                       "'%s'\n", sqlite_cmd );
        }
    }

    /************************************************************************
     *  Create the SHOW table
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Clear out the command buffer
        memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

        snprintf( sqlite_cmd, sizeof( sqlite_cmd ),
                  "CREATE TABLE IF NOT EXISTS SHOW("
                      "show_id          INTEGER   PRIMARY KEY AUTOINCREMENT,"
                      "name             TEXT      NOT NULL UNIQUE,"
                      "state            INTEGER"
                  ");" );

        //  Log the database access
        log_write( MID_INFO, "DBASE__open",
                "SqLiteCmd: '%s'\n", sqlite_cmd );

        //  Execute SQL statement
        sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd, DBASE__callback, 0, &sqlite_err_p);

        if( sqlite_rc != SQLITE_OK )
        {
            log_write( MID_WARNING, "DBASE__open",
                       "SQL error: %s\n", sqlite_err_p);
            sqlite3_free( sqlite_err_p );
            log_write( MID_FATAL, "DBASE__open",
                       "'%s'\n", sqlite_cmd );
        }
    }

    /************************************************************************
     *  Create the EPISODE table
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Clear out the command buffer
        memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

        snprintf( sqlite_cmd, sizeof( sqlite_cmd ),
                  "CREATE TABLE IF NOT EXISTS EPISODE("
                      "episode_id       INTEGER   PRIMARY KEY AUTOINCREMENT,"
                      "show_id          INTEGER   NOT NULL,"
                      "name             TEXT,"
                      "date             TEXT,"
                      "number           INTEGER,"
                      "available        INTEGER"
                  ");" );

        //  Log the database access
        log_write( MID_INFO, "DBASE__open",
                "SqLiteCmd: '%s'\n", sqlite_cmd );

        //  Execute SQL statement
        sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd, DBASE__callback, 0, &sqlite_err_p);

        if( sqlite_rc != SQLITE_OK )
        {
            log_write( MID_WARNING, "DBASE__open",
                       "SQL error: %s\n", sqlite_err_p);
            sqlite3_free( sqlite_err_p );
            log_write( MID_FATAL, "DBASE__open",
                       "'%s'\n", sqlite_cmd );
        }
    }

    /************************************************************************
     *  Create the FILE table
     ************************************************************************/

    //  Are we being told to create a new (empty) database ?
    if ( flags == DBASE_OF_CREATE )
    {
        //  YES:    Clear out the command buffer
        //  Clear out the command buffer
        memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

        snprintf( sqlite_cmd, sizeof( sqlite_cmd ),
                  "CREATE TABLE IF NOT EXISTS FILE("
                      "fingerprint      TEXT        PRIMARY KEY,"
                      "network_id       INTEGER,"
                      "station_id       INTEGER,"
                      "episode_id       INTEGER,"
                      "date             TEXT,"
                      "quality          INTEGER,"
                      "location         TEXT"
                  ");" );

        //  Log the database access
        log_write( MID_INFO, "DBASE__open",
                "SqLiteCmd: '%s'\n", sqlite_cmd );

        //  Execute SQL statement
        sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd, DBASE__callback, 0, &sqlite_err_p);

        if( sqlite_rc != SQLITE_OK )
        {
            log_write( MID_WARNING, "DBASE__open",
                       "SQL error: %s\n", sqlite_err_p);
            sqlite3_free( sqlite_err_p );
            log_write( MID_FATAL, "DBASE__open",
                       "'%s'\n", sqlite_cmd );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc == SQLITE_OK ? true : false );
}

/****************************************************************************/
