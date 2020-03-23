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
/**
 *  @param  MP3__bitrate_table  Table to lookup bitrates                    */
uint16_t                MP3__bitrate_table[ 128 ] =
    {
    //    0    1    2    3    4    5    6    7
    //  V1L1 V1L2 V1L3 V1L4 V2L1 V2L2 V2L3 V2L3         Bitrate Index
           0,   0,   0,   0,   0,   0,   0,   0,    //   0
          32,  32,  32,   0,  31,   8,   8,   0,    //   1
          64,  48,  40,   0,  48,  16,  16,   0,    //   2
          96,  56,  48,   0,  56,  24,  24,   0,    //   3
         128,  64,  56,   0,  64,  32,  32,   0,    //   4
         160,  80,  64,   0,  80,  40,  40,   0,    //   5
         192,  96,  80,   0,  96,  48,  48,   0,    //   6
         224, 112,  96,   0, 112,  56,  56,   0,    //   7
         256, 128, 112,   0, 128,  64,  64,   0,    //   8
         288, 160, 128,   0, 144,  80,  80,   0,    //   9
         320, 192, 160,   0, 160,  96,  96,   0,    //  10
         352, 224, 192,   0, 176, 112, 112,   0,    //  11
         384, 256, 224,   0, 192, 128, 128,   0,    //  12
         416, 320, 256,   0, 224, 144, 144,   0,    //  13
         448, 384, 320,   0, 256, 160, 160,   0,    //  14
           0,   0,   0,   0,   0,   0,   0,   0     //  15
    };
/**
 *  @param  MP3__sample_frequency   Table to lookup bitrates                */
uint32_t                MP3__sample_frequency[ 16 ] =
    {
    //      0      1      2      3
        11025, 12000,  8000,     0,     //  MPEG-2.5    (00)
            0,     0,     0,     0,     //  RESERVED    (01)
        22050, 24000, 16000,     0,     //  MPEG-2      (02)
        44100, 48000, 32000,     0      //  MPEG-1      (03)
};
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/


/****************************************************************************/
/**
 *  Initialize mp3 operations
 *
 *  @param  none
 *
 *  @return none
 *
 *  @note
 *
 ****************************************************************************/

