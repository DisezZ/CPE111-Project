/* 
 *  dateCalendarManager.c
 *      This section will handle day off list related 
 *      and also handle weekend too
 *      
 *      Created by ->
 *      Aplusplus Members:
 *          63070503437 Pataraphum
 *          63070503448 Lutee           Deemae  (Lut)
 *          63070503462 Jidapa          
 *          63070503466 Purachet        
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define __USE_XOPEN
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include <time.h>
#include "dateCalendarManager.h"
#include "validateDate.h"

DATE_T *pHead = NULL;       /*head of date list*/
DATE_T *pTail = NULL;       /*tail of date list*/
DATE_T *pCurrent = NULL;    /*current date struct*/
int dateSize = 0;
int weekendStatus = 0; /* 0 means work on weekend, 1 means no work on weekend */

/* Get weekend status 
 * Return:
 *      0 - if working day include weekend
 *      1 - if working day exclude weekend
 * */
int getWeekendStatus()
{
    return weekendStatus;
}

/* Set weekend status to opposite of current weekend status value
 * */
void setWeekendStatus()
{
    weekendStatus = !weekendStatus;
}

/* Get total day off of project
 * Return:
 *      total day off in list of project
 * */
int getTotalDayOff()
{
    return dateSize;
}

/* Get pointer to head of 
 * Return:
 *      0 - if working day include weekend
 *      1 - if working day exclude weekend
 * */
DATE_T *getDateListHead()
{
    return pHead;
}

/* Add day off to list by unix timestamp
 * Parameter:
 *      unixTime    - day off in form of unix timestamp at 00:00:00 of that day
 * Return:
 *      0 - memory allocation failed
 *      1 - succesfully added day off to list
 * */
int addDateToList(time_t unixTime)
{
    struct tm tm = {0};         /*time struct from time.h*/
    DATE_T *pPrev = NULL;       /*previous date struct*/
    DATE_T *pDateTime = NULL;   /*date struct*/
    int i;                      /*count number*/
    int status = 0;             /*status of function*/

    pDateTime = calloc(1, sizeof(DATE_T));
    if (pDateTime)
    {
        pDateTime->unixTime = unixTime;
        pCurrent = pHead;
        if (pCurrent == NULL)
        {
            pCurrent = pHead = pTail = pDateTime;
            status = 1;
            ++dateSize;
        }
        else if (unixTime < pHead->unixTime)
        {
            pDateTime->pNext = pHead;
            pHead = pDateTime;
            status = 1;
            ++dateSize;
        }
        else if (unixTime > pTail->unixTime)
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
                if (unixTime > pPrev->unixTime && unixTime < pCurrent->unixTime)
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

/* Remove day off from list by unix timestamp
 * Parameter:
 *      unixTime    - day off in form of unix timestamp at 00:00:00 of that day
 * Return:
 *      0 - not found day off with given unix timestamp
 *      1 - succesfully remove day off to list
 * */
int removeDateFromList(time_t unixTime)
{
    DATE_T *pPrev = NULL;   /*previous date struct*/
    DATE_T *pFound = NULL;  /*date struct that found by search*/
    int status = 0;

    if (pHead)
    {
        if (unixTime == pHead->unixTime)
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
                if (unixTime == pCurrent->unixTime)
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

/* Free all element of day off list and set weekend status to 0
 * */
void freeDateList()
{
    DATE_T *pCurrent = pHead;   /*head of date list*/
    DATE_T *pPrev = NULL;       /*previous date struct*/
    while (pCurrent)
    {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
        free(pPrev);
    }
    weekendStatus = 0;
    pHead = NULL;
    pTail = NULL;
}

/* Calculate end date from given starting date by considering day off and weekend
 * Parameter:
 *      startDate   - string of starting date
 *      dayWork     - how many working days
 *      endDate     - string to recieve end date from calculating
 *      size        - size of endDate string
 * */
void calculateEndDate(char *startDate, int dayWork, char *endDate, size_t size)
{
    int status = 0;                             /*function status*/
    int one_day = 86400;                        /*sec in one day*/
    char dateCheck[16];                         /*name of day*/
    struct tm tm = {0};                         /*time struct from time.h*/
    struct tm *ptm = NULL;                      /*time struct from time.h*/
    time_t unixTime;                            /*time struct from time.h*/
    strptime(startDate, "%d/%m/%Y ", &tm);
    unixTime = mktime(&tm);
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
            if (unixTime == pCurrent->unixTime)
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