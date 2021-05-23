/*  
 *  abstractNetwork.h
 *
 *  Defines the necessary functions for a network, that is a 
 *  graph with weights on each edge
 *
 *  Created by Sally Goldin on 1 Feb 2012 for CPE 113
 *  Modified with permission by [Lutfee] on [Deemae]. 
 */

#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

typedef struct _adjVertex
{
    void *pVertex;            /* pointer to adjacent vertex */
    int weight;               /* weight of path to taht vertex */
    struct _adjVertex *pNext; /* pointer to next vertex in adjacent vertex list */
} EDGE_T;

typedef struct _vertex
{
    char *name;                    /* pointer to allocated task's name */
    char *description;             /* pointer to allocated task's description */
    int dayWork;                   /* task weight */
    int totalDay;                  /* total weight so far excluded current vertex */
    int color;                     /* color state of the vertex */
    struct _vertex *pNext;         /* pointer to next vertex in list */
    struct _vertex *pParentVertex; /* pointer to parent vertex using in find longest path */
    EDGE_T *adjListHead;           /* pointer to head of adjacent vertex list */
    EDGE_T *adjListTail;           /* pointer to tail of adjacent vertex list */
} VERTEX_T;

/* Calculate longest path of all vertex in network  
 * which is total day work before can working on current vertex.
 * */
void LongestPath();

/* get pointer to the head of vertex list.
 * 
 * Return pointer to vertex list head, NULL if network isn't initialize 
 * or no members in list yet.
 * */
VERTEX_T *getVertexListHead();

/* get pointer to start vertex which is special vertex
 * which will point to every other vertex in network.
 *
 * Return pointer to start vertex, NULL if network isn't initialize
 * */
VERTEX_T *getStartVertex();

/* get total vertex number in the network
 * 
 * Return total vertex in network.
 * */
int getTotalVertex();

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

/* Modify vertex name base on given key
 * 
 * Parameter:
 *      key     - key string of vertex
 *      newKey  - new key string that want to changed to
 * Return:
 *      1       - if change key successful
 *      0       - if memory allocation failed
 *      -1      - if vertex with given key not exist
 *      -2      - if given new key already exist
 * */
int modifyVertexName(char *key, char *newKey);

/* Modify vertex description base on given key
 * 
 * Parameter:
 *      key             - key string of vertex
 *      newDescription  - new description string that want to changed to
 * Return:
 *      1               - if change description successful
 *      0               - if memory allocation failed
 *      -1              - if vertex with given key not exist
 * */
int modifyVertexDescription(char *key, char *newDescription);

/* Modify vertex weight base on given key
 * 
 * Parameter:
 *      key             - key string of vertex
 *      newWeight  - new weight int that want to changed to
 * Return:
 *      1               - if change weight successful
 *      0               - if given weight less than 1
 *      -1              - if vertex with given key not exist
 * */
int modifyVertexWeight(char *key, int newWeight);

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

/*  This function will add a edge into network from given key
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

/* Change dependency destination, each given key need to be unique
 * Parameter:
 *      fromKey     - key of vertex that has outdegree to oldToKey
 *      oldToKey    - key of vertex that has indegree from fromKey
 *      newToKey    - key of vertex that fromKey will point to
 * Return:
 *      1           - if change dependency destination successfully
 *      -1          - if one of given key vertex not exist
 *      -2          - if given key not unique
 *      -3          - if edge from fromKey to newToKey already exist
 * */
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

/*  Free memory of the network
 * */
void freeNetwork();

#endif // !ABSTRACTNETWORK_H