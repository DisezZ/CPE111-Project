
#ifndef MAIN_H
#define MAIN_H

/*
--> projectOptionFlowManager
This function is the project options for user to add, to enter, to delete
, or to exit the project with display the UI.

Parameter :
    fileOpenStatus  ;   The currently status of open project.
                        (0) == not open the project.
                        (1) == currently open the project.
*/
void projectOptionFlowManager(int *fileOpenStatus);

/*
--> taskOptionFlowManager
This function is the task options for user choice together with display the UI.
    (1) add new task
    (2) modify the task
    (3) delete the task
    (4) display specific task
    (5) display all task information
    (6) calculate project schedule
    (7) modify working day
    (8) change project name
    (9) change project sedcription
    (10) back to project option

Parameter :
    fileOpenStatus  ;   The currently status of open project.
                        (0) == not open the project.
                        (1) == currently open the project.
*/
void taskOptionFlowManager(int *fileOpenStatus);

/*
--> modifyTaskOptionFlowManger
This function is modify task options for user choice
    (1) go to changeTaskInfoOptionFlowManager ( to change task inforamtion )
    (2) add dependency
    (3) change dependency
    (4) delete dependency
    (5) delete dependency

Parameter :
    taskName    ;   the task which user want to modify
*/
void modifyTaskOptionFlowManager(char *taskName);

/*
--> modifyWorkingDayOptionFlowManager
This function is the options to modify working days by user choice
    (1) change weekend
    (2) add day off
    (3) remove day off
    (4) display every day off
    (5) exit the options
*/
void modifyWorkingDayOptionFlowManager();

/* to process when user choose to add new project
 * Parameter:
 *      projectNameIn   - name of project to open from command line
 * Return:
 *      1 if open project success
 *      0 if open project failed
 * */
void addNewProject(int *fileOpenStatus);

/* to process when user choose to enter
 * Parameter:
 *      fileOpenStatus  - 1 if currently open project, 0 if no project is open
 * */
void enterExistProject(int *fileOpenStatus);

/* to process when user choose to delete an existing project
 * Parameter:
 *      fileOpenStatus  - 1 if currently open project, 0 if no project is open
 * */
void deleteExistProject(int *fileOpenStatus);

/* to process when user's choose to rename project
 * */
void renameProject();

/*  to process when user choose to add task
 * */
void addTask();

/* get all task name and store in array of string
 * Parameter:
 *      searchResultList    - array of sting of all task name
 * Return:
 *      total vertex found in network
 * */
int getAllTask(char **searchResultList);

/* to proecess when user choose option that need searching for task first
 * Parameter:
 *      searchTask  - string to keep track what user's search/find
 *      size        - size of searchTask
 * Return:
 *      1 - if searching vertex is found and user's select that task
 *      0 - if something went wrong
 * */
int findTask(char *searchTask, size_t size);

/* to process when user choose to change task name
 * Parameter:
 *      taskName    - name of task that going to be changed
 * */
void changeTaskName(char *taskName);

/* to process when user choose to change task description
 * Parameter:
 *      taskName    - name of task that going to be changed
 * */
void changeTaskDescription(char *taskName);

/* to process when user choose to change task weight
 * Parameter:
 *      taskName    - name of task that going to be changed
 * */
void changeTaskWeight(char *taskName);

/* to process when user choose to add dependent on to vertex
 * Parameter:
 *      taskName    - name of task that will be add dependent to
 * */
void addDependency(char *taskName);

/* to process when user choose to change dependent from one to another
 * Parameter:
 *      taskName    - name of task that will be changing dependent destination
 * */
void changeDependency(char *taskName);

/* to process when user choose to delete dependency
 * Parameter:
 *      taskName    - name of task that will be changing dependent destination
 * */
void deleteDependency(char *taskName);

/* to process when user choose to display task
 * Parameter:
 *      taskName    - name of task that will be display information
 * */
void displayTask(char *taskName);

/* to process when user choose to delete task
 * Parameter:
 *      taskName    - name of task that going to be delete
 * */
void deleteTask(char *taskName);

/* to process when user's choose to display all task by display all task name
 * */
void displayAllTask();

/* calculate start and end date for each task and then display it
 * */
void calculateProjectSchedule();

/* to change project description when user's choose to change project description
 * */
void changeProjectDescription();

/*
--> free array of string.
    To free all element in string

Paramter :
    size            ;   array size of stringToFree
    stringToFree    ;   the string to be free 
*/
void freeStringArray(int size, char **stringToFree);

/* to display every day off of that project in database
 * Return:
 *      0 - if no day off in database
 *      1 - if at least 1 day off in database
 * */
int displayEveryDayOff();

/* to remove day off from database
 * */
void removeDayOff();

/* to process when user choose to add day off to database, get day off and add to database
 * */
void addDayOff();

/* get project description from main
 * */
void getProjectDescription(char *string);

/* set project description in main to given string
 * */
void setProjectDescription(char *string);

/* to handle the task information changing flow
 * Parameter:
 *      taskName    - key of task currently process
 * */
void changeTaskInfoOptionFlowManager(char *taskName);

/* to process the program if there is a command line argument at least 1 
 * for name of project
 * Parameter:
 *      projectNameIn   - name of project to open from command line
 * Return:
 *      1 if open project success
 *      0 if open project failed
 * */
int argvOneProcess(char *projectNameIn);
#endif