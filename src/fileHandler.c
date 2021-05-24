/*=======================================================================
 *  fileHandler.c
 *
 *  This program will manage file to read and store the information
 *  in database folder.
 * 
 *  create by
 *      Name:Pattaraphum chuamuangphan
 *      ID:63070503437
 * 
 *                  _______
 *                 < Hello >
 *                  -------
 *                         \   ^__^
 *                          \  (oo)\_______
 *                             (__)\       )\/\
 *                                 ||----w |
 *                                 ||     ||
 *
 *
 *  The version of this file is 1.01
 *=======================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "abstractNetwork.h"
#include "dateCalendarManager.h"

/*
typedef struct _adjVertex
{
    void *pVertex;
    int weight;
    struct _adjVertex *pNext;
} EDGE_T;

typedef struct _vertex
{
    char *name;
    char *description;
    int dayWork;
    int totalDay;
    int color;
    struct _vertex *pNext;
    struct _vertex *pParentVertex;
    EDGE_T *adjListHead;
    EDGE_T *adjListTail;
} VERTEX_T;
*/
/************************************************
*	This function is from stack overflow
*	REF:https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
*	
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*	
*/

//#define addressDatabaseDirectory "../fileReadtest/project_list_database/testDirectory"
//#define addressDatabaseDirectory "../CPE_PROJECT/project_list_database"

void printMenu();
char **findProjectFileDatabase(char *addressFolder, int *totalProject);             /*This function will find database file in directory*/
int addNewProjectFile(char projectName[], char *addressFolder);                     /*This function add new project in database*/
int existProjectFileCheck(char projectName[], char *addressFolder);                 /*This function will check exist project in databse*/
int renameProjectFile(char oldFileName[], char newFileName[], char *addressFolder); /*This function will rename exist poject*/
int deleteProjectFile(char projectName[], char *addressFolder);                     /*This function will read information in databse file*/
int readInformationFile(char projectName[], char *addressFolder);                   /*This function will write all information in datbase file*/
char *findProjectDatabaseDirectory();                                               /*This function will find execute program directort*/

/*int main(int argc,char* argv[])
{
    char input[64] = {0};
    char userProjectName[64] = {0};
    char userProjectNew[64] = {0};
    int numberInput = 0;
    int status = 0;
    int i = 0;
    char ** allProjectName = NULL;
    char * testDirectory = NULL;
    char * tempDirectory = NULL;
    char * mainDirectory = NULL;

    mainDirectory = findProjectDatabaseDirectory();
    testDirectory = findProjectDatabaseDirectory();
    while(numberInput >= 0)
    {
        printf("Directory scan of /home:\n");
        printMenu();
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%d",&numberInput);
        //strcpy(*tempDirectory,addressDatabaseDirectory);
        switch(numberInput)
        {
            case 1:
                printf("\nFile in folder[].......\n");
                printf("------------------------------\n");
                printf("DIR IS = %s\n",testDirectory);
                allProjectName = findProjectFileDatabase(testDirectory);
                if(allProjectName == NULL)
                {
                    printf("\n\n-----error------\n");
                }
                mainDirectory = findProjectDatabaseDirectory();
                printf("DIR IS = %s\n",mainDirectory);
                printf("test1\n");
                printf("argv[0] = %s\n",argv[0]);
                if(allProjectName != NULL)
                {
                    while(allProjectName[i] != NULL)
                    {
                        printf("Name: %s\n",allProjectName[i]);
                        i++;
                    }
                }
                printf("------------------------------\n");
                
                i=0;
                break;
            case 2:
                tempDirectory = testDirectory;
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                status = addNewProjectFile(userProjectName,tempDirectory);
                if(status == 1)
                {
                    printf("good\n");
                }
                break;
            case 3:
                printf("File in folder[].......\n");
                allProjectName = findProjectFileDatabase(testDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                i=0;
                printf("Enter project old:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                printf("Enter project new:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectNew);
                allProjectName = findProjectFileDatabase(testDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                i=0;
                status = renameProjectFile(userProjectName, userProjectNew, testDirectory);
                if(status == 1)
                {
                    printf("rename success\n");
                }
                else if(status == 0)
                {
                    printf("\n\nrename not success: error on function rename()\n");
                }
                else if(status == -1)
                {
                    printf("\n\nrename not success: already have a project name\n");
                }
                else if(status == -2)
                {
                    printf("\n\nrename not success: not found project\n");
                }
                break;
                allProjectName = findProjectFileDatabase(testDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                i=0;
                break;
            case 4:
                allProjectName = findProjectFileDatabase(testDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                i=0;
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                status = deleteProjectFile(userProjectName,testDirectory);
                if(status == 1) 
                {
                    printf("\n\n----Delete Success----\n");
                }
                else if(status == 0) 
                {
                    printf("\n\n----Some thing error----\n");
                }
                else if(status == -1) 
                {
                    printf("\n\n----Can not find project name----\n");
                }
                allProjectName = findProjectFileDatabase(testDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                i=0;
                break;
            case 5:
                allProjectName = findProjectFileDatabase(addressDatabaseDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                readInformationFile(userProjectName,addressDatabaseDirectory);
                i=0;
                break;
            default:
                printf("all of above\n");
            break;
        }
        printf("\n\n");
    }
}
/*#########################################################*/
void printMenu()
{
    printf("Test project program\n");
    printf("-------------------------------\n");
    printf("1-Print directory\n");
    printf("2-Create new project file\n");
    printf("3-Project rename\n");
    printf("4-Delete project\n");
    printf("5-Read file and built node\n");
    printf("\nSelect(-1 to exit):");
}

