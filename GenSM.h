#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define SM_TRUE  1
#define SM_FALSE 0

typedef void*				SM_HANDLE;
typedef char                SM_CHAR;
typedef signed char         SM_INT8;
typedef unsigned char       SM_UINT8;
typedef signed short        SM_INT16;
typedef unsigned short      SM_UINT16;
typedef signed int          SM_INT32;
typedef unsigned int        SM_UINT32;
typedef signed long long    SM_INT64;
typedef unsigned long long  SM_UINT64;
typedef unsigned int 		SM_BOOL;

typedef enum
{
    LOG_LEVEL_NONE           =  0,
	LOG_LEVEL_INIT           =  1,
	LOG_LEVEL_INFORMATION    =  2,
	LOG_LEVEL_WARNING        =  3,
	LOG_LEVEL_ERROR          =  4,
	LOG_LEVEL_FATAL_ERROR    =  5,
	LOG_LEVEL_MAX            =  6,
} ELogLevel;

typedef void (*TGenSM_action) (void *pData);


typedef  struct
{
    SM_UINT32       uNextState; /**< next state in transition */
    TGenSM_action   fAction;    /**< action function */
} TGenSM_actionCell;


typedef TGenSM_actionCell *TGenSM_matrix;


typedef struct
{
    SM_HANDLE       hOS;               /**< OS handle */ 
    TGenSM_matrix   tMatrix;           /**< next state/action matrix */
    SM_UINT32       uStateNum;         /**< Number of states in the matrix */
    SM_UINT32       uEventNum;         /**< Number of events in the matrix */
    SM_UINT32       uCurrentState;     /**< Current state */
    SM_UINT32       uEvent;            /**< Last event sent */
    void            *pData;            /**< Last event data */
    SM_BOOL         bEventPending;     /**< Event pending indicator */
    SM_BOOL         bInAction;         /**< Evenet execution indicator */
    SM_UINT32       uModuleLogIndex;   /**< Module index to use for printouts */
    SM_INT8         *pGenSMName;       /**< state machine name */
    SM_INT8         **pStateDesc;      /**< State description strings */
    SM_INT8         **pEventDesc;      /**< Event description strings */
} TGenSM;



SM_HANDLE   genSM_Create (SM_HANDLE hOS);
void        genSM_Unload (SM_HANDLE hGenSM);
void        genSM_Init (SM_HANDLE hGenSM, SM_HANDLE hReport);
void genSM_SetDefaults (SM_HANDLE hGenSM, SM_UINT32 uStateNum, SM_UINT32 uEventNum,
                        TGenSM_matrix pMatrix, SM_UINT32 uInitialState, SM_INT8 *pGenSMName, 
                        SM_INT8 **pStateDesc, SM_INT8 **pEventDesc, SM_UINT32 uModuleLogIndex);
void        genSM_Event (SM_HANDLE hGenSM, SM_UINT32 uEvent, void *pData);
SM_UINT32   genSM_GetCurrentState (SM_HANDLE hGenSM);