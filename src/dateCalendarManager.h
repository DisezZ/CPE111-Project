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

/* Get weekend status 
 * Return:
 *      0 - if working day include weekend
 *      1 - if working day exclude weekend
 * */
int getWeekendStatus();

/* Set weekend status to opposite of current weekend status value
 * */
void setWeekendStatus();

/* Get total day off of project
 * Return:
 *      total day off in list of project
 * */
int getTotalDayOff();

/* Get pointer to head of 
 * Return:
 *      0 - if working day include weekend
 *      1 - if working day exclude weekend
 * */
DATE_T *getDateListHead();

/* Add day off to list by unix timestamp
 * Parameter:
 *      unixTime    - day off in form of unix timestamp at 00:00:00 of that day
 * Return:
 *      0 - memory allocation failed
 *      1 - succesfully added day off to list
 * */
int addDateToList(time_t unixTime);

/* Remove day off from list by unix timestamp
 * Parameter:
 *      unixTime    - day off in form of unix timestamp at 00:00:00 of that day
 * Return:
 *      0 - not found day off with given unix timestamp
 *      1 - succesfully remove day off to list
 * */
int removeDateFromList(time_t unixTime);

/* Free all element of day off list and set weekend status to 0
 * */
void freeDateList();

/* Calculate end date from given starting date by considering day off and weekend
 * Parameter:
 *      startDate   - string of starting date
 *      dayWork     - how many working days
 *      endDate     - string to recieve end date from calculating
 *      size        - size of endDate string
 * */
void calculateEndDate(char *startDate, int dayWork, char *endDate, size_t size);

#endif