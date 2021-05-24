# CPE111-Project
CPE111 - Project Scheduling System Project

This program is Scheduling System Project.
This program is Capability of:
    - Create New Project
    - Delete Project
    -> Open Project
        - Add Task
        -> Modify Task
            -> Modify Task Information
                - Change Task Name
                - Change Task Description
                - Change Task Weight
            - Add Dependent
            - Change Dependent
            - Delete Dependent
        - Display Task Information
        - Display All Task
        - Calculate Project Schedule
        -> Modify Working Day
            - Invert Weekend Status
            - Add Day off
            - Remove Day off
            - Display Every Day off
        - Change Project Name
        - Change Project Description

-> = have sub options
\- = sub option

 Important !!! Read first.
=======================================================================
The executable file is named "projectSchedule".

All .c and .h files will be in the "src" directory.

By running "make" in the command line will create a .o files
in the "bin" directory and "projectSchedule" same directory as Makefile

when run our program, be sure to be in the same directory as Makefile.

To run just type "./projectSchedule" and hit enter. Another way
is by type "./projectSchedule projectName" which "projectName"
is a name of project that you want to open.

The database file will be in the "dataBase" directory.

There will have "test-database.dat" in that will conatain sample data 
and the picture file "Test Network.png" represent visualization 
of that "test-database.dat" file.

-----------------------------------------------------------------------