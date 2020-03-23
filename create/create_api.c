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

#define ALLOC_CREATE            ( "ALLOCATE STORAGE FOR CREATE" )

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
#include <create_api.h>         //  API for all create_*            PUBLIC
#include "create_lib.h"         //  API for all CREATE__*           PRIVATE
                                //*******************************************
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
 *  Build (or rebuild) the dBase.
 *
 *  @param  void
 *
 *  @return create_rc               TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
create_dbase(
    char                    *   directory_p
    )
{
    /**
     * @param create_rc         Function return code                        */
    int                             create_rc;
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
    create_rc = true;

    /************************************************************************
     *  Delete everything from the existing dBase
     ************************************************************************/

    //  Remove all existing data then rebuild the tables without any data.
    dbase_open( DBASE_OF_CREATE );

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
            log_write( MID_WARNING, "main",
                          "The file name is too big for the buffer provided. \n" );
            log_write( MID_FATAL, "main",
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
            //  YES:    Is this a ".TXT" file ?
            if ( strncasecmp( &file_ext_p[ 1 ], "txt",  3 ) == 0 )
            {
                //  Process the MP3 file
                CREATE__process_file( input_file_name, file_info_p );
            }
        }
    }

    //  Close the database
    dbase_close( );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( create_rc );
}

/****************************************************************************/
