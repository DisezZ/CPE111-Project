/* 
 *  dateCalendarManager.h
 *      This section will handle day off list related 
 *      and also handle weekend too
 *      
 *      This is header file of dateCalendarManager.c functions
 *      using in KMUTT CPE111 Course Project
 *      Created by ->
 *      Aplusplus Members:
 *          63070503437 Pataraphum
 *          63070503448 Lutee           Deemae  (Lut)
 *          63070503462 Jidapa          
 *          63070503466 Purachet        
 * 
 * */

#ifndef DATECALENDARMANAGER_H
#define DATECALENDARMANAGER_H

typedef struct _dateList
{
    time_t unixTime;
    struct _dateList *pNext;
} DATE_T;

int getWeekendStatus();

void setWeekendStatus();

int getTotalDayOff();

DATE_T *getDateListHead();

int addDateToList(time_t unixTime);

int removeDateFromList(time_t unixTime);

void calculateEndDate(char *startDate, int dayWork, char *endDate, size_t size);

#endif