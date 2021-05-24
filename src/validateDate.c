#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validateDate.h"
#include "userInterface.h"
#include "dateFunctions.h"

/*
--> removeTerminator
This function is for removing the terminator '\n' or '\r'
if input data has terminator in the last element.
Parameter   :
    inputString ; the input string to remove the terminator in the last element
*/
void removeTerminator(char inputString[])
{
    int i = strlen(inputString) - 1;
    while ((inputString[i] == '\n') || (inputString[i] == '\r')) /*remove terminator 'n' or 'r'*/
    {
        inputString[i] = 0;
        i--;
    }
}

/*
--> checkDateFormat
This function is for checking the format of date that it must be in dd/mm/yyyy form.

Parameter   :
    inputDate ; The string input of date to check date format
Return  :
    if date form is correct     ;  valid   return 1
    if date form is incorrecnt  ;  valid   return 0
*/
int checkDateFormat(char inputDate[])
{
    int valid = 0; /*for check valid (1 is true, 0 is false)*/

    removeTerminator(inputDate);
    if (strlen(inputDate) == 10 && inputDate[2] == '/' && inputDate[5] == '/') /*check that string of date contain 11 length*/
    {
        for (int i = 0; i < 2; i++) /*check day if it contain 2 digit or not*/
        {
            if (isdigit(inputDate[i]) != 0)
            {
                valid = 1;
            }
            else
            {
                valid = 0;
                break;
            }
        }
        if (valid != 0) //check that valid is still true
        {
            for (int i = 3; i < 5; i++) //check that month contain 2 digit
            {
                if (isdigit(inputDate[i]) != 0)
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
        if (valid != 0) //check that valid is still true
        {
            for (int i = 6; i < 10; i++) // check that year contain 4 digit (element 6 to 9 of inputData)
            {
                if (isdigit(inputDate[i]) != 0)
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

/* 
--> checkDatePast
This function calls dateToday and dateCompare function to check if the input date is in the past or not.

Parameter   :
    day     ;   the day digit from user input
    month   ;   the month digit from user input
    year    ;   the year digit from user input
Return  :
    if date is today or future (difference == 1 is furture , 0 is today)    ; valid  return 1
    if date is in the past (diffence == -1 is the past)                     ; valid  return 0
*/
int checkDatePast(int day, int month, int year)
{
    int valid = 0;      /*Function status*/
    int pDay = 0;       /*day for check difference*/
    int pMonth = 0;     /*month for check difference*/
    int pYear = 0;      /*year for check difference*/
    int difference = 0; /*return value of dateCompare function*/

    dateToday(&pDay, &pMonth, &pYear);                               /*receive date today data*/
    difference = dateCompare(day, month, year, pDay, pMonth, pYear); /*Compare date*/
    if (difference == 1 || difference == 0)                          /*diference == 1; mean future, diffence == 0; mean today*/
    {
        valid = 1;
    }

    return valid;
}

/*
--> validateDate
This function call checkDateFormat and checkDatePast function to check
that the input date form is in dd/mm/yyyy and input date is not the date
that has passed, Then it check that number of day of each month in each
year (check leap year) is correct.

Paramter    :
    date ; The string input of date to validate
Return  :
    if everything is correct     ; valid return 1
    if incorrect                 ; valid return 0
*/
int validateDate(char date[])
{

    int checkMonth = 0;        /*Check validation of month*/
    int checkDay = 0;          /*Check validation of day*/
    int checkYear = 0;         /*Check validation of year*/
    int statusDateFormat = -1; /*return value after call checkDateFormat function*/
    int statusDatePast = -1;   /*return value after call checkDatePast function*/
    int valid = 0;             /*Function status*/

    statusDateFormat = checkDateFormat(date);
    if (statusDateFormat == 0)
    {
        displayInvalidMessage("The date format is invalid, Please type again.");
        valid = 0;
    }
    else if (statusDateFormat == 1)
    {
        sscanf(date, "%d/%d/%d", &checkDay, &checkMonth, &checkYear);
        statusDatePast = checkDatePast(checkDay, checkMonth, checkYear);
        if (statusDatePast == 0)
        {
            displayInvalidMessage("The date is in the past, Please type again.");
            valid = 0;
        }
        else if (statusDatePast == 1)
        {
            //check year
            if ((checkYear >= 1900) && (checkYear <= 9999))
            {
                valid = 1;
                if (checkMonth >= 1 && checkMonth <= 12) //check month that is digit between 1 to 12
                {
                    valid = 1;
                    if ((checkDay >= 1 && checkDay <= 31) && (checkMonth == 1 || checkMonth == 3 || checkMonth == 5 || checkMonth == 7 || checkMonth == 8 || checkMonth == 10 || checkMonth == 12))
                        valid = 1; //Verify the days of the 30-day month.
                    else if ((checkDay >= 1 && checkDay <= 30) && (checkMonth == 4 || checkMonth == 6 || checkMonth == 9 || checkMonth == 11))
                        valid = 1; //Verify the days of the 31-day month.
                    else if ((checkDay >= 1 && checkDay <= 28) && (checkMonth == 2))
                        valid = 1; //Verify the days of the 28-day month.
                    else if (checkDay == 29 && checkMonth == 2 && (checkYear % 400 == 0 || (checkYear % 4 == 0 && checkYear % 100 != 0)))
                        valid = 1; //Verify the days of the 29-day month.
                    else
                    {
                        displayInvalidMessage("Day is invalid");
                        valid = 0;
                    }
                }
                else
                {
                    displayInvalidMessage("Month is invalid");
                    valid = 0;
                }
            }
            else
            {
                displayInvalidMessage("Year is invalid");
            }
        }
    }
    return valid;
}