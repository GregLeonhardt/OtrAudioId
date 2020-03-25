/*******************************  COPYRIGHT  ********************************/
/*
 *  Author? "Gregory N. Leonhardt"
 *  License? "CC BY-NC 2.0"
 *           "https://creativecommons.org/licenses/by-nc/2.0/"
 *
 ****************************************************************************/

#ifndef DBASE_LIB_H
#define DBASE_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'dbase' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_DBASE
   #define DBASE_EXT
#else
   #define DBASE_EXT            extern
#endif

/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define SQLITE_CMD_L            ( 512 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  db_network          Contains network information                */
DBASE_EXT
struct  network_t                   db_network;
/**
 *  @param  db_station          Contains station information                */
DBASE_EXT
struct  station_t                   db_station;
/**
 *  @param  db_show             Contains show information                   */
DBASE_EXT
struct  show_t                      db_show;
/**
 *  @param  db_episode          Contains episode information                */
DBASE_EXT
struct  episode_t                   db_episode;
/**
 *  @param  db_file             Contains file information                   */
DBASE_EXT
struct  file_t                      db_file;
/**
 *  @param  db_access           Contains SqLite dBase access information    */
DBASE_EXT
sqlite3                         *   otr_db;

//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
int
DBASE__callback(
    void                        *   NotUsed,
    int                             argc,
    char                        **  argv,
    char                        **  azColName
    );
//----------------------------------------------------------------------------
int
DBASE__open(
    );
//---------------------------------------------------------------------------
int
DBASE__get_show(
    struct  show_t              *   show_p
    );
//---------------------------------------------------------------------------
int
DBASE__put_show(
    struct  show_t              *   show_p
    );
//---------------------------------------------------------------------------
int
DBASE__get_episode(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------
struct  list_base_t *
DBASE__get_episode_list(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------
void
DBASE__delete_episode_list(
    struct  list_base_t         *   episode_list_p
    );
//---------------------------------------------------------------------------
int
DBASE__put_episode(
    struct  episode_t           *   episode_p
    );
//---------------------------------------------------------------------------
int
DBASE__update_episode(
    struct  episode_t           *   episode_set_p,
    struct  episode_t           *   episode_where_p
    );
//---------------------------------------------------------------------------
int
DBASE__get_file(
    struct  file_t              *   file_p
    );
//---------------------------------------------------------------------------
struct  list_base_t *
DBASE__get_file_list(
    struct  file_t              *   file_p
    );
//---------------------------------------------------------------------------
void
DBASE__delete_file_list(
    struct  list_base_t         *   file_list_p
    );
//---------------------------------------------------------------------------
int
DBASE__put_file(
    struct  file_t              *   file_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DBASE_LIB_H
