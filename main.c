/***************************************************************************
 * 
 *  main.c
 *      This function be the flow of the main program of 
 *      project scheduling system program and do some of validate 
 *      and alse print the user interface
 * 
 *      Created by Aplusplus team member
 *      From KMUTT34
 * 
 * *************************************************************************
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "main.h"
#include "abstractNetwork.h"
#include "fileManagement.h"
#include "userInterface.h"

#define DATABASE_DIRECTORY "../CPE111 - Project/dataBase"

char workingProjectName[128] = {0};

int argvOneProcess(char *projectNameIn)
{
    int returnStatus = 0;
    char terminalInput[128];

    returnStatus = readInformationFile(projectNameIn, DATABASE_DIRECTORY);
    if (returnStatus == 0)
    {
        printf("Oops!, Can't see the file name.\n");
        getTerminalInput(terminalInput, "Do you want to create new one? [YES=y|NO=else] : ");
        if (strcmp(terminalInput, "y") == 0)
        {
            returnStatus = addNewProjectFile(projectNameIn, DATABASE_DIRECTORY);
            returnStatus = 1;
        }
    }
    else if (returnStatus == -1)
    {
        returnStatus = 0;
    }
    else
    {
        memset(workingProjectName, 0, sizeof(workingProjectName));
        strcpy(workingProjectName, projectNameIn);
    }
    return returnStatus;
}

void addNewProject(int *fileOpenStatus)
{
    int returnStatus;
    char projectNameIn[128];
    char choice[8];

    getTerminalInput(projectNameIn, "Enter new project name : ");
    if (!strlen(projectNameIn))
    {
        displayInvalidMessage("File name can't be empty");
    }
    else
    {
        returnStatus = addNewProjectFile(projectNameIn, DATABASE_DIRECTORY);
        if (!returnStatus)
        {
            printf("Oops, the file with given file name already exist.\n");
            getTerminalInput(choice, "Do you want to overwrite this file? [YES=y|NO=else] : ");
            if (strcmp(choice, "y") == 0)
            {
                if (deleteProjectFile(projectNameIn, DATABASE_DIRECTORY) &&
                    addNewProjectFile(projectNameIn, DATABASE_DIRECTORY))
                {
                    displaySuccessMessage("Success to create new project");
                    getTerminalInput(choice, "Do you want to working this file [YES=y|NO=else]: ");
                    if (strcmp(choice, "y") == 0)
                    {
                        strcpy(workingProjectName, projectNameIn);
                        *fileOpenStatus = 1;
                    }
                }
                else
                {
                    displayErrorMessage("While overwriting to file \"%s\"");
                }
            }
        }
    }

    if (*fileOpenStatus)
        taskOptionFlowManager(fileOpenStatus);
}

void enterExistProject(int *fileOpenStatus)
{
    char choice[8];
    char projectNameIn[128];
    char **projectNameList;
    int totalProject;
    int returnStatus;
    int searchStatus;

    projectNameList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalProject);
    displayAllProjectAvailable(projectNameList, totalProject);
    getTerminalInput(projectNameIn, "Enter file name: ");
    if (strlen(projectNameIn) == 0)
    {
        displayInvalidMessage("File name can't be empty");
    }
    else
    {
        returnStatus = readInformationFile(projectNameIn, DATABASE_DIRECTORY);
        searchStatus = 0;
        for (int i = 0; i < totalProject; i++)
        {
            if (strcmp(projectNameIn, projectNameList[i]) == 0)
            {
                searchStatus = 1;
            }
        }
        if (returnStatus == 0 || searchStatus == 0)
        {
            printf("Oops, the file with given file name is not exist.\n");
            getTerminalInput(choice, "Do you want to create new one ? [YES=y][NO=else] : ");
            if (strcmp(choice, "y\n") == 0)
            {
                returnStatus = addNewProjectFile(projectNameIn, DATABASE_DIRECTORY);
                strcpy(workingProjectName, projectNameIn);
                *fileOpenStatus = 1;
            }
        }
        else
        {
            strcpy(workingProjectName, projectNameIn);
            *fileOpenStatus = 1;
        }
    }

    if (*fileOpenStatus)
        taskOptionFlowManager(fileOpenStatus);
}

void deleteExistProject(int *fileOpenStatus)
{
    int returnStatus;              /*return status*/
    char projectNameDelete[128];   /*project file name that want to delete*/
    char **projectNameList = NULL; /*list of project name*/
    int totalProject;              /*total file in database*/

    projectNameList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalProject);
    displayAllProjectAvailable(projectNameList, totalProject);
    getTerminalInput(projectNameDelete, "Enter task name to delete: ");
    if (strlen(projectNameDelete) == 0)
    {
        printf("Try again, file name can't be empty.\n");
    }
    else
    {
        returnStatus = deleteProjectFile(projectNameDelete, DATABASE_DIRECTORY);
        if (returnStatus == 1)
        {
            printf("Success to delete the file.\n");
        }
        else
        {
            printf("Oops, Can't see the file which you want to delete.\n");
        }
    }
}

