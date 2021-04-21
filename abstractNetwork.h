/*
 *  abstractNetwork.h
 * 
 * 
 * 
 * 
 * */

#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

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

/*
 *
 *
 * 
 * 
 * */
int modifyVertex(char *key, char *option, void *newInfo);

/*  This function will delete a vertex from network with given key.
 *  Parameter:
 *      key - task name as key of the vertex to delete
 *  Return:
 *      1   - if deleted vertex successfully
 *      -1  - if vertex with given key not exist in graph
 * */
int deleteVertex(char *key);

void *findVertex(char *key);

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