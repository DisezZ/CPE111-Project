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

char *getTerminalInput(char *bufferString, char *printString)
{
    printf("%s", printString);
    memset(bufferString, 0, sizeof(bufferString));
    fgets(bufferString, sizeof(bufferString), stdin);
    sscanf(bufferString, "%[^\n]", bufferString);
}

void displayErrorMessage(char *errorMessage)
{
    printf(">>> Error - %s.\n", errorMessage);
}

void displaySuccessMessage(char *successMessage)
{
    printf(">>> %s.\n", successMessage);
}

void displayInvalidMessage(char *invalidMessage)
{
    printf(">> Invalid - %s.\n", invalidMessage);
}

void displayAllProjectAvailable(char **projectNameList, int totalProject)
{
    printf("total :%d\n", totalProject);
    for (int i = 0; i < sizeof(projectNameList) / sizeof(char *); i++)
    {
        printf("%d) %s\n", i + 1, projectNameList[i]);
    }
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
    printf("\t(4) Search task\n");
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