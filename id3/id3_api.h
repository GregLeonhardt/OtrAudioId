/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef ID3_API_H
#define ID3_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'id3' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#ifdef ALLOC_ID3
   #define ID3_EXT
#else
   #define ID3_EXT              extern
#endif
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------


/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define ID3V1_ID_L              (  3 )
#define ID3V1_TITLE_L           ( 30 )
#define ID3V1_ARTIST_L          ( 30 )
#define ID3V1_ALBUM_L           ( 30 )
#define ID3V1_YEAR_L            (  4 )
#define ID3V1_COMMENT_L         ( 28 )
#define ID3V1_COMMENT_EXT_L     ( 30 )
#define ID3V1_TRACK_STORE_L     (  1 )
#define ID3V1_TRACK_NUM_L       (  1 )
#define ID3V1_GENRE_L           (  1 )

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
struct  id3v1_t
{
/**
 *  @param  file_id             == 'ID3'                                    */
    uint8_t                     file_id[      ID3V1_ID_L            ];
/**
 *  @param  title               TITLE:                                      */
    uint8_t                     title[        ID3V1_TITLE_L         ];
/**
 *  @param  artist              ARTIST:                                     */
    uint8_t                     artist[       ID3V1_ARTIST_L        ];
/**
 *  @param  album               ALBUM:                                      */
    uint8_t                     album[        ID3V1_ALBUM_L         ];
/**
 *  @param  year                YEAR:                                       */
    uint8_t                     year[         ID3V1_YEAR_L          ];
/**
 *  @param  comment             COMMENT:                                    */
    uint8_t                     comment[      ID3V1_COMMENT_L       ];
/**
 *  @param  track_stored        Track number stored flag                    */
    uint8_t                     track_stored[ ID3V1_TRACK_STORE_L   ];
/**
 *  @param  track_number        TRACK-NUMBER                                */
    uint8_t                     track_number[ ID3V1_TRACK_NUM_L     ];
/**
 *  @param  genre               GENRE:                                      */
    uint8_t                     genre[        ID3V1_GENRE_L         ];
};
//----------------------------------------------------------------------------
//  Enhanced TAG (TAG+)
struct  id3v1p_t
{
/**
 *  @param  file_id             == 'TAG+'                                    */
    uint8_t                     file_id[      4 ];
/**
 *  @param  title               TITLE:                                      */
    uint8_t                     title[       60 ];
/**
 *  @param  artist              ARTIST:                                     */
    uint8_t                     artist[      60 ];
/**
 *  @param  album               ALBUM:                                      */
    uint8_t                     album[       60 ];
/**
 *  @param  speed               0, 1-slow, 2-medium, 3-fast, 4-hardcore     */
    uint8_t                     speed[        1 ];
/**
 *  @param  genre               GENRE:                                      */
    uint8_t                     genre[       30 ];
/**
 *  @param  start_time          mmm:sss                                     */
    uint8_t                     start_time[   6 ];
/**
 *  @param  end-time            mmm:sss                                     */
    uint8_t                     end_time[     6 ];
};
//----------------------------------------------------------------------------
struct  id3v2_t
{
/**
 *  @param  file_id             == 'ID3'                                    */
    uint8_t                     file_id[ 3 ];
/**
 *  @param  version             Sets the ID3 version number                 */
    uint8_t                     major_version;
    uint8_t                     minor_version;
/**
 *  @param  flags               ID3 flags                                   */
    uint8_t                     flags;
/**
 *  @param  length              Length of the ID3 parameters                */
    uint32_t                    length;
};
//----------------------------------------------------------------------------
struct  id3v2_2_tag_t
{
/**
 *  @param  frame_id            FRAME-ID:                                   */
    uint8_t                     frame_id[ 3 ];
/**
 *  @param  length              Length of this frame                        */
    uint32_t                    length;
/**
 *  @param  flags               ID3 flags                                   */
    uint16_t                    flags;
};
//----------------------------------------------------------------------------
struct  id3v2_3_frame_header_t
{
/**
 *  @param  frame_id            FRAME-ID:                                   */
    uint8_t                     frame_id[ 4 ];
/**
 *  @param  length              Length of this frame                        */
    uint32_t                    length;
/**
 *  @param  flags               ID3 flags                                   */
    uint8_t                     flags[ 2 ];
};
//----------------------------------------------------------------------------
struct  tag_t
{
    /**
     *  @param  mnemonic            Tag mnemonic                            */
    char                            mnemonic[ 5 + 1 ];
    uint32_t                        length;
    char                            flag_1;
    char                            flag_2;
    char                        *   data_p;
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
/**
 *  @param  id3v1_p             ID3v1 information                           */
ID3_EXT
struct  id3v1_t                 id3v1;
//---------------------------------------------------------------------------
/**
 *  @param  id3v1p_p            ID3v1+ information                          */
ID3_EXT
struct  id3v1p_t                id3v1p;
//---------------------------------------------------------------------------
/**
 *  @param  id3v2_p             ID3v2 header                                */
ID3_EXT
struct  id3v2_t                 id3v2;
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
int
id3_process_frame(
    char                        *   frame_data_p,
    int                             frame_data_l
    );
//---------------------------------------------------------------------------
char    *
id3_get_v2_tag(
    char                        *   tag_mnemonic_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    ID3_API_H
