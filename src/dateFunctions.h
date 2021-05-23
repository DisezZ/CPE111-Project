/*   dateFunctions.h
 *
 *   This header file defines data structures and declares functions
 *   that are located in dateFunctions.c. These functions parse, check
 *   and manipulate dates.
 *
 *   Created by Sally Goldin for CPE 111  on 27 August 2008
 *   Updated 14 November 2012 to use enum for error codes.
 *   Updated 13 October 2017 to remove structures since not studied yet
 *   Updated 27 October 2018 to remove enum, just use #define for error codes
 */


/* Define a set of possible errors that can occur in checkDate */
#define  DATE_OK 0
#define  ERR_BADYEAR 1
  /* year must be between 1900 to 2100 inclusive */
#define ERR_BADMONTH  2
  /* month is something other than 1 to 12 */
#define ERR_BADDAY 3   
  /* day is outside of possible range for the month */
#define ERR_BADFORMAT 4 
  /* date string is not ten chars, or not all digits,*/
  /* or does not have  delimiters in the correct locations */

/* define a synonym for the int return value */
typedef int DATE_STATUS;

/* Parses a date ('dateString')  in the form dd/mm/yyyy. 
 * If the date is valid, returns DATE_OK and sets the values of the 
 * the three output arguments 'pDay', 'pMonth' and 'pYear'.
 * to match the information in the string.
 * If the date is not valid, returns an enum value indicating 
 * what type of error. See enum above for more information
 */
DATE_STATUS checkDate(char* dateString, int* pDay, int *pMonth, int* pYear);

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
                    int day2,int month2,int year2);


/* Sets the pointer for day, month and year 
 * to the correct values for today. */
void dateToday(int* pDay, int *pMonth, int* pYear);

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
                int day2,int month2,int year2);

