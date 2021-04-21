/*############################################
 *  fileHandler.c
 *  hello world 
 * 
 *  -directory read
 *  -savefile
 *##############################################
 */
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

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
void findProjectFileDatabase(char *addressFolder);
int addNewProjectFile(char projectName[],char *addressFolder);
int existProjectFileCheck(char projectName[],char* addressFolder);
int renameProjectFile(char* addressFolder);
int deleteProjectFile(char* addressFolder);
int readInformationfile(char projectName[],char* addressFolder);

int main()
{
    char input[64] = {0};
    char userProjectName[64] = {0};
    int numberInput = 0;
    int status = 0;

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
                findProjectFileDatabase(addressDatabaseDirectory);
                printf("------------------------------\n");
                break;
            case 2:
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                addNewProjectFile(userProjectName,addressDatabaseDirectory);
                break;
            
            case 3:
                printf("File in folder[].......\n");
                findProjectFileDatabase(addressDatabaseDirectory);
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
                findProjectFileDatabase(addressDatabaseDirectory);
                deleteProjectFile(addressDatabaseDirectory);
                break;
            case 5:
                findProjectFileDatabase(addressDatabaseDirectory);
                printf("Enter project name:");
                fgets(input,sizeof(input),stdin);
                sscanf(input,"%s",userProjectName);
                readInformationfile(userProjectName,addressDatabaseDirectory);
                break;
            default:
                printf("all of above\n");
            break;
        }
        printf("\n\n");
        //printf("\n\nDone.\n");
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
/*#########################################################*/
void findProjectFileDatabase(char *addressFolder)
{
    DIR *directory;
    struct dirent *projectDatabase;
    char projectName[128];
    int nameLength = 0;

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
            memset(projectName,0,sizeof(projectName));
            sscanf(projectDatabase->d_name,"%s",projectName);
            nameLength = strlen(projectName);
            //printf("name lengt = %d\n",nameLength);
            for(int i = nameLength-4; i<nameLength; i++)
            {
                projectName[i] = 0;
            }
            printf("\t-%s\n",projectName);
        }
    }
    chdir("..");
    closedir(directory);
}
/*#########################################################*/
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
/*#########################################################*/
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
/*#########################################################*/
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
/*#########################################################*/
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

int readInformationfile(char projectName[],char* addressFolder)
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

    FILE * databaseFile = NULL;

    if(existProjectFileCheck(projectName,addressFolder) == 1)
    {
        chdir(addressDatabaseDirectory);
        sscanf(projectName,"%s",projectFileName);
        strcat(projectFileName,"-database.dat");
        databaseFile = fopen(projectFileName,"r+");
        while(fgets(inputLine,sizeof(inputLine),databaseFile) != NULL)
        {
            if(sscanf(inputLine,"NAME:%[^;];INFORMATION:%[^;];WEIGHT:%[^;];"
                    ,taskName,information,charWeight) == 3)
            {
                printf("Task name:%s\n",taskName);
                printf("Task information:%s\n",information);
                sscanf(charWeight,"%d",&weight);
                printf("Task weight:%d\n",weight);
            }
            if(sscanf(inputLine,"FROM:%[^;];TO:%[^;];",keyEdgeOne,keyEdgeTwo) == 2)
            {
                printf("add edge from %s -> %s\n",keyEdgeOne,keyEdgeTwo);
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