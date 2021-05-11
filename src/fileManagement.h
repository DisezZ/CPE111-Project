/*############################################
 *  fileManagement.h
 *  
 *  Create by
 *      Name:Pattaraphum chuamuangphan
 *      ID:63070503437
 *############################################
 */
#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#define addressDatabaseDirectory "../fileReadtest/project_list_database"

/*==========================================================================
* This function will read all porject file in the folder database directory 
* and then return the project name list.
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
*this function will return the project name list in folder.                     
*===========================================================================
*/
char **findProjectFileDatabase(char *addressFolder, int *totalFile);

/*=================================================================
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
* and return 0 if can there is file already exist
*==================================================================
*/
int addNewProjectFile(char projectName[], char *addressFolder);

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
int renameProjectFile(char oldFileName[], char newFileName[], char *addressFolder);

/*=================================================================
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
*==================================================================
*/
int existProjectFileCheck(char projectName[], char *addressFolder);

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
int deleteProjectFile(char projectName[], char *addressFolder);

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
int readInformationFile(char projectName[], char *addressFolder);

int writeInformationFile(char projectName[], char *addressFolder);

char *findProjectDatabaseDirectory();

#endif