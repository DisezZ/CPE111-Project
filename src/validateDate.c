#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validateDate.h"
#include "userInterface.h"
#include "dateFunctions.h"

/*
 to remove if input data has terminator
*/
void removeTerminator(char inputString[])
{
    int i = strlen(inputString) - 1;
    while ((inputString[i] == '\n') || (inputString[i] == '\r')) /*remove terminator*/
    {
        inputString[i] = 0;
        i--;
    }
}

/*
 format is valid      return 1
 format is not valid  return 0
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

/* check date is in the past 
    today or future      return 1
    in the past          return 0
*/
int checkDatePast(int day, int month, int year)
{
    int valid = 0;
    int pDay = 0;
    int pMonth = 0;
    int pYear = 0;
    int difference = 0;

    dateToday(&pDay, &pMonth, &pYear);                               /*receive date today data*/
    difference = dateCompare(day, month, year, pDay, pMonth, pYear); /*Compare date*/
    if (difference == 1 || difference == 0)                          /*diference == 1; mean future, diffence == 0; mean today*/
    {
        valid = 1;
    }

    return valid;
}

/*
 valid      return 1
 not valid  return 0
*/
int validateDate(char date[])
{

    int checkMonth = 0;
    int checkDay = 0;
    int checkYear = 0;
    int statusDateFormat = -1;
    int statusDatePast = -1;
    int valid = 0;

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
                valid = 1; //check month
                if (checkMonth >= 1 && checkMonth <= 12)
                {
                    valid = 1; //check days
                    if ((checkDay >= 1 && checkDay <= 31) && (checkMonth == 1 || checkMonth == 3 || checkMonth == 5 || checkMonth == 7 || checkMonth == 8 || checkMonth == 10 || checkMonth == 12))
                        valid = 1;
                    else if ((checkDay >= 1 && checkDay <= 30) && (checkMonth == 4 || checkMonth == 6 || checkMonth == 9 || checkMonth == 11))
                        valid = 1;
                    else if ((checkDay >= 1 && checkDay <= 28) && (checkMonth == 2))
                        valid = 1;
                    else if (checkDay == 29 && checkMonth == 2 && (checkYear % 400 == 0 || (checkYear % 4 == 0 && checkYear % 100 != 0)))
                        valid = 1;
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