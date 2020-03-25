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
#include <strings.h>            //  BSD Functions for managing strings
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
     *  @param  copy_of_episode A local copy of the episode structure       */
    struct  episode_t               episode;
    /**
     *  @param  answer          Where the keyboard read data goes           */
    char                            answer[ 256 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Save the episode structure
    memcpy( &episode, episode_p, sizeof( episode ) );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get an episode record
    sqlite_rc = DBASE__get_episode( &episode );

    //  Was the episode record found ?
    if ( sqlite_rc != true )
    {
        //  NO:     Restore the episode structure to it original
        memcpy( &episode, episode_p, sizeof( episode ) );

        //  Remove EpisodeDate and EpisodeNumber from the search
        memset( episode.date,   '\0', sizeof( episode.date ) );
        memset( episode.number, '\0', sizeof( episode.number ) );

        //  Get an episode record
        sqlite_rc = DBASE__get_episode( &episode );

        //  Did we find the episode ?
        if ( sqlite_rc == true )
        {
            //  YES:    Ask a human if this is correct
            log_write( MID_INFO, "dbase_get_episode",
                    "Found Episode:        %4d - %8s - %s\n",
                    episode.number, episode.date, episode.name );
            log_write( MID_INFO, "dbase_get_episode",
                    "Looking for Episode:  %4d - %8s - %s\n",
                    episode_p->number, episode_p->date, episode_p->name );
            log_write( MID_INFO, "dbase_get_episode",
                    "Is this correct ? (Yes | No)");
            fgets( answer, sizeof( answer ), stdin );

            //  Yes ?
            if ( strncasecmp( answer, "YES", 3 ) != 0 )
            {
                //  NO:     Something other then "YES" was typed.
                sqlite_rc = SQLITE_ERROR;
            }
            else
            {
                //  Update the Episode record with the changes
                sqlite_rc = DBASE__update_episode( episode_p, &episode );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc );
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

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get a list of episodes
    episode_list_p = DBASE__get_episode_list( episode_p );

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
     *  Function Body
     ************************************************************************/

    //  Delete a list of episodes
    episode_list_p = DBASE__get_episode_list( episode_p );

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

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Put an episode record into the database.
    sqlite_rc = DBASE__put_episode( episode_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc );
}

/****************************************************************************/
