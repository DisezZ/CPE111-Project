/* 
 *  userInterface.h
 *      This section will handle almost all of user interface related
 *      and also input too    
 *      
 *      This is header file of userInterface.c functions
 *      using in KMUTT CPE111 Course Project
 *      Created by ->
 *      Aplusplus Members:
 *          63070503437 Pataraphum
 *          63070503448 Lutee           Deemae  (Lut)
 *          63070503462 Jidapa          
 *          63070503466 Purachet        
 * 
 * */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "abstractNetwork.h"

/* print string and get input from terminal into buffer string and
 * Parameter:
 *      bufferString    - buffer string for input from terminal
 *      size            - size of buffer string
 *      printString     - string to print before get get input from terminal
 * */
void getTerminalInput(char *bufferString, size_t size, char *printString);

/* Using for display error message in the same format
 * Parameter:
 *       errorMessage    - message to diplay when error occured
 * */
void displayErrorMessage(char *errorMessage);

/* Using for display success message in the same format
 * Parameter:
 *       successMessage    - message to diplay when operation success
 * */
void displaySuccessMessage(char *successMessage);

/* Using for display invalid message in the same format
 * Parameter:
 *       errorMessage    - message to diplay when invalid.
 * */
void displayInvalidMessage(char *invalidMessage);

/* Display all available project from given project's string array
 * Parameter:
 *      projectNameList     - array of all project name
 *      totalProject        - total prroject in string array
 * */
void displayAllProjectAvailable(char **projectNameList, int totalProject);

/* Display all task exist in project from given task's string array
 * Parameter:
 *      searchTaskList      - array of all task name
 *      totalTask           - total task in prject
 * */
void displayAllTaskAvailable(char **searchTaskList, int totalTask);

/* Display all task that dependent on given task edge list head
 * Parameter:
 *      pEdge   - pointer to edge list head
 * */
int displayAllDependentOn(EDGE_T *pEdge);

/* Display all available menu option in project's part
 * */
void displayProjectMenuOptions();

/* Display all available menu option in task's part
 * Parameter:
 *      projectName         - name of currently working on project
 *      projectDescription  - description of currently working on project  
 *      size                - size of description string
 * */
void displayTaskMenuOptions(char *projectName, char *projectDescription, int size);

/* Display all available menu option in modify task's part
 * Parameter:
 *      taskName    - name of task that currently working on
 * */
void displayModifyTaskMenuOptions(char *taskName);

/* Display all vailable menu option in change task info's part
 * */
void displayChangeTaskInfoOptions();

/* Display all available menu option in modify working day's part
 * Parameter:
 *      weekendStatus   - project's weekend information
 *      totalDayoff     - project's total day off
 * */
void displayModifyWorkingDaysOptions(int weekendStatus, int totalDayoff);

#endif // !USERINTERFACE_H