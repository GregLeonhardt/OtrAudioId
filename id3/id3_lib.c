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
#include <ctype.h>              //  Functions to determine char type
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <id3_api.h>            //  API for all id3_*             PUBLIC
#include "id3_lib.h"            //  API for all ID3__*            PRIVATE
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
#define SIZEOF_ID3V2_HEADER     ( 10 )
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
 *  Save the tag information
 *
 *  @param  source_data_p       Pointer to the source data
 *  @param  source_data_l       Size of the source data
 *
 *  @return tag_data_p          Pointer to the tag's data
 *
 *  @note
 *
 ****************************************************************************/

uint8_t *
ID3__v2_tag_data(
    uint8_t                     *   source_data_p,
    int                             source_data_l
    )
{
    /**
     * @param tag_data_p        Pointer to the tag's data area              */
    uint8_t                     *   tag_data_p;
    /**
     * @param source_data_o     Offset into the source data a              */
    int                             source_data_o;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    tag_data_p = mem_malloc( source_data_l );

    /************************************************************************
     *  UTF-8
     ************************************************************************/

    //  Is this a TEXT tag type ?
    if ( source_data_p[ 0 ] == 0x00 )
    {
        //  YES:    Copy the data
        memcpy( tag_data_p, &source_data_p[ 1 ], ( source_data_l - 1 ) );
    }

    /************************************************************************
     *  UTF-16
     ************************************************************************/

    //  Is this a TEXT tag type ?
    if (    ( source_data_p[ 0 ] == 0x01 )
         && ( source_data_p[ 1 ] == 0xFF )
         && ( source_data_p[ 2 ] == 0xFE ) )
    {
        //  YES:    Copy the data
        for( source_data_o = 3;
             source_data_o < source_data_l;
             source_data_o += 2 )
        {
            strncat( tag_data_p, &source_data_p[ source_data_o ], 1 );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tag_data_p );
}

/****************************************************************************/
/**
 *  Save the tag information
 *
 *  @param  tag_data_p              Pointer to the raw tag data
 *
 *  @return none
 *
 *  @note
 *
 ****************************************************************************/

void
ID3__v2_save_tag(
    uint8_t                     *   tag_buffer_p
    )
{
    /**
     *  @param  tag_p           Pointer to a tag structure                  */
    struct  tag_t               *   tag_p;
    /**
     *  @param  tag_p           Pointer to a tag structure                  */
    char                            tag_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Get a memory buffer for the tag
    tag_p = (struct tag_t*)mem_malloc( sizeof( struct tag_t ) );

    /************************************************************************
     *  MNEMONIC
     ************************************************************************/

    //  Copy the mnemonic to the structure
    memcpy( tag_p->mnemonic, &tag_buffer_p[ 0 ], 4 );

    /************************************************************************
     *  LENGTH
     ************************************************************************/

    //  Set the tag data length.
    tag_p->length = (   ( tag_buffer_p[ 4 ] << 24 )
                      | ( tag_buffer_p[ 5 ] << 16 )
                      | ( tag_buffer_p[ 6 ] <<  8 )
                      | ( tag_buffer_p[ 7 ]       ) );

    /************************************************************************
     *  FLAGS
     ************************************************************************/

    //  Copy the two flag bytes
    tag_p->flag_1 = tag_buffer_p[ 8 ];
    tag_p->flag_2 = tag_buffer_p[ 9 ];

    /************************************************************************
     *  DATA
     ************************************************************************/

    //  Allocate memory for a data buffer
    tag_p->data_p = mem_malloc( tag_p->length );

    //  Copy the tag data to the buffer
    memcpy( &tag_p->data_p[ 0 ], &tag_buffer_p[ 10 ], tag_p->length );

    /************************************************************************
     *  Put the tag on the tag list
     ************************************************************************/

    //  Add it to the end of the list
    list_put_last( id3_list_p, tag_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Process the ID3v1 header
 *
 *  @param  frame_data_p            Pointer to the ID3 header buffer.
 *  @param  frame_data_l            Size of the frame data buffer.
 *
 *  @return id3_rc                  The size of the ID3v2/3 header.
 *
 *  @note
 *
 ****************************************************************************/

int
ID3__id3v1(
    char                        *   frame_data_p,
    int                             frame_data_l
    )
{
    /**
     *  @param  frame_data_o        Frame Data Offset                       */
    int                             frame_data_o;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set the initial frame data offset
    frame_data_o = 0;

    /************************************************************************
     *  Buffer size check
     ************************************************************************/

    //  Is the complete ID3v1 tag here ?
    if ( frame_data_l < sizeof( struct  id3v1_t ) )
    {
        //  NO:     This is bad..
        log_write( MID_INFO, "ID3__id3v2",
                   "Some or all of the ID3v1 tag information is missing\n" );
        log_write( MID_INFO, "ID3__id3v2",
                   "We should have %d bytes left in the buffer but there are "
                   "only %d bytes remaining.\n",
                        sizeof( struct  id3v1_t ), frame_data_l );
        log_write( MID_FATAL, "ID3__id3v2",
                   "Shutting down...\n" );
    }

    /************************************************************************
     *  Get ID3 version 1 information
     ************************************************************************/

    //  FIELD-ID
    memcpy( id3v1.file_id, &frame_data_p[ frame_data_o ], ID3V1_ID_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_ID_L;

    //  TITLE
    memcpy( id3v1.title, &frame_data_p[ frame_data_o ], ID3V1_TITLE_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_TITLE_L;

    //  ARTIST
    memcpy( id3v1.artist, &frame_data_p[ frame_data_o ], ID3V1_ARTIST_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_ARTIST_L;

    //  ALBUM
    memcpy( id3v1.album, &frame_data_p[ frame_data_o ], ID3V1_ALBUM_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_ALBUM_L;

    //  YEAR
    memcpy( id3v1.year, &frame_data_p[ frame_data_o ], ID3V1_YEAR_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_YEAR_L;

    //  COMMENT
    memcpy( id3v1.comment, &frame_data_p[ frame_data_o ], ID3V1_COMMENT_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_COMMENT_L;

    //  TRACK STORED
    memcpy( id3v1.track_stored, &frame_data_p[ frame_data_o ], ID3V1_TRACK_STORE_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_TRACK_STORE_L;

    //  TRACK NUMBER
    memcpy( id3v1.track_number, &frame_data_p[ frame_data_o ], ID3V1_TRACK_NUM_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_TRACK_NUM_L;

    //  GENRE
    memcpy( id3v1.genre, &frame_data_p[ frame_data_o ], ID3V1_GENRE_L );

    //  Move the offset to past this field
    frame_data_o += ID3V1_GENRE_L;

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sizeof( struct  id3v1_t ) );
}

/****************************************************************************/
/**
 *  Process the ID3v1+ header
 *
 *  @param  frame_data_p            Pointer to the ID3 header buffer.
 *
 *  @return id3_rc                  The size of the ID3v2/3 header.
 *
 *  @note
 *
 ****************************************************************************/

int
ID3__id3v1p(
    char                        *   frame_data_p,
    int                             frame_data_l
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  ID3 version 1
     ************************************************************************/

    //  @ToDo   3   Write function 'ID3__id3v1( )'

    log_write( MID_FATAL, "ID3__id3v1p",
               "ID3v1+ header detected. Shutting down...\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sizeof( struct  id3v1p_t ) );
}

/****************************************************************************/
/**
 *  Process the ID3v2 header
 *
 *  @param  frame_data_p            Pointer to the ID3 header buffer.
 *
 *  @return id3_rc                  The size of the ID3v2/3 header.
 *
 *  @note
 *
 ****************************************************************************/

int
ID3__id3v2(
    char                        *   frame_data_p,
    int                             frame_data_l
    )
{
    /**
     *  @param  id3_length      Size of the current ID3 tag                 */
    int                         id3_length;
    /**
     *  @param  buffer_offset   Offset into the frame buffer                */
    int                         buffer_offset;
    /**
     *  @param  tag_data_p      Pointer to a tag buffer                     */
    char                    *   tag_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Create the file-list
    id3_list_p = list_new( );

    //  Initialize the buffer offset to past the ID3v2 header.
    buffer_offset = SIZEOF_ID3V2_HEADER;

    /************************************************************************
     *  Get and decode the contents of the ID3v2.3 header
     ************************************************************************/

    //  Copy the ID3 identifier tag
    id3v2.file_id[ 0 ] = frame_data_p[ 0 ];
    id3v2.file_id[ 1 ] = frame_data_p[ 1 ];
    id3v2.file_id[ 2 ] = frame_data_p[ 2 ];

    //  Copy the version level
    id3v2.major_version = frame_data_p[ 3 ];
    id3v2.minor_version = frame_data_p[ 4 ];

    //  Copy the flags
    id3v2.flags =  frame_data_p[ 5 ];

    //  Copy the length
    id3v2.length = (   ( (   frame_data_p[ 6 ] & 0x7F ) << 21 )
                       | ( ( frame_data_p[ 7 ] & 0x7F ) << 14 )
                       | ( ( frame_data_p[ 8 ] & 0x7F ) <<  7 )
                       | ( ( frame_data_p[ 9 ] & 0x7F )       ) );

    //  We have to add the header size to the overall length
    id3v2.length += 10;


    //  Do we have all the ID3v2 frame data ?
    if ( id3v2.length > frame_data_l )
    {
        //  NO:     This is bad..
        log_write( MID_INFO, "ID3__id3v2",
                   "The data buffer delivered does not have the entire "
                   "ID3v2 header. \n" );
        log_write( MID_INFO, "ID3__id3v2",
                   "The program needs to be recompiled with a buffer size "
                   "of %d bytes.\n", id3v2.length );
        log_write( MID_FATAL, "ID3__id3v2",
                   "Shutting down...\n" );
    }

    //  Is this an ID3v2.1 header ?
    if ( id3v2.major_version == 1 )
    {
        //  YES:    Set the decode state
        id3_decode_state = ID3DS_V21;
    }

    //  Is this an ID3v2.2 header ?
    else
    if ( id3v2.major_version == 2 )
    {
        //  YES:    Set the decode state
        id3_decode_state = ID3DS_V22;
    }

    //  Is this an ID3v2.3 header ?
    else
    if ( id3v2.major_version == 3 )
    {
        //  YES:    Set the decode state
        id3_decode_state = ID3DS_V23;
    }

    //  Is this an ID3v2.4 header ?
    else
    if ( id3v2.major_version == 4 )
    {
        //  YES:    Set the decode state
        id3_decode_state = ID3DS_V24;
    }

    /************************************************************************
     *  Process the ID3v2 TAGs
     ************************************************************************/

    //  Do we have ID3v2.1 or ID3v2.2 TAGs ?
    if (    ( id3_decode_state == ID3DS_V21 )
         || ( id3_decode_state == ID3DS_V22) )
    {
            //  YES:    Process another tag
            log_write( MID_INFO, "ID3__id3v2",
                       "This file contains ID3v2.1 or ID3v2.2 TAGS\n" );
            log_write( MID_FATAL, "ID3__id3v2",
                       "Shutting down...\n" );
    }
    //  Do we have ID3v2.3 or ID3v2.4 TAGs ?
    else if (    ( id3_decode_state == ID3DS_V23 )
              || ( id3_decode_state == ID3DS_V24 ) )
    {
        //  YES:    TAG processing loop
        do
        {
            //  Process another tag (if one exists)
            id3_length = ID3__v23_frame( &frame_data_p[ buffer_offset ] );

            //  Was there a frame to process ?
            if ( id3_length > 0 )
            {
                //  YES:    Allocate a buffer for the tag
                tag_data_p = mem_malloc( id3_length );

                //  Copy the dag data to the buffer
                memcpy( tag_data_p,
                        &frame_data_p[ buffer_offset ],
                        id3_length );

                //  Put the tag buffer on the list
                list_put_last( id3_list_p, tag_data_p );

                //  Update the buffer offset
                buffer_offset += id3_length;
            }

        }   while( id3_length > 0 );
    }


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( id3v2.length );
}

/****************************************************************************/
/**
 *  Process an ID3v3/4 frame header
 *
 *  @param  frame_buffer_p          Pointer to the ID3 header buffer.
 *
 *  @return id3_rc                  The size of the ID3v2/3 header.
 *
 *  @note
 *
 ****************************************************************************/

int
ID3__v23_frame(
    uint8_t                     *   frame_buffer_p
    )
{
    /**
     * @param frame_l           Size of the frame data                      */
    int                             frame_l;

    struct  id3v2_3_frame_header_t  id3_frame_header;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the frame_data_length
    frame_l = 0;

    /************************************************************************
     *  Copy contents of the ID3v2.3 header
     ************************************************************************/

    if (    ( isalnum( frame_buffer_p[ 0 ]        ) != 0 )
         && ( isalnum( frame_buffer_p[ 1 ]        ) != 0 )
         && ( isalnum( frame_buffer_p[ 2 ]        ) != 0 )
         && ( isalnum( frame_buffer_p[ 3 ]        ) != 0 )
         && (        ( frame_buffer_p[ 8 ] & 0x1F ) == 0 )
         && (        ( frame_buffer_p[ 9 ] & 0x1F ) == 0 ) )
    {
        //  YES:    Copy the frame identifier
        memcpy( id3_frame_header.frame_id , &frame_buffer_p[ 0 ], 4 );

        //  Copy the length
        id3_frame_header.length =  (   ( frame_buffer_p[ 4 ] << 24 )
                                     | ( frame_buffer_p[ 5 ] << 16 )
                                     | ( frame_buffer_p[ 6 ] <<  8 )
                                     | ( frame_buffer_p[ 7 ]       ) );


        //  YES:    Copy the frame flags
        memcpy( &id3_frame_header.flags[ 0 ], &frame_buffer_p[ 8 ], 2 );

        frame_l = id3_frame_header.length + 10;

        //  Save the tag information in a the tag list.
        ID3__v2_save_tag( frame_buffer_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( frame_l );
}

/****************************************************************************/