void projectOptionFlowManager(int *fileOpenStatus)
{
    char choice[8];
    if (*fileOpenStatus)
    {
        taskOptionFlowManager(fileOpenStatus);
    }
    while (1)
    {
        displayProjectMenuOptions();
        getTerminalInput(choice, "Enter your option: ");
        if (strcmp(choice, "1") == 0) // add project
        {
            addNewProject(fileOpenStatus);
        }
        else if (strcmp(choice, "2") == 0) // enter exist
        {
            enterExistProject(fileOpenStatus);
        }
        else if (strcmp(choice, "3") == 0) // delete exist project
        {
            deleteExistProject(fileOpenStatus);
        }
        else if (strcmp(choice, "4") == 0) // exit program
        {
            printf("Thankyou for use our service...\n"); // ui print bye to user
            exit(0);
        }
        else
        {
            displayInvalidMessage("Please enter option again");
        }
    }
}

void renameProject()
{
    char fileNameRename[128]; /* new file name to be raname into */
    int returnStatus;         /* store return value from function */

    getTerminalInput(fileNameRename, "Enter new file name: ");
    if (strlen(fileNameRename) == 0)
    {
        displayInvalidMessage("Try again, file name can't be empty");
    }
    else
    {
        returnStatus = renameProjectFile(workingProjectName, fileNameRename, DATABASE_DIRECTORY);
        if (returnStatus == 1)
        {
            strcpy(workingProjectName, fileNameRename);
            printf("Success to rename the project.\n");
        }
        else if (returnStatus == -1)
        {
            printf("Oops, the file name is already exist.\n");
        }
        else if (returnStatus == -2)
        {
            printf("Oops, not found the project to rename.\n");
        }
        else
        {
            printf("Oops, can't rename the project.\n");
        }
    }
}

void addTask()
{
    char taskName[128];        /* sore task name */
    char taskDescription[128]; /* store task information */
    char taskWeightString[8];  /* store task weight in string */
    int taskWeight;            /* store weight of task */
    int returnStatus;          /* store retrun value from function */

    getTerminalInput(taskName, "Enter task name: ");
    getTerminalInput(taskDescription, "Enter task description: ");
    getTerminalInput(taskWeightString, "Enter task weight: ");
    sscanf(taskWeightString, "%d", &taskWeight);
    if (!strlen(taskName) || !strlen(taskDescription))
    {
        printf(">>> Input error - key or data empty!\n");
    }
    else
    {
        returnStatus = addVertex(taskName, taskDescription, taskWeight);
        if (returnStatus == 1)
        {
            if (!writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead()))
            {
                displayErrorMessage("Can't save the to database file");
            }
            printf(">>> Vertex |%s| added\n", taskName);
        }
        else if (returnStatus == -1)
        {
            printf(">>> Vertex with key |%s| already exists in graph\n", taskName);
        }
        else if (returnStatus == -2)
        {
            printf(">>> Vertex with key |%s| has error assign start and end edge\n", taskName);
        }
        else
        {
            printf(">>> Memory allocation error or graph full!\n");
        }
    }
}

char **searchTask(int *totalTask, char *searchString)
{
    char terminalInput[128];  /*string in terminal*/
    char taskName[128];       /*name of task*/
    char **resultList = NULL; /*the result of function*/

    getTerminalInput(taskName, "Enter the task name : ");
    memset(searchString, 0, sizeof(searchString));
    strcpy(searchString, taskName);
    resultList = searchVertex(taskName, totalTask);

    return resultList;
}

