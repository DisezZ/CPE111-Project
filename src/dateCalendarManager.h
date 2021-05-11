

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