/***************************************************************************
 * 
 *  main.c
 *      This function be the flow of the main program of 
 *      project scheduling system program and do some of validate 
 *      and alse print the user interface
 * 
 *      Created by Lutfee   Deeame  ID 63070503448
 * 
 * *************************************************************************
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include "abstractNetwork.h"
#include "fileManagement.h"

#define DATABASE_DIRECTORY "../Aplusplus_DraftCode/dataBase"

char workingFileName[128];
extern VERTEX_T *vertexListHead;
extern VERTEX_T *vertexListTail;

void addNewProject(int *openFileCheck);                /* This fuction will addd new poject file*/
void enterExistProject(int *openFileCheck);            /* This function will enter to the exist project*/
void deleteExistProject();                             /* This function will delete the exist project in database*/
void renameProject();                                  /* This function will rename the project*/
void addTask();                                        /* This function will add task in program*/
char **searchTask(int *totalTask, char *searchString); /*this function will search task in databse*/
void changeTaskInfo(char *taskName);                   /* This funtion will modify and change task info*/
void addDependency(char *taskName);                    /* This function will do the add dependency part */
void changeDependency(char *taskName);                 /* This function will do the change dependency from old to new task */
void deleteDependency(char *taskName);                 /* This function will delete dependency from task */
void modifyTask(char *taskName);                       /* This function will do the display modify task option and also the flow */
void displayTask(char *taskName);                      /* This function will display the task information */
void deleteTask(char *taskName);                       /* This function will delete the task from graph */
void dispalyAllTask();                                 /* This function will display all the task information in graph */
void calculateProjectSchedule();                       /* This function will calculate the project schedule start date and end date of each task */

