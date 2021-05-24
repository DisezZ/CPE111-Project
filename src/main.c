/***************************************************************************
 * 
 *  main.c
 *      This function be the flow of the main program of 
 *      project scheduling system program and do some of validate 
 *      and alse print the user interface
 * 
 *      Created by Aplusplus Team Member
 * 
 * *************************************************************************
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#define __USE_XOPEN
#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include <time.h>
#include "main.h"
#include "abstractNetwork.h"
#include "fileManagement.h"
#include "userInterface.h"
#include "dateCalendarManager.h"
#include "validateDate.h"

char *dataBaseDirectory = NULL;
char projectDescription[256] = {0};
char workingProjectName[128] = {0};

int argvOneProcess(char *projectNameIn)
{
    int fileOpenStatus = 0;
    int returnStatus;
    char printString[256];
    char choice[8];

    returnStatus = readInformationFile(projectNameIn, dataBaseDirectory);
    if (returnStatus == -1)
    {
        snprintf(printString, sizeof(printString), "Not found project \"%s\"", projectNameIn);
        displayErrorMessage(printString);
        snprintf(printString, sizeof(printString), "Create new project named \"%s\"? [YES=y|NO=else] : ", projectNameIn);
        getTerminalInput(choice, sizeof(choice), printString);
        if (strcmp(choice, "y") == 0)
        {
            returnStatus = addNewProjectFile(projectNameIn, dataBaseDirectory);
            if (returnStatus)
            {
                snprintf(printString, sizeof(printString), "Project \"%s\" is created", projectNameIn);
                displaySuccessMessage(printString);
            }
            else
            {
                snprintf(printString, sizeof(printString), "While creating project \"%s\"", projectNameIn);
                displayErrorMessage(printString);
            }
        }
    }
    else if (returnStatus == 0)
    {
        snprintf(printString, sizeof(printString), "While reading information from \"C%s\"", projectNameIn);
        displayErrorMessage(printString);
    }
    else
    {
        memset(workingProjectName, 0, sizeof(workingProjectName));
        strcpy(workingProjectName, projectNameIn);
        fileOpenStatus = 1;
    }
    return fileOpenStatus;
}

void changeTaskInfoOptionFlowManager(char *taskName)
{
    char choice[8]; /* store choice from user */
    VERTEX_T *pFound = NULL;

    while (1)
    {
        pFound = findVertex(taskName);
        displayChangeTaskInfoOptions(pFound->name, pFound->description, pFound->dayWork);
        getTerminalInput(choice, sizeof(choice), "Enter tour option: ");
        if (strcmp(choice, "1") == 0)
        {
            changeTaskName(taskName);
        }
        else if (strcmp(choice, "2") == 0)
        {
            changeTaskDescription(taskName);
        }
        else if (strcmp(choice, "3") == 0)
        {
            changeTaskWeight(taskName);
        }
        else if (strcmp(choice, "4") == 0)
        {
            break;
        }
        else
        {
            printf("\n----- Invalid option, please select option again -----\n\n");
        }
    }
}

void addNewProject(int *fileOpenStatus)
{
    char printString[256];
    char projectNameIn[128];
    char choice[8];
    int returnStatus;

    getTerminalInput(projectNameIn, sizeof(projectNameIn), "Enter new project name : ");
    if (!strlen(projectNameIn))
    {
        displayInvalidMessage("Project name can't be empty");
    }
    else
    {
        returnStatus = addNewProjectFile(projectNameIn, dataBaseDirectory);
        if (!returnStatus)
        {
            snprintf(printString, sizeof(printString), "Projetc \"%s\" already exist", projectNameIn);
            displayErrorMessage(printString);
            snprintf(printString, sizeof(printString), "Do you want to continue(overwrite) \"%s\"? [YES=y|NO=else] : ", projectNameIn);
            getTerminalInput(choice, sizeof(choice), printString);
            if (strcmp(choice, "y") == 0)
            {
                if (deleteProjectFile(projectNameIn, dataBaseDirectory) &&
                    addNewProjectFile(projectNameIn, dataBaseDirectory))
                {
                    snprintf(printString, sizeof(printString), "Project \"%s\" is created", projectNameIn);
                    displaySuccessMessage(printString);
                    snprintf(printString, sizeof(printString), "Do you want to enter to project \"%s\" [YES=y|NO=else] : ", projectNameIn);
                    getTerminalInput(choice, sizeof(choice), printString);
                    if (strcmp(choice, "y") == 0)
                    {
                        strcpy(workingProjectName, projectNameIn);
                        *fileOpenStatus = 1;
                    }
                }
                else
                {
                    snprintf(printString, sizeof(printString), "While overwriting to project \"%s\"", projectNameIn);
                    displayErrorMessage(printString);
                }
            }
        }
        else
        {
            snprintf(printString, sizeof(printString), "Added project \"%s\" to database", projectNameIn);
            displaySuccessMessage(printString);
        }
    }
    if (*fileOpenStatus)
        taskOptionFlowManager(fileOpenStatus);
}

