/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef DBASE_API_H
#define DBASE_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'dbase' library.
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
#include <main.h>               //  Global stuff for this application
                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *                              Show archive state                          */
#define DB_SS_OTRR_UNKNOWN      ( 0x00000000 )
#define DB_SS_OTRR_COMPLETE     ( 0x00000001 )
#define DB_SS_OTRR_ACCURATE     ( 0x00000002 )
#define DB_SS_OTRR_MAINTAINED   ( 0x00000004 )
//----------------------------------------------------------------------------
/**
 *                              Maximum string size for the following       */
#define DB_COMMAND_L            ( 8192 )
#define DB_NETWORK_NAME_L       (   32 )
#define DB_STATION_NAME_L       (   32 )
#define DB_TITLE_L              (   60 )
#define DB_DATE_L               (   10 )
#define DB_TIME_L               (   10 )
#define DB_NUMBER_L             (    6 )
#define DB_FINGERPRINT_L        (   80 )
#define DB_LOCATION_L           (  256 )
//----------------------------------------------------------------------------
/**
 *                              Recording Quality                           */
#define DB_RQ_UNKNOWN           (          0 )
#define DB_RQ_POOR              (          1 )
#define DB_RQ_FAIR              (          2 )
#define DB_RQ_GOOD              (          3 )
#define DB_RQ_EXCELLENT         (          4 )
//----------------------------------------------------------------------------
/**
 *                              Show availability                           */
#define DB_SA_AVAILABLE         ( 0x00000001 )
#define DB_SA_EXISTS            ( 0x00000002 )
#define DB_SA_HAVE              ( 0x00000004 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *                              Defines how the open is done                */
enum    dbase_open_flags_e
{
    DBASE_OF_INVALID        =   0x00,   //  Invalid use
    DBASE_OF_CREATE         =   0x01,   //  Create a new (empty) database
    DBASE_OF_ACCESS         =   0x02    //  Open for read & write.
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  network_t           Broadcast network information               */
struct   network_t  
{
    /**
     *  @param  network_id          Unique network identifier               */
    int                             network_id;
    /**
     *  @param  name                Network name                            */
    char                            name[           DB_NETWORK_NAME_L   +1  ];
};
//----------------------------------------------------------------------------
/**
 *  @param  station_t           Broadcast station information               */
struct   station_t  
{
    /**
     *  @param  station_id          Unique station identifier               */
    int                             station_id;
    /**
     *  @param  name                Station name                            */
    char                            name[           DB_STATION_NAME_L   +1  ];
};
//----------------------------------------------------------------------------
/**
 *  @param  show_name_t         dBase structure to hold show information    */
struct   show_t     
{
    /**
     *  @param  show_id             Unique show number identifier           */
    int                             show_id;
    /**
     *  @param  name                Show name                               */
    char                            name[           SHOW_TITLE_L         +1  ];
    /**
     *  @param  state               Current archive state of the show       */
    int                             state;
};
//----------------------------------------------------------------------------
/**
 *  @param  episode_t           dBase structure to hold episode information */
struct   episode_t
{
    /**
     *  @param  episode_id          Unique episode identifier               */
    int                             episode_id;
    /**
     *  @param  show_id             Show-ID this episode belongs to         */
    int                             show_id;
    /**
     *  @param  name                Episode name                            */
    char                            name[           DB_TITLE_L          +1  ];
    /**
     *  @param  date                Broadcast date of this episode          */
    char                            date[           DB_DATE_L           +1  ];
    /**
     *  @param  number              Show episode number                     */
    char                            number[         DB_NUMBER_L         +1  ];
    /**
     *  @param  available           Availability of a show                  */
    int                             available;
};
//----------------------------------------------------------------------------
/**
 *  @param  file_t              dBase structure to hold file information    */
struct   file_t    
{
    /**
     *  @param  fingerprint         Fingerprint for this recording          */
    char                            fingerprint[    DB_FINGERPRINT_L    +1  ];
    /**
     *  @param  network_id          Recorded from this network              */
    int                             network_id;
    /**
     *  @param  station_id          Recorded from this station              */
    int                             station_id;
    /**
     *  @param  episode_id          Episode identifier                      */
    int                             episode_id;
    /**
     *  @param  date                Date this file was created              */
    char                            date[           DB_DATE_L           +1  ];
    /**
     *  @param  time                Time this file was created              */
    char                            time[           DB_TIME_L           +1  ];
    /**
     *  @param  quality             Quality of this recording               */
    int                             quality;    
    /**
     *  @param  location            Where this file is located              */
    char                            number[         DB_LOCATION_L       +1  ];
};
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
int
dbase_open(
    enum    dbase_open_flags_e      flags
    );
//---------------------------------------------------------------------------
int
dbase_close(
    );
//---------------------------------------------------------------------------
int
dbase_get_show(
    struct  show_t              *   show_p
    );
//---------------------------------------------------------------------------
int
dbase_put_show(
    struct  show_t              *   show_p
    );
//---------------------------------------------------------------------------
int
dbase_get_episode(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------
struct  list_base_t *
dbase_get_episode_list(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------
void
dbase_delete_episode_list(
    struct  list_base_t         *   episode_list_p
    );
//---------------------------------------------------------------------------
int
dbase_put_episode(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DBASE_API_H
