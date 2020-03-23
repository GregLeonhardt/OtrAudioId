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

#define ALLOC_ID3                 ( "ALLOCATE STORAGE FOR ID3" )

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
#include "id3_lib.h"
#include "id3_api.h"            //  API for all ID3__*            PRIVATE
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
 *  Process an ID3 frame
 *
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return id3_rc                  Length of the ID3 frame or header
 *
 *  @note
 *
 ****************************************************************************/

int
id3_process_frame(
    char                        *   frame_data_p,
    int                             frame_data_l
    )
{
    /**
     * @param id3_rc            Function return code                        */
    int                             id3_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    id3_rc = 0;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this an ID3v2 header ?
    if ( memcmp( frame_data_p, "ID3", 3 ) == 0 )
    {
        //  YES:    Copy the header data
        id3_rc = ID3__id3v2( frame_data_p, frame_data_l );
    }
    //  NO:     Is this an ID3v1 Plus header ?
    else if ( memcmp( frame_data_p, "TAG+", 4 ) == 0 )
    {
        //  YES:    Copy the header data
        id3_rc = ID3__id3v1p( frame_data_p, frame_data_l );
    }
    //  NO:     Is this an ID3v1 Plus header ?
    else if ( memcmp( frame_data_p, "TAG", 3 ) == 0 )
    {
        //  YES:    Copy the header data
        id3_rc = ID3__id3v1( frame_data_p, frame_data_l );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( id3_rc );
}

/****************************************************************************/
/**
 *  Get a version 2 tag from the list
 *
 *  @param  file_info_p             Pointer to a file information structure
 *
 *  @return id3_rc                  Length of the ID3 frame or header
 *
 *  @note
 *
 ****************************************************************************/

char    *
id3_get_v2_tag(
    char                        *   tag_mnemonic_p
    )
{
    /**
     * @param tag_data_p        Pointer to the tag's data area              */
    uint8_t                     *   tag_data_p;
    /**
     *  @param  tag_p           Pointer to a tag structure                  */
    struct  tag_t               *   tag_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    tag_data_p = NULL;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( tag_p = list_get_first( id3_list_p );
         tag_p != NULL;
         tag_p = list_get_next( id3_list_p, tag_p ) )
    {
        //  Is this the tag we are looking for ?
        if ( memcmp( tag_mnemonic_p, tag_p->mnemonic, 4 ) == 0 )
        {
            //  Is this a TEXT tag type ?
            if ( tag_p->mnemonic[ 0 ] == 'T' )
            {
                //  YES:    Get TEXT data from the buffer
                tag_data_p = ID3__v2_tag_data( tag_p->data_p, tag_p->length );
            }
            //  Is this a COMMENT tag ?
            else if ( memcmp( tag_mnemonic_p, "COMM", 4 ) == 0 )
            {
                //  YES:    Get TEXT data from the buffer
                tag_data_p = ID3__v2_tag_data( &tag_p->data_p[ 4 ],
                                             ( tag_p->length - 4 ) );
            }

            //  All Done
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tag_data_p );
}

/****************************************************************************/