int main(int argc, char *argv[])
{
    char terminalInput[128];        /*string in the terminal from user input*/
    char fileNameIn[128];           /*input file name*/
    char searchString[128];         /*string that user want to search*/
    char fileNameDelete[128];       /*project file name that want to delete*/
    char choice[8];                 /*choice in condition*/
    char **databaseFileList = NULL; /*name list in folder return from function*/
    char **searchResultList = NULL; /*the result of searching*/
    int totalFile;                  /*file count in database folder*/
    int totalResult;                /*total result return from function*/
    int openFileCheck = 0;          /*openfile status*/
    int returnStatus;               /*return status*/

    // check for database file;
    databaseFileList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalFile);
    if (!initNetwork())
    {
        printf("Oops, Can't initialize network.\n"); // ui print can't init network
    }

    if (argc > 1)
    {
        sscanf(argv[1], "%s", fileNameIn);
        openFileCheck = readInformationFile(fileNameIn, DATABASE_DIRECTORY);
        if (openFileCheck == 0)
        {
            printf("Oops!, Can't see the file name.");                   // ui print invalid terminal input file name
            printf("Do you want to create new one? [YES=y|NO=else] : "); // ui print if want to create new file database
            // if y = yes else = no
            memset(terminalInput, 0, sizeof(terminalInput));
            fgets(terminalInput, sizeof(terminalInput), stdin);
            if (strcasecmp(terminalInput, "y\n") == 0)
            {
                //create new file with fileNameIn and open
                returnStatus = addNewProjectFile(fileNameIn, DATABASE_DIRECTORY);
                openFileCheck = 1;
            }
        }
    }

    while (1)
    {
        while (openFileCheck == 0)
        {
            printf("There are 4 main options :\n");
            printf("\t(1) Add project\n");
            printf("\t(2) Enter existed project\n");
            printf("\t(3) Delete existed project\n");
            printf("\t(4) Exit\n"); // ui print projetct option ex. add enter exist delete exit
            printf("Enter your option: ");
            memset(terminalInput, 0, sizeof(terminalInput));
            fgets(terminalInput, sizeof(terminalInput), stdin);
            sscanf(terminalInput, "%s", choice);
            if (strcmp(choice, "1") == 0) // add project
            {
                addNewProject(&openFileCheck);
            }
            else if (strcmp(choice, "2") == 0) // enter exist
            {
                enterExistProject(&openFileCheck);
            }
            else if (strcmp(choice, "3") == 0) // delete exist project
            {
                deleteExistProject();
            }
            else if (strcmp(choice, "4") == 0) // exit program
            {
                printf("Thankyou for use our service...\n"); // ui print bye to user
                exit(0);
            }
            else
            {

                printf("\n----- Invalid option, please select option again -----\n\n"); // ui print invalid choice
            }
        }
        while (openFileCheck == 1)
        {
            printf("There are 7 of task options :\n");
            printf("\t(1) Add task\n");
            printf("\t(2) Modify task\n");
            printf("\t(3) Delete task\n");
            printf("\t(4) Search task\n");
            printf("\t(5) Display all task\n");
            printf("\t(6) Calculate project schedule\n");
            printf("\t(7) Change project name\n");
            printf("\t(8) Back to project selection\n");
            printf("\t(9) Exit\n"); // ui print task option ex. add delete modify
            printf("Enter your option: ");
            memset(terminalInput, 0, sizeof(terminalInput));
            fgets(terminalInput, sizeof(terminalInput), stdin);
            sscanf(terminalInput, "%s", choice);
            if (strcmp(choice, "1") == 0) // add task
            {
                addTask();
            }
            else if (strcmp(choice, "2") == 0 || strcmp(choice, "3") == 0 || strcmp(choice, "4") == 0) // modify, display, delte
            {
                searchResultList = searchTask(&totalResult, searchString);
                if (totalResult == 0)
                {
                    printf("Oops, Not found the task with given name.\n"); // ui print task with given name not exist
                }
                else
                {
                    //printf("search: %s\n", searchString);
                    if (totalResult == 1 && strcasecmp(searchResultList[0], searchString) == 0)
                    {
                        searchResultList[0] = strdup(searchString);
                    }
                    else if (totalResult > 0)
                    {
                        printf("There are %d task which are similarly :\n", totalResult); // ui print tell user there are more than 1 result and print all resul name
                        for (int i = 0; i < totalResult; i++)
                        {
                            printf("\t%s\n", searchResultList[i]);
                        }
                        returnStatus = 0;
                        while (1)
                        {
                            printf("Which task do you want to select : "); // ui print tell user to select
                            memset(terminalInput, 0, sizeof(terminalInput));
                            fgets(terminalInput, sizeof(terminalInput), stdin);
                            sscanf(terminalInput, "%s", terminalInput);
                            sscanf(terminalInput, "%s", searchString);
                            for (int i = 0; i < totalResult; i++)
                            {
                                if (strcmp(searchString, searchResultList[i]) == 0)
                                {
                                    returnStatus = 1;
                                    totalResult = 1;
                                    printf("found 1\n");
                                    searchResultList[0] = searchString;
                                    break;
                                }
                            }
                            if (returnStatus == 0)
                            {
                                printf("Oops, the task isn't in the list.\n[BACK TO TASK OPTIONS = y][NO = else] : "); // ui print not in result list want to back "y" to back?
                                memset(terminalInput, 0, sizeof(terminalInput));
                                fgets(terminalInput, sizeof(terminalInput), stdin);
                                sscanf(terminalInput, "%s", terminalInput);
                                terminalInput[strlen(terminalInput) - 1] = 0;
                                if (strcasecmp(terminalInput, "y"))
                                {
                                    break;
                                }
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    if (totalResult == 1)
                    {
                        if (strcmp(choice, "2") == 0)
                        {
                            modifyTask(searchResultList[0]);
                        }
                        else if (strcmp(choice, "3") == 0)
                        {
                            deleteTask(searchResultList[0]);
                        }
                        else
                        {
                            displayTask(searchResultList[0]);
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
                openFileCheck = 0;
                break;
            }
            else if (strcmp(choice, "9") == 0) // exit
            {
                printf("Thankyou for use our service...\n"); // ui print bye to user
                exit(0);
            }
            else
            {
                printf("\n----- Invalid option, please select option again -----\n\n"); // ui print invalid choice
            }
        }
    }
}

void addNewProject(int *openFileCheck)
{
    int returnStatus;        /* store return value from function */
    char terminalInput[128]; /* store terminal input buffer */
    char fileNameIn[128];    /* store name of input file */

    while (1)
    {
        printf("Enter the new project name : "); // ui print tell user to enter file name
        memset(terminalInput, 0, sizeof(terminalInput));
        fgets(terminalInput, sizeof(terminalInput), stdin);
        sscanf(terminalInput, "%s", fileNameIn);
        if (strlen(fileNameIn) == 0)
        {
            printf("Try again, file name can't be empty\n"); // ui print input can't be empty
        }
        else
        {
            returnStatus = addNewProjectFile(fileNameIn, DATABASE_DIRECTORY);
            //printf("%d\n", returnStatus);
            if (returnStatus == 0)
            {
                printf("Oops, the file name already exist.\n");                       // ui print error file already exist
                printf("Are you sure to overwrite the old file? [YES=y|NO=else] : "); // ui print if want to create new file database with fileNameIn
                // if y = yes else = no
                memset(terminalInput, 0, sizeof(terminalInput));
                fgets(terminalInput, sizeof(terminalInput), stdin);
                sscanf(terminalInput, "%s", terminalInput);
                if (strcasecmp(terminalInput, "y") == 0)
                {
                    returnStatus = deleteProjectFile(fileNameIn, DATABASE_DIRECTORY);
                    returnStatus = addNewProjectFile(fileNameIn, DATABASE_DIRECTORY);
                    printf("Success to create new project.\n");                 // ui print add new database succesfully
                    printf("Do you want to enter this file [YES=y|NO=else]: "); // ui ask user if want to enter recently add project or do something else
                    memset(terminalInput, 0, sizeof(terminalInput));
                    fgets(terminalInput, sizeof(terminalInput), stdin);
                    sscanf(terminalInput, "%s", terminalInput);
                    if (strcasecmp(terminalInput, "y") == 0) // enter recently added
                    {
                        strcpy(workingFileName, fileNameIn);
                        *openFileCheck = 1;
                        break;
                    }
                    else // back to project option selection
                    {
                        break;
                    }
                }
            }
            else
            {
                printf("Doyo want to enter to project recently created[YES=y|NO=else]: ");
                memset(terminalInput, 0, sizeof(terminalInput));
                fgets(terminalInput, sizeof(terminalInput), stdin);
                sscanf(terminalInput, "%[^\n]s", terminalInput);
                if (strcasecmp(terminalInput, "y") == 0)
                {
                    strcpy(workingFileName, fileNameIn);
                    *openFileCheck = 1;
                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }
}

void enterExistProject(int *openFileCheck)
{
    int returnStatus;              /* store return value from function */
    int searchStatus;              /* store searching status */
    char terminalInput[128];       /* store terminal input buffer */
    char fileNameIn[128];          /* sore name of input file */
    char **projectNameList = NULL; /* store list of search result */
    int totalFile;                 /* store total file searched */

    projectNameList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalFile);
    printf("total :%d\n", totalFile);
    for (int i = 0; i < totalFile; i++)
    {
        printf("%d) %s\n", i + 1, projectNameList[i]);
    }
    while (1)
    {
        printf("Enter file name: ");
        memset(terminalInput, 0, sizeof(terminalInput));
        fgets(terminalInput, sizeof(terminalInput), stdin);
        sscanf(terminalInput, "%s", fileNameIn);
        if (strlen(fileNameIn) == 0)
        {
            printf("Try again, file name can't be empty.\n"); // ui tell user that input can't be empty
        }
        else
        {
            returnStatus = readInformationFile(fileNameIn, DATABASE_DIRECTORY);
            searchStatus = 0;
            for (int i = 0; i < totalFile; i++)
            {
                if (strcmp(fileNameIn, projectNameList[i]) == 0)
                {
                    searchStatus = 1;
                }
            }
            if (returnStatus == 0 || searchStatus == 0)
            {
                printf("Oops, The file name is not found.\n");                 // ui print invalid terminal input file name
                printf("Do you want to create new one ? [YES=y][NO=else] : "); // ui print if want to create new file database
                // if y = yes else = no
                memset(terminalInput, 0, sizeof(terminalInput));
                fgets(terminalInput, sizeof(terminalInput), stdin);
                if (strcasecmp(terminalInput, "y\n") == 0)
                {
                    //create new file with fileNameIn and open
                    returnStatus = addNewProjectFile(fileNameIn, DATABASE_DIRECTORY);
                    strcpy(workingFileName, fileNameIn);
                    *openFileCheck = 1;
                    break;
                }
            }
            else
            {
                strcpy(workingFileName, fileNameIn);
                *openFileCheck = 1;
                break;
            }
        }
    }
}

void deleteExistProject()
{
    int returnStatus;              /*return status*/
    char terminalInput[128];       /*string in terminal*/
    char fileNameDelete[128];      /*project file name that want to delete*/
    char **projectNameList = NULL; /*list of project name*/
    int totalFile;                 /*total file in database*/

    projectNameList = findProjectFileDatabase(DATABASE_DIRECTORY, &totalFile);
    for (int i = 0; i < totalFile; i++)
    {
        printf("%d) %s\n", i + 1, projectNameList[i]);
    }

    printf("Enter task name to delete: ");
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    sscanf(terminalInput, "%s", fileNameDelete);
    if (strlen(fileNameDelete) == 0)
    {
        printf("Try again, file name can't be empty.\n"); // ui print input cant be empty
    }
    else
    {
        returnStatus = deleteProjectFile(fileNameDelete, DATABASE_DIRECTORY);
        if (returnStatus == 1)
        {
            printf("Success to delete the file.\n"); // ui print succesfully delete file from database
        }
        else
        {
            printf("Oops, Can't see the file which you want to delete.\n"); // ui prin file not found
        }
    }
}

void renameProject()
{
    int returnStatus;         /* store return value from function */
    char terminalInput[128];  /* store terminal input buffer */
    char fileNameRename[128]; /* new file name to be raname into */

    printf("Enter new file name: ");
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    sscanf(terminalInput, "%s", fileNameRename);
    if (strlen(fileNameRename) == 0)
    {
        printf("Try again, file name can't be empty.\n"); // ui print input can't be empty
    }
    else
    {
        returnStatus = renameProjectFile(workingFileName, fileNameRename, DATABASE_DIRECTORY);
        //printf("file: %s\n",workingFileName);
        if (returnStatus == 1)
        {
            strcpy(workingFileName, fileNameRename);
            printf("Success to rename the project.\n"); // ui print that rename succesfully
        }
        else if (returnStatus == -1)
        {
            printf("Oops, the file name is already exist.\n"); // ui print that file fileNameRename already exist
        }
        else if (returnStatus == -2)
        {
            printf("Oops, not found the project to rename.\n"); // ui print that file fileNameRename already exist
        }
        else
        {
            printf("Oops, can't rename the project.\n");
        }
    }
}

void addTask()
{
    char terminalInput[128]; /* store terminal input buffer */
    char taskName[128];      /* sore task name */
    char taskInfo[128];      /* store task information */
    int taskWeight;          /* store weight of task */
    int returnStatus;        /* store retrun value from function */

    memset(terminalInput, 0, sizeof(terminalInput));
    printf("Enter key value for vertex: ");
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", taskName);
    memset(terminalInput, 0, sizeof(terminalInput));
    printf("Enter description for vertex: ");
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", taskInfo);
    memset(terminalInput, 0, sizeof(terminalInput));
    printf("Enter day work for vertex: ");
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%d", &taskWeight);
    if (strlen(taskName) == 0 || strlen(taskInfo) == 0)
    {
        printf(">>> Input error - key or data empty!\n");
    }
    else
    {
        returnStatus = addVertex(taskName, taskInfo, taskWeight);
        if (returnStatus == 1)
        {
            printf("%s\n", workingFileName);
            if (!writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead))
            {
                printf("cant write");
            }
            //printf("%s %s %d\n", vertexListHead->name, vertexListHead->description, vertexListHead->dayWork);
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
    char terminalInput[128];          /*string in terminal*/
    char taskName[128];               /*name of task*/
    char **resultList = NULL;         /*the result of function*/
    printf("Enter the task name : "); // ui print tell user to enter task name
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", taskName);
    strcpy(searchString, taskName);
    resultList = searchVertex(taskName, totalTask);
    return resultList;
}

void changeTaskInfo(char *taskName)
{
    char terminalInput[128];     /* store input buffer */
    char newTaskName[128] = {0}; /* store task name */
    char newTaskInfo[128] = {0}; /* store task info */
    int newTaskWeight = 0;       /* store task weight */
    int returnStatus;            /* store return value from function */

    printf("Enter new task name : "); // ui print tell user to enter new task name
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]s", newTaskName);
    printf("Enter new task information : "); // ui print tell user to enter new task info
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]s", newTaskInfo);
    printf("Enter new task weight : "); // ui print tell user to enter new task weight
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%d", &newTaskWeight);
    sscanf(terminalInput, "%s", terminalInput);
    if (newTaskWeight < 0)
    {
        printf("Task weight can't be less than 0\n");
    }
    else
    {
        returnStatus = modifyVertex(taskName, newTaskName, newTaskInfo, newTaskWeight);
        if (returnStatus == 1)
        {
            writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead);
            strcpy(taskName, newTaskName);
            printf("Success to change task information.\n"); // ui print change task info successfullt
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n"); // ui tell user can't find the task with that name
        }
    }
}

void addDependency(char *taskName)
{
    char terminalInput[128]; /*string in terminal*/
    char toTaskName[128];    /*name of task*/
    int returnStatus;        /*status of this function*/

    printf("Enter dependency destination : "); // ui tell user to enter dependency destination
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", toTaskName);
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n"); // ui print data cant be empty
    }
    else
    {
        returnStatus = addEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead);
            printf("Success to add dependency destination.\n"); // ui tell user add dependeny succesfully
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n"); // ui tell user can't find the task with that name
        }
        else if (returnStatus == -2)
        {
            printf("Oops, Dependency destination already exist.\n"); // ui tell user that that dependency already exist
        }
        else if (returnStatus == -3)
        {
            printf("Oops, It will be loop, if adding that edge.\n"); // ui tell user that add that edge will create loop
        }
        else
        {
            printf("Error to allocate memory.\n"); // ui tell user that memory allocation failed
        }
    }
}

void changeDependency(char *taskName)
{
    char terminalInput[128]; /*string in terminal from user input*/
    char fromTaskName[128];  /*vertex that direct to other*/
    char toTaskName[128];    /*destination vertex*/
    int returnStatus;        /*function status*/

    printf("Enter old dependency destination which you want to change : "); // ui tell user to enter old dependency destination
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", fromTaskName);
    printf("Enter new dependency destination : "); // ui tell user to enter new dependency destination
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", toTaskName);
    if (strlen(fromTaskName) == 0 || strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n"); // ui print data cant be empty
    }
    else
    {
        returnStatus = modifyEdge(taskName, fromTaskName, toTaskName);
        if (returnStatus == 1)
        {
            writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead);
            printf("Success to change dependency destination.\n"); // ui tell user change dependeny destination succesfully
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n"); // ui tell user can't find the task with that name
        }
        else if (returnStatus == -2)
        {
            printf("Oops, The name of each key must be unique. Can't be the same.\n"); // ui tell user that each key enter has to be unique not the same
        }
        else if (returnStatus == -3)
        {
            printf("Oops, The name of new dependency destination already exist.\n"); // ui tell user that dependency taskName -> toTaskName already exist
        }
        else
        {
            printf("Oops, The name of old dependency destination already exist.\n"); // ui tell user that dependency taskName -> fromTaskName not exist
        }
    }
}

void deleteDependency(char *taskName)
{
    char terminalInput[128]; /* store input buffer */
    char toTaskName[128];    /* store task name to */
    int returnStatus;        /* store return value from function */

    printf("Enter dependency destination which you want to delete : "); // ui tell user to enter dependency destination that want to delete
    memset(terminalInput, 0, sizeof(terminalInput));
    fgets(terminalInput, sizeof(terminalInput), stdin);
    terminalInput[strlen(terminalInput) - 1] = '\0';
    sscanf(terminalInput, "%[^\n]", toTaskName);
    if (strlen(toTaskName) == 0)
    {
        printf("Oops, dependency destination can't be empty.\n"); // ui print data cant be empty
    }
    else
    {
        returnStatus = deleteEdge(taskName, toTaskName);
        if (returnStatus == 1)
        {
            //printf("before\n");
            writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead);
            printf("Success to delete dependency destination.\n"); // ui tell user change dependeny destination succesfully
        }
        else if (returnStatus == -1)
        {
            printf("Oops, Can't find the task name.\n"); // ui tell user that given task name not exist
        }
        else
        {
            printf("Oops, Can't find that dependency destination.\n"); // ui tell that user given dependency doesn't exist
        }
    }
}

