

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

void getTerminalInput(char *bufferString, size_t size, char *printString);

void displayErrorMessage(char *errorMessage);

void displaySuccessMessage(char *successMessage);

void displayInvalidMessage(char *invalidMessage);

void displayAllProjectAvailable(char **projectNameList, int totalProject);

void displayAllTaskAvailable(char **searchTaskList, int totalTask);

void displayProjectMenuOptions();

void displayTaskMenuOptions(char *projectName);

void displayModifyTaskMenuOptions();

#endif