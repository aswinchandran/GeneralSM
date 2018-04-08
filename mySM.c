
typedef struct
{
    void*       hOS;
    void*       hGenSm;
    int         bRunning;

} TmySm;

TGenSM_actionCell tSmMatrix[ SM_NUMBER_OF_STATES ][ SM_NUMBER_OF_EVENTS ] =
    {
        { /* SM_STATE_2 */
            { SM_STATE_1, myFunc },             /* SM_EVENT_1 */
            { SM_STATE_2, myFunc },          /* SM_EVENT_2 */
            { SM_STATE_2, myFunc },          /* SM_EVENT_3 */
            { SM_STATE_2, myFunc },          /* SM_EVENT_4 */
            { SM_STATE_2, myFunc },          /* SM_EVENT_5 */
            { SM_STATE_2, myFunc },      /* SM_EVENT_6 */
        },
        { /* SM_STATE_1 */
            { SM_STATE_1, myFunc },  /* SM_EVENT_1 */
            { SM_STATE_2, myFunc },                      /* SM_EVENT_2 */
            { SM_STATE_3, myFunc },            /* SM_EVENT_3 */
            { SM_STATE_1, myFunc },  /* SM_EVENT_4 */
            { SM_STATE_1, myFunc },  /* SM_EVENT_5 */
            { SM_STATE_1, myFunc },             /* SM_EVENT_6 */
        },
        { /* SM_STATE_3 */
            { SM_STATE_3, myFunc },      /* SM_EVENT_1 */
            { SM_STATE_4, myFunc },         /* SM_EVENT_2 */
            { SM_STATE_5, myFunc },             /* SM_EVENT_3 */
            { SM_STATE_3, myFunc },      /* SM_EVENT_4 */
            { SM_STATE_1, myFunc },    /* SM_EVENT_5 */
            { SM_STATE_4, myFunc },         /* SM_EVENT_6 */
        },
        { /* SM_STATE_5 */
            { SM_STATE_5, myFunc },    /* SM_EVENT_1 */
            { SM_STATE_4, myFunc },      /* SM_EVENT_2 */
            { SM_STATE_5, myFunc },  /* SM_EVENT_3 */
            { SM_STATE_6, myFunc },       /* SM_EVENT_4 */
            { SM_STATE_1, myFunc },    /* SM_EVENT_5 */
            { SM_STATE_4, myFunc },      /* SM_EVENT_6 */
        },
        { /* SM_STATE_6 */
            { SM_STATE_6, myFunc },     /* SM_EVENT_1 */
            { SM_STATE_4, myFunc },       /* SM_EVENT_2 */
            { SM_STATE_6, myFunc },     /* SM_EVENT_3 */
            { SM_STATE_6, myFunc },     /* SM_EVENT_4 */
            { SM_STATE_1, myFunc },    /* SM_EVENT_5 */
            { SM_STATE_4, myFunc },       /* SM_EVENT_6 */
        },
        { /* SM_STATE_4 */
            { SM_STATE_4, myFunc }, /* SM_EVENT_1 */
            { SM_STATE_4, myFunc },        /* SM_EVENT_2 */
            { SM_STATE_4, myFunc }, /* SM_EVENT_3 */
            { SM_STATE_4, myFunc }, /* SM_EVENT_4 */
            { SM_STATE_1, myFunc },    /* SM_EVENT_5 */
            { SM_STATE_4, myFunc },        /* SM_EVENT_6 */
        }
    };

TI_INT8*  uStateDescription[] =
    {
        "STATE-1",
        "STATE-2",
        "STATE-3",
        "STATE-4",
        "STATE-5",
        "STATE-6"
    };

TI_INT8*  uEventDescription[] =
    {
        "EVENT-1",
        "EVENT-2",
        "EVENT-3",
        "EVENT-4",
        "EVENT-5",
        "EVENT-6"
    };

void myFunc(void* hMySm)
(
    printf("myFunc\n");
)



void* sme_Create (void* hOS)
{
    TmySm    *pMySm;

	if (NULL == pMySm)
    {
        //WLAN_OS_REPORT (("sme_Create: unable to allocate memor yfor SME object. SME craetion failed\n"));
        return NULL;
    }
    os_memoryZero (hOS, (void*)pMySm, sizeof (TmySm));
    pMySm->hOS = hOS;
    pMySm->hGenSm = genSM_Create (hOS);

    if (NULL == pMySm->hGenSm)
    {
        sme_Destroy ((void*)pMySm);
        return NULL;
    }
    return (void*)pMySm;
}


void sme_Destroy (void* hMySm)
{
    TmySm *pMySm = (TmySm*)hMySm;

    if (NULL != pMySm->hGenSm)
    {
        genSM_Unload (pMySm->hGenSm);
    }

    os_memoryFree (hMySm, sizeof (TmySm));
}

void sme_SetDefaults (void* hMySm)
{
    TmySm        *pMySm = (TmySm*)hMySm;

    pMySm->bRunning = TI_FALSE;

    /* Initialize the SME state-machine */
    genSM_SetDefaults (pMySm->hGenSm, SM_NUMBER_OF_STATES, SM_NUMBER_OF_EVENTS, (TGenSM_matrix)tSmMatrix,
                       SM_STATE_1, "SME SM", uStateDescription, uEventDescription, 1);
}

void sme_SmEvent(SM_UINT32 uEvent, void* hMySm)
{
    TmySm *pMySm = (TmySm*)hMySm;
    genSM_Event(pMySm->hGenSm, uEvent, hMySm);
}
