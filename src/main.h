
#ifndef MAIN_H
#define MAIN_H

void projectOptionFlowManager(int *fileOpenStatus);
void taskOptionFlowManager(int *fileOpenStatus);
void modifyTaskOptionFlowManager(char *taskName);
void modifyWorkingDayOptionFlowManager();
void addNewProject(int *fileOpenStatus);      /* This fuction will addd new poject file*/
void enterExistProject(int *fileOpenStatus);  /* This function will enter to the exist project*/
void deleteExistProject(int *fileOpenStatus); /* This function will delete the exist project in database*/
void renameProject();                         /* This function will rename the project*/
void addTask();                               /* This function will add task in program*/
int getAllTask(char **searchResultList);
int findTask(char *searchTask, size_t size);
void changeTaskName(char *taskName);
void changeTaskDescription(char *taskName);
void changeTaskWeight(char *taskName);
void addDependency(char *taskName);    /* This function will do the add dependency part */
void changeDependency(char *taskName); /* This function will do the change dependency from old to new task */
void deleteDependency(char *taskName); /* This function will delete dependency from task */
void displayTask(char *taskName);      /* This function will display the task information */
void deleteTask(char *taskName);       /* This function will delete the task from graph */
void dispalyAllTask();                 /* This function will display all the task information in graph */
void calculateProjectSchedule();       /* This function will calculate the project schedule start date and end date of each task */
void freeStringArray(int size, char **stringToFree);
void displayEveryDayOff();
void removeDayOff();
void addDayOff();
#endif