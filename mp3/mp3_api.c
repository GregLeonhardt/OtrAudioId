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

#define ALLOC_MP3                 ( "ALLOCATE STORAGE FOR MP3" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Functions to determine char type
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <mp3_api.h>            //  API for all mp3_*             PUBLIC
#include "mp3_lib.h"            //  API for all MP3__*            PRIVATE
                                //*******************************************
#include <id3_api.h>            //  API for all id3_*             PUBLIC
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
#define MP3_STATE_IDLE          ( 0 )
#define MP3_STATE_ID3V2         ( 1 )
#define MP3_STATE_MP3_SYNC      ( 2 )
#define MP3_STATE_MP3           ( 3 )
#define MP3_STATE_APETAGEX      ( 4 )
#define MP3_STATE_ID3V1         ( 5 )
#define MP3_STATE_END           ( 6 )
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
 *  Initialize mp3 operations
 *
 *  @param  file_name_p             Pointer to the file name.
 *  @param  fingerprint_p           Pointer to a buffer to store the fingerprint
 *
 *  @return mp3_rc                  TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
mp3_process_file(
    char                        *   file_name_p,
    uint8_t                     *   fingerprint_p
    )
{
    /**
     * @param mp3_rc            Function return code                        */
    int                             mp3_rc;
    /**
     * @param state             Decode state                                */
    int                             state;
    /**
     * @param frame_l           Length of the frame just processed         */
    int                             frame_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    mp3_rc = true;

    //  Initialize SHA1
    sha1_init( &sha1_context );

    //  Initialize the decode state
    state = MP3_STATE_ID3V2;

    /************************************************************************
     *  Process the entire file
     ************************************************************************/

    //  Fill or refill the read data buffer.
    while( MP3__fill_data_buffer( file_name_p ) == true )
    {
        //  What is the current state ?
        switch( state )
        {

            /****************************************************************
             *  ID3v2 Tag area
             ****************************************************************/

            case    MP3_STATE_ID3V2:
            {
                // Process the ID3v2 tag area
                frame_l = id3_process_frame( MP3__rdb_beg_p,
                                              ( MP3__rdb_end_p - MP3__rdb_beg_p ) );

                //  Was something processed ?
                if ( frame_l != 0 )
                {
                    //  YES:    Remove the frame from the buffer
                    MP3__remove_frame( frame_l );
                }

//  @DEBUG
#if 0
log_write( MID_INFO, "mp3_process_file", "TENC: %s\n", id3_get_v2_tag( "TENC" ) );
log_write( MID_INFO, "mp3_process_file", "TCON: %s\n", id3_get_v2_tag( "TCON" ) );
log_write( MID_INFO, "mp3_process_file", "TIT2: %s\n", id3_get_v2_tag( "TIT2" ) );
log_write( MID_INFO, "mp3_process_file", "TPE1: %s\n", id3_get_v2_tag( "TPE1" ) );
log_write( MID_INFO, "mp3_process_file", "TALB: %s\n", id3_get_v2_tag( "TALB" ) );
log_write( MID_INFO, "mp3_process_file", "TYER: %s\n", id3_get_v2_tag( "TYER" ) );
log_write( MID_INFO, "mp3_process_file", "TRCK: %s\n", id3_get_v2_tag( "TRCK" ) );
log_write( MID_INFO, "mp3_process_file", "COMM: %s\n", id3_get_v2_tag( "COMM" ) );
#endif

                //  Move to the next step
                state = MP3_STATE_MP3_SYNC;

            }   break;

            /****************************************************************
             *  MP3 Search for SYNC
             ****************************************************************/

            case    MP3_STATE_MP3_SYNC:
            {
                //  Scan for a MP3 header sync.
                frame_l = MP3__sync_search( MP3__rdb_beg_p,
                                            ( MP3__rdb_end_p - MP3__rdb_beg_p ) );

                //  Are we at the start of a new frame ?
                if ( frame_l != 0 )
                {
                    //  NO:     Remove the pad data from the buffer
                    MP3__remove_frame( frame_l );
                }

                //  NO:     Move to the next step
                state = MP3_STATE_MP3;

            }   break;

            /****************************************************************
             *  MP3 frame_data
             ****************************************************************/

            case    MP3_STATE_MP3:
            {
                //  Process a possible MP3 frame data
                frame_l = MP3__process_frame( MP3__rdb_beg_p );

                //  Were we at the beginning of a frame ?
                if ( frame_l != 0 )
                {
                    //  YES:    Remove the frame from the buffer
                    MP3__remove_frame( frame_l );
                }
                else
                {
                    //  NO:     Scan for a MP3 header sync.
                    frame_l = MP3__sync_search( MP3__rdb_beg_p,
                                              ( MP3__rdb_end_p - MP3__rdb_beg_p ) );

                    //  Did we find one ?
                    if ( frame_l != 0 )
                    {
                        //  YES:    Update the audio checksum
                        sha1_update( &sha1_context, MP3__rdb_beg_p, frame_l );

                        //  Remove the frame data from the buffer
                        MP3__remove_frame( frame_l );
                    }
                    else
                    {
                        //  NO:     Move to the next step
                        state = MP3_STATE_APETAGEX;
                    }
                }

            }   break;

            /****************************************************************
             *  APE tag extended
             ****************************************************************/

            case    MP3_STATE_APETAGEX:
            {
                //  @ToDo   3   NOTE:
                //          This is a hack.  A function library needs to be
                //          created to process this frame.

                /**
                 *  @param  frame_l         Size of this frame              */
                int                         frame_l;

                //  Set the initial frame size.
                frame_l = 0;

                //  Is there enough buffer space for this frame
                if ( ( MP3__rdb_end_p - MP3__rdb_beg_p ) > 32 )
                {
                    //  Is this the correct frame tag name ?
                    if ( memcmp( MP3__rdb_beg_p, "APETAGEX", 8 ) == 0 )
                    {
                        //  YES:    Get the size of the frame
                        frame_l  = (   ( MP3__rdb_beg_p[ 15 ] << 24 )
                                     + ( MP3__rdb_beg_p[ 14 ] << 16 )
                                     + ( MP3__rdb_beg_p[ 13 ] <<  8 )
                                     + ( MP3__rdb_beg_p[ 12 ]       ) );

                        //  Add the frame header size
                        frame_l += 32;
                    }

                   //  Did we find a frame ?
                    if ( frame_l != 0 )
                    {
                        //  YES:    Update the audio checksum
                        sha1_update( &sha1_context, MP3__rdb_beg_p, frame_l );

                        //  Remove the frame data from the buffer
                        MP3__remove_frame( frame_l );
                    }
                }

                //  Move to the next step
                state = MP3_STATE_ID3V1;

            }   break;

            /****************************************************************
             *  ID3v1 Tag area
             ****************************************************************/

            case    MP3_STATE_ID3V1:
            {
                // Process the ID3v2 tag area
                frame_l = id3_process_frame( MP3__rdb_beg_p,
                                             ( MP3__rdb_end_p - MP3__rdb_beg_p ) );

                //  Was something processed ?
                if ( frame_l != 0 )
                {
                    //  YES:    Remove the frame from the buffer
                    MP3__remove_frame( frame_l );
                }

                //  Move to the next step
                state = MP3_STATE_END;

            }   break;

            /****************************************************************
             *  END
             ****************************************************************/

            case    MP3_STATE_END:
            {
                //  Is there anything still in the buffer ?
                if ( MP3__rdb_beg_p != MP3__rdb_end_p )
                {
                    //  YES:    OOPS.. This shouldn't happen
                    log_write( MID_FATAL, "mp3_process_file",
                                  "Reached the end of the decode but there "
                                  "is still data in the buffer.\n" );
                }

            }   break;
        }
    }

    /************************************************************************
     *  Save the audio fingerprint
     ************************************************************************/

    //  Finalize the SHA1 operation
    sha1_final( &sha1_context, (char*)fingerprint_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Close the input file
    file_close( MP3__in_file_fp );
    MP3__in_file_fp = NULL;

    //  DONE!
    return( mp3_rc );
}

/****************************************************************************/
