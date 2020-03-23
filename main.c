/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This is the main entry point for the application.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MAIN            ( "ALLOCATE STORAGE FOR MAIN" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <stdlib.h>             //  ANSI standard library.
#include <unistd.h>             //  UNIX standard library.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
#include <main.h>               //  Global stuff for this application
                                //*******************************************
#include <create_api.h>         //  API for all create_*            PUBLIC
#include <add_api.h>            //  API for all add_*               PUBLIC
#include <mp3_api.h>            //  API for all mp3_*               PUBLIC
#include <dbase_api.h>          //  API for all dbase_*             PUBLIC
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    cmd_line_parm_e
{
    CMD_PARM_ID             =   0x01,
    CMD_PARM_OD             =   0x02,
    CMD_PARM_IF             =   0x04,
    CMD_PARM_CREATE         =   0x08,
    CMD_PARM_ADD            =   0x10,
    CMD_PARM_FREE_20        =   0x20,
    CMD_PARM_FREE_40        =   0x40,
    CMD_PARM_FREE_80        =   0x80
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define NO_IF_OR_ID             ( 1 )
#define BOTH_IF_AND_ID          ( 2 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param in_file_name_p    Pointer to the input file name                  */
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p     Pointer to the input directory name             */
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p        Pointer to the output directory name        */
char                        *   out_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param create_dir_name_p     Pointer to the output directory name        */
char                        *   create_dir_name_p;
//----------------------------------------------------------------------------
/**
 * @param cmd_line_parm     Pointer to the input file name                  */
enum    cmd_line_parm_e         in_line_parm;
//---------------------------------------------------------------------------

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return main_rc             TRUE when all parameters are successfully
 *                              processed, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

static
void
help(
    int                             error_id
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    switch( error_id )
    {
        case    NO_IF_OR_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Missing -if or -id     "
                          "Did not find a file name or directory name that "
                          "is to be scanned.\n" );
        }   break;
        case    BOTH_IF_AND_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Both -if and -id found "
                          "Only one of the two may be used.\n" );
        }   break;
    }

    //  Command line options
    log_write( MID_INFO, "main: help",
                  "-if {file_name}          Input file name\n" );
    log_write( MID_INFO, "main: help",
                  "-id {directory_name}     Input directory name\n" );
    log_write( MID_FATAL, "main: help",
                  "-od {directory_name}     Output directory name\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return void                Nothing is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

static
void
command_line(
    int                             argc,
    char                        *   argv[]
      )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the pointers
    in_file_name_p = NULL;
    in_dir_name_p  = NULL;
    out_dir_name_p = NULL;

    /************************************************************************
     *  -if                     Input File
     ************************************************************************/

    //  Does the parameter exist ?
    if ( is_cmd_line_parm( argc, argv, "if" ) == true )
    {
        //  YES:    Set the flag
        in_line_parm |= CMD_PARM_IF;

        //  And get the parameter data.
        in_file_name_p = get_cmd_line_parm( argc, argv, "if" );
    }

    /************************************************************************
     *  -id                     Input Directory
     ************************************************************************/

    //  Does the parameter exist ?
    if ( is_cmd_line_parm( argc, argv, "id" ) == true )
    {
        //  YES:    Set the flag
        in_line_parm |= CMD_PARM_ID;

        //  And get the parameter data.
        in_dir_name_p = get_cmd_line_parm( argc, argv, "id" );
    }

    /************************************************************************
     *  -od                     Output Directory
     ************************************************************************/

    //  Does the parameter exist ?
    if ( is_cmd_line_parm( argc, argv, "od" ) == true )
    {
        //  YES:    Set the flag
        in_line_parm |= CMD_PARM_OD;

        //  And get the parameter data.
        out_dir_name_p = get_cmd_line_parm( argc, argv, "od" );
    }

    /************************************************************************
     *  -create                 CREATE
     ************************************************************************/

    //  Does the parameter exist ?
    if ( is_cmd_line_parm( argc, argv, "create" ) == true )
    {
        //  YES:    Set the flag
        in_line_parm |= CMD_PARM_CREATE;

        //  And get the parameter data.
        create_dir_name_p = get_cmd_line_parm( argc, argv, "create" );
    }

    /************************************************************************
     *  -add                    ADD
     ************************************************************************/

    //  Does the parameter exist ?
    if ( is_cmd_line_parm( argc, argv, "add" ) == true )
    {
        //  YES:    Set the flag
        in_line_parm |= CMD_PARM_ADD;
    }

    /************************************************************************
     *  DEBUG CODE TO SET COMMAND LINE PARAMETERS
     ************************************************************************/

    //  DEBUG DEFAULTS
    if (    ( in_file_name_p       == NULL )
         && ( in_dir_name_p        == NULL )
         && ( create_dir_name_p    == NULL ) )
    {
#if 1
        in_dir_name_p        = "/home/greg/Music";
        in_line_parm |= CMD_PARM_ID;
        out_dir_name_p       = "/tmp";
        in_line_parm |= CMD_PARM_OD;
        in_line_parm |= CMD_PARM_ADD;
#else
        create_dir_name_p    = "/home/greg/share/OTTER-DB/Logs";
        in_line_parm |= CMD_PARM_CREATE;
#endif
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  This is the main entry point for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return                     Zero for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
main(
    int                             argc,
    char                        *   argv[ ]
    )
{
    /**
     * @param main_rc           Return code from called functions.          */
    enum    queue_rc_e              main_rc;
    /**
     * @param input_file_fp     Input File pointer                          */
    FILE                        *   in_file_fp;
    /**
     * @param read_data_p       Pointer to the read data                    */
    char                        *   read_data_p;
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
     * @param source_info_p     Pointer to source data information          */
    struct  source_info_t       *   source_info_p;
    /**
     *  @param  queue_rc        Return code from queue management           */
    enum    queue_rc_e              queue_rc;
    /**
     *  @param  extention       Buffer to hold file extention               */
    char                            extention[ 32 ];

    /************************************************************************
     *  Application Initialization
     ************************************************************************/

    //  Initialize the CommonCode memory process
    token_init( );
    mem_init( );
    store_init( );

    /************************************************************************
     *  Initialize the log process
     ************************************************************************/

    //  Initialize the log handler
    log_init( "MP3_Real-ID.log" );

    //  Mark the start of the run in the log file
    log_write( MID_INFO, "main",
                  "|==============================================|\n" );
    log_write( MID_INFO, "main",
                  "Starting: MP3-Real-ID Version %s %s.\n",
                  __DATE__, __TIME__ );

    //  Log the event
    log_write( MID_INFO, "main",
                  "Log initialization complete.\n" );

    /************************************************************************
     *  Command line processing
     ************************************************************************/

    //  Process the command line parameters
    command_line( argc, argv );

    /************************************************************************
     *  Application Code
     ************************************************************************/

    //  Is this a 'CREATE' operation ?
    if (    ( ( in_line_parm & CMD_PARM_CREATE ) != 0    )
         && ( create_dir_name_p                  != NULL ) )
    {
        //  YES:    Build a new dBase
        main_rc = create_dbase( create_dir_name_p );
    }

    //  Is this a 'ADD' file operation ?
    if (    ( ( in_line_parm & CMD_PARM_ADD ) != 0 )
         && ( ( in_line_parm & CMD_PARM_IF  ) != 0 )
         && (   in_file_name_p                != NULL ) )
    {
        //  YES:    Build a new dBase
        main_rc = add_file( in_file_name_p );
    }

    //  Is this a 'ADD' directory operation ?
    if (    ( ( in_line_parm & CMD_PARM_ADD ) != 0 )
         && ( ( in_line_parm & CMD_PARM_ID  ) != 0 )
         && (   in_dir_name_p                 != NULL ) )
    {
        //  YES:    Build a new dBase
        main_rc = add_dir( in_dir_name_p );
    }

    /************************************************************************
     *  Application Exit
     ************************************************************************/

    //  Mark the end of the run in the log file
    log_write( MID_INFO, "main",
                  "End\n" );
    log_write( MID_INFO, "main",
                  "|==============================================|\n\n" );

    //  DONE!
    return( 0 );
}

/****************************************************************************/