/*========================================================================================
* This function will read all porject file in the folder database directory 
* and then print out all the exist project that have in folder.
*
* This function is from stack overflow
* REF:https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
*
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       addressFolder   -   the address of the database folder 
* This function will return the project name list                         
*========================================================================================
*/
char **findProjectFileDatabase(char *addressFolder, int *totalProject)
{
    DIR *directory;                 /*Directory struct in dirent.h*/
    struct dirent *projectDatabase; /*Directory struct in dirent.h*/

    int nameLength = 0;            /*Length of file name*/
    int listIndex = 0;             /*List of file in directory*/
    char projectName[128];         /*Project name*/
    char *outputName[64] = {0};    /*project name*/
    char **projectNameList = NULL; /*List of project file in directory*/
    if ((directory = opendir(addressFolder)) == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", addressFolder);
    }
    else
    {
        while ((projectDatabase = readdir(directory)) != NULL)
        {
            if (strcmp(projectDatabase->d_name, ".") == 0 ||
                strcmp(projectDatabase->d_name, "..") == 0)
                continue;
            ++*totalProject;
        }
        closedir(directory);
        if ((directory = opendir(addressFolder)) != NULL)
        {
            projectNameList = calloc(*totalProject, sizeof(outputName));
            while ((projectDatabase = readdir(directory)) != NULL)
            {
                if (strcmp(projectDatabase->d_name, ".") == 0 ||
                    strcmp(projectDatabase->d_name, "..") == 0)
                    continue;
                memset(projectName, 0, sizeof(projectName));
                sscanf(projectDatabase->d_name, "%s", projectName);
                nameLength = strlen(projectName);
                for (int i = nameLength - 13; i < nameLength; i++)
                {
                    projectName[i] = 0;
                }
                projectNameList[listIndex] = calloc(1, sizeof(outputName));
                strcpy(projectNameList[listIndex], projectName);
                listIndex++;
            }
        }
    }
    return projectNameList;
    chdir("..");
    closedir(directory);
}

/*=========================================================================================
* This function will add new project file in database folder directory 
* 
*	create by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       projectName     -   the project name that user enter
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if success to add new project file in database
* and return 0 if found duplicated the project name.
*==========================================================================================
*/
int addNewProjectFile(char projectName[], char *addressFolder)
{
    DIR *databasedirectory;    /*Directory struct in dirent.h*/
    char projectFileName[128]; /*Project file name*/
    int addNew_status = -1;    /*Function status*/

    FILE *outputFileproject = NULL; /*Database file*/

    sscanf(projectName, "%s", projectFileName);
    addNew_status = existProjectFileCheck(projectFileName, addressFolder);
    if (addNew_status == 0)
    {
        chdir(addressFolder);
        strcat(projectFileName, "-database.dat");

        outputFileproject = fopen(projectFileName, "w");
        fclose(outputFileproject);
        addNew_status = 1;
    }
    else
    {
        addNew_status = 0;
    }
    return addNew_status;
}

