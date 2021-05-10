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
#include "main.h"
#include "abstractNetwork.h"
#include "fileManagement.h"
#include "userInterface.h"

char *dataBaseDirectory = NULL;
char workingProjectName[128] = {0};

int argvOneProcess(char *projectNameIn)
{
    int fileOpenStatus = 0;
    int returnStatus;
    char choice[8];

    returnStatus = readInformationFile(projectNameIn, dataBaseDirectory);
    if (returnStatus == -1)
    {
        displayErrorMessage("Can't find a file with given name in project's folder");
        getTerminalInput(choice, sizeof(choice), "Create new project with given name? [YES=y|NO=else] : ");
        if (strcmp(choice, "y") == 0)
        {
            returnStatus = addNewProjectFile(projectNameIn, dataBaseDirectory);
            if (returnStatus)
                displaySuccessMessage("New project is created");
            else
                displayErrorMessage("While creating project's file with given name");
        }
    }
    else if (returnStatus == 0)
    {
        displayErrorMessage("While reading information from database");
    }
    else
    {
        memset(workingProjectName, 0, sizeof(workingProjectName));
        strcpy(workingProjectName, projectNameIn);
        fileOpenStatus = 1;
    }
    return fileOpenStatus;
}

void addNewProject(int *fileOpenStatus)
{
    int returnStatus;
    char projectNameIn[128];
    char choice[8];

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
            displayErrorMessage("File with given file name already exist");
            getTerminalInput(choice, sizeof(choice), "Do you want to continue(overwrite) this file? [YES=y|NO=else] : ");
            if (strcmp(choice, "y") == 0)
            {
                if (deleteProjectFile(projectNameIn, dataBaseDirectory) &&
                    addNewProjectFile(projectNameIn, dataBaseDirectory))
                {
                    displaySuccessMessage("Success to create new project");
                    getTerminalInput(choice, sizeof(choice), "Do you want to working this file [YES=y|NO=else]: ");
                    if (strcmp(choice, "y") == 0)
                    {
                        strcpy(workingProjectName, projectNameIn);
                        *fileOpenStatus = 1;
                    }
                }
                else
                {
                    displayErrorMessage("While overwriting to database file");
                }
            }
        }
        else
        {
            displaySuccessMessage("Added project to database folder");
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
            displayErrorMessage("Project with given file name is not exist");
            getTerminalInput(choice, sizeof(choice), "Create new project with given name? [YES=y][NO=else] : ");
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
    int returnStatus;              /*return status*/
    char projectNameDelete[128];   /*project file name that want to delete*/
    char **projectNameList = NULL; /*list of project name*/
    int totalProject = 0;          /*total file in database*/

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
            displaySuccessMessage("Project deleted");
        }
        else
        {
            displayErrorMessage("Project with given name doesn't exist");
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
            strcpy(workingProjectName, projectNameRename);
            displaySuccessMessage("Project renamed");
        }
        else if (returnStatus == -1)
        {
            displayErrorMessage("Project with given name is already exist");
        }
        else if (returnStatus == -2)
        {
            displayErrorMessage("Not found project with given name");
        }
        else
        {
            displayErrorMessage("While rename the project");
        }
    }
}

void addTask()
{
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
        printf(">>> Input error - key or data empty!\n");
    }
    else
    {
        returnStatus = addVertex(taskName, taskDescription, taskWeight);
        if (returnStatus == 1)
        {
            if (!writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead()))
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

int allTask(char **searchResultList)
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

/*char **searchTask(int *totalTask, char *searchString)
{
    char terminalInput[128];  /*string in terminal
    char taskName[128];       /*name of task
    char **resultList = NULL; /*the result of function

    getTerminalInput(taskName, sizeof(taskName), "Enter the task name : ");
    memset(searchString, 0, sizeof(searchString));
    strcpy(searchString, taskName);
    resultList = searchVertex(taskName, totalTask);

    return resultList;
}*/

int findTask(char *searchTask, size_t size)
{
    char **searchTaskList = NULL;
    char choice[8];
    int totalTask = 0;
    int returnStatus = 0;

    totalTask = getTotalVertex();
    searchTaskList = calloc(totalTask, sizeof(char *));
    allTask(searchTaskList);
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
            displayInvalidMessage("Given task not exist in the database");
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

void changeTaskInfo(char *taskName)
{
    char newTaskName[128];        /* store task name */
    char newTaskDescription[128]; /* store task info */
    char newTaskWeightString[128];
    int newTaskWeight = 0; /* store task weight */
    int returnStatus;      /* store return value from function */

    getTerminalInput(newTaskName, sizeof(newTaskName), "Enter new task name : ");
    getTerminalInput(newTaskDescription, sizeof(newTaskDescription), "Enter new task description : ");
    getTerminalInput(newTaskWeightString, sizeof(newTaskWeightString), "Enter new task working day : ");
    sscanf(newTaskWeightString, "%d", &newTaskWeight);
    if (newTaskWeight < 1)
    {
        displayInvalidMessage("Task weight can't empty or less than 1");
    }
    else
    {
        returnStatus = modifyVertex(taskName, newTaskName, newTaskDescription, newTaskWeight);
        if (returnStatus == 1)
        {
            writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead());
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
            writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead());
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
            writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead);
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
            writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead());
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
        getTerminalInput(choice, sizeof(choice), "Enter tour option: ");
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
    writeInformationFile(workingProjectName, dataBaseDirectory, getVertexListHead);
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
    char startDate[16];
    int date;
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pCurrentVertex = getStartVertex();
    EDGE_T *pCurrentEdge = pCurrentVertex->adjListHead;
    LongestPath();
    getTerminalInput(startDate, sizeof(startDate), "Enter project's start date: ");
    sscanf(startDate, "%d", &date);
    /*while (pCurrentEdge)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        printf("Name: %s\nTotal Day: %d\n", pAdjVertex->name, pAdjVertex->totalDay);
        pCurrentEdge = pCurrentEdge->pNext;
    }*/
    printf("\n");
    while (pCurrentEdge)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        if (strcmp(pAdjVertex->name, "start") != 0 && strcmp(pAdjVertex->name, "end") != 0)
        {
            printf(">>> Task: %s\n", pAdjVertex->name);
            printf(">>> Work Day: %d\n", pAdjVertex->dayWork);
            printf(">>> Start: %d\n", date + pAdjVertex->totalDay);
            printf(">>> Finished: %d\n", date + pAdjVertex->totalDay + pAdjVertex->dayWork - 1);
            printf("\n");
        }

        pCurrentEdge = pCurrentEdge->pNext;
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
        displayTaskMenuOptions(workingProjectName);
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
        else if (strcmp(choice, "7") == 0) // change project name
        {
            renameProject();
        }
        else if (strcmp(choice, "8") == 0) // back to project selection
        {
            *fileOpenStatus = 0;
            freeNetwork();
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