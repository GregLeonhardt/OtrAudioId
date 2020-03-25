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
#define ED_EPISODE_NUM_E        14
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
    /**
     *  @param  episode_num_o   Offset to the episode number                */
    int                             episode_num_o;

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

        //  Is this show already in the dBase ?
        if ( create_rc == false )
        {
            //  @ToDo   0014    What to do when dbase_put_show( ) fails.
            log_write( MID_FATAL, "CREATE__process_file",
                       "Failed to create a record for this show\n" );
        }

        //  Finally read it back to get the SHOW-ID
        create_rc = dbase_get_show( &show );

        //  Is this show already in the dBase ?
        if ( create_rc == false )
        {
            //  @ToDo   0015    What to do when dbase_get_show( ) fails.
            log_write( MID_FATAL, "CREATE__process_file",
                       "Failed to read the show record\n" );
        }
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
            //  Log the new record.
            log_write( MID_LOGONLY, "CREATE__process_file", "%s\n", read_data_p );

            //  Is there an episode number in the input record ?
            if ( isdigit( read_data_p[ ED_EPISODE_NUM_O + ED_EPISODE_NUM_L ] ) != 0 )
            {
                //  NO:     Put something there for the sscanf to work
                read_data_p[ ED_EPISODE_NUM_O + ED_EPISODE_NUM_L ] = '#' ;
            }

            //  Clear the episode information structure.
            memset( &episode, 0x00, sizeof( struct episode_t ) );

            /****************************************************************
             *  EPISODE NUMBER
             ****************************************************************/

            //  Copy the SHOW-ID
            episode.show_id = show.show_id;


            /****************************************************************
             *  EPISODE EXISTS
             ****************************************************************/

            //  Does this episode exist ?
            if (    ( strlen( read_data_p ) >= ED_EXISTS_O )
                 && ( read_data_p[ ED_EXISTS_O ] == '*' ) )
            {
                //  YES:    Mark it as existing
                episode.available = DB_SA_EXISTS;
            }
            else
            {
                //  YES:    Mark it as existing
                episode.available = 0;
            }

            /****************************************************************
             *  EPISODE DATE
             ****************************************************************/

            //  Is there a date ?
            if (    ( strlen( read_data_p ) >= ED_DATE_O )
                 && ( isalnum( read_data_p[ ED_DATE_O ] ) != 0 ) )
            {
                //  YES:    Copy the information
                strncpy( episode.date,
                        &read_data_p[ ED_DATE_O ],
                         ED_DATE_L );
            }


            /****************************************************************
             *  EPISODE NUMBER
             ****************************************************************/

            //  @NOTE:  Extra processing here because the EpisodeNumber is
            //          right justified.

            //  Is there an episode number ?
            if (    ( strlen( read_data_p ) >= ED_EPISODE_NUM_E )
                 && ( isdigit( read_data_p[ ED_EPISODE_NUM_E ] ) != 0 ) )
            {
                //  YES:    Find the start of the episode number
                for ( episode_num_o = ED_EPISODE_NUM_O;
                      read_data_p[ episode_num_o ] == ' ';
                      episode_num_o += 1 );

                //  Subtract the starting offset to get the actual offset
                episode_num_o -= ED_EPISODE_NUM_O;

                //  Copy the information
                strncpy( episode.number,
                        &read_data_p[ ED_EPISODE_NUM_O + episode_num_o ],
                        ( ED_EPISODE_NUM_L - episode_num_o ) );
            }

            /****************************************************************
             *  EPISODE TITLE
             ****************************************************************/

            //  Is there an episode title ?
            if (    ( strlen( read_data_p ) >= ED_EPISODE_NAME_O )
                 && ( isprint( read_data_p[ ED_EPISODE_NAME_O ] ) != 0 ) )
            {
                //  YES:    Copy the information
                strncpy( episode.name,
                        &read_data_p[ ED_EPISODE_NAME_O ],
                         sizeof( episode.name ) );
            }

            //  We finally get to add the episode to the dBase
            create_rc = dbase_put_episode( &episode );

            //  Is this show already in the dBase ?
            if ( create_rc == false )
            {
                //  @ToDo   0016    What to do when dbase_put_episode( ) fails.
                log_write( MID_FATAL, "CREATE__process_file",
                           "Failed to create a record for this episode\n" );
            }
        }

    }   while( read_data_p != END_OF_FILE );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Close the input file
    file_close( in_file_fp );


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( create_rc );
}

/****************************************************************************/
