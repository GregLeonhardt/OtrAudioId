/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef MP3_API_H
#define MP3_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'mp3' library.
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
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
void
mp3_init(
    );
//---------------------------------------------------------------------------
int
mp3_process_file(
    char                        *   file_name_p,
    uint8_t                     *   fingerprint_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    MP3_API_H
