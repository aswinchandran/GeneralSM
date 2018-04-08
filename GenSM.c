#include "GenSM.h"


int bSeverityLevel=LOG_LEVEL_INIT;

#define SM_PRINT(level, args ...) \
    do {if ((int)level >= bSeverityLevel) printf(args);} while(0)

SM_HANDLE os_memoryAlloc (SM_UINT32 Length)
{
	return malloc (Length);
}

void os_memoryZero ( void* pMemPtr, SM_UINT32 Length)
{
	memset (pMemPtr,0,Length);
}

void os_memoryFree ( void* pMemPtr, SM_UINT32 Length)
{
	free (pMemPtr);
}

void os_memoryCopy (void* pDstPtr, void* pSrcPtr, SM_UINT32 Size)
{
   memcpy (pDstPtr,pSrcPtr,Size);
}

SM_HANDLE genSM_Create (SM_HANDLE hOS)
{
    TGenSM *pGenSM = NULL;

    pGenSM = os_memoryAlloc (sizeof(TGenSM));
    if (NULL != pGenSM)
    {
		os_memoryZero (pGenSM, sizeof(TGenSM));
        pGenSM->hOS = hOS;
    }
    return (SM_HANDLE)pGenSM;
}

void genSM_Unload (SM_HANDLE hGenSM)
{
    TGenSM      *pGenSM =       (TGenSM*)hGenSM;

    os_memoryFree (hGenSM, sizeof (TGenSM));
}


void genSM_SetDefaults (SM_HANDLE hGenSM, SM_UINT32 uStateNum, SM_UINT32 uEventNum,
                        TGenSM_matrix pMatrix, SM_UINT32 uInitialState, SM_INT8 *pGenSMName, 
                        SM_INT8 **pStateDesc, SM_INT8 **pEventDesc, SM_UINT32 uModuleLogIndex)
{
    TGenSM *pGenSM = (TGenSM*)hGenSM;

    /* set values */
    pGenSM->uStateNum       = uStateNum;
    pGenSM->uEventNum       = uEventNum;
    pGenSM->tMatrix         = pMatrix;
    pGenSM->uCurrentState   = uInitialState;
    pGenSM->pGenSMName      = pGenSMName;
    pGenSM->pStateDesc      = pStateDesc;
    pGenSM->pEventDesc      = pEventDesc;
    pGenSM->uModuleLogIndex = uModuleLogIndex;
    pGenSM->bEventPending   = SM_FALSE;
    pGenSM->bInAction       = SM_FALSE;
}

void genSM_Event (SM_HANDLE hGenSM, SM_UINT32 uEvent, void *pData)
{
    TGenSM *pGenSM = (TGenSM*)hGenSM;
    TGenSM_actionCell *pCell;

    /* sanity check */
	if (pGenSM == NULL)
	{ 
		SM_PRINT(LOG_LEVEL_ERROR,"genSM_Event: Handle is NULL!!\n");
		return;
	}

	SM_PRINT(LOG_LEVEL_INFORMATION,"genSM_Event: module: %d received event %d, prev event %d , pending:%d,uEventNum:%d\n"
																				, pGenSM->uModuleLogIndex
																				, uEvent
																				, pGenSM->uEvent
																				, pGenSM->bEventPending
																				,pGenSM->uEventNum);

    pGenSM->bEventPending = SM_TRUE;

    pGenSM->uEvent = uEvent;
    pGenSM->pData = pData;

    if (SM_TRUE == pGenSM->bInAction)
    {
        SM_PRINT(LOG_LEVEL_INFORMATION, ": module: %d delaying execution of event \n", pGenSM->uModuleLogIndex);
        return;
    }

    while (SM_TRUE == pGenSM->bEventPending)
    {
        /* get the cell pointer for the current state and event */
        pCell = &(pGenSM->tMatrix[ (pGenSM->uCurrentState * pGenSM->uEventNum) + pGenSM->uEvent ]);
        

		SM_PRINT(LOG_LEVEL_INFORMATION, "genSM_Event: module %d <currentState = %d, event = %d> --> nextState = %d\n", pGenSM->uModuleLogIndex, pGenSM->uCurrentState, uEvent, pCell->uNextState);

        /* mark that event execution is in place */
        pGenSM->bInAction = SM_TRUE;

        /* mark that pending event is being handled */
        pGenSM->bEventPending = SM_FALSE;
        
        /* update current state */
        pGenSM->uCurrentState = pCell->uNextState;

        /* run transition function */
        (*(pCell->fAction)) (pGenSM->pData);

        /* mark that event execution is complete */
        pGenSM->bInAction = SM_FALSE;
    }
}

SM_UINT32 genSM_GetCurrentState (SM_HANDLE hGenSM)
{
    TGenSM *pGenSM = (TGenSM*)hGenSM;

	if (pGenSM == NULL)
	{
        SM_PRINT(LOG_LEVEL_ERROR,"genSM_GetCurrentState: Handle is NULL!!\n");
		return 0;
	}
    return pGenSM->uCurrentState;
}
