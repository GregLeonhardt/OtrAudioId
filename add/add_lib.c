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
#include <stdlib.h>             //  ANSI standard library.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <add_api.h>            //  API for all add_*               PUBLIC
#include "add_lib.h"            //  API for all ADD__*              PRIVATE
                                //*******************************************
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
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

struct  episode_id_t    *
ADD__get_id_from_filename(
    char                        *   filename_p
    )
{
    /**
     * @param episode_id_p      Function return code                        */
    struct  episode_id_t        *   episode_id_p;
    /**
     * @param search_p          Pointer to start of file extension          */
    char                        *   search_p;
    /**
     * @param segment_o         Segment offset                              */
    int                             segment_o;
    /**
     * @param episode_num       Episode number as an integer                */
    int                             episode_num;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Allocate space for the structure
    episode_id_p = (struct episode_id_t*)mem_malloc( sizeof( struct episode_id_t ) );

    /************************************************************************
     *  Remove the file extension
     ************************************************************************/

    //  Find the last period in the file name
    search_p = strrchr( filename_p, '.' );

    //  Did we find it ?
    if ( search_p != NULL )
    {
        //  Just NULL terminate the filename here
        search_p[ 0 ] = '\0';
    }
    else
    {
        //  No file extension
        log_write( MID_WARNING, "ADD__get_id_f...",
                      "There is no file extension in the current file\n" );
        log_write( MID_FATAL, "ADD__get_id_f...",
                      "%s\n",filename_p );
    }

    /************************************************************************
     *  Episode Title
     ************************************************************************/

    //  Set the search offset
    segment_o = ( strlen( filename_p ) - 3 );

    do
    {
        //  Is this the segment separator.
        if ( memcmp( &filename_p[ segment_o ], " - ", 3 ) == 0 )
        {
            //  YES:    Copy the episode title
            strncpy( episode_id_p->episode_title,
                     &filename_p[ segment_o + 3 ],
                     sizeof( episode_id_p->episode_title ) );

            //  Just NULL terminate the filename here
            filename_p[ segment_o ] = '\0';

            //  We are done with the episode title.
            break;
        }
        else
        {
            //  NO:     Decrement the offset and try again.
            segment_o -= 1;
        }

    }   while( segment_o > 0 );

    //  Are we at the beginning of the filename ?
    if( segment_o == 0 )
    {
        //  No file extension
        log_write( MID_WARNING, "ADD__get_id_f...", "'%s'\n", filename_p );
        log_write( MID_WARNING, "ADD__get_id_f...",
                      "Improper EpisodeTitle.  The format must be:\n" );
        log_write( MID_FATAL, "ADD__get_id_f...",
                      "Show Title - Episode Number - Episode Air Date - Episode Title\n" );
    }

    /************************************************************************
     *  Episode Number
     ************************************************************************/

    //  Set the search offset
    segment_o = ( strlen( filename_p ) - 3 );

    do
    {
        //  Is this the segment separator.
        if ( memcmp( &filename_p[ segment_o ], " - ", 3 ) == 0 )
        {
            //  YES:    Is this a '(123)' episode format ?
            if ( filename_p[ segment_o + 3 ] == '(' )
            {
                //  YES:    Copy the episode title
                strncpy( episode_id_p->episode_num,
                         &filename_p[ segment_o + 4 ],
                         sizeof( episode_id_p->episode_num ) );

                //  Is there a trailing ')'
                if ( episode_id_p->episode_num[ strlen( episode_id_p->episode_num ) ] == ')' )
                {
                    //  YES:    Remove it.
                    episode_id_p->episode_num[ strlen( episode_id_p->episode_num ) ] = '\0';
                }
            }
            else
            //  Is this a 'ep123' episode format ?
            if (    ( filename_p[ segment_o + 3 ] == 'e' )
                 && ( filename_p[ segment_o + 4 ] == 'p' ) )
            {
                //  YES:    Copy the episode title
                strncpy( episode_id_p->episode_num,
                         &filename_p[ segment_o + 5 ],
                         sizeof( episode_id_p->episode_num ) );
            }
            else
            //  Is this a 'e123' episode format ?
            if ( filename_p[ segment_o + 3 ] == 'e' )
            {
                //  YES:    Copy the episode title
                strncpy( episode_id_p->episode_num,
                         &filename_p[ segment_o + 4 ],
                         sizeof( episode_id_p->episode_num ) );
            }
            else
            //  Is this a '#123' episode format ?
            if ( filename_p[ segment_o + 3 ] == '#' )
            {
                //  YES:    Copy the episode title
                strncpy( episode_id_p->episode_num,
                         &filename_p[ segment_o + 4 ],
                         sizeof( episode_id_p->episode_num ) );
            }
            else
            {
                //  Must be a simple '123' format
                strncpy( episode_id_p->episode_num,
                         &filename_p[ segment_o + 3 ],
                         sizeof( episode_id_p->episode_num ) );
            }

            //  Just NULL terminate the filename here
            filename_p[ segment_o ] = '\0';

            //  We are done with the episode title.
            break;
        }
        else
        {
            //  NO:     Decrement the offset and try again.
            segment_o -= 1;
        }

    }   while( segment_o > 0 );

    //  Are we at the beginning of the filename ?
    if( segment_o == 0 )
    {
        //  No file extension
        log_write( MID_WARNING, "ADD__get_id_f...", "'%s'\n", filename_p );
        log_write( MID_WARNING, "ADD__get_id_f...",
                      "Improper EpisodeNumber.  The format must be:\n" );
        log_write( MID_FATAL, "ADD__get_id_f...",
                      "Show Title - Episode Number - Episode Air Date - Episode Title\n" );
    }
    else
    {
        //  Load the episode number as an integer.
        episode_num = atoi( episode_id_p->episode_num );

        //  Save it without leading zero's
        snprintf( episode_id_p->episode_num, sizeof( episode_id_p->episode_num ),
                  "%d", episode_num );
    }

    /************************************************************************
     *  Episode Date
     ************************************************************************/

    //  Set the search offset
    segment_o = ( strlen( filename_p ) - 3 );

    do
    {
        //  Is this the segment separator.
        if ( memcmp( &filename_p[ segment_o ], " - ", 3 ) == 0 )
        {
            //  YES:    Copy the episode title
            strncpy( episode_id_p->episode_date,
                     &filename_p[ segment_o + 3 ],
                     sizeof( episode_id_p->episode_date ) );

            //  Just NULL terminate the filename here
            filename_p[ segment_o ] = '\0';

            //  We are done with the episode title.
            break;
        }
        else
        {
            //  NO:     Decrement the offset and try again.
            segment_o -= 1;
        }

    }   while( segment_o > 0 );

    //  Are we at the beginning of the filename ?
    if( segment_o == 0 )
    {
        //  No file extension
        log_write( MID_WARNING, "ADD__get_id_f...", "'%s'\n", filename_p );
        log_write( MID_WARNING, "ADD__get_id_f...",
                      "Improper EpisodeDate.  The format must be:\n" );
        log_write( MID_FATAL, "ADD__get_id_f...",
                      "Show Title - Episode Number - Episode Air Date - Episode Title\n" );
    }
    else
    {
        //  Does the date string have a four digit year ?
        //      1)  1949-06-17 || 1949.06.17
        //      2)  19490617
        if (    ( strlen( episode_id_p->episode_date ) > 8 )
             || (    (    ( strlen( episode_id_p->episode_date ) == 8 )
                       && ( strchr( episode_id_p->episode_date, '-' ) == NULL ) )
                  && (    ( strlen( episode_id_p->episode_date ) == 8 )
                       && ( strchr( episode_id_p->episode_date, '.' ) == NULL ) ) ) )
        {
            //  Simply remove the leading two characters
            text_remove( episode_id_p->episode_date, 0, 2 );
        }

        //  Is the remaining format 490617 ?
        if ( strlen( episode_id_p->episode_date ) == 6 )
        {
            //  YES:    Insert the two '/' to set the format as 49/06/17
            text_insert( episode_id_p->episode_date,
                         sizeof( episode_id_p->episode_date ),
                         2, "/" );
            text_insert( episode_id_p->episode_date,
                         sizeof( episode_id_p->episode_date ),
                         5, "/" );
        }

        //  Is the remaining format 49-06-17 ?
        if ( strchr( episode_id_p->episode_date, '-' ) != NULL )
        {
            //  YES:    Replace '-' with '/' to set the format as 49/06/17
            episode_id_p->episode_date[ 2 ] = '/';
            episode_id_p->episode_date[ 5 ] = '/';
        }

        //  Is the remaining format 49.06.17 ?
        if ( strchr( episode_id_p->episode_date, '.' ) != NULL )
        {
            //  YES:    Replace '.' with '/' to set the format as 49/06/17
            episode_id_p->episode_date[ 2 ] = '/';
            episode_id_p->episode_date[ 5 ] = '/';
        }
    }

    /************************************************************************
     *  Show Title
     ************************************************************************/

    //  YES:    Copy the episode title
    strncpy( episode_id_p->show_title,
             &filename_p[ 0 ],
             sizeof( episode_id_p->show_title ) );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( episode_id_p );
}

/****************************************************************************/
