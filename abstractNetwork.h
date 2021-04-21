/*
 *  abstractNetwork.h
 * 
 * 
 * 
 * 
 * */

#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

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

/*  This function will initialize the graph by create end and starting vertex
 *  Return :
 *      1 - if initialize succes
 *      0 - if there is memory allocation error
 * */
int initNetwork();

/*  This function will add a vertex into network from given key, 
 *  description and weight of the vertex.
 *  Parameter:
 *      key         - task name as key of the vertex
 *      description - information about the vertex
 *      weight      - working day of task as outdegree weight of vertex
 *  Return:
 *      1   - if added vertex to netowrk succesfully
 *      0   - if memory allocation failed
 *      -1  - if vertex with given key already exist
 * */
int addVertex(char *key, char *description, int weight);

/*  This function will modify the vertex information and replace with new given info 
 *  Parameter:
 *      key             - task name that want to change information
 *      newKey          - new task name for replacing
 *      newDescription  - new task information for replacing
 *      newWeight       - new task working day for replacing
 *  Return:
 *      1   - if modify vertex succesfully
 *      -1  - if can't find the vertex
 * */
int modifyVertex(char *key, char *newKey, char *newDescription, int newWeight);

/*  This function will delete a vertex from network with given key.
 *  Parameter:
 *      key - task name as key of the vertex to delete
 *  Return:
 *      1   - if deleted vertex successfully
 *      -1  - if vertex with given key not exist in graph
 * */
int deleteVertex(char *key);

/*  This function will find the exact task with given key
 *  Parameter:
 *      key - task name as key of a vertex to find
 *  Return:
 *      NULL        - if vertex with given key not exist in graph
 *      VERTEX_T*   - if there exist a vertex with exact given key
 * */
void *findVertex(char *key);

/*  This function will find the vertex that have substring or whole of a given key
 *  Parameter:
 *      key     - task name as key of a vertex to search
 *      *total  - interger to count total number of vertex that contain
 *                  substring with given key to search
 *  Return:
 *      NULL    - if there is no vertex that containg key as substring
 *      char**  - if there exist a vertex with given key as substring
 * */
char **searchVertex(char *key, int *status);

/*  This function will add a vertex into network from given key, 
 *  description and weight of the vertex.
 *  Parameter:
 *      fromKey     - key of vertex that will be outdegree of added edge
 *      toKey       - key of vertex that will be indegree of added edge
 *  Return:
 *      1   - if added edge to netowrk succesfully
 *      0   - if memory allocation failed
 *      -1  - if any vertex with given key not exist
 *      -2  - if edge between two given key already exist
 *      -3  - if create a loop from adding edge
 * */
int addEdge(char *fromKey, char *toKey);

int modifyEdge(char *fromKey, char *oldToKey, char *newToKey);

/*  This function will add a vertex into network from given key, 
 *  description and weight of the vertex.
 *  Parameter:
 *      fromKey     - key of vertex that will be outdegree of added edge
 *      toKey       - key of vertex that will be indegree of added edge
 *  Return:
 *      1   - if deleted edge from netowrk succesfully
 *      0   - if memory allocation failed
 *      -1  - if any vertex with given key not exist
 *      -2  - if edge not between two given key exist
 * */
int deleteEdge(char *fromKey, char *toKey);

#endif // !ABSTRACTNETWORK_H