void
MP3__init(
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Initialize the file pointer.
    MP3__in_file_fp = NULL;

    //  Set the beginning pointer of the read data buffer.
    MP3__rdb_beg_p = &MP3__rdb[ 0 ];

    //  Set the ending pointer of the read data buffer.
    MP3__rdb_end_p = &MP3__rdb[ 0 ];

    //  Set the current size of the read data buffer.
    MP3__rdb_l = 0;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Read data from the input file until the read data buffer is full.
 *
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return mp3_rc                  TRUE when there is data in the read
 *                                  data buffer, else FALSE.
 *
 *  @note
 *
 ****************************************************************************/

int
MP3__fill_data_buffer(
    uint8_t                     *   file_name_p
    )
{
    /**
     * @param mp3_rc            Function return code                        */
    int                             mp3_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    mp3_rc = true;

    /************************************************************************
     *  Open the input file
     ************************************************************************/

    //  Is the input file already open ?
    if ( MP3__in_file_fp == NULL )
    {
        //  NO:     Initialize the read data buffer.
        MP3__init( );

        //  Open it.
        MP3__in_file_fp = file_open_read( file_name_p );

        //  Was there a successful file open ?
        if ( MP3__in_file_fp == NULL )
        {
            //  NO:     Can't open the file so we're done.
            log_write( MID_FATAL, "MP3__fill_data_buffer",
                          "File OPEN error: ", stderr );
        }
        else
        {
            //  YES:    Log the file opening
            log_write( MID_INFO, "MP3__fill_data_buffer",
                          "Successfully opened file '%s'\n", file_name_p );
        }
    }

    /************************************************************************
     *  Fill the read data buffer
     ************************************************************************/

    //  While there is room in the data buffer for another read
    while( MP3__rdb_l < ( MP3__RDB_L - MP3__READ_L ) )
    {
        int                             read_data_l;

        //  Read another data buffer
        read_data_l = file_read_data( MP3__in_file_fp,
                                      MP3__rdb_end_p,
                                      MP3__READ_L );

        //  Did we read any data ?
        if ( read_data_l > 0 )
        {
            //  YES:    Adjust the end pointer
            MP3__rdb_end_p = ( MP3__rdb_end_p + read_data_l );

            //  Add the new data length to the buffer length
            MP3__rdb_l += read_data_l;
        }
        else
        {
            //  NO:     Stop reading data
            break;
        }
    }

    /************************************************************************
     *  Buffer empty test
     ************************************************************************/

    //  Is the read data buffer empty ?
    if ( MP3__rdb_l == 0 )
    {
        //  YES:    We are at EOF (End-Of-File)
        mp3_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mp3_rc );

}

/****************************************************************************/
/**
 *  Remove a frame buffer from the main input buffer
 *
 *  @param  frame_l                 Length of the frame to be removed
 *
 *  @return mp3_rc                  Number of bytes removed
 *
 *  @note
 *
 ****************************************************************************/

int
MP3__remove_frame(
    int                             frame_l
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Code
     ************************************************************************/

    //  Remove the frame buffer from the main buffer
    memcpy( &MP3__rdb_beg_p[ 0 ],
            &MP3__rdb_beg_p[ frame_l ],
            ( MP3__rdb_l - frame_l ) );

    //  Adjust the size of the read data buffer
    MP3__rdb_l -= frame_l;

    //  Adjust the buffer end pointer
    MP3__rdb_end_p = &MP3__rdb_beg_p[ MP3__rdb_l ];

    //  Clean the empty buffer segment
    memset( MP3__rdb_end_p, '\0', frame_l );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( frame_l );
}

/****************************************************************************/
/**
 *  Search for a MP3 header sync field
 *
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return mp3_rc                  Length of the MP3 frame
 *
 *  @note
 *
 ****************************************************************************/

int
MP3__sync_search(
    uint8_t                     *   frame_data_p,
    int                             frame_data_l
    )
{
    /**
     * @param mp3_sync_o        Sync offset from the start of the buffer    */
    int                             mp3_sync_o;
    /**
     * @param mp3_rc            MP3 frame header                            */
    uint32_t                        mp3_frame_header;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the MP3 frame size.
    mp3_sync_o = 0;

    /************************************************************************
     *  Frame Sync
     ************************************************************************/

    do
    {
        //  Is this a tag that follows the MP3 data ?
        if (    (   memcmp( &frame_data_p[ mp3_sync_o ], "ID3",      3 ) == 0 )
             || (   memcmp( &frame_data_p[ mp3_sync_o ], "TAG",      3 ) == 0 )
             || (   memcmp( &frame_data_p[ mp3_sync_o ], "TAG+",     4 ) == 0 )
             || (   memcmp( &frame_data_p[ mp3_sync_o ], "APETAGEX", 8 ) == 0 ) )
        {
            //  YES:    We are past all MP3 headers and data.
            break;
        }

        //  Build a possible MP3 frame header
        mp3_frame_header = (   ( frame_data_p[ mp3_sync_o + 0 ] << 24 )
                             | ( frame_data_p[ mp3_sync_o + 1 ] << 16 )
                             | ( frame_data_p[ mp3_sync_o + 2 ] <<  8 )
                             | ( frame_data_p[ mp3_sync_o + 3 ]       ) );

        //  Is it a valid MP3 frame header ?
        if (     ( ( mp3_frame_header & FRAME_SYNC_MASK ) != FRAME_SYNC_MASK )
             ||  ( ( mp3_frame_header & MPEG_ID_MASK    ) ==      0x00080000 )
             ||  ( ( mp3_frame_header & LAYER_MASK      ) ==      0x00000000 )
             ||  ( ( mp3_frame_header & BIT_RATE_MASK   ) ==      0x0000F000 )
             ||  ( ( mp3_frame_header & BIT_RATE_MASK   ) ==      0x00000000 )
             ||  ( ( mp3_frame_header & SAMPLE_MASK     ) ==      0x00000C00 ) )
        {
            //  NO:     This isn't a valid MP3 header!
            mp3_sync_o += 1;
        }
        else
        {
            //  YES:    It's a valid header.
            break;
        }

    }   while( mp3_sync_o < frame_data_l );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mp3_sync_o );
}

