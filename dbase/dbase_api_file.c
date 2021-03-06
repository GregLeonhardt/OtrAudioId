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
#include <unistd.h>             //  UNIX standard library.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
#include "dbase_lib.h"          //  API for all DBASE__*            PRIVATE
                                //*******************************************
#include <sqlite3.h>            //  Everything SqLite-3
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
 *  Access the database locate an file name and return it's information
 *
 *  @param  file_p                  FILE information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_get_file(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get the file record.
    sqlite_rc = DBASE__get_file( file_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc );
}

/****************************************************************************/
/**
 *  Access the database locate an file name and return it's information
 *
 *  @param  file_p                  Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

struct  list_base_t *
dbase_get_file_list(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  file_list_p  Pointer to a list of files                     */
    struct  list_base_t         *   file_list_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Get the file list.
    file_list_p = DBASE__get_file_list( file_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( file_list_p );
}

/****************************************************************************/
/**
 *  Delete the file list
 *
 *  @param  file_list_p          The list that is to be deleted.
 *
 *  @return void                    No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
dbase_delete_file_list(
    struct  list_base_t         *   file_list_p
    )
{
    /**
     *  @param  file_p          Pointer to a new file record             */
    struct  file_t              *   file_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Delete the file list
    DBASE__delete_file_list( file_list_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Access the database to insert a new file.
 *
 *  @param  file_p               Show information
 *
 *  @return dbase_rc                TRUE  = Record was     found.
 *                                  FALSE = Record was not found.
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_put_file(
    struct  file_t              *   file_p
    )
{
    /**
     *  @param  sqlite_rc       Return code from a SqLite function call     */
    int                             sqlite_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Put the file record into the database
    sqlite_rc = DBASE__put_file( file_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( sqlite_rc );
}

/****************************************************************************/
