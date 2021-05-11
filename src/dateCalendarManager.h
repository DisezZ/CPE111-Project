

#ifndef DATECALENDARMANAGER_H
#define DATECALENDARMANAGER_H

typedef struct _dateList
{
    time_t *pData;
    struct _dateList *pNext;
} DATE_T;

int getWeekendStatus();

void setWeekendStatus();

int getTotalDayOff();

DATE_T *getDateListHead();

int addDateToList(char *dateString);

int removeDateFromList(char *dateSring);

void calculateEndDate(char *startDate, int dayWork, char *endDate, size_t size);

#endif