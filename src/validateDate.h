#ifndef VALIDATEDATE_H
#define VALIDATEDATE_H

/*
--> validateDate
This function call checkDateFormat and checkDatePast function to check
that the input date form is in dd/mm/yyyy and input date is not the date
that has passed, Then it check that number of day of each month in each
year (check leap year) is correct.

Paramter    :
    date ; The string indut of ate to calidate
Return  :
    if everything is correct     ; valid return 1
    if incorrect                 ; valid return 0
*/
int validateDate(char date[]);

/*
--> removeTerminator
This function is for removing the terminator '\n' or '\r'
if input data has terminator in the last element.
Parameter   :
    inputString ; the input string to remove the terminator in the last element
*/
void removeTerminator(char inputString[]);

/*
--> checkDateFormat
This function is for checking the format of date that it must be in dd/mm/yyyy form.

Parameter   :
    inputDate ; The string input of d teto check date format

Return  :
    if date form is correct     ;  valid   return 1
    if date form is incorrecnt  ;  valid   return 0
*/
int checkDateFormat(char inputDate[]);

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
int checkDatePast(int day, int month, int year);

#endif // !VALIDATEDATE_H