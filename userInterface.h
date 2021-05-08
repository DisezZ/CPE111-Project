

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

char *getTerminalInput(char *bufferString, char *printString);

void displayErrorMessage(char *errorMessage);

void displaySuccessMessage(char *successMessage);

void displayInvalidMessage(char *invalidMessage);

void displayAllProjectAvailable(char **projectNameList, int totalProject);

void displayProjectMenuOptions();

void displayTaskMenuOptions(char *projectName);

void displayModifyTaskMenuOptions();

#endif