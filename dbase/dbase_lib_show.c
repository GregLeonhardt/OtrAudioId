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
 *  Access the database locate a show name and return it's information
 *
 *  @param  show_p                  Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__get_show(
    struct  show_t              *   show_p
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

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/


    //  Build the SqLite command statement
    sqlite_cmd_p = sqlite3_mprintf(
            "SELECT show_id, name, state from SHOW where name = '%q';",
                    show_p->name );

    //  Log the database access
    log_write( MID_INFO, "DBASE__get_show",
            "SqLiteCmd: '%s'\n", sqlite_cmd_p );

    //  Perform the query
    sqlite_rc = sqlite3_prepare_v2( otr_db, sqlite_cmd_p, -1, &sqlite_res_p, 0 );

    if( sqlite_rc == SQLITE_OK )
    {
        //
        sqlite_step = sqlite3_step( sqlite_res_p );

        //  Access the first (or next) record matching the query
        if ( sqlite_step == SQLITE_ROW )
        {
            //  Clean the record buffer
            memset( show_p, 0x00, sizeof( struct show_t ) );

            //  Get     SHOW-ID
            show_p->show_id = sqlite3_column_int( sqlite_res_p, 0 );

            //  Get     NAME
            strncpy( show_p->name,
                     (char*)sqlite3_column_text( sqlite_res_p, 1 ),
                     sizeof( show_p->name ) );

            //  Get     STATE
            show_p->state   = sqlite3_column_int( sqlite_res_p, 2 );
        }
        else
        {
            //  The record was not found
            sqlite_rc = SQLITE_ERROR;
        }

        //  Finalize and release a prepared statement
        sqlite3_finalize( sqlite_res_p );
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
/**
 *  Access the database to insert a new show.
 *
 *  @param  show_p                  Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
DBASE__put_show(
    struct  show_t              *   show_p
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
              "INSERT INTO SHOW ( name,state ) VALUES ( '%q', '%d' );",
                    show_p->name, show_p->state );

    //  Log the database access
    log_write( MID_INFO, "DBASE__put_show",
            "SqLiteCmd: '%s'\n", sqlite_cmd_p );

    //  Execute the record insertion
    sqlite_rc = sqlite3_exec( otr_db, sqlite_cmd_p, DBASE__callback, 0, &sqlite_error_p);

    //  Was the command successful ?
    if( sqlite_rc != SQLITE_OK )
    {
        //  NO:     Log the failure
        log_write( MID_WARNING, "DBASE__put_show",
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
