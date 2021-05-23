#ifndef VALIDATEDATE_H
#define VALIDATEDATE_H

int validateDate(char date[]);

void removeTerminator(char inputString[]);

int checkDateFormat(char inputDate[]);

int checkDatePast(int day, int month, int year);

#endif // !VALIDATEDATE_H