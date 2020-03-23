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

#define ALLOC_DBASE               ( "ALLOCATE STORAGE FOR DBASE" )

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
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
#include "dbase_lib.h"          //  API for all DBASE__*            PRIVATE
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
 *  Open the database.  If necessary create the database.
 *
 *  @param  flags                   Hor to open the database.
 *
 *  @return dbase_rc                TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_open(
    enum    dbase_open_flags_e      flags
    )
{
    /**
     * @param dbase_rc              Function return code                    */
    int                             dbase_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    dbase_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Open the database
    dbase_rc = DBASE__open( flags );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
/**
 *  Open the database.  If necessary create the database.
 *
 *  @param  flags                   Hor to open the database.
 *
 *  @return dbase_rc                TRUE = Success
 *                                  FALSE = Failure
 *
 *  @note
 *
 ****************************************************************************/

int
dbase_close(
    )
{
    /**
     * @param dbase_rc              Function return code                    */
    int                             dbase_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return code
    dbase_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Close the database
    sqlite3_close( otr_db );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( dbase_rc );
}

/****************************************************************************/
