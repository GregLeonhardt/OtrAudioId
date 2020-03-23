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
 *  Access the database locate an episode name and return it's information
 *
 *  @param  episode_p               Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_get_episode(
    struct  episode_t           *   episode_p
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
            "SELECT episode_id, show_id, name, date, number, available "
            "from EPISODE where ", sizeof( sqlite_cmd ) );

    /************************************************************************
     *  SHOW-ID
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( episode_p->show_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "show_id = '%d' ", episode_p->show_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND show_id = '%d' ", episode_p->show_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-TITLE
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->name ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "name = '%q' ", episode_p->name );
        else
            //  NO:     Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND name = '%q' ", episode_p->name );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-DATE
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->date ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "date = '%q' ", episode_p->date );
        else
            //  NO:     Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND date = '%q' ", episode_p->date );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-NUMBER
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->date ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "number = '%q' ", episode_p->number );
        else
            //  NO:     Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND number = '%q' ", episode_p->number );

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


    //  Build the SqLite command statement
#if 0
    sqlite_cmd_p = sqlite3_mprintf(
            "SELECT episode_id, show_id, name, date, number, available "
            "from EPISODE where name = '%q';",
                    episode_p->name );
#endif

    //  Log the database access
    log_write( MID_INFO, "dbase_get_episode",
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
            memset( episode_p, 0x00, sizeof( struct episode_t ) );

            //  Get     EPISODE-ID
            episode_p->episode_id = sqlite3_column_int( sqlite_res_p, 0 );

            //  Get     SHOW-ID
            episode_p->show_id = sqlite3_column_int( sqlite_res_p, 1 );

            //  Get     NAME
            strncpy( episode_p->name,
                     (char*)sqlite3_column_text( sqlite_res_p, 2 ),
                     sizeof( episode_p->name ) );

            //  Get     DATE
            strncpy( episode_p->date,
                     (char*)sqlite3_column_text( sqlite_res_p, 3 ),
                     sizeof( episode_p->date ) );

            //  Get     NUMBER
            strncpy( episode_p->number,
                     (char*)sqlite3_column_text( sqlite_res_p, 4 ),
                     sizeof( episode_p->number ) );

            //  Get     AVAILABLE
            episode_p->available = sqlite3_column_int( sqlite_res_p, 5 );
        }
        else
        {
            //  The record was not found
            sqlite_rc = SQLITE_ERROR;
        }
    }

    //  Release the data buffer
    sqlite3_free( sqlite_cmd_p );

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
 *  Access the database locate an episode name and return it's information
 *
 *  @param  episode_p               Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

struct  list_base_t *
dbase_get_episode_list(
    struct  episode_t           *   episode_p
    )
{
    /**
     *  @param  episode_list_p  Pointer to a list of episodes               */
    struct  list_base_t         *   episode_list_p;
    /**
     *  @param  get_episode_p   Pointer to a new episode record             */
    struct  episode_t           *   get_episode_p;
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
            "SELECT episode_id, show_id, name, date, number, available "
            "from EPISODE where ", sizeof( sqlite_cmd ) );

    //  Create a new list
    episode_list_p = list_new( );

    /************************************************************************
     *  SHOW-ID
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( episode_p->show_id != -1 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "show_id = '%d' ", episode_p->show_id );
        else
            //  NO:     Build the show_id search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND show_id = '%d' ", episode_p->show_id );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-TITLE
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->name ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "name = '%q' ", episode_p->name );
        else
            //  NO:     Build the name search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND name = '%q' ", episode_p->name );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-DATE
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->date ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "date = '%q' ", episode_p->date );
        else
            //  NO:     Build the date search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND date = '%q' ", episode_p->date );

        //  Not the first search string anymore.
        first = false;

        //  Append the new part of the command to the complete command
        strncat( sqlite_cmd, sqlite_cmd_p, ( sizeof( sqlite_cmd ) - strlen( sqlite_cmd ) ) );

        //  Release the data buffer
        sqlite3_free( sqlite_cmd_p );
    }

    /************************************************************************
     *  EPISODE-NUMBER
     ************************************************************************/

    //  Is there an episode title to search for ?
    if ( strlen( episode_p->date ) != 0 )
    {
        //  YES:    Is this the first search string ?
        if ( first == true )
            //  YES:    Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "number = '%q' ", episode_p->number );
        else
            //  NO:     Build the number search string.
            sqlite_cmd_p = sqlite3_mprintf( "AND number = '%q' ", episode_p->number );

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
    log_write( MID_INFO, "dbase_get_episode",
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
                get_episode_p = (struct episode_t*)mem_malloc( sizeof(struct episode_t) );

                //  Get     EPISODE-ID
                get_episode_p->episode_id = sqlite3_column_int( sqlite_res_p, 0 );

                //  Get     SHOW-ID
                get_episode_p->show_id = sqlite3_column_int( sqlite_res_p, 1 );

                //  Get     NAME
                strncpy( get_episode_p->name,
                         (char*)sqlite3_column_text( sqlite_res_p, 2 ),
                         sizeof( get_episode_p->name ) );

                //  Get     DATE
                strncpy( get_episode_p->date,
                         (char*)sqlite3_column_text( sqlite_res_p, 3 ),
                         sizeof( get_episode_p->date ) );

                //  Get     NUMBER
                strncpy( get_episode_p->number,
                         (char*)sqlite3_column_text( sqlite_res_p, 4 ),
                         sizeof( get_episode_p->number ) );

                //  Get     AVAILABLE
                get_episode_p->available = sqlite3_column_int( sqlite_res_p, 5 );

                //  Add it to the list of records.
                list_put_last( episode_list_p, get_episode_p );
            }

        }   while( sqlite_step == SQLITE_ROW );
    }

    //  Release the data buffer
    sqlite3_free( sqlite_cmd_p );

    //  Finalize and release a prepared statement
    sqlite3_finalize( sqlite_res_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( episode_list_p );
}

/****************************************************************************/
/**
 *  Delete the episode list
 *
 *  @param  episode_list_p          The list that is to be deleted.
 *
 *  @return void                    No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
dbase_delete_episode_list(
    struct  list_base_t         *   episode_list_p
    )
{
    /**
     *  @param  episode_p       Pointer to a new episode record             */
    struct  episode_t           *   episode_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Was an episode_list pointer provided ?
    if ( episode_list_p != NULL )
    {
        //  Loop through the entire list
        for( episode_p = list_get_first( episode_list_p );
             episode_p != NULL;
             episode_p = list_get_next( episode_list_p, episode_p ) )
        {
            //  Remove it from the list
            list_delete( episode_list_p, episode_p );

            //  Release it's memory buffer.
            mem_free( episode_p );
        }

        //  Remove the episode list
        list_kill( episode_list_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Access the database to insert a new episode.
 *
 *  @param  episode_p               Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_put_episode(
    struct  episode_t           *   episode_p
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
              "INSERT INTO EPISODE ( show_id, name, date, number, available ) "
              "VALUES ( '%d', '%q', '%q', '%q', '%d' );",
                    episode_p->show_id,
                    episode_p->name,
                    episode_p->date,
                    episode_p->number,
                    episode_p->available );

    //  Log the database access
    log_write( MID_INFO, "dbase_put_episode",
            "SqLiteCmd: '%s'\n", sqlite_cmd_p );

    //  Execute the record insertion
    sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd_p, DBASE__callback, 0, &sqlite_error_p);

    //  Was the command successful ?
    if( sqlite_rc != SQLITE_OK )
    {
        //  NO:     Log the failure
        log_write( MID_WARNING, "dbase_put_episode",
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