void enterExistProject(int *fileOpenStatus)
{
    char choice[8];
    char projectNameIn[128];
    char printString[256];
    char **projectNameList;
    int totalProject = 0;
    int returnStatus;
    int searchStatus;

    projectNameList = findProjectFileDatabase(dataBaseDirectory, &totalProject);
    displayAllProjectAvailable(projectNameList, totalProject);
    getTerminalInput(projectNameIn, sizeof(projectNameIn), "Enter project name: ");
    if (strlen(projectNameIn) == 0)
    {
        displayInvalidMessage("Project name can't be empty");
    }
    else
    {
        returnStatus = readInformationFile(projectNameIn, dataBaseDirectory);
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
            snprintf(printString, sizeof(printString), "Project \"%s\" is not exist", projectNameIn);
            displayErrorMessage(printString);
            snprintf(printString, sizeof(printString), "Create new project named \"%s\"? [YES=y|NO=else] : ", projectNameIn);
            getTerminalInput(choice, sizeof(choice), printString);
            if (strcmp(choice, "y\n") == 0)
            {
                returnStatus = addNewProjectFile(projectNameIn, dataBaseDirectory);
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

    freeStringArray(totalProject, projectNameList);
    if (*fileOpenStatus)
        taskOptionFlowManager(fileOpenStatus);
}

void deleteExistProject(int *fileOpenStatus)
{
    char printString[256];
    char projectNameDelete[128];   /*project file name that want to delete*/
    char **projectNameList = NULL; /*list of project name*/
    int totalProject = 0;          /*total file in database*/
    int returnStatus;              /*return status*/

    projectNameList = findProjectFileDatabase(dataBaseDirectory, &totalProject);
    displayAllProjectAvailable(projectNameList, totalProject);
    getTerminalInput(projectNameDelete, sizeof(projectNameDelete), "Enter project name to delete: ");
    if (strlen(projectNameDelete) == 0)
    {
        displayInvalidMessage("Project name can't be empty");
    }
    else
    {
        returnStatus = deleteProjectFile(projectNameDelete, dataBaseDirectory);
        if (returnStatus == 1)
        {
            snprintf(printString, sizeof(printString), "Project \"%s\" deleted", projectNameDelete);
            displaySuccessMessage("Project deleted");
        }
        else
        {
            snprintf(printString, sizeof(printString), "Project \"%s\" doesn't exist", projectNameDelete);
            displayErrorMessage(printString);
        }
    }
    freeStringArray(totalProject, projectNameList);
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
        getTerminalInput(choice, sizeof(choice), "Enter your option: ");
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
    char printString[384];
    char projectNameRename[128]; /* new file name to be raname into */
    int returnStatus;            /* store return value from function */

    getTerminalInput(projectNameRename, sizeof(projectNameRename), "Enter new project name: ");
    if (strlen(projectNameRename) == 0)
    {
        displayInvalidMessage("Project name can't be empty");
    }
    else
    {
        returnStatus = renameProjectFile(workingProjectName, projectNameRename, dataBaseDirectory);
        if (returnStatus == 1)
        {
            snprintf(printString, sizeof(printString), "Project \"%s\" renamed to \"%s\"", workingProjectName, projectNameRename);
            displaySuccessMessage(printString);
            strcpy(workingProjectName, projectNameRename);
        }
        else if (returnStatus == -1)
        {
            snprintf(printString, sizeof(printString), "Project \"%s\" already exist", projectNameRename);
            displayErrorMessage(printString);
        }
        else if (returnStatus == -2)
        {
            snprintf(printString, sizeof(printString), "Project \"%s\" doesn't exist", projectNameRename);
            displayErrorMessage(printString);
        }
        else
        {
            snprintf(printString, sizeof(printString), "While rename project \"%s\" to \"%s\"", workingProjectName, projectNameRename);
            displayErrorMessage(printString);
        }
    }
}

void addTask()
{
    char printString[256];
    char taskName[128];         /* sore task name */
    char taskDescription[128];  /* store task information */
    char taskWeightString[128]; /* store task weight in string */
    int taskWeight = -1;        /* store weight of task */
    int returnStatus;           /* store retrun value from function */

    getTerminalInput(taskName, sizeof(taskName), "Enter task's name: ");
    getTerminalInput(taskDescription, sizeof(taskDescription), "Enter task's description: ");
    getTerminalInput(taskWeightString, sizeof(taskWeightString), "Enter task's number of working days : ");
    sscanf(taskWeightString, "%d", &taskWeight);
    if (!strlen(taskName) || !strlen(taskDescription))
    {
        displayInvalidMessage("Task's name or description can't be empty");
    }
    else
    {
        returnStatus = addVertex(taskName, taskDescription, taskWeight);
        if (returnStatus == 1)
        {
            if (!writeInformationFile(workingProjectName, dataBaseDirectory))
            {
                snprintf(printString, sizeof(printString), "Can't save task |%s| to database", taskName);
                displayErrorMessage(printString);
            }
            else
            {
                snprintf(printString, sizeof(printString), "Task |%s| added", taskName);
                displayErrorMessage(printString);
            }
        }
        else if (returnStatus == -1)
        {
            snprintf(printString, sizeof(printString), "Task |%s| already exist\n", taskName);
            displayErrorMessage(printString);
        }
        else if (returnStatus == -2)
        {
            snprintf(printString, sizeof(printString), "While assign task |%s| to start and end\n", taskName);
            displayErrorMessage(printString);
        }
        else
        {
            displayErrorMessage("While allocating memory");
        }
    }
}

int getAllTask(char **searchResultList)
{
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pCurrentVertex = getStartVertex();
    EDGE_T *pCurrentEdge = pCurrentVertex->adjListHead;
    int totalVertex = getTotalVertex();
    int i = 0;
    while (pCurrentEdge && i < totalVertex)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        if (pAdjVertex && strcmp(pAdjVertex->name, "start") != 0 && strcmp(pAdjVertex->name, "end") != 0)
        {
            searchResultList[i] = calloc(strlen(pAdjVertex->name), sizeof(char));
            strcpy(searchResultList[i], pAdjVertex->name);
            ++i;
        }
        pCurrentEdge = pCurrentEdge->pNext;
    }
    return i;
}

int findTask(char *searchTask, size_t size)
{
    char printString[256];
    char **searchTaskList = NULL;
    char choice[8];
    int totalTask = 0;
    int returnStatus = 0;

    totalTask = getTotalVertex();
    searchTaskList = calloc(totalTask, sizeof(char *));
    getAllTask(searchTaskList);
    while (1)
    {

        displayAllTaskAvailable(searchTaskList, totalTask);
        getTerminalInput(searchTask, size, "Which task do you want to select : ");
        for (int i = 0; i < totalTask; i++)
        {
            if (strcmp(searchTask, searchTaskList[i]) == 0)
            {
                returnStatus = 1;
                break;
            }
        }
        if (returnStatus == 0)
        {
            snprintf(printString, sizeof(printString), "Task |%s| not exist", searchTask);
            displayInvalidMessage(printString);
            getTerminalInput(choice, sizeof(choice), "[BACK = y][CONTINUE = else] : ");
            if (strcmp(choice, "y"))
                break;
        }
        else
            break;
    }
    freeStringArray(totalTask, searchTaskList);
    return returnStatus;
}

void changeTaskName(char *taskName)
{
    char printString[256];
    char newTaskName[128];
    int returnStatus;

    getTerminalInput(newTaskName, sizeof(newTaskName), "Enter new task name : ");
    if (strlen(newTaskName) == 0)
    {
        snprintf(printString, sizeof(printString), "Task |%s| name can't empty", taskName);
        displayInvalidMessage(printString);
    }
    else
    {
        returnStatus = modifyVertexName(taskName, newTaskName);
        if (returnStatus == 1)
        {
            if (!writeInformationFile(workingProjectName, dataBaseDirectory))
            {
                snprintf(printString, sizeof(printString), "Can't save changed |%s| to database", newTaskName);
                displayErrorMessage(printString);
            }
            else
            {
                snprintf(printString, sizeof(printString), "Task |%s| name changed to |%s|", taskName, newTaskName);
                strcpy(taskName, newTaskName);
                displaySuccessMessage(printString);
            }
        }
        else if (returnStatus == -2)
        {
            snprintf(printString, sizeof(printString), "Task |%s| already exist", newTaskName);
            displayErrorMessage(printString);
        }
        else
        {
            displayErrorMessage("While allocating memory");
        }
    }
}

void changeTaskDescription(char *taskName)
{
    char printString[256];
    char newTaskDescription[128];
    int returnStatus;

    getTerminalInput(newTaskDescription, sizeof(newTaskDescription), "Enter new task description : ");
    if (strlen(newTaskDescription) == 0)
    {
        snprintf(printString, sizeof(printString), "Task \"%s\" description can't empty", taskName);
        displayInvalidMessage(printString);
    }
    else
    {
        returnStatus = modifyVertexDescription(taskName, newTaskDescription);
        if (returnStatus == 1)
        {
            if (!writeInformationFile(workingProjectName, dataBaseDirectory))
            {
                snprintf(printString, sizeof(printString), "Can't save changes |%s| to database", taskName);
                displayErrorMessage(printString);
            }
            else
            {
                snprintf(printString, sizeof(printString), "Task |%s| description changed to |%s|", taskName, newTaskDescription);
                displaySuccessMessage(printString);
            }
        }
        else
        {
            displayErrorMessage("While allocating memory");
        }
    }
}

void changeTaskWeight(char *taskName)
{
    char printString[256];
    char newTaskWeightString[8];
    int newTaskWeight = 0;
    int returnStatus;

    getTerminalInput(newTaskWeightString, sizeof(newTaskWeightString), "Enter new task work days : ");
    sscanf(newTaskWeightString, "%d", &newTaskWeight);
    if (newTaskWeight < 1)
    {
        snprintf(printString, sizeof(printString), "Task \"%s\" work days can't empty or less than 1", taskName);
        displayInvalidMessage(printString);
    }
    else
    {
        modifyVertexWeight(taskName, newTaskWeight);
        if (!writeInformationFile(workingProjectName, dataBaseDirectory))
        {
            snprintf(printString, sizeof(printString), "Can't save changed |%d| to database", newTaskWeight);
            displayErrorMessage(printString);
        }
        else
        {
            snprintf(printString, sizeof(printString), "Task |%s| day works changed to |%d|", taskName, newTaskWeight);
            displaySuccessMessage(printString);
        }
    }
}

void addDependency(char *taskName)
{
    char toTaskName[128]; /*name of task*/
    int returnStatus;     /*status of this function*/

    getTerminalInput(toTaskName, sizeof(toTaskName), "Enter dependency destination : ");
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = addEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, dataBaseDirectory);
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

    getTerminalInput(fromTaskName, sizeof(fromTaskName), "Enter old dependency destination which you want to change : ");
    getTerminalInput(toTaskName, sizeof(toTaskName), "Enter new dependency destination : ");
    if (strlen(fromTaskName) == 0 || strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = modifyEdge(taskName, fromTaskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, dataBaseDirectory);
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

    getTerminalInput(toTaskName, sizeof(toTaskName), "Enter dependency destination which you want to delete : ");
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n");
    }
    else
    {
        returnStatus = deleteEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, dataBaseDirectory);
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
    VERTEX_T *pVertex = findVertex(taskName);
    while (1)
    {
        displayModifyTaskMenuOptions(taskName);
        getTerminalInput(choice, sizeof(choice), "Enter tour option: ");
        if (strcmp(choice, "1") == 0)
        {
            changeTaskInfoOptionFlowManager(taskName);
        }
        else if (strcmp(choice, "2") == 0)
        {
            addDependency(taskName);
        }
        else if (strcmp(choice, "3") == 0)
        {
            if (displayAllDependentOn(pVertex->adjListHead))
            {
                changeDependency(taskName);
            }
        }
        else if (strcmp(choice, "4") == 0)
        {
            if (displayAllDependentOn(pVertex->adjListHead))
            {
                deleteDependency(taskName);
            }
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
    writeInformationFile(workingProjectName, dataBaseDirectory);
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
    char projectStartDate[16];
    char taskStartDate[16];
    char taskEndDate[16];
    int date;
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pCurrentVertex = getStartVertex();
    EDGE_T *pCurrentEdge = pCurrentVertex->adjListHead;
    LongestPath();
    getTerminalInput(projectStartDate, sizeof(projectStartDate), "Enter project's start date: ");
    if (validateDate(projectStartDate))
    {
        printf("\n");
        while (pCurrentEdge)
        {
            pAdjVertex = pCurrentEdge->pVertex;
            if (strcmp(pAdjVertex->name, "start") != 0 && strcmp(pAdjVertex->name, "end") != 0)
            {
                calculateEndDate(projectStartDate, pAdjVertex->totalDay, taskStartDate, sizeof(taskStartDate));
                calculateEndDate(projectStartDate, pAdjVertex->totalDay + pAdjVertex->dayWork - 1, taskEndDate, sizeof(taskEndDate));
                printf(">>> Task: %s\n", pAdjVertex->name);
                printf(">>> Work Day: %d\n", pAdjVertex->dayWork);
                printf(">>> Start: %s\n", taskStartDate);
                printf(">>> Finished: %s\n", taskEndDate);
                printf("\n");
            }

            pCurrentEdge = pCurrentEdge->pNext;
        }
    }
}

void changeProjectDescription()
{
    char newDescription[256];
    if (strlen(projectDescription))
        printf("About Project:\n\t%s\n", projectDescription);
    else
        printf("\"%s\" has no description yet\n", workingProjectName);
    getTerminalInput(projectDescription, sizeof(newDescription), "Enter new project description: ");
    if (!writeInformationFile(workingProjectName, dataBaseDirectory))
    {
        displayErrorMessage("While saving changes to database");
    }
    else
    {
        displaySuccessMessage("Project description changed");
    }
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
        returnStatus = 0;
        displayTaskMenuOptions(workingProjectName, projectDescription, strlen(projectDescription));
        getTerminalInput(choice, sizeof(choice), "Enter your option: ");
        if (strcmp(choice, "1") == 0)
        {
            addTask();
        }
        else if (strcmp(choice, "2") == 0 || strcmp(choice, "3") == 0 || strcmp(choice, "4") == 0)
        {
            returnStatus = findTask(searchString, sizeof(searchString));
            if (returnStatus)
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
        else if (strcmp(choice, "5") == 0) // display all task info
        {
            dispalyAllTask();
        }
        else if (strcmp(choice, "6") == 0) // calcluate project schedule
        {
            calculateProjectSchedule();
        }
        else if (strcmp(choice, "7") == 0) // modify working day
        {
            modifyWorkingDayOptionFlowManager();
        }
        else if (strcmp(choice, "8") == 0) // change project name
        {
            renameProject();
        }
        else if (strcmp(choice, "9") == 0) // change project description
        {
            changeProjectDescription();
        }
        else if (strcmp(choice, "10") == 0) // back to project selection
        {
            *fileOpenStatus = 0;
            freeNetwork();
            freeDateList();
            memset(projectDescription, 0, sizeof(projectDescription));
            break;
        }
        else if (strcmp(choice, "11") == 0) // exit
        {
            printf("Thankyou for use our service...\n");
            exit(0);
        }
        else
        {
            displayInvalidMessage("Please select option again");
        }
    }
}

void addDayOff()
{
    char printString[256];
    char dateString[16];
    int returnStatus;
    struct tm tm = {0};
    time_t unixTime;

    getTerminalInput(dateString, sizeof(dateString), "Enter date ex. 01/01/2021: ");
    if (!strlen(dateString))
    {
        displayInvalidMessage("Date can't be empty");
    }
    else if (validateDate(dateString))
    {
        strptime(dateString, "%d/%m/%Y", &tm);
        unixTime = mktime(&tm);
        returnStatus = addDateToList(unixTime);
        if (returnStatus && writeInformationFile(workingProjectName, dataBaseDirectory))
        {
            snprintf(printString, sizeof(printString), "Added day off to \"%s\"", workingProjectName);
            displaySuccessMessage(printString);
        }
        else
        {
            snprintf(printString, sizeof(printString), "While add day off to \"%s\"", workingProjectName);
            displayErrorMessage(printString);
        }
    }
}

void removeDayOff()
{
    char printString[256];
    char dateString[16];
    int returnStatus;
    struct tm tm = {0};
    time_t unixTime;

    getTerminalInput(dateString, sizeof(dateString), "Enter date ex. 01/01/2021: ");
    if (!strlen(dateString))
    {
        displayInvalidMessage("Date can't be empty");
    }
    else if (validateDate(dateString))
    {
        strptime(dateString, "%d/%m/%Y", &tm);
        unixTime = mktime(&tm);
        returnStatus = removeDateFromList(unixTime);
        if (returnStatus && writeInformationFile(workingProjectName, dataBaseDirectory))
        {
            snprintf(printString, sizeof(printString), "Deleted day off from \"%s\"", workingProjectName);
            displaySuccessMessage(printString);
        }
        else
        {
            snprintf(printString, sizeof(printString), "While delete day off from \"%s\"", workingProjectName);
            displayErrorMessage(printString);
        }
    }
}

int displayEveryDayOff()
{
    char dateString[16];
    DATE_T *pCurrent = NULL;
    struct tm *tm = NULL;
    int totalDate;
    int status = 0;
    int i = 1;

    totalDate = getTotalDayOff();
    if (!totalDate)
        printf(">>> There is no day off yet!.\n");
    else
    {
        status = 1;
        printf("\nTotal %d:\n", totalDate);
        pCurrent = getDateListHead();
        while (pCurrent)
        {
            tm = localtime(&pCurrent->unixTime);
            strftime(dateString, sizeof(dateString), "%d/%m/%Y", tm);
            printf("%d) %s\n", i, dateString);
            pCurrent = pCurrent->pNext;
        }
    }
    printf("\n");
}

void modifyWorkingDayOptionFlowManager()
{
    char choice[8];
    int weekendStatus;
    int totalDayOff;

    while (1)
    {
        displayModifyWorkingDaysOptions(getWeekendStatus(), getTotalDayOff());
        getTerminalInput(choice, sizeof(choice), "Enter your option: ");
        if (strcmp(choice, "1") == 0)
        {
            setWeekendStatus();
            if (!writeInformationFile(workingProjectName, dataBaseDirectory))
                displayErrorMessage("While changing weekend day");
            else
                displaySuccessMessage("Weekend changed");
        }
        else if (strcmp(choice, "2") == 0)
        {
            addDayOff();
        }
        else if (strcmp(choice, "3") == 0)
        {
            if (displayEveryDayOff())
            {
                removeDayOff();
            }
        }
        else if (strcmp(choice, "4") == 0)
        {
            displayEveryDayOff();
        }
        else if (strcmp(choice, "5") == 0)
        {
            break;
        }
        else
        {
            displayInvalidMessage("Please select option again");
        }
    }
}

void freeStringArray(int size, char **stringToFree)
{
    for (int i = 0; i < size; i++)
    {
        free(stringToFree[i]);
        stringToFree[i] = NULL;
    }
    free(stringToFree);
    stringToFree = NULL;
}

void getProjectDescription(char *string)
{
    sscanf(projectDescription, "%s", string);
}

void setProjectDescription(char *string)
{
    sscanf(string, "%s", projectDescription);
}

int main(int argc, char *argv[])
{
    char fileNameIn[128];           /*input file name*/
    char **databaseFileList = NULL; /*name list in folder return from function*/
    int totalFile;                  /*file count in database folder*/
    int fileOpenStatus = 0;         /*openfile status*/

    dataBaseDirectory = findProjectDatabaseDirectory();
    databaseFileList = findProjectFileDatabase(dataBaseDirectory, &totalFile);
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