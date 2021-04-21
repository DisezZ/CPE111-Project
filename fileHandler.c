/*############################################
 *  fileHandler.c
 *  hello world 
 * 
 *  -directory read
 *  -savefile
 *############################################
 */
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "abstractNetwork.h"

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
char ** findProjectFileDatabase(char *addressFolder);
int addNewProjectFile(char projectName[],char *addressFolder);
int existProjectFileCheck(char projectName[],char* addressFolder);
int renameProjectFile(char* addressFolder);
int deleteProjectFile(char* addressFolder);
int readInformationFile(char projectName[],char* addressFolder);

int main()
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
*                           
*========================================================================================
*/
char ** findProjectFileDatabase(char *addressFolder)
{
    DIR *directory;
    struct dirent *projectDatabase;

    int nameLength = 0;
    int projectCount = 0;
    int listIndex = 0;
    char projectName[128];
    char * outputName[64] = {0};
    char ** projectNameList = NULL;


    if((directory = opendir(addressFolder)) == NULL) 
    {
        fprintf(stderr,"cannot open directory: %s\n",addressFolder);
    }
    else
    {
        while((projectDatabase = readdir(directory)) != NULL)
        {
            if(strcmp(projectDatabase->d_name,".") == 0 ||
                strcmp(projectDatabase->d_name,"..") == 0 )
                continue;
            projectCount++;
        }
        closedir(directory);
        if((directory = opendir(addressFolder)) != NULL) 
        {
            projectNameList = calloc(projectCount,sizeof(outputName));
            while((projectDatabase = readdir(directory)) != NULL)
            {
                if(strcmp(projectDatabase->d_name,".") == 0 ||
                    strcmp(projectDatabase->d_name,"..") == 0 )
                    continue;
                memset(projectName,0,sizeof(projectName));
                sscanf(projectDatabase->d_name,"%s",projectName);
                nameLength = strlen(projectName);
                for(int i = nameLength-13; i<nameLength; i++)
                {
                    projectName[i] = 0;
                }
                projectNameList[listIndex] = calloc(1,sizeof(outputName));
                strcpy(projectNameList[listIndex],projectName);
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
* and return 0 if can not add new project file.
*==========================================================================================
*/
int addNewProjectFile(char projectName[],char *addressFolder)
{   
    DIR *databasedirectory;
    char projectFileName[128];
    int existProject_status = -1;

    FILE * outputFileproject = NULL;

    sscanf(projectName,"%s",projectFileName);
    strcat(projectFileName,"-database.dat");
    existProject_status = existProjectFileCheck(projectFileName,addressFolder);
    if(existProject_status == 0)
    {
        chdir(addressFolder);
        outputFileproject = fopen(projectFileName,"w");
        fclose(outputFileproject);
    }
    else
    {
        printf("'%s' is dupicated project name\n",projectName);
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
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if fond the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==========================================================================================
*/
int renameProjectFile(char* addressFolder)
{
    char input[64] = {0};
    char oldProjectName[64] = {0};
    char newProjectName[64] = {0};

    int rename_status = 0;

    printf("Old project name:");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",oldProjectName);
    if(existProjectFileCheck(oldProjectName,addressFolder) == 0)
    {
        printf("can not find '%s' in project name\n",oldProjectName);
    }
    else
    {
        printf("new project name:");
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%s",newProjectName);
        if(existProjectFileCheck(newProjectName,addressFolder) == 1)
        {
            printf("'%s' is dupicated project name\n",newProjectName);
        }
        else
        {
            chdir(addressFolder);
            strcat(oldProjectName,"-database.dat");
            strcat(newProjectName,"-database.dat");
            rename_status = rename(oldProjectName,newProjectName);
            if(rename_status == 0)
            {
                rename_status = 1;
            }
            else
            {
                rename_status = 0;
            }
            return  rename_status;
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
* This functino will return 1 if fond the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==================================================================================================
*/
int existProjectFileCheck(char projectName[],char* addressFolder)
{
    char projectNameTemp[128];
    FILE * existProjectFileName = NULL;

    chdir(addressFolder);
    memset(projectNameTemp,0,sizeof(projectNameTemp));
    sscanf(projectName,"%s",projectNameTemp);
    strcat(projectNameTemp,"-database.dat");
    //printf("user enter project: %s\n",projectNameTemp);
    if(existProjectFileName = fopen(projectNameTemp,"r"))
    {
        return 1;
    }
    else
    {
        return 0;
    }
    fclose(existProjectFileName);
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
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if fond the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==========================================================================================
*/
int deleteProjectFile(char* addressFolder)
{
    char input[64] = {0};
    char conditions[64] = {0};
    char deleteProjectName[64] = {0};

    int delete_status = 0;

    printf("..<!>..[Delete project]..<!>..\n");
    printf("Enter project Name:");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",deleteProjectName);
    if(existProjectFileCheck(deleteProjectName,addressFolder) == 1)
    {
        printf("Are you sure to delete '%s' project(YES|NO):",deleteProjectName);
        fgets(input,sizeof(input),stdin);
        sscanf(input,"%s",conditions);
        if(strcasecmp(conditions,"yes") == 0)
        {
            chdir(addressFolder);
            strcat(deleteProjectName,"-database.dat");
            delete_status = remove(deleteProjectName);
            if(delete_status == 0)
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
            printf("\t'%s' doesn't delete",deleteProjectName);
        }
    }
    else
    {
        printf("\tcan not find '%s' in project name\n",deleteProjectName);
    }
    chdir("..");
}

int readInformationFile(char projectName[],char* addressFolder)
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
    int  weight = 0;
    int add_edge_status = -4;
    int add_vertex_status = -2;

    FILE * databaseFile = NULL;

    if(existProjectFileCheck(projectName,addressFolder) == 1)
    {
        chdir(addressDatabaseDirectory);
        sscanf(projectName,"%s",projectFileName);
        strcat(projectFileName,"-database.dat");
        databaseFile = fopen(projectFileName,"r+");
        initNetwork();
        while(fgets(inputLine,sizeof(inputLine),databaseFile) != NULL)
        {
            if(sscanf(inputLine,"NAME:%[^;];INFORMATION:%[^;];WEIGHT:%[^;];"
                    ,taskName,information,charWeight) == 3)
            {
                add_vertex_status = addVertex(taskName,information,weight); 
                if(add_vertex_status == 1)
                {
                    continue;
                }
                else if(add_vertex_status == 0)
                {
                    printf("\tMemory allocation failed\n");
                }
                else if(add_vertex_status == -1)
                {
                    printf("\tDuplicated task name '%s'\n",taskName);
                }
            }
            if(sscanf(inputLine,"FROM:%[^;];TO:%[^;];",keyEdgeOne,keyEdgeTwo) == 2)
            {
                add_edge_status = addEdge(keyEdgeOne,keyEdgeTwo);
                if(add_edge_status == 1)
                {
                    continue;
                }
                else if(add_edge_status == 0)
                {
                    printf("\tMemory allocation failed\n");
                }
                else if(add_edge_status == -1)
                {
                    printf("\t'%s' or '%s' does not in vertex.\n",keyEdgeOne,keyEdgeTwo);
                }
                else if(add_edge_status == -2)
                {
                    printf("\tEdge between '%s' to '%s' already exist.\n",keyEdgeOne,keyEdgeTwo);
                }
                else if(add_edge_status == -3)
                {
                    printf("\tCan not create '%s' to '%s' cause make the loop.\n",keyEdgeOne,keyEdgeTwo);
                }
            }
        }
        fclose(databaseFile);
    }
    else
    {
        printf("\tcan not find '%s' in project name\n",projectName);
    }
    chdir("..");
}
/*
int writeInformationfile(char projectName[],char* addressFolder,void* vertexstruct)
{
    char input[64] = {0};
    char inputLine[128] = {0};
    char projectFileName[128] = {0};

    File * databaseFile = NULL;

    sscanf(projectName,"%s"projectFileName);
    strcat(projectFileName,"-database.dat");
    databaseFile = fopen(fileName,"r+");
    if(existProjectFileCheck(projectFileName,addressFolder) == 1)
    {
        writeVertexInFile()
    }

}
*/