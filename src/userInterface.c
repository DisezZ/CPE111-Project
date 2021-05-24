/* 
 *  userInterface.c
 *      This section will handle almost all of user interface related
 *      and also input too    
 *      
 *      Created by ->
 *      Aplusplus Members:
 *          63070503437 Pataraphum
 *          63070503448 Lutee           Deemae  (Lut)
 *          63070503462 Jidapa          
 *          63070503466 Purachet        
 * 
 * */

#include <stdio.h>
#include <string.h>
#include "abstractNetwork.h"
#include "userInterface.h"

/* print string and get input from terminal into buffer string and
 * Parameter:
 *      bufferString    - buffer string for input from terminal
 *      size            - size of buffer string
 *      printString     - string to print before get get input from terminal
 * */
void getTerminalInput(char *bufferString, size_t size, char *printString)
{
    printf("%s", printString);
    memset(bufferString, 0, size);
    fgets(bufferString, size, stdin);
    bufferString[strcspn(bufferString, "\n")] = 0;
}

/* Using for display error message in the same format
 * Parameter:
 *       errorMessage    - message to diplay when error occured
 * */
void displayErrorMessage(char *errorMessage)
{
    fprintf(stderr, ">>> Error - %s.\n", errorMessage);
}

/* Using for display success message in the same format
 * Parameter:
 *       successMessage    - message to diplay when operation success
 * */
void displaySuccessMessage(char *successMessage)
{
    fprintf(stdout, ">>> Success - %s.\n", successMessage);
}

/* Using for display invalid message in the same format
 * Parameter:
 *       errorMessage    - message to diplay when invalid.
 * */
void displayInvalidMessage(char *invalidMessage)
{
    fprintf(stderr, ">> Invalid - %s.\n", invalidMessage);
}

/* Display all available project from given project's string array
 * Parameter:
 *      projectNameList     - array of all project name
 *      totalProject        - total prroject in string array
 * */
void displayAllProjectAvailable(char **projectNameList, int totalProject)
{
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("Total Project Found: %d\n", totalProject);
    for (int i = 0; i < totalProject; i++)
    {
        printf("Project-%d) %s\n", i + 1, projectNameList[i]);
    }
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

/* Display all task exist in project from given task's string array
 * Parameter:
 *      searchTaskList      - array of all task name
 *      totalTask           - total task in prject
 * */
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

/* Display all task that dependent on given task edge list head
 * Parameter:
 *      pEdge   - pointer to edge list head
 * */
int displayAllDependentOn(EDGE_T *pEdge)
{
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pTempt = pEdge;
    int i = 0;
    if (pTempt)
    {
        while (pTempt)
        {
            pAdjVertex = pTempt->pVertex;
            if (strcmp(pAdjVertex->name, "end") != 0)
            {
                ++i;
            }
            pTempt = pTempt->pNext;
        }
        pTempt = pEdge;
        printf("\n Total Dependent on: %d\n", i + 1);
        printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        i = 0;
        while (pTempt)
        {
            pAdjVertex = pTempt->pVertex;
            if (strcmp(pAdjVertex->name, "end") != 0)
            {
                printf("\t%d) %s\n", i + 1, pAdjVertex->name);
                ++i;
            }
            pTempt = pTempt->pNext;
        }
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    }
    else
        printf("There is no dependent on yet\n");
    return i;
}

/* Display all available menu option in project's part
 * */
void displayProjectMenuOptions()
{
    printf("\n****************************************\n");
    printf("Here's your 4 options:\n");
    printf("\t(1) Add project\n");
    printf("\t(2) Enter existed project\n");
    printf("\t(3) Delete existed project\n");
    printf("\t(4) Exit\n");
    printf("****************************************\n");
}

/* Display all available menu option in task's part
 * Parameter:
 *      projectName         - name of currently working on project
 *      projectDescription  - description of currently working on project  
 *      size                - size of description string
 * */
void displayTaskMenuOptions(char *projectName, char *projectDescription, int size)
{
    printf("\n****************************************\n");
    printf("Project Name: \"%s\"\n", projectName);
    printf("Description:\n");
    for (int i = 0; i < size; i++)
    {
        if (i % 50 == 0)
        {
            if (i != 0)
                printf("\n");
            printf("\t");
        }
        printf("%c", projectDescription[i]);
    }
    printf("\nHere's your 11 options:\n");
    printf("\t(1) Add task\n");
    printf("\t(2) Modify task\n");
    printf("\t(3) Delete task\n");
    printf("\t(4) Display task\n");
    printf("\t(5) Display all task\n");
    printf("\t(6) Calculate project schedule\n");
    printf("\t(7) Modify working days\n");
    printf("\t(8) Change project name\n");
    printf("\t(9) Change project description\n");
    printf("\t(10) Back to project selection\n");
    printf("\t(11) Exit\n");
    printf("****************************************\n");
}

/* Display all available menu option in modify task's part
 * Parameter:
 *      taskName    - name of task that currently working on
 * */
void displayModifyTaskMenuOptions(char *taskName)
{
    printf("\n****************************************\n");
    printf("Task name: |%s|\n", taskName);
    printf("There are 5 choices in modify task option :\n");
    printf("\t(1) Modify task information\n");
    printf("\t(2) Add edge\n");
    printf("\t(3) Change dependency destination\n");
    printf("\t(4) Delete dependency destination\n");
    printf("\t(5) Back to Task Option Menu\n");
    printf("****************************************\n");
}

/* Display all vailable menu option in change task info's part
 * */
void displayChangeTaskInfoOptions(char *taskName, char *taskDescription, int dayWork)
{
    printf("\n****************************************\n");
    printf("Task name: |%s|\n", taskName);
    printf("Task description: |%s|\n", taskDescription);
    printf("Task day work: |%d|\n", dayWork);
    printf("There are 4 choices in modify task option :\n");
    printf("\t(1) Change task name\n");
    printf("\t(2) Change task description\n");
    printf("\t(3) Change task weight\n");
    printf("\t(4) Back to Modify Task Option Menu\n");
    printf("****************************************\n");
}

/* Display all available menu option in modify working day's part
 * Parameter:
 *      weekendStatus   - project's weekend information
 *      totalDayoff     - project's total day off
 * */
void displayModifyWorkingDaysOptions(int weekendStatus, int totalDayoff)
{
    printf("\n****************************************\n");
    printf("Working day %s\n", weekendStatus ? "exclude weekend" : "include weekend");
    printf("Total day off %d\n", totalDayoff);
    printf("There are 5 choices of modify working days option :\n");
    printf("\t(1) Set working day to %s\n", weekendStatus ? "include weekend" : "exclude weekend");
    printf("\t(2) Add day off\n");
    printf("\t(3) Remove day off\n");
    printf("\t(4) Display every day off\n");
    printf("\t(5) Back to Task Option Menu\n");
    printf("****************************************\n");
}