void modifyTask(char *taskName)
{
    char terminalInput[128]; /* store terminal input buffer */
    char choice[8];          /* store choice from user */

    while (1)
    {
        printf("There are 5 choices of modify task option :\n");
        printf("\t(1) Modify task information\n");
        printf("\t(2) Add edge\n");
        printf("\t(3) Change dependency destination\n");
        printf("\t(4) Delete dependency destination\n");
        printf("\t(5) Back\n"); // ui print modify task option 1) modify task info 2) add edge 3) chage dependecy dest 4) delete dependency 5) back
        printf("Enter tour option: ");
        memset(terminalInput, 0, sizeof(terminalInput));
        fgets(terminalInput, sizeof(terminalInput), stdin);
        terminalInput[strlen(terminalInput) - 1] = '\0';
        sscanf(terminalInput, "%s", choice);
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
            printf("\n----- Invalid option, please select option again -----\n\n"); // ui print invalid choice
        }
    }
}

void displayTask(char *taskName)
{
    VERTEX_T *pFound = NULL;  /* find vertex struct */
    VERTEX_T *pVertex = NULL; /* store vertex struct */
    EDGE_T *pEdge = NULL;     /* store edge struct */
    pFound = findVertex(taskName);
    printf("Task Name: |%s|\n", pFound->name); // ui print info about task
    printf("Task Description: %s\n", pFound->description);
    printf("Task Working Day: %d\n", pFound->dayWork);
    printf("Dependency Of:\n");
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
}

void deleteTask(char *taskName)
{
    int returnStatus; /* store function return value */

    returnStatus = deleteVertex(taskName);
    writeInformationFile(workingFileName, DATABASE_DIRECTORY, vertexListHead);
    printf("Success to delete task.\n"); // ui print delete task succesful
}

void dispalyAllTask()
{
    VERTEX_T *pCurrentVertex = NULL; /* loop through vertex list */
    VERTEX_T *pAdjVertex = NULL;     /* store adjacent vertex */
    VERTEX_T *pCurrentEdge = NULL;   /* loop through edge list */

    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        displayTask(pCurrentVertex->name);
        pCurrentVertex = pCurrentVertex->pNext;
    }
}

void calculateProjectSchedule()
{
}
