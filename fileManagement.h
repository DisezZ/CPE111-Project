/*############################################
 *  fileManagement.c
 *  hello world 
 * 
 *  -directory read
 *  -savefile
 *############################################
 */
#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#define addressDatabaseDirectory "../fileReadtest/project_list_database"

/*==========================================================================
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
*===========================================================================
*/
void findProjectFileDatabase(char *addressFolder);

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

/*=================================================================
* This function will rename the exist project name 
* This function will ask the exist project name that want to rename 
* and ask
*	modify by
*		NAME:Pattaraphum chuamuangphan 
*		ID:63070503437
*
*   Arguments
*       addressFolder   -   the address of the database folder 
* This functino will return 1 if fond the exist project name that user enter in project list
* and return 0 if not found the exist project name.
*==================================================================
*/
int renameProjectFile(char *addressFolder);

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

int deleteProjectFile(char *addressFolder);

int readInformationFile(char projectName[], char *addressFolder);

#endif