int findTask(char **searchTaskList, char *searchTask, int size)
{
    char choice[8];
    int returnStatus = 0;

    while (1)
    {
        printf("There are %d task which are similarly to %s :\n", size + 1, searchTask);
        for (int i = 0; i < size; i++)
        {
            printf("\t%d) %s.\n", i + 1, searchTaskList[i]);
        }
        getTerminalInput(searchTask, "Which task do you want to select : ");
        for (int i = 0; i < size; i++)
        {
            if (strcmp(searchTask, searchTaskList[i]) == 0)
            {
                returnStatus = 1;
                break;
            }
        }
        if (returnStatus == 0)
        {
            getTerminalInput(choice, "Oops, the task isn't in the list.\n[BACK TO TASK OPTIONS = y][NO = else] : ");
            if (strcmp(choice, "y"))
                break;
        }
        else
            break;
    }

    return returnStatus;
}

void changeTaskInfo(char *taskName)
{
    char newTaskName[128];        /* store task name */
    char newTaskDescription[128]; /* store task info */
    char newTaskWeightString[128];
    int newTaskWeight = 0; /* store task weight */
    int returnStatus;      /* store return value from function */

    getTerminalInput(newTaskName, "Enter new task name : ");
    getTerminalInput(newTaskDescription, "Enter new task description : ");
    getTerminalInput(newTaskWeightString, "Enter new task weight : ");
    sscanf(newTaskWeightString, "%d", &newTaskWeight);
    if (newTaskWeight < 0)
    {
        printf("Task weight can't be less than 0\n");
    }
    else
    {
        returnStatus = modifyVertex(taskName, newTaskName, newTaskDescription, newTaskWeight);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead());
            strcpy(taskName, newTaskName);
            printf("Success to change task information.\n");
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n");
        }
    }
}

void addDependency(char *taskName)
{
    char toTaskName[128]; /*name of task*/
    int returnStatus;     /*status of this function*/

    getTerminalInput(toTaskName, "Enter dependency destination : ");
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = addEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead());
            printf("Success to add dependency destination.\n");
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n");
        }
        else if (returnStatus == -2)
        {
            printf("Oops, Dependency destination already exist.\n");
        }
        else if (returnStatus == -3)
        {
            printf("Oops, It will be loop, if adding that edge.\n");
        }
        else
        {
            printf("Error to allocate memory.\n");
        }
    }
}

void changeDependency(char *taskName)
{
    char fromTaskName[128]; /*vertex that direct to other*/
    char toTaskName[128];   /*destination vertex*/
    int returnStatus;       /*function status*/

    getTerminalInput(fromTaskName, "Enter old dependency destination which you want to change : ");
    getTerminalInput(toTaskName, "Enter new dependency destination : ");
    if (strlen(fromTaskName) == 0 || strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = modifyEdge(taskName, fromTaskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead);
            printf("Success to change dependency destination.\n");
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n");
        }
        else if (returnStatus == -2)
        {
            printf("Oops, The name of each key must be unique. Can't be the same.\n");
        }
        else if (returnStatus == -3)
        {
            printf("Oops, The name of new dependency destination already exist.\n");
        }
        else
        {
            printf("Oops, The name of old dependency destination already exist.\n");
        }
    }
}

void deleteDependency(char *taskName)
{
    char toTaskName[128]; /* store task name to */
    int returnStatus;     /* store return value from function */

    getTerminalInput(toTaskName, "Enter dependency destination which you want to delete : ");
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = deleteEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead());
            printf("Success to delete dependency destination.\n");
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n");
        }
        else
        {
            printf("Oops, Can't find that dependency destination.\n");
        }
    }
}

void modifyTaskOptionFlowManager(char *taskName)
{
    char choice[8]; /* store choice from user */

    while (1)
    {
        displayModifyTaskMenuOptions();
        getTerminalInput(choice, "Enter tour option: ");
        if (strcmp(choice, "1") == 0)
        {
            changeTaskInfo(taskName);
        }
        else if (strcmp(choice, "2") == 0)
        {
            addDependency(taskName);
        }
        else if (strcmp(choice, "3") == 0)
        {
            changeDependency(taskName);
        }
        else if (strcmp(choice, "4") == 0)
        {
            deleteDependency(taskName);
        }
        else if (strcmp(choice, "5") == 0)
        {
            break;
        }
        else
        {
            printf("\n----- Invalid option, please select option again -----\n\n");
        }
    }
}