/*=========================================================================================
* This function will deltete exist project in database file 
* This function will ask user to enter exist project name and then delete that user 
*
*	create by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       projectName     -   the project name that user enter
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if delete success, return 0 if can not delete the project
* and return -1 if does not found the project name.
*==========================================================================================
*/
int deleteProjectFile(char projectName[], char *addressFolder)
{
    char input[64] = {0};             /*Input from user*/
    char conditions[64] = {0};        /*Condition in loop*/
    char deleteProjectName[64] = {0}; /*Project name*/
    char *currentDirectory = NULL;    /*Directory of execute program*/

    int delete_status = 0;

    if (existProjectFileCheck(projectName, addressFolder) == 1)
    {
        chdir(addressFolder);
        sscanf(projectName, "%s", deleteProjectName);
        strcat(deleteProjectName, "-database.dat");
        delete_status = remove(deleteProjectName);
        if (delete_status == 0)
        {
            delete_status = 1;
        }
        else
        {
            delete_status = 0;
        }
    }
    else
    {
        delete_status = -1;
    }
    return delete_status;
}

/*=========================================================================================
* This function will rename the exist project name 
* This function will ask the exist project name that want to rename 
* and ask new project name and then rename it.
*	create by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*		oldProjectName	-	the old project name 
*		newProjectName 	-	the new project name
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if rename success, return 0 if can not rename the file,
* return -1 if already have exist project and return -2 if does not found the project name.
*==========================================================================================
*/
int renameProjectFile(char oldProjectName[], char newProjectName[], char *addressFolder)
{
    char input[64] = {0};       /*Input from user*/
    char oldFileName[64] = {0}; /*Old file name that want to rename*/
    char newFileName[64] = {0}; /*New file name that want to rename*/

    int rename_status = -2; /*Function status*/

    sscanf(oldProjectName, "%s", oldFileName);
    if (existProjectFileCheck(oldFileName, addressFolder) == 0)
    {
        rename_status = -2;
    }
    else
    {
        sscanf(newProjectName, "%s", newFileName);
        if (existProjectFileCheck(newFileName, addressFolder) == 1)
        {
            rename_status = -1;
        }
        else
        {
            chdir(addressFolder);
            strcat(oldFileName, "-database.dat");
            strcat(newFileName, "-database.dat");
            rename_status = rename(oldFileName, newFileName);

            if (rename_status == 0)
            {
                rename_status = 1;
            }
            else
            {
                rename_status = 0;
            }
        }
    }
    return rename_status;
}