/****************************************************************************/
/**
 *  Process a MP3 frame
 *
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return mp3_rc                  Length of the MP3 frame
 *
 *  @note
 *
 ****************************************************************************/

int
MP3__process_frame(
    uint8_t                     *   frame_data_p
    )
{
    /**
     * @param mp3_rc            Function return code                        */
    int                             mp3_rc;
    /**
     * @param mp3_rc            MP3 frame header                            */
    uint32_t                        mp3_frame_header;
    /**
     * @param mp3_valid_frame   Flag showing that the frame is valid        */
    int                             mp3_valid_frame;
    /**
     * @param mp3_frame_size    MP3 frame size                              */
    int                             mp3_frame_size;
    /**
     * @param mp3_version       MP3 version                                 */
    int                             mp3_version;
    /**
     * @param mp3_layer         MP3 layer                                   */
    int                             mp3_layer;
    /**
     * @param protect           MP3 protect                                 */
    int                             mp3_protect;
    /**
     * @param mp3_bitrate_ndx   MP3 bit rate index                          */
    int                             mp3_bitrate_ndx;
    /**
     * @param mp3_bitrate       MP3 bit rate                                */
    int                             mp3_bitrate;
    /**
     * @param mp3_samples       MP3 samples per frame                       */
    int                             mp3_samples;
    /**
     * @param mp3_frequency     MP3 frequency                               */
    int                             mp3_frequency;
    /**
     * @param mp3_padding       MP3 padding                                 */
    int                             mp3_padding;
    /**
     * @param ndx               index into bitrate table                    */
    int                             ndx;
    /**
     * @param sample_per_frame  The number of samples in each frame         */
    int                             sample_per_frame;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the MP3 frame size.
    mp3_frame_size = 0;

    //  Assume the frame is valid
    mp3_valid_frame = true;

    //  Set the frame header pointer.
    mp3_frame_header = (   ( frame_data_p[ 0 ] << 24 )
                         | ( frame_data_p[ 1 ] << 16 )
                         | ( frame_data_p[ 2 ] <<  8 )
                         | ( frame_data_p[ 3 ]       ) );

    /************************************************************************
     *  Frame Sync
     ************************************************************************/

    //  Is this is a valid MP3 frame header ?
    if ( ( mp3_frame_header & FRAME_SYNC_MASK ) != FRAME_SYNC_MASK )
    {
        //  NO:     This isn't a valid MP3 header!
        mp3_valid_frame = false;
    }


    /************************************************************************
     *  MPEG Audio version ID
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the version ID from the header.
        mp3_version = ( ( mp3_frame_header & MPEG_ID_MASK ) >> 19 );

        //  Is there a valid version ID ?
        if ( mp3_version == MPEG_RESERVED )
        {
            //  NO:     This isn't a valid MP3 header!
            mp3_valid_frame = false;
        }
    }

    /************************************************************************
     *  MPEG Layer
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the version ID from the header.
        mp3_layer = ( ( mp3_frame_header & LAYER_MASK ) >> 17 );

        //  Is there a valid version ID ?
        if ( mp3_layer == LAYER_RESERVED )
        {
            //  NO:     This isn't a valid MP3 header!
            mp3_valid_frame = false;
        }
    }

    /************************************************************************
     *  MPEG Protection
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the version ID from the header.
        mp3_protect = ( ( mp3_frame_header & PROTECT_MASK ) >> 16 );

    }

    /************************************************************************
     *  Bit Rate index
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the version ID from the header.
        mp3_bitrate_ndx = ( ( mp3_frame_header & BIT_RATE_MASK ) >> 12 );

        //  Is there a valid version ID ?
        if (    ( mp3_bitrate_ndx == 0x00 )
             || ( mp3_bitrate_ndx == 0x0F ) )
        {
            //  NO:     This isn't a valid MP3 header!
            mp3_valid_frame = false;
        }
    }

    /************************************************************************
     *  Sampling Rate
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the version ID from the header.
        mp3_samples = ( ( mp3_frame_header & SAMPLE_MASK ) >>  10 );

        //  Is there a valid version ID ?
        if ( mp3_samples == SAMPLE_RATE_3 )
        {
            //  NO:     This isn't a valid MP3 header!
            mp3_valid_frame = false;
        }
        else
        {
            //  YES:    Set the sample frequency
            ndx = mp3_samples;
            ndx |= ( mp3_version << 2 );
            mp3_frequency = MP3__sample_frequency[ ndx ];
        }
    }

    /************************************************************************
     *  Padding
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  YES:    Extract the pad bit from the header.
        mp3_padding = ( ( mp3_frame_header & PAD_MASK ) >>  9 );
    }

    /************************************************************************
     *  Bit Rate Table
     ************************************************************************/

    //  Is the frame good to this point ?
    if ( mp3_valid_frame == true )
    {
        //  NDX     |  0   |  BR  |  BR  |  BR  |  BR  | VER  | LYR  | LYR  |

        //  Reset the Index
        ndx = 0;

        //  Add in the MP3 Layer
             if ( mp3_layer == LAYER_1 ) {  ndx |= 0x00; }  // MPEG Layer-1
        else if ( mp3_layer == LAYER_2 ) {  ndx |= 0x01; }  // MPEG Layer-2
        else if ( mp3_layer == LAYER_3 ) {  ndx |= 0x02; }  // MPEG Layer-3

        //  Add in the MPEG Version
             if ( mp3_version == MPEG_1  ){ ndx |= 0x00; }  //  MPEG Version 1
        else if ( mp3_version == MPEG_2  ){ ndx |= 0x04; }  //  MPEG Version 2
        else if ( mp3_version == MPEG_25 ){ ndx |= 0x04; }  //  MPEG Version 2.5

        // Add in the bitrate index
        ndx |= ( mp3_bitrate_ndx << 3 );

        //  Pull the actual bitrate from the bitrate table.
        mp3_bitrate = MP3__bitrate_table[ ndx ];

        //  Set the samples per frame
             if ( ( mp3_layer == LAYER_1 ) && ( mp3_version == MPEG_1  ) ) sample_per_frame =  384;
        else if ( ( mp3_layer == LAYER_1 ) && ( mp3_version == MPEG_2  ) ) sample_per_frame =  384;
        else if ( ( mp3_layer == LAYER_1 ) && ( mp3_version == MPEG_25 ) ) sample_per_frame =  384;
        else if ( ( mp3_layer == LAYER_2 ) && ( mp3_version == MPEG_1  ) ) sample_per_frame = 1152;
        else if ( ( mp3_layer == LAYER_2 ) && ( mp3_version == MPEG_2  ) ) sample_per_frame = 1152;
        else if ( ( mp3_layer == LAYER_2 ) && ( mp3_version == MPEG_25 ) ) sample_per_frame = 1152;
        else if ( ( mp3_layer == LAYER_3 ) && ( mp3_version == MPEG_1  ) ) sample_per_frame = 1152;
        else if ( ( mp3_layer == LAYER_3 ) && ( mp3_version == MPEG_2  ) ) sample_per_frame =  576;
        else if ( ( mp3_layer == LAYER_3 ) && ( mp3_version == MPEG_25 ) ) sample_per_frame =  576;

        //  YES:    Calculate the frame size
        //  Frame Size = ((samplesPerFrame / 8) * (BitRate * 1000) / Sampling Rate) + Padding
        mp3_frame_size = (   (   ( sample_per_frame / 8 )
                               * ( mp3_bitrate * 1000   )
                               / ( mp3_frequency        ) )
                            +      mp3_padding );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mp3_frame_size );
}

/****************************************************************************/
