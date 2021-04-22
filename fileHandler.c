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
 *=======================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "abstractNetwork.h"

/*typedef struct _adjVertex
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
} VERTEX_T;*/

/************************************************
*	This function is from stack overflow
*	REF:https://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
*	
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*	
*/

#define addressDatabaseDirectory "../fileReadtest/project_list_database"

void printMenu();
char **findProjectFileDatabase(char *addressFolder, int *totalFile);
int addNewProjectFile(char projectName[], char *addressFolder);
int existProjectFileCheck(char projectName[], char *addressFolder);
int renameProjectFile(char oldFileName[], char newFileName[], char *addressFolder);
int deleteProjectFile(char projectName[], char *addressFolder);
int readInformationFile(char projectName[], char *addressFolder);

/*int main()
{
    char input[64] = {0};
    char userProjectName[64] = {0};
    int numberInput = 0;
    int status = 0;
    int i = 0;
    char ** allProjectName = NULL;

    while(numberInput >= 0)
    {
        printf("Directory scan of /home:\n");
        printMenu();
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%d",&numberInput);
        switch(numberInput)
        {

            case 1:
                printf("\nFile in folder[].......\n");
                printf("------------------------------\n");
                allProjectName = findProjectFileDatabase(addressDatabaseDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                printf("------------------------------\n");
                i=0;
                break;
            
            case 2:
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                addNewProjectFile(userProjectName,addressDatabaseDirectory);
                break;
            
            case 3:
                printf("File in folder[].......\n");
                allProjectName = findProjectFileDatabase(addressDatabaseDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                status = renameProjectFile(addressDatabaseDirectory);
                if(status == 1)
                {
                    printf("rename success\n");
                }
                else
                {
                    printf("rename not success\n");
                }
                break;
            case 4:
                allProjectName = findProjectFileDatabase(addressDatabaseDirectory);
                while(allProjectName[i] != NULL)
                {
                    printf("Name: %s\n",allProjectName[i]);
                    i++;
                }
                deleteProjectFile(addressDatabaseDirectory);
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
#########################################################
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
*/

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
char **findProjectFileDatabase(char *addressFolder, int *totalFile)
{
    DIR *directory;
    struct dirent *projectDatabase;

    int nameLength = 0;
    int projectCount = 0;
    int listIndex = 0;
    char projectName[128];
    char *outputName[64] = {0};
    char **projectNameList = NULL;

    directory = opendir(addressFolder);
    if (directory == NULL)
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
            projectCount++;
        }
        closedir(directory);
        if ((directory = opendir(addressFolder)) != NULL)
        {
            projectNameList = calloc(projectCount, sizeof(outputName));
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
    *totalFile = projectCount;
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
    DIR *databasedirectory;
    char projectFileName[128];
    int addNew_status = -1;

    FILE *outputFileproject = NULL;

    addNew_status = existProjectFileCheck(projectFileName, addressFolder);
    if (addNew_status == 0)
    {
        sscanf(projectName, "%s", projectFileName);
        strcat(projectFileName, "-database.dat");
        chdir(addressFolder);
        outputFileproject = fopen(projectFileName, "w");
        fclose(outputFileproject);
        addNew_status = 1;
    }
    else
    {
        addNew_status = 0;
    }
    return addNew_status;
    chdir("..");
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
    char input[64] = {0};
    char conditions[64] = {0};
    char deleteProjectName[64] = {0};

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
        return delete_status;
    }
    else
    {
        delete_status = -1;
    }
    chdir("..");
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
    char input[64] = {0};
    char oldFileName[64] = {0};
    char newFileName[64] = {0};

    int rename_status = -2;

    //sscanf(oldProjectName, "%s", oldFileName);
    strcat(oldFileName, "-database.dat");
    if (existProjectFileCheck(oldFileName, addressFolder) == 0)
    {
        rename_status = -2;
    }
    else
    {
        sscanf(newProjectName, "%s", newFileName);
        strcat(newFileName, "-database.dat");
        if (existProjectFileCheck(newFileName, addressFolder) == 1)
        {
            rename_status = -1;
        }
        else
        {
            chdir(addressFolder);
            rename_status = rename(oldProjectName, newProjectName);
            if (rename_status == 0)
            {
                rename_status = 1;
            }
            else
            {
                rename_status = 0;
            }
            return rename_status;
        }
    }
    chdir("..");
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
* This functino will return 1 if found the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==================================================================================================
*/
int existProjectFileCheck(char projectName[], char *addressFolder)
{
    char projectNameTemp[128];
    FILE *existProjectFileName = NULL;

    chdir(addressFolder);
    memset(projectNameTemp, 0, sizeof(projectNameTemp));
    sscanf(projectName, "%s", projectNameTemp);
    strcat(projectNameTemp, "-database.dat");
    //printf("user enter project: %s\n", projectNameTemp);
    existProjectFileName = fopen(projectNameTemp, "r");
    if (existProjectFileName == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    fclose(existProjectFileName);
    chdir("..");
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
    char input[64] = {0};
    char inputLine[256] = {0};
    char projectFileName[128] = {0};
    char taskName[64] = {0};
    char information[128] = {0};
    char charWeight[8] = {0};
    char temp[128] = {0};
    char keyEdgeOne[64] = {0};
    char keyEdgeTwo[64] = {0};
    int weight = 0;
    int add_edge_status = -4;
    int add_vertex_status = -2;
    int status = 1;

    FILE *databaseFile = NULL;

    if (existProjectFileCheck(projectName, addressFolder) == 1)
    {
        chdir(addressDatabaseDirectory);
        sscanf(projectName, "%s", projectFileName);
        strcat(projectFileName, "-database.dat");
        databaseFile = fopen(projectFileName, "r+");
        initNetwork();
        while (fgets(inputLine, sizeof(inputLine), databaseFile) != NULL)
        {
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
int writeInformationFile(char projectName[], char *addressFolder, void *vertexStruct)
{
    char input[64] = {0};
    char inputLine[128] = {0};
    char projectFileName[128] = {0};
    char taskName[64] = {0};
    char taskInformation[128] = {0};
    char charWeight[8] = {0};
    char temp[128] = {0};
    char keyEdgeOne[64] = {0};
    char keyEdgeTwo[64] = {0};
    int taskWeight = 0;
    int status = 0;

    VERTEX_T *currentVertex = NULL;
    VERTEX_T *currentEdge = NULL;
    EDGE_T *currentAdjencent = NULL;

    FILE *currentProjectFile = NULL;

    sscanf(projectName, "%s", projectFileName);
    strcat(projectFileName, "-database.dat");
    //printf("%s", projectFileName);
    if (existProjectFileCheck(projectName, addressFolder) == 1)
    {
        currentProjectFile = fopen(projectFileName, "w");

        currentVertex = vertexStruct;
        fprintf(currentProjectFile, "VERTEX:\n");
        while (currentVertex != NULL)
        {
            strcpy(taskName, currentVertex->name);
            strcpy(taskInformation, currentVertex->description);
            taskWeight = currentVertex->dayWork;
            //printf("%s %s %d\n", taskName, taskInformation, taskWeight);
            fprintf(currentProjectFile, "NAME:%s;INFORMATION:%s;WEIGHT:%d;\n", taskName, taskInformation, taskWeight);
            currentVertex = currentVertex->pNext;
        }
        //printf("After\n");
        currentEdge = vertexStruct;
        fprintf(currentProjectFile, "EDGE:\n");
        while (currentEdge != NULL)
        {
            currentAdjencent = currentEdge->adjListHead;
            while (currentAdjencent != NULL)
            {
                currentVertex = currentAdjencent->pVertex;
                if (strcasecmp(currentVertex->name, "end") != 0)
                {
                    strcpy(keyEdgeOne, currentEdge->name);
                    strcpy(keyEdgeTwo, currentVertex->name);
                    fprintf(currentProjectFile, "FROM:%s;TO:%s;\n", keyEdgeOne, keyEdgeTwo);
                }
                currentAdjencent = currentAdjencent->pNext;
            }
            currentEdge = currentEdge->pNext;
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
