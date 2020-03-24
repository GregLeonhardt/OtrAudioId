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
 *  Access the database locate an file name and return it's information
 *
 *  @param  file_p                  FILE information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__get_file(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;
    /**
     *  @param  sqlite_cmd_p    Pointer to a command buffer                 */
    char                        *   sqlite_cmd_p;
    /**
     *  @param  sqlite_res_p    SqLite statement handle                     */
    sqlite3_stmt                *   sqlite_res_p;
    /**
     *  @param  sqlite_step     SqLite step value                           */
    int                             sqlite_step;
    /**
     *  @param  sqlite_cmd      Full SqLite command                         */
    char                            sqlite_cmd[ SQLITE_CMD_L ];
    /**
     *  @param  first           First search string added to the command    */
    int                             first;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clear the command buffers
    memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

    //  No search strings have been added yet.
    first = true;

    //  Start building the command
    strncpy( sqlite_cmd,
            "SELECT fingerprint, network_id, station_id, episode_id, "
            "date, time, quality, location "
            "from FILE where ", sizeof( sqlite_cmd ) );

    /************************************************************************
     *  FILE-FINGERPRINT
     ************************************************************************/

    //  Is there an file fingerprint to search for ?
    if ( strlen( file_p->fingerprint ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "fingerprint = '%q' ", file_p->fingerprint );
        else
            //  NO:     Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND fingerprint = '%q' ", file_p->fingerprint );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  NETWORK-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->network_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "network_id = '%d' ", file_p->network_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND network_id = '%d' ", file_p->network_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  STATION-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->station_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "station_id = '%d' ", file_p->station_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND station_id = '%d' ", file_p->station_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->episode_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "episode_id = '%d' ", file_p->episode_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND episode_id = '%d' ", file_p->episode_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-DATETIME
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( strlen( file_p->date_time ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "date_time = '%q' ", file_p->date_time );
        else
            //  NO:     Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND date_time = '%q' ", file_p->date_time );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-QUALITY
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->quality != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "quality = '%d' ", file_p->quality );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND quality = '%d' ", file_p->quality );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-LOCATION
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( strlen( file_p->location ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "location = '%q' ", file_p->location );
        else
            //  NO:     Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND location = '%q' ", file_p->location );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    //  Append the command termination string to the complete command.
    strncat( sqlite_cmd, ";", ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

    /************************************************************************
     *  Command cleanup and run
     ************************************************************************/

    //  Log the database access
    log_write( MID_INFO, "dbase_get_file",
            "SqLiteCmd: '%s'\n", sqlite_cmd );

    //  Perform the query
    sqlite_rc = sqlite3_prepare_v2( otr_db, sqlite_cmd, -1, &sqlite_res_p, 0 );

    if( sqlite_rc == SQLITE_OK )
    {
        //
        sqlite_step = sqlite3_step( sqlite_res_p );

        //  Access the first (or next) record matching the query
        if ( sqlite_step == SQLITE_ROW )
        {
            //  Clean the record buffer
            memset( file_p, 0x00, sizeof( struct file_t ) );

            //  Get     FINGERPRINT
            strncpy( file_p->fingerprint,
                     (char*)sqlite3_column_text( sqlite_res_p, 0 ),
                     sizeof( file_p->fingerprint ) );

            //  Get     NETWORK-ID
            file_p->network_id = sqlite3_column_int( sqlite_res_p, 1 );

            //  Get     STATION-ID
            file_p->station_id = sqlite3_column_int( sqlite_res_p, 2 );

            //  Get     EPISODE-ID
            file_p->episode_id = sqlite3_column_int( sqlite_res_p, 3 );

            //  Get     DATE
            strncpy( file_p->date_time,
                     (char*)sqlite3_column_text( sqlite_res_p, 4 ),
                     sizeof( file_p->date_time ) );

            //  Get     QUALITY
            file_p->quality = sqlite3_column_int( sqlite_res_p, 5 );

            //  Get     LOCATION
            strncpy( file_p->location,
                     (char*)sqlite3_column_text( sqlite_res_p, 6 ),
                     sizeof( file_p->location ) );
        }
        else
        {
            //  The record was not found
            sqlite_rc = SQLITE_ERROR;
        }
    }

    //  Finalize and release a prepared statement
    sqlite3_finalize( sqlite_res_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc == SQLITE_OK ? true : false );
}

/****************************************************************************/
/**
 *  Access the database locate an file name and return it's information
 *
 *  @param  file_p                  Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

struct  list_base_t *
DBASE__get_file_list(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  file_list_p  Pointer to a list of files                     */
    struct  list_base_t         *   file_list_p;
    /**
     *  @param  get_file_p   Pointer to a new file record                   */
    struct  file_t              *   get_file_p;
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;
    /**
     *  @param  sqlite_cmd_p    Pointer to a command buffer                 */
    char                        *   sqlite_cmd_p;
    /**
     *  @param  sqlite_res_p    SqLite statement handle                     */
    sqlite3_stmt                *   sqlite_res_p;
    /**
     *  @param  sqlite_step     SqLite step value                           */
    int                             sqlite_step;
    /**
     *  @param  sqlite_cmd      Full SqLite command                         */
    char                            sqlite_cmd[ SQLITE_CMD_L ];
    /**
     *  @param  first           First search string added to the command    */
    int                             first;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clear the command buffers
    memset( sqlite_cmd, '\0', sizeof( sqlite_cmd ) );

    //  No search strings have been added yet.
    first = true;

    //  Start building the command
    strncpy( sqlite_cmd,
            "SELECT fingerprint, network_id, station_id, episode_id, "
            "date, time, quality, location "
            "from FILE where ", sizeof( sqlite_cmd ) );

    //  Create a new list
    file_list_p = list_new( );

    /************************************************************************
     *  FILE-FINGERPRINT
     ************************************************************************/

    //  Is there an file fingerprint to search for ?
    if ( strlen( file_p->fingerprint ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "fingerprint = '%q' ", file_p->fingerprint );
        else
            //  NO:     Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND fingerprint = '%q' ", file_p->fingerprint );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  NETWORK-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->network_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "network_id = '%d' ", file_p->network_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND network_id = '%d' ", file_p->network_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  STATION-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->station_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "station_id = '%d' ", file_p->station_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND station_id = '%d' ", file_p->station_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-ID
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->episode_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "episode_id = '%d' ", file_p->episode_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND episode_id = '%d' ", file_p->episode_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-DATETIME
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( strlen( file_p->date_time ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "date_time = '%q' ", file_p->date_time );
        else
            //  NO:     Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND date_time = '%q' ", file_p->date_time );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-QUALITY
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( file_p->quality != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "quality = '%d' ", file_p->quality );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND quality = '%d' ", file_p->quality );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  FILE-LOCATION
     ************************************************************************/

    //  Is there an file title to search for ?
    if ( strlen( file_p->location ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "location = '%q' ", file_p->location );
        else
            //  NO:     Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND location = '%q' ", file_p->location );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    //  Append the command termination string to the complete command.
    strncat( sqlite_cmd, ";", ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

    /************************************************************************
     *  Command cleanup and run
     ************************************************************************/

    //  Log the database access
    log_write( MID_INFO, "dbase_get_file",
            "SqLiteCmd: '%s'\n", sqlite_cmd );

    //  Perform the query
    sqlite_rc = sqlite3_prepare_v2( otr_db, sqlite_cmd, -1, &sqlite_res_p, 0 );

    if( sqlite_rc == SQLITE_OK )
    {
        //  Get all records that match the query
        do
        {
            //  Access the first (or next) record.
            sqlite_step = sqlite3_step( sqlite_res_p );

            //  Access the first (or next) record matching the query
            if ( sqlite_step == SQLITE_ROW )
            {
                //  Allocate storage for a new record
                get_file_p = (struct file_t*)mem_malloc( sizeof(struct file_t) );

                //  Get     FINGERPRINT
                strncpy( file_p->fingerprint,
                         (char*)sqlite3_column_text( sqlite_res_p, 0 ),
                         sizeof( file_p->fingerprint ) );

                //  Get     NETWORK-ID
                file_p->network_id = sqlite3_column_int( sqlite_res_p, 1 );

                //  Get     STATION-ID
                file_p->station_id = sqlite3_column_int( sqlite_res_p, 2 );

                //  Get     EPISODE-ID
                file_p->episode_id = sqlite3_column_int( sqlite_res_p, 3 );

                //  Get     DATE
                strncpy( file_p->date_time,
                         (char*)sqlite3_column_text( sqlite_res_p, 4 ),
                         sizeof( file_p->date_time ) );

                //  Get     QUALITY
                file_p->quality = sqlite3_column_int( sqlite_res_p, 5 );

                //  Get     LOCATION
                strncpy( file_p->location,
                         (char*)sqlite3_column_text( sqlite_res_p, 6 ),
                         sizeof( file_p->location ) );

                //  Add it to the list of records.
                list_put_last( file_list_p, get_file_p );
            }

        }   while( sqlite_step == SQLITE_ROW );
    }

    //  Finalize and release a prepared statement
    sqlite3_finalize( sqlite_res_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( file_list_p );
}

/****************************************************************************/
/**
 *  Delete the file list
 *
 *  @param  file_list_p          The list that is to be deleted.
 *
 *  @return void                    No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DBASE__delete_file_list(
    struct  list_base_t         *   file_list_p
    )
{
    /**
     *  @param  file_p          Pointer to a new file record             */
    struct  file_t              *   file_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Was an file_list pointer provided ?
    if ( file_list_p != NULL )
    {
        //  Loop through the entire list
        for( file_p = list_get_first( file_list_p );
             file_p != NULL;
             file_p = list_get_next( file_list_p, file_p ) )
        {
            //  Remove it from the list
            list_delete( file_list_p, file_p );

            //  Release it's memory buffer.
            mem_free( file_p );
        }

        //  Remove the file list
        list_kill( file_list_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Access the database to insert a new file.
 *
 *  @param  file_p               Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__put_file(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;
    /**
     *  @param  sqlite_cmd_p    Pointer to a command buffer                 */
    char                        *   sqlite_cmd_p;
    /**
     *  @param  sqlite_error_p  Pointer to an error message (if failed)     */
    char                        *   sqlite_error_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the error message pointer
    sqlite_error_p = NULL;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Build the SqLite command statement
    sqlite_cmd_p = sqlite3_mprintf(
              "INSERT INTO FILE ( fingerprint, network_id, station_id, "
                    "episode_id, date, quality, location ) "
                    "VALUES ( '%q', '%d', '%d', '%d', '%q', '%d', '%q' );",
                    file_p->fingerprint,
                    file_p->network_id,
                    file_p->station_id,
                    file_p->episode_id,
                    file_p->date_time,
                    file_p->quality,
                    file_p->location );

    //  Log the database access
    log_write( MID_INFO, "dbase_put_file",
            "SqLiteCmd: '%s'\n", sqlite_cmd_p );

    //  Execute the record insertion
    sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd_p, DBASE__callback, 0, &sqlite_error_p);

    //  Was the command successful ?
    if( sqlite_rc != SQLITE_OK )
    {
        //  NO:     Log the failure
        log_write( MID_WARNING, "dbase_put_file",
                   "SQL error: %s\n", sqlite_error_p);

        //  Release the error message
        sqlite3_free( sqlite_error_p );
    }

    //  Release the data buffer
    sqlite3_free( sqlite_cmd_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc == SQLITE_OK ? true : false );
}

/****************************************************************************/
