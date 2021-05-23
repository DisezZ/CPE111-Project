/*   dateFunctions.c
 *
 *   This module provides the implementation of some functions that 
 *   parse, check and manipulate dates.
 *
 *   Created by Sally Goldin for CPE 111  on 27 August 2008
 *   Updated 1 September 2010
 *   Updated 14 November 2012 to use an enum for error codes
 *   Updated 30 October 2013 to fix problem in error checking
 *   Updated 13 October 2017 to use pointers rather than structures
 *   Updated 22 October 2017 to fix bug due to previous revision.
 *   Updated 27 October 2018 to remove enum 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "dateFunctions.h"
/* We must include our header file so that we will have the
 * the symbols for error codes
 */

/* Number of days in each month. */
int monthDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


/* Local function. , not exposed in the header file.
 * Returns true (1) if 'year' is leap year, else false (0)
 */
int isLeapYear(int year)
{
   if ((year % 4 == 0) && (year % 100 != 0)) 
       return 1;
   else
       return 0;  
}

/* Local function, not exposed in the header file.
 * calculates the difference in days between Jan 1 of 
 * two different years (year1 and year2), and returns that value.
 */ 
long yearDayDifference(int year1, int year2)
{
    int y1, y2, yr;
    long totalDays = 0;
    if (year1 > year2) 
       {
       y1 = year2;
       y2 = year1;
       }
    else
       {
       y1 = year1;
       y2 = year2;
       }
    for (yr = y1; yr < y2; yr++)
       {
       if (isLeapYear(yr))
	   totalDays += 366;
       else
           totalDays += 365;
       }
    return totalDays;
}

/* Local function, not exposed in the header file. 
 * Given a day, month and year, calculates the day in the year
 * of the date (1 to 366)
 */
int yearDay(int day, int month, int year)
{
    int days = 0;
    int m = 0; /* month counter */
     for (m = 0; m < (month - 1); m++)
       {
       days += monthDays[m]; 
       }
    days += day;
    if ((isLeapYear(year)) && (month > 2))
       days += 1; /* adjust for leap year */
    return days;
}   

/***** PUBLIC FUNCTIONS, included in dateFunctions.h ******/

/* Parses a date ('dateString')  in the form dd/mm/yyyy. 
 * If the date is valid, returns DATE_OK and sets the values of the 
 * the three output arguments 'pDay', 'pMonth' and 'pYear'.
 * to match the information in the string.
 * If the date is not valid, returns an enum value indicating 
 * what type of error. See enum above for more information
 */
DATE_STATUS checkDate(char* dateString, int* pDay, int *pMonth, int* pYear)
{
    int d, m, y;   /* temp variables for day, month, year */
    DATE_STATUS status = DATE_OK;
    int i = 0;
    if (strlen(dateString) != 10)
       {
       status = ERR_BADFORMAT;
       }
    else if ((dateString[2] != '/') || (dateString[5] != '/'))  
       {
       status = ERR_BADFORMAT;
       }
    else
       {
       /* check that all other chars are digits */ 
       for (i = 0; (i < 10) && (status == DATE_OK); i++)
           {
	   if ((i == 2) || (i == 5)) 
               continue; /* skip the slashes */
           else if (!isdigit(dateString[i]))
               status = ERR_BADFORMAT; 
           }
       /* If the status is still positive, then we know the format is good */ 
       if (status == DATE_OK)
           {
	   sscanf(dateString,"%d/%d/%d",&d,&m,&y);
           if ((y > 2100) || (y < 1900))
               status = ERR_BADYEAR;
	   else if ((m>12) || (m < 1))
               status = ERR_BADMONTH;
           else 
	       {
	       int maxDay = monthDays[m-1];
               if ((m == 2) && (isLeapYear(y)))
                   maxDay = 29;
	       if ((d < 1) || (d > maxDay))
                   status = ERR_BADDAY;
	       }
           }
       /* if we get to this point and status is still DATE_OK
        * we know that the date is good.
        */
       if (status == DATE_OK)
           {
	   *pYear = y;
	   *pMonth = m;
	   *pDay = d;
           }
       }
    return status;
}
 

/* Calculates and returns the absolute difference in days 
 * between the first and the second date. The code is designed
 * to work correctly where the second date is later than the 
 * first date. If this is not the case, the function calls
 * itself, with the arguments reversed.
 * Arguments: day1   - day of first date
 *            month1 - month of first date
 *            year1  - year of first date
 *            day2   - day of second date
 *            month2 - month of second date
 *            year2  - year of second date
 * Returns absolute number of days difference.
 */
long dateDifference(int day1,int month1,int year1, 
                    int day2,int month2,int year2)
{
    long difference = 0;
    int compareResult = dateCompare(day1,month1,year1,day2,month2,year2);
    if (compareResult > 0)
       {
       difference = dateDifference(day2,month2,year2,day1,month1,year1);
       } 
    else if (compareResult < 0)
       {
       difference = yearDayDifference(year1,year2);
       difference -= yearDay(day1,month1,year1);
       difference += yearDay(day2,month2,year2);
       }
    return difference;  /* will return zero if the dates are the same */
}


/* Sets the pointer for day, month and year 
 * to the correct values for today. */
void dateToday(int* pDay, int *pMonth, int* pYear)
{
    time_t seconds = 0;    
    struct tm * pTimeStruct;

    seconds = time(&seconds);
    pTimeStruct = localtime(&seconds);
    *pDay = pTimeStruct->tm_mday;
    *pMonth = pTimeStruct->tm_mon + 1;
    *pYear = pTimeStruct->tm_year + 1900;
}


/* Compares two dates. Returns 1 if the 
 * first date is later than the second, -1 if the 
 * first date is earlier than the second, and 0 if
 * they are the same.
 * Arguments: day1   - day of first date
 *            month1 - month of first date
 *            year1  - year of first date
 *            day2   - day of second date
 *            month2 - month of second date
 *            year2  - year of second date
 */
int dateCompare(int day1,int month1,int year1, 
                int day2,int month2,int year2)
{
    int compareValue = 0;
    if (year1 > year2)
       {
       compareValue = 1;
       }
    else if (year1 < year2)
       {
       compareValue = -1;
       } 
    else if (month1 > month2)
       {
       compareValue = 1;
       }
    else if (month1 < month2)
       {
       compareValue = -1;
       } 
    else if (day1 > day2)
       {
       compareValue = 1;
       }
    else if (day1 < day2)
       {
       compareValue = -1;
       } 
    /* otherwise they are the same */

    return compareValue;
}    
