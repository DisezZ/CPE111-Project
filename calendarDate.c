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
#include <string.h>
#include <time.h>

void addDate(int day,int month,int year,int numberDay);

typedef struct
{
    int day;
    int month;
    int year;
}DATE_T;


int main()
{   /*
                        printf("Enter start task date");
                        printf("Enter how long task is finished");
    printf("Enter holidays");
    printf("Enter normal weekend day");
    */
    char input[128];// user input the data
    DATE_T* date = NULL; 
    int numberDay = 0;// the number of days which tasks will finish
    

    date = (DATE_T*) calloc(1,sizeof(DATE_T));
    if(date == NULL)
	{
		printf("Error! allocating array\n");
		exit(0);
	}

    printf("Enter the start date of tasks (dd/mm/yyyy): ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%d/%d/%d",&date->day,&date->month,&date->year);

    printf("Enter the number of days which tasks will finish: ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%d",&numberDay);

    addDate(date->day,date->month,date->year,numberDay);
    free(date);
}

/*
This function is called addDate which does not return any value to main function.
It will calculate the end date from given start date 
and number of days which tasks will finish by using TIME function and structure.

no return
*/
void addDate(int day,int month,int year,int numberDay)
{
    struct tm countDate;
    int endDay;
    int endMonth;
    int endYear;
  
    memset(&countDate,0,sizeof(struct tm));
    countDate.tm_year = year-1900; 
    countDate.tm_mon = month-1;
    countDate.tm_mday = day;
    countDate.tm_mday = countDate.tm_mday + numberDay-1;
    mktime(&countDate);
    endDay = countDate.tm_mday;
    endMonth = countDate.tm_mon+1;
    endYear = countDate.tm_year+1900;
    printf("The end date of tasks: %.2d/%.2d/%d\n",endDay,endMonth,endYear);
}