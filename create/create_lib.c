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


/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Determine the type contained
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <create_api.h>         //  API for all create_*            PUBLIC
#include "create_lib.h"         //  API for all CREATE__*           PRIVATE
                                //*******************************************
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param                      Data column for episode data                */

//  '_O' = Offset from byte 0, '_L' = Length
#define ED_EXISTS_O              0
#define ED_EXISTS_L              1
#define ED_DATE_O                2
#define ED_DATE_L                8
#define ED_EPISODE_NUM_O        11
#define ED_EPISODE_NUM_L         4
#define ED_EPISODE_NAME_O       16
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
 *  Process a '.TXT' file adding SHOW and EPISODE information into
 *  the dBase.
 *
 *  @param  file_name_p             Pointer to the file name.
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return mp3_rc                  TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
CREATE__process_file(
    char                        *   file_name_p,
    struct  file_info_t         *   file_info_p
    )
{
    /**
     *  @param  create_rc       Function return code                        */
    int                             create_rc;
    /**
     *  @param  input_file_fp   Input File pointer                          */
    FILE                        *   in_file_fp;
    /**
     *  @param  read_data_p     Read data input buffer                      */
    char                        *   read_data_p;
    /**
     *  @param  start_p         Starting point of a string                  */
    char                        *   start_p;
    /**
     *  @param  show_p          Show information                            */
    struct  show_t                  show;
    /**
     *  @param  episode_p       Episode information                         */
    struct  episode_t               episode;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    create_rc = true;

    //  Clean the data buffer
    memset( &show, 0x00, sizeof( struct show_t ) );

    /************************************************************************
     *  Open the file for reading
     ************************************************************************/

    //  Open the input file
    in_file_fp = file_open_read( file_name_p );

    //  Log the event
    log_write( MID_INFO, "main",
               "Working on file: '%s'\n", file_name_p );

    /************************************************************************
     *  Capture the show name from the file name
     ************************************************************************/

    //  Locate the start of the file name
    start_p = strrchr( file_name_p, '/' );

    //  Was there a directory name included ?
    if ( start_p == NULL )
    {
        //  NO:     We will use the whole name passed in.
        start_p = file_name_p;
    }
    else
    {
        //  YES:    Move the pointer past the '/' character.
        start_p += 1;
    }

    //  Copy the file name to the series name buffer
    strncpy( show.name, start_p, ( strlen( start_p ) -1 ) );

    //  Remove '.txt' from the end of the string
    start_p = strrchr( show.name, '.' );

    //  "No way this can fail. We already know it it a '.TXT' file
    if ( start_p != NULL )
    {
        //  Null terminate here
        start_p[ 0 ] = '\0';
    }

    /************************************************************************
     *  Add the series name to the Shows table
     ************************************************************************/

    //  Is this show already in the dBase ?
    create_rc = dbase_get_show( &show );

    //  Is this show already in the dBase ?
    if ( create_rc == false )
    {
        //  NO:     Set the show state to "UNKNOWN"
        show.state = DB_SS_OTRR_UNKNOWN;

        //  Now add it to the dBase.
        create_rc = dbase_put_show( &show );

        //  Finally read it back to get the SHOW-ID
        create_rc = dbase_get_show( &show );
    }

    /************************************************************************
     *  Process the data in this file
     ************************************************************************/

    //  Main read data loop
    do
    {
        /**
         *  @param  exists          The episode does exist                  */
        char                            exists;

        //  Read another line from the file
        read_data_p = file_read_text( in_file_fp, 0 );

        //  Was the read successful ?
        if (    ( read_data_p != END_OF_FILE )
             && ( read_data_p != NULL        ) )
        {
            log_write( MID_DEBUG_3, "CREATE__process_file", "%s\n", read_data_p );

            //  Is there an episode number in the input record ?
            if ( isdigit( read_data_p[ ED_EPISODE_NUM_O + ED_EPISODE_NUM_L ] ) != 0 )
            {
                //  NO:     Put something there for the sscanf to work
                read_data_p[ ED_EPISODE_NUM_O + ED_EPISODE_NUM_L ] = '#' ;
            }

            //  Clear the episode information structure.
            memset( &episode, 0x00, sizeof( struct episode_t ) );

            //  Copy the SHOW-ID
            episode.show_id = show.show_id;

            //  Parse the input record for episode information
            sscanf( read_data_p, "%c %s %s %s",
                    &exists, episode.date, episode.number, episode.name );

            //  Is there an episode name available in the input data ?
            if ( strlen( episode.name ) > 0 )
            {
                //  YES:    Now let's get the episode name (title)
                strncpy( episode.name,
                        &read_data_p[ ED_EPISODE_NAME_O ],
                         sizeof( episode.name ) );
            }

            //  Does this episode exist ?
            if ( exists == '*' )
            {
                //  YES:    Mark it as existing
                episode.available = DB_SA_EXISTS;
            }
            else
            {
                //  YES:    Mark it as existing
                episode.available = 0;
            }

            //  We finally get to add the episode to the dBase
            create_rc = dbase_put_episode( &episode );
        }

    }   while( read_data_p != END_OF_FILE );

    /************************************************************************
     *  Function Body
     ************************************************************************/



    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( create_rc );
}

/****************************************************************************/