/*=================================================================================================
* This function will check the exist project in file directory.
*
* This function is from stack overflow
* REF:https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
*
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       projectName     -   the project name that user enter
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if fond the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==================================================================================================
*/
int existProjectFileCheck(char projectName[], char *addressFolder)
{
    char projectNameTemp[128];         /*Project name*/
    FILE *existProjectFileName = NULL; /*Exist project in directory*/

    chdir(addressFolder);
    memset(projectNameTemp, 0, sizeof(projectNameTemp));
    sscanf(projectName, "%s", projectNameTemp);
    strcat(projectNameTemp, "-database.dat");
    //printf("user enter project: %s\n", projectNameTemp);
    existProjectFileName = fopen(projectNameTemp, "r");
    if (existProjectFileName != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    fclose(existProjectFileName);
}

/*=========================================================================================
* This function will read the database file and then create vertex and add edge of any vertex
*
*	create by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       projectName     -   the project name that user enter
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if every thing success well, return 0 if something wrong when
* add vertex or add edge and return -1 if the project name does not exist.
*==========================================================================================
*/
int readInformationFile(char projectName[], char *addressFolder)
{
    char input[64] = {0};               /*Input from user*/
    char inputLine[256] = {0};          /*Line in file*/
    char projectFileName[128] = {0};    /*Project file name*/
    char taskName[64] = {0};            /*Task name*/
    char information[128] = {0};        /*Task information*/
    char charWeight[8] = {0};           /*Day work of task*/
    char temp[128] = {0};               /*Temp text*/
    char keyEdgeOne[64] = {0};          /*Task name*/
    char keyEdgeTwo[64] = {0};          /*Task name*/
    char projectDescription[256] = {0}; /*Description of project*/
    int weight = 0;                     /*Task weight*/
    int add_edge_status = -4;           /*Function status*/
    int add_vertex_status = -2;         /*Function status*/
    int status = 1;                     /*Function status*/
    int weekend = 0;                    /*Weekend status*/
    time_t unixTime;                    /*Time struct in time.h*/

    FILE *databaseFile = NULL; /*Database file*/

    if (existProjectFileCheck(projectName, addressFolder) == 1)
    {
        chdir(addressFolder);
        sscanf(projectName, "%s", projectFileName);
        strcat(projectFileName, "-database.dat");
        databaseFile = fopen(projectFileName, "r+");
        initNetwork();
        while (fgets(inputLine, sizeof(inputLine), databaseFile) != NULL)
        {
            if (sscanf(inputLine, "ABOUT:%[^;];", projectDescription) == 1)
            {
                printf("projectDescription:%s\n", projectDescription);
                setProjectDescription(projectDescription);
            }
            if (sscanf(inputLine, "NAME:%[^;];INFORMATION:%[^;];WEIGHT:%[^;];", taskName, information, charWeight) == 3)
            {
                sscanf(charWeight, "%d", &weight);
                add_vertex_status = addVertex(taskName, information, weight);
                if (add_vertex_status == 1)
                {
                    continue;
                }
                else if (add_vertex_status == 0)
                {
                    printf("\tMemory allocation failed\n");
                    status = 0;
                }
                else if (add_vertex_status == -1)
                {
                    printf("\tDuplicated task name '%s'\n", taskName);
                    status = 0;
                }
            }
            if (sscanf(inputLine, "FROM:%[^;];TO:%[^;];", keyEdgeOne, keyEdgeTwo) == 2)
            {
                add_edge_status = addEdge(keyEdgeOne, keyEdgeTwo);
                if (add_edge_status == 1)
                {
                    continue;
                }
                else if (add_edge_status == 0)
                {
                    printf("\tMemory allocation failed\n");
                    status = 0;
                }
                else if (add_edge_status == -1)
                {
                    printf("\t'%s' or '%s' does not in vertex.\n", keyEdgeOne, keyEdgeTwo);
                    status = 0;
                }
                else if (add_edge_status == -2)
                {
                    printf("\tEdge between '%s' to '%s' already exist.\n", keyEdgeOne, keyEdgeTwo);
                    status = 0;
                }
                else if (add_edge_status == -3)
                {
                    printf("\tCan not create '%s' to '%s' cause make the loop.\n", keyEdgeOne, keyEdgeTwo);
                    status = 0;
                }
            }
            if (sscanf(inputLine, "WEEKEND:%d;", &weekend) == 1)
            {
                if (weekend == 1)
                {
                    setWeekendStatus();
                }
            }
            if (sscanf(inputLine, "UNIXTIME:%[^;];", input) == 1)
            {
                sscanf(input, "%ld", &unixTime);
                status = addDateToList(unixTime);
            }
        }
        fclose(databaseFile);
    }
    else
    {
        printf("\tcan not find '%s' in project name\n", projectName);
        status = -1;
    }
    return status;
    chdir("..");
}
/*=========================================================================================
* This function will write all information of project into the database file.
*
*	create by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       projectName     -   the project name that user enter
*       addressFolder   -   the address of the database folder 
*       vertexStruct    -   the pointer of head vertesx structure 
* This functino will return 1 if can write the information in file and return 0 if can not
* open the file;
*==========================================================================================
*/
int writeInformationFile(char projectName[], char *addressFolder)
{
    char input[64] = {0};               /*Input from user*/
    char inputLine[128] = {0};          /*Line in file*/
    char projectFileName[128] = {0};    /*Project file name*/
    char taskName[64] = {0};            /*Task name*/
    char taskInformation[128] = {0};    /*Task information*/
    char charWeight[8] = {0};           /*Weigth of task*/
    char temp[128] = {0};               /*Temp text*/
    char keyEdgeOne[64] = {0};          /*Task name*/
    char keyEdgeTwo[64] = {0};          /*Task name*/
    char projectDescription[256] = {0}; /*Description of project*/
    int taskWeight = 0;                 /*Weight of task*/
    int status = 0;                     /*Function status*/
    int weekend = 0;                    /*Weekend status*/
    time_t wUnixTime;                   /*Time struct in time.h*/

    VERTEX_T *currentVertex = NULL;   /*Current task strcut*/
    VERTEX_T *currentAdjacent = NULL; /*Current adjacent struct*/
    EDGE_T *currentEdge = NULL;       /*Current edge struct*/
    DATE_T *timeList = NULL;          /*List of time*/

    FILE *currentProjectFile = NULL; /*Project file*/

    sscanf(projectName, "%s", projectFileName);
    strcat(projectFileName, "-database.dat");
    if (existProjectFileCheck(projectName, addressFolder) == 1)
    {
        currentProjectFile = fopen(projectFileName, "w");
        getProjectDescription(projectDescription);
        fprintf(currentProjectFile, "PROJECT DESCRIPTION:\n");
        fprintf(currentProjectFile, "ABOUT:%s;\n", projectDescription);
        currentVertex = getVertexListHead();
        fprintf(currentProjectFile, "VERTEX:\n");
        while (currentVertex != NULL)
        {
            strcpy(taskName, currentVertex->name);
            strcpy(taskInformation, currentVertex->description);
            taskWeight = currentVertex->dayWork;
            fprintf(currentProjectFile, "NAME:%s;INFORMATION:%s;WEIGHT:%d;\n", taskName, taskInformation, taskWeight);
            currentVertex = currentVertex->pNext;
        }
        currentVertex = getVertexListHead();
        fprintf(currentProjectFile, "EDGE:\n");
        while (currentVertex != NULL)
        {
            currentEdge = currentVertex->adjListHead;
            while (currentEdge != NULL)
            {
                currentAdjacent = currentEdge->pVertex;
                if (strcmp(currentAdjacent->name, "end") != 0)
                {
                    strcpy(keyEdgeOne, currentVertex->name);
                    strcpy(keyEdgeTwo, currentAdjacent->name);
                    fprintf(currentProjectFile, "FROM:%s;TO:%s;\n", keyEdgeOne, keyEdgeTwo);
                }
                currentEdge = currentEdge->pNext;
            }
            currentVertex = currentVertex->pNext;
        }
        fprintf(currentProjectFile, "WEEKEND:%d;\n", getWeekendStatus());
        fprintf(currentProjectFile, "DAYOFF:\n");
        timeList = getDateListHead();
        while (timeList != NULL)
        {
            wUnixTime = timeList->unixTime;
            fprintf(currentProjectFile, "UNIXTIME:%ld;\n", wUnixTime);
            timeList = timeList->pNext;
        }
        status = 1;
        fclose(currentProjectFile);
    }
    else
    {
        printf("can not find '%s' in project name\n", projectName);
        status = 0;
    }
    return status;
}

/*=================================================================================================
* This function will find the working program folder directory.
*
* This function is from stack overflow
* REF:https://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program
*
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
* This functino will return the database directory.
*==================================================================================================
*/
char *findProjectDatabaseDirectory()
{
    int findDatabase_status = -2;      /*Function status*/
    char cwd[256];                     /*Directory name*/
    char databaseDirectory[128] = {0}; /*Database directory*/
    char *addressDirectory = NULL;     /*address of directory*/

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        sscanf(cwd, "%[^\n]", databaseDirectory);
        strcat(databaseDirectory, "/dataBase");
        addressDirectory = calloc(1, sizeof(databaseDirectory));
        strcpy(addressDirectory, databaseDirectory);
    }
    else
    {
        perror("getcwd() error: can not return the directory of database\n");
    }
    return addressDirectory;
}
