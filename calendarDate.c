/*
 *  calendarDate.c
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int weekendStatus = 0;

typedef struct
{
    int day;
    int month;
    int year;
} DATE_T;

/*
This function is called addDate which does not return any value.
It will calculate the end date from given start date 
and number of days which tasks will finish by using TIME function and structure.

no return
*/
void addDate(int day, int month, int year, int numberDay)
{
    struct tm countDate;
    int endDay = 0;   // day of end tasks
    int endMonth = 0; // month of end tasks
    int endYear = 0;  // year of end tasks

    memset(&countDate, 0, sizeof(struct tm));
    countDate.tm_year = year - 1900;
    countDate.tm_mon = month - 1;
    countDate.tm_mday = day;
    countDate.tm_mday = countDate.tm_mday + numberDay - 1;
    mktime(&countDate);
    endDay = countDate.tm_mday;
    endMonth = countDate.tm_mon + 1;
    endYear = countDate.tm_year + 1900;
    printf("The end date of tasks: %.2d/%.2d/%d\n", endDay, endMonth, endYear);
}

int main()
{
    char input[128]; // user input the data
    DATE_T *date = NULL;
    int numberDay = 0; // the number of days which tasks will finish

    date = (DATE_T *)calloc(1, sizeof(DATE_T));
    if (date == NULL)
    {
        printf("Error! allocating array\n");
        exit(0);
    }

    printf("Enter the start date of tasks (dd/mm/yyyy): ");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d/%d/%d", &date->day, &date->month, &date->year);

    printf("Enter the number of days which tasks will finish: ");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &numberDay);

    addDate(date->day, date->month, date->year, numberDay);
    free(date);
}

/*
    start new function here
*/

void setWeekend()
{
    weekendStatus = !weekendStatus;
}

int addDayOff()
{
    
}

/*
 format is valid      return 1
 format is not valid  return 0
*/
int checkDateFormat()
{

}



/* check date is in the past 
    today or future      return 1
    in the past          return 0
*/
int checkDatePast()
{



}




/*
 valid      return 1
 not valid  return 0
*/
int validateDay(char date[])
{
    
    int checkMonth = 0;
    int checkDay = 0;   
    int checkYear = 0;
    int statusDateFormat = -1;
    int statusDatePast = -1;
    

    statusDateFormat = checkDateFormat(date);
    if(statusDateFormat == 0)
    {
        printf("The date format is not valid.\n");
    }
    else
    {
        sscanf(date,"%d/%d/%d",&checkDay,&checkMonth,&checkYear);
        statusDatePast = checkDatePast(checkDay,checkMonth,checkYear);
        if(statusDatePast == 0)
        {
            printf("The date is in the past.\n");
        }
    }
    //check year
    if((checkYear >= 1900) && (checkYear <= 9999))
    {
        //check month
        if(checkMonth>=1 && checkMonth<=12)
        {
            //check days
            if((checkDay>=1 && checkDay<=31) && (checkMonth==1 || checkMonth==3 || checkMonth==5 || checkMonth==7 || checkMonth==8 || checkMonth==10 || checkMonth==12))
                printf("Date is valid.\n");
            else if((checkDay>=1 && checkDay<=30) && (checkMonth==4 || checkMonth==6 || checkMonth==9 || checkMonth==11))
                printf("Date is valid.\n");
            else if((checkDay>=1 && checkDay<=28) && (checkMonth==2))
                printf("Date is valid.\n");
            else if(checkDay==29 && checkMonth==2 && (checkYear%400==0 ||(checkYear%4==0 && checkYear%100!=0)))
                printf("Date is valid.\n");
            else
                printf("Day is invalid.\n");
        }
        else
        {
            printf("Month is not valid.\n");
        }
    }
    else
    {
        printf("Year is not valid.\n");
    }
}





//1.3 Function for check that dates from user input are correct (check that year is in range of +/- 100 from 2020, check that each month that is not FEB have 30 or 31 day, check if it is leap year or not, if it is leap year FEB will have 29 day not 28 as usual )
int checkLeapYear(int year)
	{
	int leapYear = 0;
	if ( (year%4!=0) || ( (year%4==0) && (year%100==0) && (year%400!=0) ) ) /*check year if it is leap year or not*/
		{
		leapYear = 0;
		}
	else
		{
		leapYear = 1;
		}
	return leapYear;
	}
	
//1.2 Function for check the form to make sure that day contain 2 digit, month contain 2 digit, year contain 4 digit and it has space between them    
int validDateCharacter(char inputDate[])
	{
	int valid = 0; /*for check valid (1 is true, 0 is false)*/
	if(strlen(inputDate) == 10) /*check that string of date contain 11 length*/
		{
		for (int i=0; i<2; i++) /*check day if it contain 2 digit or not*/
			{
			if(isdigit(inputDate[i]) != 0)
				{
				valid = 1;
				}
			else
				{
				valid = 0;
				break;
				}
			}
		if ( (valid != 0) && (inputDate[2]==45) ) //check that valid is still true and inputdate[3] is '-'
			{
			for (int i=3; i<5; i++) //check that month contain 2 digit
				{
				if(isdigit(inputDate[i]) != 0)
					{
					valid = 1;
					}
				else
					{
					valid = 0;
					break;
					}
				}
			}
		if ( (valid != 0) && (inputDate[5]==45) ) //check that valid is still true and date has space between month and year
			{
			for (int i=6; i<10; i++) // check that year contain 4 digit (element 6 to 9 of inputData)
				{
				if(isdigit(inputDate[i]) != 0)
					{
					valid = 1;
					}
				else
					{
					valid = 0;
					break;
					}
				}
			}
		}
	return valid;	
	}
	
//1.1 Function to valid Date
int validDate(char inputDate[])
	{
	int valid = 1; /*for check valid (1 is valid, 0 is invalid)*/
	int day = 0; /*collect value of day*/
	int month = 0; /*collect value of month*/
	int year = 0; /*collect value of year*/
	int leapYear = 0; /*for check leap year (1 is true, 0 is false)*/
	
	sscanf(inputDate,"%d/%d/%d",&day,&month,&year); /*convert string from inputDate to 3 part, then pass value to day(integer), month(string), and year(integer)*/
	leapYear = checkLeapYear(year);
	if (year < 1920 || year > 2120) /*check year if it is in range of + or - 100 year from 2020 or not*/
		{
		valid = 0;
		}
	else if ( (month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)) /*check month (JAN,MAR,MAY,JUL,AUG,OCT,DEC have 31 day)*/
		{
		if (day <= 31)
			{
			valid = 1;
			}
		}
	else if ( (month == 4) || (month == 6 ) || (month == 9) || (month == 11) ) /*check month (APR,JUN,SEP,NOV have 30 day)*/
		{
		if (day <= 30)
			{
			valid = 1;
			}
		}
	else if ( month == 02 ) /*check month (for FEB only)*/
		{
		if ( leapYear == 0) /*If year is not leap year ; FEB have 28 day*/
			{
			if (day <= 28)
				{
				valid = 1;
				}
			}
		else if ( leapYear == 1 ) /*If year is leap year ; FEB have 29 day*/
			{
			if (day <= 29)
				{
				valid = 1;
				}
			}
		}
	return valid;
	}
