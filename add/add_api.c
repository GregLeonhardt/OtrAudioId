/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_bof' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ADD               ( "ALLOCATE STORAGE FOR ADD" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
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
#include <mp3_api.h>            //  API for all mp3_*               PUBLIC
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Public API Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Create a checksum fingerprint for the file then update the dBase with it.
 *
 *  @param  void
 *
 *  @return add_rc                  TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
add_file(
    char                    *   file_p
    )
{
    /**
     * @param   add_rc          Function return code                        */
    int                             add_rc;
    /**
     *  @param  file_info_p     Pointer to a file statistics structure  */
    struct  file_info_t         *   file_info_p;
    /**
     *  @param  input_file_name Buffer to hold the directory/file name      */
    char                            input_file_name[ ( FILE_NAME_L * 3 ) ];
    /**
     * @param   file_ext_p      Pointer to the read data                    */
    char                        *   file_ext_p;
    /**
     *  @param  episode_id_p    Pointer to an episode structure             */
    struct  episode_id_t        *   episode_id_p;
    /**
     *  @param  fingerprint     The audio fingerprint for the current file  */
    unsigned char                   fingerprint[ SHA1_DIGEST_SIZE + sizeof( int ) ];
    /**
     *  @param  show            Contains SHOW table information             */
    struct  show_t                  show;
    /**
     *  @param  episode         Contains EPISODE table information          */
    struct  episode_t               episode;
    /**
     *  @param  file_t          Contains FILE table information             */
    struct  file_t                  file;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    add_rc = true;

    //  Establish a connection to the dBase.
    dbase_open( DBASE_OF_ACCESS );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get the file information for the input file
    file_info_p = file_stat( file_p, file_p );

    //  Will the fully qualified file name will fit in the buffer ?
    if (     (   ( strlen( file_info_p->dir_name  ) )
               + ( strlen( file_info_p->file_name ) ) )
          >= ( sizeof( input_file_name ) ) )
    {
        //  NO:     This is bad..
        log_write( MID_WARNING, "add_file",
                      "The file name is too big for the buffer provided. \n" );
        log_write( MID_FATAL, "add_file",
                      "'%s/%s'\n",
                      file_info_p->dir_name, file_info_p->file_name );
    }
    else
    {
        //  YES:    build the full file name.
        snprintf( input_file_name, sizeof( input_file_name ) - 1,
                   "%s/%s",
                   file_info_p->dir_name, file_info_p->file_name );
    }

    //  Locate the start of the file extention.
    file_ext_p = strrchr( file_info_p->file_name, '.' );

    //  Is there one ?
    if ( file_ext_p != NULL )
    {
        //  YES:    Is this a ".MP3" file ?
            if (    ( strlen( file_ext_p ) == 4 )
                 && ( strncasecmp( &file_ext_p[ 1 ], "mp3",  3 ) == 0 ) )
        {
            //  Pull information from the file name
            episode_id_p = ADD__get_id_from_filename( file_info_p->file_name );

//  @DEBUG
#if 0
{

    strncpy( show.name, episode_id_p->show_title, sizeof( show.name ) );

    if ( dbase_get_show( &show ) == true )
    {
        episode.show_id = show.show_id;
#if 0
        strncpy( episode.name, episode_id_p->episode_title, sizeof( episode.name ) );
        strncpy( episode.date, episode_id_p->episode_date, sizeof( episode.date ) );
        strncpy( episode.number, episode_id_p->episode_num, sizeof( episode.number ) );

        if ( dbase_get_episode( &episode ) == true )
        {
            log_write( MID_INFO, "add_file",
              "====> Located the dBase record\n" );
        }
        else
        {
            log_write( MID_INFO, "add_file",
              "====> Unable to locate a dBase record\n" );
        }
#else
    struct  list_base_t         *   episode_list_p;
    struct  episode_t           *   list_episode_p;

    episode_list_p = dbase_get_episode_list( &episode );

    log_write( MID_INFO, "add_file",
        "Found %d matching records.\n", list_query_count( episode_list_p ) );

    log_write( MID_INFO, "add_file", "\n" );

    //  Loop through the entire list
    for( list_episode_p = list_get_first( episode_list_p );
         list_episode_p != NULL;
         list_episode_p = list_get_next( episode_list_p, list_episode_p ) )
    {
    log_write( MID_INFO, "add_file",
        "%4d - %4d - %04X - %s - %4s - %s\n",
            list_episode_p->show_id,    list_episode_p->episode_id,
            list_episode_p->available,  list_episode_p->date,
            list_episode_p->number,     list_episode_p->name      );
    }

    dbase_delete_episode_list( episode_list_p );

#endif
    }
}
#endif

            //  Process the MP3 file
            add_rc = mp3_process_file( input_file_name, &fingerprint[ 0 ] );
        }
    }

    /************************************************************************
     *  Add the file to the file table
     ************************************************************************/

    //  Initialize
    memset( &show,      0x00, sizeof( struct show_t ) );
    memset( &episode,   0x00, sizeof( struct episode_t ) );
    memset( &file,      0x00, sizeof( struct file_t ) );

    //  Copy the SHOW title to the show query structure
    strncpy( show.name, episode_id_p->show_title, sizeof( show.name ) );

    //  Did we locate the show in the dBase ?
    if ( dbase_get_show( &show ) != true )
    {
        //  NO:     @ToDo   0006   What to do when the ShowTitle isn't found.
        log_write( MID_FATAL, "add_file",
                "SHOW: '%s' is not in the dBase.\n", show.name );
    }
    else
    {
        //  Load all the EPISODE query parameters
        episode.show_id = show.show_id;
        strncpy( episode.name, episode_id_p->episode_title, sizeof( episode.name ) );
        strncpy( episode.date, episode_id_p->episode_date, sizeof( episode.date ) );
        strncpy( episode.number, episode_id_p->episode_num, sizeof( episode.number ) );

        //  Is this episode in the dBase
        if ( dbase_get_episode( &episode ) != true )
        {
            //  NO:     @ToDo   0007   What to do when an Episode* isn't found.
            log_write( MID_FATAL, "add_file",
                    "EPISODE: %8s - EP:%s - '%s' is not in the dBase.\n",
                        episode.date, episode.number, episode.name );
        }
        else
        {
            //  YES:    Add the new file information.
            snprintf( file.fingerprint, sizeof( file.fingerprint ),
                      "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
                      "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                      fingerprint[  0 ], fingerprint[  1 ], fingerprint[  2 ],
                      fingerprint[  3 ], fingerprint[  4 ], fingerprint[  5 ],
                      fingerprint[  6 ], fingerprint[  7 ], fingerprint[  8 ],
                      fingerprint[  9 ], fingerprint[ 10 ], fingerprint[ 11 ],
                      fingerprint[ 12 ], fingerprint[ 13 ], fingerprint[ 14 ],
                      fingerprint[ 15 ], fingerprint[ 16 ], fingerprint[ 17 ],
                      fingerprint[ 18 ], fingerprint[ 19 ],
                      ( add_rc & 0xFF000000 ) >> 24, ( add_rc & 0x00FF0000 ) >> 16,
                      ( add_rc & 0x0000FF00 ) >>  8, ( add_rc & 0x000000FF ) );

            //  This query is only for  fingerprint and episode-id.
            file.network_id = -1;
            file.station_id = -1;
            file.episode_id = episode.episode_id;
            file.quality    = -1;

            //  Is this fingerprint already in the dBase ?
            if ( dbase_get_file( &file ) == true )
            {
                //  NO:     Write a message and continue with the next file.
                log_write( MID_INFO, "add_file", " \n" );
                log_write( MID_INFO, "add_file",
                        "File: %s\n", file_p );
                log_write( MID_INFO, "add_file", "Has the same fingerprint as:\n" );
                log_write( MID_INFO, "add_file",
                        "File: %s\n", file.location );
                log_write( MID_INFO, "add_file", " \n" );
            }
            else
            {
                //  NO:     Add the remaining parts of the record
                file.network_id = -1;
                file.station_id = -1;
                file.episode_id = episode.episode_id;
                strncpy( file.date_time, file_info_p->date_time, sizeof( file.date_time ) );
                file.quality    = -1;
                strncpy( file.location, file_p, sizeof( file.location ) );

                //  Put this fingerprint into the FILE table.
                add_rc = dbase_put_file( &file );
                if ( add_rc != true )
                {
                    //  NO:     @ToDo   0009   What to do when the File* put fails.
                    log_write( MID_FATAL, "add_file",
                            "dbase_put_file failed with RD = %d\n", add_rc );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Close the database
    dbase_close( );

    //  DONE!
    return( add_rc );
}

/****************************************************************************/
/**
 *  Create a list of files in the defined directory then process them.
 *
 *  @param  void
 *
 *  @return add_rc                  TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
add_dir(
    char                    *   directory_p
    )
{
    /**
     * @param add_rc            Function return code                        */
    int                             add_rc;
    /**
     *  @param  file_list       Pointer to a list of files                  */
    struct  list_base_t         *   file_list_p;
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t         *   file_info_p;
    /**
     *  @param  input_file_name Buffer to hold the directory/file name      */
    char                            input_file_name[ ( FILE_NAME_L * 3 ) ];
    /**
     * @param file_ext_p        Pointer to the read data                    */
    char                        *   file_ext_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    add_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Create the file-list
    file_list_p = list_new( );

    //  YES:    Build the file list
    file_ls( file_list_p, directory_p, NULL );


    //  Scan the list
    for( file_info_p = list_get_first( file_list_p );
         file_info_p != NULL;
         file_info_p = list_get_next( file_list_p, file_info_p ) )
    {
        //  Remove it from the list
        list_delete( file_list_p, file_info_p );

        //  Will the fully qualified file name will fit in the buffer ?
        if (     (   ( strlen( file_info_p->dir_name  ) )
                   + ( strlen( file_info_p->file_name ) ) )
              >= ( sizeof( input_file_name ) ) )
        {
            //  NO:     This is bad..
            log_write( MID_WARNING, "add_dir",
                          "The file name is too big for the buffer provided. \n" );
            log_write( MID_FATAL, "add_dir",
                          "'%s/%s'\n",
                          file_info_p->dir_name, file_info_p->file_name );
        }
        else
        {
            //  YES:    build the full file name.
            snprintf( input_file_name, sizeof( input_file_name ) - 1,
                       "%s/%s",
                       file_info_p->dir_name, file_info_p->file_name );
        }

        //  Locate the start of the file extention.
        file_ext_p = strrchr( file_info_p->file_name, '.' );

        //  Is there one ?
        if ( file_ext_p != NULL )
        {
            //  YES:    Is this a ".MP3" file ?
            if (    ( strlen( file_ext_p ) == 4 )
                 && ( strncasecmp( &file_ext_p[ 1 ], "mp3",  3 ) == 0 ) )
            {
                //  Process the MP3 file
                add_file( input_file_name );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( add_rc );
}

/****************************************************************************/