void displayTask(char *taskName)
{
    VERTEX_T *pFound = NULL;  /* find vertex struct */
    VERTEX_T *pVertex = NULL; /* store vertex struct */
    EDGE_T *pEdge = NULL;     /* store edge struct */
    pFound = findVertex(taskName);
    printf(">>> Task Name: |%s|\n", pFound->name);
    printf(">>> Task Description: %s\n", pFound->description);
    printf(">>> Task Working Day: %d\n", pFound->dayWork);
    printf(">>> Dependency Of:\n");
    pEdge = pFound->adjListHead;
    while (pEdge != NULL)
    {
        pVertex = pEdge->pVertex;
        if (strcmp(pVertex->name, "end") != 0)
        {
            printf("\t|%s|\n", pVertex->name);
        }
        pEdge = pEdge->pNext;
    }
    printf("\n");
}

void deleteTask(char *taskName)
{
    int returnStatus; /* store function return value */

    returnStatus = deleteVertex(taskName);
    writeInformationFile(workingProjectName, DATABASE_DIRECTORY, getVertexListHead);
    printf("Success to delete task.\n");
}

void dispalyAllTask()
{
    VERTEX_T *pCurrentVertex = NULL; /* loop through vertex list */
    VERTEX_T *pAdjVertex = NULL;     /* store adjacent vertex */
    VERTEX_T *pCurrentEdge = NULL;   /* loop through edge list */

    pCurrentVertex = getVertexListHead();
    while (pCurrentVertex != NULL)
    {
        displayTask(pCurrentVertex->name);
        pCurrentVertex = pCurrentVertex->pNext;
    }
}

void calculateProjectSchedule()
{
}

void taskOptionFlowManager(int *fileOpenStatus)
{
    char **searchResultList = NULL;
    char choice[8];
    char searchString[128];
    int totalResult;
    int returnStatus;

    while (1)
    {
        displayTaskMenuOptions(workingProjectName);
        getTerminalInput(choice, "Enter your option: ");
        if (strcmp(choice, "1") == 0)
        {
            addTask();
        }
        else if (strcmp(choice, "2") == 0 || strcmp(choice, "3") == 0 || strcmp(choice, "4") == 0)
        {
            searchResultList = searchTask(&totalResult, searchString);
            if (totalResult == 0)
            {
                printf("Oops, Not found the task with given name.\n");
            }
            else
            {
                if (totalResult > 1)
                {
                    returnStatus = findTask(searchResultList, searchString, totalResult);
                }
                if (returnStatus == 1)
                {
                    if (strcmp(choice, "2") == 0) // modify task option
                    {
                        modifyTaskOptionFlowManager(searchString);
                    }
                    else if (strcmp(choice, "3") == 0)
                    {
                        deleteTask(searchString); // delete task
                    }
                    else
                    {
                        displayTask(searchString); // display specific task
                    }
                }
            }
        }
        else if (strcmp(choice, "5") == 0) // display all task info
        {
            dispalyAllTask();
        }
        else if (strcmp(choice, "6") == 0) // calcluate project schedule
        {
            calculateProjectSchedule();
        }
        else if (strcmp(choice, "7") == 0) // change project name
        {
            renameProject();
        }
        else if (strcmp(choice, "8") == 0) // back to project selection
        {
            fileOpenStatus = 0;
            break;
        }
        else if (strcmp(choice, "9") == 0) // exit
        {
            printf("Thankyou for use our service...\n");
            exit(0);
        }
        else
        {
            printf("\n----- Invalid option, please select option again -----\n\n");
        }
    }
}

int main(int argc, char *argv[])
{
    char fileNameIn[128];           /*input file name*/
    char **databaseFileList = NULL; /*name list in folder return from function*/
    int totalFile;                  /*file count in database folder*/
    int fileOpenStatus = 0;         /*openfile status*/

    databaseFileList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalFile);
    if (!initNetwork())
    {
        displayErrorMessage("Can't initialize network");
        exit(0);
    }

    if (argc > 1)
    {
        sscanf(argv[1], "%s", fileNameIn);
        fileOpenStatus = argvOneProcess(fileNameIn);
    }

    projectOptionFlowManager(&fileOpenStatus);
}