/********************************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 * */

#include <stdio.h>
#include <string.h>
#include "userInterface.h"

void getTerminalInput(char *bufferString, size_t size, char *printString)
{
    printf("%s", printString);
    memset(bufferString, 0, size);
    fgets(bufferString, size, stdin);
    bufferString[strcspn(bufferString, "\n")] = 0;
    //printf("%s\n", bufferString);
    //printf("%ld\n", (size_t)strlen(bufferString));
}

void displayErrorMessage(char *errorMessage)
{
    fprintf(stderr, ">>> Error - %s.\n", errorMessage);
}

void displaySuccessMessage(char *successMessage)
{
    fprintf(stdout, ">>> Success - %s.\n", successMessage);
}

void displayInvalidMessage(char *invalidMessage)
{
    fprintf(stderr, ">> Invalid - %s.\n", invalidMessage);
}

void displayAllProjectAvailable(char **projectNameList, int totalProject)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("Total Project Found: %d\n", totalProject);
    for (int i = 0; i < totalProject; i++)
    {
        printf("Project-%d) %s\n", i + 1, projectNameList[i]);
    }
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

void displayAllTaskAvailable(char **searchTaskList, int totalTask)
{
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("Total Task Found: %d\n", totalTask);
    for (int i = 0; i < totalTask; i++)
    {
        printf("Task-%d) %s\n", i + 1, searchTaskList[i]);
    }
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

void displayProjectMenuOptions()
{
    printf("****************************************\n");
    printf("Here's your 4 options:\n");
    printf("\t(1) Add project\n");
    printf("\t(2) Enter existed project\n");
    printf("\t(3) Delete existed project\n");
    printf("\t(4) Exit\n");
    printf("****************************************\n");
}

void displayTaskMenuOptions(char *projectName)
{
    printf("****************************************\n");
    printf("Project Name: %s\n", projectName);
    printf("Here's your 9 options:\n");
    printf("\t(1) Add task\n");
    printf("\t(2) Modify task\n");
    printf("\t(3) Delete task\n");
    printf("\t(4) Display task\n");
    printf("\t(5) Display all task\n");
    printf("\t(6) Calculate project schedule\n");
    printf("\t(7) Change project name\n");
    printf("\t(8) Back to project selection\n");
    printf("\t(9) Exit\n");
    printf("****************************************\n");
}

void displayModifyTaskMenuOptions()
{
    printf("****************************************\n");
    printf("There are 5 choices of modify task option :\n");
    printf("\t(1) Modify task information\n");
    printf("\t(2) Add edge\n");
    printf("\t(3) Change dependency destination\n");
    printf("\t(4) Delete dependency destination\n");
    printf("\t(5) Back to Task Option Menu\n");
    printf("****************************************\n");
}