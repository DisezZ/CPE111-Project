
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define __USE_XOPEN
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include <time.h>
#include "dateCalendarManager.h"
#include "validate.h"

DATE_T *pHead = NULL;
DATE_T *pTail = NULL;
DATE_T *pCurrent = NULL;
int dateSize = 0;
int weekendStatus = 0; /* 0 means work on weekend, 1 means no work on weekend */

int getWeekendStatus()
{
    return weekendStatus;
}

void setWeekendStatus()
{
    weekendStatus = !weekendStatus;
}

int getTotalDayOff()
{
    return dateSize;
}

DATE_T *getDateListHead()
{
    return pHead;
}

int addDateToList(char *dateString)
{
    time_t *pTime = NULL;
    struct tm tm = {0};
    DATE_T *pPrev = NULL;
    DATE_T *pDateTime = NULL;
    int i;
    int status = 0;

    pTime = calloc(1, sizeof(time_t));
    strptime(dateString, "%d/%m/%Y", &tm);
    *pTime = mktime(&tm);
    pDateTime = calloc(1, sizeof(DATE_T));
    if (pDateTime)
    {
        pDateTime->pData = pTime;
        pCurrent = pHead;
        if (pCurrent == NULL)
        {
            pCurrent = pHead = pTail = pDateTime;
            status = 1;
            ++dateSize;
        }
        else if ((*pTime) < (*pHead->pData))
        {
            pDateTime->pNext = pHead;
            pHead = pDateTime;
            status = 1;
            ++dateSize;
        }
        else if ((*pTime) > (*pTail->pData))
        {
            pTail->pNext = pDateTime;
            pTail = pDateTime;
            status = 1;
            ++dateSize;
        }
        else
        {
            pPrev = pCurrent;
            pCurrent = pCurrent->pNext;
            while (pCurrent)
            {
                if (((*pTime) > (*pPrev->pData)) && ((*pTime) < (*pCurrent->pData)))
                {
                    pPrev->pNext = pDateTime;
                    pDateTime->pNext = pCurrent;
                    status = 1;
                    ++dateSize;
                    break;
                }
                pPrev = pCurrent;
                pCurrent = pCurrent->pNext;
            }
        }
    }
    else
        status = 0;

    return status;
}

int removeDateFromList(char *dateString)
{
    time_t pTime;
    struct tm tm = {0};
    DATE_T *pPrev = NULL;
    DATE_T *pFound = NULL;
    int status = 0;
    strptime(dateString, "%d/%m/%Y", &tm);
    pTime = mktime(&tm);
    if (pHead)
    {
        if (pTime == *pHead->pData)
        {
            pFound = pHead;
            if (pHead == pTail)
                pTail = NULL;
            pHead = pHead->pNext;
            status = 1;
            --dateSize;
            free(pFound);
            pFound = NULL;
        }
        else
        {
            pCurrent = pHead;
            pPrev = pCurrent;
            pCurrent = pCurrent->pNext;
            while (pCurrent)
            {
                if (pTime == (*pCurrent->pData))
                {
                    pFound = pCurrent;
                    pPrev->pNext = pCurrent->pNext;
                    if (pCurrent == pTail)
                    {
                        pTail = pPrev;
                    }
                    free(pFound);
                    pFound = NULL;
                    status = 1;
                    --dateSize;
                    break;
                }
                pPrev = pCurrent;
                pCurrent = pCurrent->pNext;
            }
        }
    }

    return status;
}

void calculateEndDate(char *startDate, int dayWork, char *endDate, size_t size)
{
    int status = 0;
    int one_day = 86400;
    char dateCheck[16];
    struct tm tm = {0};
    struct tm *ptm = NULL;
    time_t unixTime;
    strptime(startDate, "%d/%m/%Y ", &tm);
    unixTime = mktime(&tm);
    //unixTime += dayWork * one_day;
    for (int i = 0; i <= dayWork; i++)
    {
        ptm = NULL;
        ptm = localtime(&unixTime);
        strftime(dateCheck, sizeof(dateCheck), "%a", ptm);
        if (weekendStatus) // not work on weekend
        {
            if (strcmp(dateCheck, "Sat") == 0)
            {
                unixTime += one_day * 2;
            }
            else if (strcmp(dateCheck, "Sun") == 0)
            {
                unixTime += one_day;
            }
        }
        pCurrent = pHead;
        while (pCurrent)
        {
            if (unixTime == *pCurrent->pData)
            {
                unixTime += one_day;
                ptm = NULL;
                ptm = localtime(&unixTime);
                strftime(dateCheck, sizeof(dateCheck), "%a", ptm);
                if (weekendStatus) // not work on weekend
                {
                    if (strcmp(dateCheck, "Sat") == 0)
                    {
                        unixTime += one_day * 2;
                    }
                    else if (strcmp(dateCheck, "Sun") == 0)
                    {
                        unixTime += one_day;
                    }
                }
            }
            pCurrent = pCurrent->pNext;
        }
        if (i != dayWork)
            unixTime += one_day;
    }
    ptm = NULL;
    ptm = localtime(&unixTime);
    strftime(endDate, size, "%d/%m/%Y", ptm);
}