/**
 *  linkedListNetworkMod.c
 *
 *  Implements an abstractNetwork using adjacency lists (linked lists).
 *  This is a structure with two levels of list. There is a master
 *  list of vertices, linked in series. Each vertex points to a subsidiary
 *  linked list with references to all the other vertices to which it
 *  is connected.
 *
 *  Each vertex has an integer weight and a pointer to a parent vertex 
 *  which can be used for route finding and spanning tree algorithms
 *
 *  Key values are strings and are copied when vertices are inserted into
 *  the graph. Every vertex has a void* pointer to ancillary data which
 *  is simply stored. 
 *
 *  Created by Sally Goldin, 1 February 2012 for CPE 113
 *  Modified 18 March 2013 to improve naming.
 *  Modified 20 April 2018 to use JavaDoc keywords in comments
 * 
 *  Modified with permission by [Lutfee] on [Deemae].
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"
#include "abstractQueue.h"
#include "fileManagement.h"

#define WHITE 0 /* neverseen vertex state */
#define GRAY 1  /* queued but unvisited vertex state */
#define BLACK 2 /* visited vertex state */

VERTEX_T *startVertex = NULL;    /* special vertex as start point that every other vertex are dependent on */
VERTEX_T *endVertex = NULL;      /* special vertex as ending point that dependent on every other vertex */
VERTEX_T *vertexListHead = NULL; /* head of the vertex list */
VERTEX_T *vertexListTail = NULL; /* tail of the vertex list */
int totalVertex = 0;             /* total vertex in the graph including special vertex too */

/******************************/
/** Private function section **/
/******************************/

/** Finds the vertex that holds the passed key
 * (if any) and returns a pointer to that vertex.
 *
 *@param key    -  Key we are looking for
 *@param pPrev  -  used to return the predecessor if any
 *@return pointer to the vertex structure if one is found       
 */
VERTEX_T *findVertexByKey(char *key, VERTEX_T **pPrev)
{
    VERTEX_T *pFound = NULL;
    VERTEX_T *pCurrent = NULL;

    pCurrent = vertexListHead;
    while (pCurrent != NULL)
    {
        if (strcmp(pCurrent->name, key) == 0)
        {
            pFound = pCurrent;
            break;
        }
        *pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }
    return pFound;
}

/* Add edge by given vertex struct from->to
 * Parameter:
 *      pFrom       - vertex struct that will be outdegree of edge
 *      pTo         - vertex struct that will be indegree of edge
 * Return:
 *      pointer to the allocated edge, NULL if there is memory error
 * */
EDGE_T *addEdgeByStruct(VERTEX_T *pFrom, VERTEX_T *pTo)
{
    EDGE_T *pEdge = NULL;

    pEdge = calloc(sizeof(EDGE_T), 1);
    if (pEdge != NULL)
    {
        pEdge->pVertex = pTo;
        if (pFrom->adjListHead == NULL)
        {
            pFrom->adjListHead = pFrom->adjListTail = pEdge;
        }
        else
        {
            pFrom->adjListTail->pNext = pEdge;
            pFrom->adjListTail = pEdge;
        }
    }
    return pEdge;
}

/* Delete edge by given vertex struct from->to
 * Parameter:
 *      pFrom       - vertex struct that has outdegree of deleted edge
 *      pTo         - vertex struct that has indegree of deleted edge
 * */
void deleteEdgeByStruct(VERTEX_T *pFrom, VERTEX_T *pTo)
{
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    EDGE_T *pPrevEdge = NULL;

    pCurrentEdge = pFrom->adjListHead;
    while (pCurrentEdge)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        if (pTo == pAdjVertex)
        {
            if (pCurrentEdge == startVertex->adjListHead)
            {
                startVertex->adjListHead = pCurrentEdge->pNext;
                if (startVertex->adjListHead == NULL)
                {
                    startVertex->adjListTail = NULL;
                }
            }
            else if (pCurrentEdge == startVertex->adjListTail)
            {
                startVertex->adjListTail = pPrevEdge;
                startVertex->adjListTail->pNext = NULL;
            }
            else
            {
                pPrevEdge->pNext = pCurrentEdge->pNext;
            }
            free(pCurrentEdge);
            pCurrentEdge = NULL;
            break;
        }
        pPrevEdge = pCurrentEdge;
        pCurrentEdge = pCurrentEdge->pNext;
    }
}

/* Delete edge that have outdegree from target vertex
 * and indegree target vertex.
 * Parameter:
 *      pTargetVertex   - target vertex struct that will delete all edge
 * */
void deleteEdgesOfVertex(VERTEX_T *pTargetVertex)
{
    VERTEX_T *pTemptTail = NULL;
    VERTEX_T *pTemptPrev = NULL;
    VERTEX_T *pTemptCurrent = NULL;
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pCurrentVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    EDGE_T *pPrevEdge = NULL;

    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        if (pCurrentVertex == pTargetVertex)
        {
            pCurrentEdge = pCurrentVertex->adjListHead;
            while (pCurrentEdge != NULL)
            {
                pPrevEdge = pCurrentEdge;
                pCurrentEdge = pCurrentEdge->pNext;
                free(pPrevEdge);
                pPrevEdge = NULL;
            }
        }
        else
        {
            pCurrentEdge = pCurrentVertex->adjListHead;
            pPrevEdge = NULL;
            while (pCurrentEdge != NULL)
            {
                pTemptCurrent = pCurrentEdge->pVertex;
                if (pCurrentEdge->pVertex == pTargetVertex)
                {
                    pAdjVertex = pCurrentEdge->pVertex;
                    if (pCurrentEdge == pCurrentVertex->adjListHead)
                    {
                        pCurrentVertex->adjListHead = pCurrentEdge->pNext;
                        if (pCurrentVertex->adjListHead == NULL)
                        {
                            pCurrentVertex->adjListTail = NULL;
                        }
                        free(pCurrentEdge);
                        pCurrentEdge = NULL;
                    }
                    else if (pCurrentEdge == pCurrentVertex->adjListTail)
                    {
                        pCurrentVertex->adjListTail = pPrevEdge;
                        pPrevEdge->pNext = NULL;
                        free(pCurrentEdge);
                        pCurrentEdge = NULL;
                    }
                    else
                    {
                        pPrevEdge->pNext = pCurrentEdge->pNext;
                        free(pCurrentEdge);
                        pCurrentEdge = NULL;
                    }
                    break;
                }
                pPrevEdge = pCurrentEdge;
                pCurrentEdge = pCurrentEdge->pNext;
            }
        }
        pCurrentVertex = pCurrentVertex->pNext;
    }
    pCurrentEdge = startVertex->adjListHead;
    while (pCurrentEdge)
    {
        pTemptCurrent = pCurrentEdge->pVertex;
        if (pCurrentEdge->pVertex == pTargetVertex)
        {
            pAdjVertex = pCurrentEdge->pVertex;
            if (pCurrentEdge == startVertex->adjListHead)
            {
                startVertex->adjListHead = pCurrentEdge->pNext;
                if (startVertex->adjListHead == NULL)
                {
                    startVertex->adjListTail = NULL;
                }
                free(pCurrentEdge);
                pCurrentEdge = NULL;
            }
            else if (pCurrentEdge == startVertex->adjListTail)
            {
                startVertex->adjListTail = pPrevEdge;
                pPrevEdge->pNext = NULL;
                free(pCurrentEdge);
                pCurrentEdge = NULL;
            }
            else
            {
                pPrevEdge->pNext = pCurrentEdge->pNext;
                free(pCurrentEdge);
                pCurrentEdge = NULL;
            }
            break;
        }
        pPrevEdge = pCurrentEdge;
        pCurrentEdge = pCurrentEdge->pNext;
    }
}

/** Color all vertices to the passed color.
 *  @param  A color constant
 */
void colorAllVertex(int color)
{
    VERTEX_T *pCurrentVertex = vertexListHead;
    startVertex->color = color;
    endVertex->color = color;
    while (pCurrentVertex != NULL)
    {
        pCurrentVertex->color = color;
        pCurrentVertex = pCurrentVertex->pNext;
    }
}

/* Reset all vertex included special vertex to white and total work day to 0
 * Parameter:
 *      pFrom       - vertex struct that will be outdegree of edge
 *      pTo         - vertex struct that will be indegree of edge
 * Return:
 *      pointer to the allocated edge, NULL if there is memory error
 * */
void resetAllVertex()
{
    VERTEX_T *pCurrentVertex = vertexListHead;

    startVertex->pParentVertex = NULL;
    endVertex->pParentVertex = startVertex;
    startVertex->color = WHITE;
    endVertex->color = WHITE;
    startVertex->totalDay = 0;
    endVertex->totalDay = 0;
    while (pCurrentVertex != NULL)
    {
        pCurrentVertex->pParentVertex = startVertex;
        pCurrentVertex->color = WHITE;
        pCurrentVertex->totalDay = 0;
        pCurrentVertex = pCurrentVertex->pNext;
    }
}

/** Execute a breadth first traversal from a vertex,
 * calling the passed function (*vFunction) on each vertex
 * as we visit it and color it black.
 * @param pVertex  Starting vertex for traversal
 */
void traverseBreadthFirst(VERTEX_T *pVertex, void (*function)(VERTEX_T *pCurrenVertex))
{
    VERTEX_T *pCurrentVertex = NULL;
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    queueClear();
    colorAllVertex(WHITE);
    pVertex->color = GRAY;
    enqueue(pVertex);
    while (queueSize() > 0)
    {
        pCurrentVertex = (VERTEX_T *)dequeue();
        if (function != NULL)
        {
            (*function)(pCurrentVertex);
        }
        pCurrentVertex->color = BLACK;
        pCurrentEdge = pCurrentVertex->adjListHead;
        while (pCurrentEdge != NULL)
        {
            if (pCurrentEdge->pVertex != startVertex && pCurrentEdge->pVertex != endVertex)
            {
                pAdjVertex = pCurrentEdge->pVertex;
                if (pAdjVertex->color == WHITE)
                {
                    pAdjVertex->color = GRAY;
                    enqueue(pAdjVertex);
                }
            }
            pCurrentEdge = pCurrentEdge->pNext;
        }
    }
}

/** Execute a depth first traversal from a single vertex,
 * calling the passed function (*vFunction) on the lowest level
 * vertex we visit, and coloring it black.
 * @param pVertex Starting vertex for traversal
 */
void traverseDepthFirst(VERTEX_T *pCurrentVertex, void (*function)(VERTEX_T *))
{
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pCurrentVertex->color = GRAY;
    pCurrentEdge = pCurrentVertex->adjListHead;
    while (pCurrentEdge != NULL)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        if (pAdjVertex != NULL && pAdjVertex->color == WHITE)
        {
            pAdjVertex->color = GRAY;
            traverseDepthFirst(pAdjVertex, function);
        }
        pCurrentEdge = pCurrentEdge->pNext;
    }
    pCurrentVertex->color = BLACK;
    (*function)(pCurrentVertex);
}

/**  Function to print the information about a vertex
 * @param pVertex Vertex we want to print
 */
void printVertexData(char *key)
{
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;
    pFound = findVertexByKey(key, &pPrev);
    if (pFound != NULL)
    {
        printf("\tKey = %s\n\tDescription = %s\n\tdayWork = %d\n", pFound->name, pFound->description, pFound->dayWork);
    }
    else
    {
        printf("\tSorry - not found node according to provided key\n");
    }
}

/* Find edge by two given vertex struct
 * Parameter:
 *      pFrom       - vertex struct that has outdegree of edge
 *      pTo         - vertex struct that has indegree of edge
 *      pPrevEdge   - edge struct to hold previous edge of outdegree vertex
 * Return:
 *      pointer to the edge found, NULL if edge not found
 * */
EDGE_T *findEdgeByStruct(VERTEX_T *pFrom, VERTEX_T *pTo, EDGE_T **pPrevEdge)
{
    EDGE_T *pCurrentEdge = NULL;
    EDGE_T *pFound = NULL;

    pCurrentEdge = pFrom->adjListHead;
    while (pCurrentEdge != NULL)
    {
        if (pCurrentEdge->pVertex == pTo)
        {
            pFound = pCurrentEdge;
            break;
        }
        *pPrevEdge = pCurrentEdge;
        pCurrentEdge = pCurrentEdge->pNext;
    }
    return pFound;
}

/* Sorting recursive function that will sort the vertex with depth first traversal
 * and then assign the the vertex to array with sorted order
 * use to assist sortTopological function.
 * Parameter:
 *      pCurrentVertex      - vertex struct normally will be special vertex "startVertex"
 *      topSortVertices     - array that will hold vertex list in topological order
 *      traverse            - index integer to keep track of index each vertex belong when
 *                            do topological sorting.
 * */
void indexAssign(VERTEX_T *pCurrentVertex, VERTEX_T **topSortVertices, int *traverseIndex)
{
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pCurrentVertex->color = GRAY;
    pCurrentEdge = pCurrentVertex->adjListHead;
    while (pCurrentEdge != NULL)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        if (pAdjVertex != NULL && pAdjVertex->color == WHITE)
        {
            pAdjVertex->color = GRAY;
            indexAssign(pAdjVertex, topSortVertices, traverseIndex);
        }
        pCurrentEdge = pCurrentEdge->pNext;
    }
    pCurrentVertex->color = BLACK;
    topSortVertices[*traverseIndex] = pCurrentVertex;
    --(*traverseIndex);
}

/*  will sort vertex list by topological order by using recursive method
 *  which use indexAssign function to assist
 *  Parameter:
 *      topSortVertices     - array that sorted order vertex will store
 *      traverseIndex       - index to keep track of order which vertex belong when sorting
 * Return
 *      0                   - if sorted vertex array has not been allocated
 *      1                   - if succesfully sorted in topological order
 * */
int sortTopoligcal(VERTEX_T **topSortVertices, int *traverseIndex)
{
    int status = 1;

    if (topSortVertices == NULL)
    {
        status = 0;
    }
    else
    {
        *traverseIndex = totalVertex - 1;
        colorAllVertex(WHITE);
        indexAssign(startVertex, topSortVertices, traverseIndex);
        vertexListHead = topSortVertices[1];
        vertexListTail = topSortVertices[totalVertex - 1];
        for (int i = 0; i < totalVertex; i++)
        {
            if (i == totalVertex - 1)
            {
                topSortVertices[i]->pNext = NULL;
            }
            else
            {
                topSortVertices[i]->pNext = topSortVertices[i + 1];
            }
        }
    }

    return status;
}

/*****************************/
/** Public function section **/
/*****************************/

/* get pointer to the head of vertex list.
 * 
 * Return pointer to vertex list head, NULL if network isn't initialize 
 * or no members in list yet.
 * */
VERTEX_T *getVertexListHead()
{
    return vertexListHead;
}

/* get pointer to start vertex which is special vertex
 * which will point to every other vertex in network.
 *
 * Return pointer to start vertex, NULL if network isn't initialize
 * */
VERTEX_T *getStartVertex()
{
    return startVertex;
}

/* get total vertex number in the network
 * 
 * Return total vertex in network.
 * */
int getTotalVertex()
{
    return totalVertex - 2;
}

/* Calculate longest path of all vertex in network  
 * which is total day work before can working on current vertex.
 * */
void LongestPath()
{
    VERTEX_T **topSortVertices = NULL;
    VERTEX_T *pCurrentVertex = NULL;
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    int traverseIndex;
    int weight = 0;

    topSortVertices = calloc(totalVertex, sizeof(VERTEX_T *));
    sortTopoligcal(topSortVertices, &traverseIndex);
    resetAllVertex();
    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        pCurrentVertex->color = BLACK;
        pCurrentEdge = pCurrentVertex->adjListHead;
        weight = pCurrentVertex->dayWork;
        while (pCurrentEdge != NULL)
        {
            pAdjVertex = pCurrentEdge->pVertex;
            if (pAdjVertex != NULL && pAdjVertex->color == WHITE)
            {
                if (pCurrentVertex->totalDay + weight > pAdjVertex->totalDay)
                {
                    pAdjVertex->totalDay = pCurrentVertex->totalDay + weight;
                    pAdjVertex->pParentVertex = pCurrentVertex;
                }
            }
            pCurrentEdge = pCurrentEdge->pNext;
        }
        pCurrentVertex = pCurrentVertex->pNext;
    }
    free(topSortVertices);
}

/*  This function will initialize the graph by create end and starting vertex
 *  Return :
 *      1 - if initialize succes
 *      0 - if there is memory allocation error
 * */
int initNetwork()
{
    EDGE_T *pTemptEdge = NULL;
    char endName[] = "end";
    int status = 1;

    startVertex = calloc(1, sizeof(VERTEX_T));
    endVertex = calloc(1, sizeof(VERTEX_T));
    if (startVertex == NULL || endVertex == NULL)
    {
        status = 0;
    }
    startVertex->name = "start";
    startVertex->description = "start vertex as dependent of all vertex";
    endVertex->name = "end";
    endVertex->description = "start vertex as depend on all vertex";

    pTemptEdge = calloc(1, sizeof(EDGE_T));
    if (pTemptEdge == NULL)
    {
        status = 0;
    }
    pTemptEdge->pVertex = endVertex;
    startVertex->adjListHead = startVertex->adjListTail = pTemptEdge;
    totalVertex = 2;
}

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
int addVertex(char *key, char *description, int weight)
{
    int status = 1;
    int addStatus1;
    int addStatus2;
    char *pKey;
    char *pDescription;
    VERTEX_T *pFound = NULL;
    VERTEX_T *pNewVertex = NULL;
    VERTEX_T *pPrev = NULL;
    EDGE_T *pEdgeStart = NULL;
    EDGE_T *pEdgeEnd = NULL;

    pFound = findVertexByKey(key, &pPrev);
    if (pFound != NULL)
    {
        status = -1;
    }
    else
    {
        pNewVertex = calloc(1, sizeof(VERTEX_T));
        if (pNewVertex == NULL)
        {
            status = 0;
        }
        else
        {
            //pKey = strdup(key);
            //pDescription = strdup(description);
            pKey = calloc(strlen(key), sizeof(char));
            strcpy(pKey, key);
            pDescription = calloc(strlen(description), sizeof(char));
            strcpy(pDescription, description);
            pNewVertex->name = pKey;
            pNewVertex->description = pDescription;
            pNewVertex->dayWork = weight;
            if (vertexListHead == NULL)
            {
                vertexListHead = vertexListTail = pNewVertex;
            }
            else
            {
                vertexListTail->pNext = pNewVertex;
                vertexListTail = pNewVertex;
            }
            pEdgeStart = addEdgeByStruct(startVertex, pNewVertex);
            pEdgeEnd = addEdgeByStruct(pNewVertex, endVertex);
            if (pEdgeStart == NULL || pEdgeEnd == NULL)
            {
                status = 0;
            }
            else
            {
                ++totalVertex;
            }
        }
    }

    return status;
}

/* Modify vertex name base on given key
 * 
 * Argument:
 *      key     - key string of vertex
 *      newKey  - new key string that want to changed to
 * Return:
 *      1       - if change key successful
 *      0       - if memory allocation failed
 *      -1      - if vertex with given key not exist
 *      -2      - if given new key already exist
 * */
int modifyVertexName(char *key, char *newKey)
{
    int status = 0;
    char *stringTempt = NULL;
    VERTEX_T *pFound1 = NULL;
    VERTEX_T *pFound2 = NULL;
    VERTEX_T *pPrev1 = NULL;
    VERTEX_T *pPrev2 = NULL;

    pFound1 = findVertexByKey(key, &pPrev1);
    pFound2 = findVertexByKey(newKey, &pPrev2);
    if (pFound1 == NULL)
    {
        status = -1;
    }
    else if (pFound2)
    {
        status = -2;
    }
    else
    {
        stringTempt = calloc(strlen(newKey), sizeof(char));
        if (stringTempt)
        {
            free(pFound1->name);
            pFound1->name = stringTempt;
            strcpy(pFound1->name, newKey);
            status = 1;
        }
    }

    return status;
}

/* Modify vertex description base on given key
 * 
 * Argument:
 *      key             - key string of vertex
 *      newDescription  - new description string that want to changed to
 * Return:
 *      1               - if change description successful
 *      0               - if memory allocation failed
 *      -1              - if vertex with given key not exist
 * */
int modifyVertexDescription(char *key, char *newDescription)
{
    int status = 0;
    char *stringTempt = NULL;
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pFound = findVertexByKey(key, &pPrev);
    if (pFound == NULL)
    {
        status = -1;
    }
    else
    {
        stringTempt = calloc(strlen(newDescription), sizeof(char));
        if (pFound->description)
        {
            free(pFound->description);
            pFound->description = stringTempt;
            strcpy(pFound->description, newDescription);
            status = 1;
        }
    }

    return status;
}

/* Modify vertex weight base on given key
 * 
 * Argument:
 *      key             - key string of vertex
 *      newWeight  - new weight int that want to changed to
 * Return:
 *      1               - if change weight successful
 *      0               - if given weight less than 1
 *      -1              - if vertex with given key not exist
 * */
int modifyVertexWeight(char *key, int newWeight)
{
    int status = 0;
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pFound = findVertexByKey(key, &pPrev);
    if (pFound == NULL)
    {
        status = -1;
    }
    else if (newWeight > 0)
    {
        pFound->dayWork = newWeight;
        status = 1;
    }

    return status;
}

/*  This function will delete a vertex from network with given key.
 *  Parameter:
 *      key - task name as key of the vertex to delete
 *  Return:
 *      1   - if deleted vertex successfully
 *      -1  - if vertex with given key not exist in graph
 * */
int deleteVertex(char *key)
{
    int status = 1;
    EDGE_T *pCurrentEdge = NULL;
    EDGE_T *pPrevEdge = NULL;
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;
    pFound = findVertexByKey(key, &pPrev);
    if (pFound == NULL)
    {
        status = -1;
    }
    else
    {
        deleteEdgesOfVertex(pFound);
        if (pFound == vertexListHead)
        {
            vertexListHead = pFound->pNext;
            if (vertexListHead == NULL)
            {
                vertexListTail = NULL;
            }
        }
        else if (pFound == vertexListTail)
        {
            vertexListTail = pPrev;
            vertexListTail->pNext = NULL;
        }
        else
        {
            pPrev->pNext = pFound->pNext;
        }
        deleteEdgeByStruct(startVertex, startVertex);
        free(pFound->name);
        free(pFound->description);
        free(pFound);
        printf("%s\n", pFound->name);
        --totalVertex;
        pFound = NULL;
    }
    return status;
}

/*  This function will find the exact task with given key
 *  Parameter:
 *      key - task name as key of a vertex to find
 *  Return:
 *      NULL        - if vertex with given key not exist in graph
 *      VERTEX_T*   - if there exist a vertex with exact given key
 * */
void *findVertex(char *key)
{
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;

    pFound = findVertexByKey(key, &pPrev);
    return pFound;
}

/*  This function will find the vertex that have substring or whole of a given key
 *  Parameter:
 *      key     - task name as key of a vertex to search
 *      *total  - interger to count total number of vertex that contain
 *                  substring with given key to search
 *  Return:
 *      NULL    - if there is no vertex that containg key as substring
 *      char**  - if there exist a vertex with given key as substring
 * */
char **searchVertex(char *key, int *status)
{
    VERTEX_T *pCurrentVertex = NULL;
    char **resultList = calloc(totalVertex, sizeof(char *));
    char *result = NULL;
    int i = 0;

    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        result = strstr(pCurrentVertex->name, key);
        if (result != NULL)
        {
            //resultList[i] = strdup(pCurrentVertex->name);
            //printf("%s\n", searchResultList[i]);
            resultList[i] = calloc(strlen(pCurrentVertex->name), sizeof(char));
            strcpy(resultList[i], pCurrentVertex->name);
            ++i;
        }
        pCurrentVertex = pCurrentVertex->pNext;
    }
    *status = i;
    return resultList;
}

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
int addEdge(char *fromKey, char *toKey)
{
    int status = 1;
    int reachable;
    VERTEX_T *pFound1 = NULL;
    VERTEX_T *pFound2 = NULL;
    VERTEX_T *pPrev1 = NULL;
    VERTEX_T *pPrev2 = NULL;
    EDGE_T *pEdge = NULL;
    EDGE_T *pEdgeList = NULL;
    EDGE_T *pPrevEdge = NULL;

    //printf("before\n");
    pFound1 = findVertexByKey(fromKey, &pPrev1);
    pFound2 = findVertexByKey(toKey, &pPrev2);
    if (pFound1 == NULL || pFound2 == NULL)
    {
        status = -1;
    }
    else
    {
        if (findEdgeByStruct(pFound1, pFound2, &pPrevEdge) != NULL)
        {
            status = -2;
        }
        else
        {
            colorAllVertex(WHITE);
            traverseBreadthFirst(pFound2, NULL);
            if (pFound1->color == BLACK)
            {
                status = -3;
            }
            else
            {
                pEdge = calloc(sizeof(EDGE_T), 1);
                if (pEdge == NULL)
                {
                    status = 0;
                }
                else
                {
                    pEdge->pVertex = pFound2;
                    pEdge->weight = pFound1->dayWork;
                    if (pFound1->adjListHead == NULL)
                    {
                        pFound1->adjListHead = pFound1->adjListTail = pEdge;
                    }
                    else
                    {
                        pFound1->adjListTail->pNext = pEdge;
                        pFound1->adjListTail = pEdge;
                    }
                }
            }
        }
    }

    return status;
}

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
int modifyEdge(char *fromKey, char *oldToKey, char *newToKey)
{
    int status = -4;
    VERTEX_T *pFound1 = NULL;
    VERTEX_T *pFound2 = NULL;
    VERTEX_T *pFound3 = NULL;
    VERTEX_T *pPrev1 = NULL;
    VERTEX_T *pPrev2 = NULL;
    VERTEX_T *pPrev3 = NULL;
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pFound1 = findVertexByKey(fromKey, &pPrev1);
    pFound2 = findVertexByKey(oldToKey, &pPrev2);
    pFound3 = findVertexByKey(newToKey, &pPrev3);
    if (pFound1 == NULL || pFound2 == NULL || pFound3 == NULL)
    {
        status = -1;
    }
    else if (pFound1 == pFound2 || pFound1 == pFound3 || pFound2 == pFound3)
    {
        status = -2;
    }
    else
    {
        traverseBreadthFirst(pFound1, NULL);
        if (pFound3->color == BLACK)
        {
            status = -3;
        }
        else
        {
            pCurrentEdge = pFound1->adjListHead;
            while (pCurrentEdge != NULL)
            {
                pAdjVertex = pCurrentEdge->pVertex;
                if (pAdjVertex == pFound2)
                {
                    status = 1;
                    pCurrentEdge->pVertex = pFound3;
                }
                pCurrentEdge = pCurrentEdge->pNext;
            }
        }
    }
    return status;
}

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
int deleteEdge(char *fromKey, char *toKey)
{
    int status = 1;
    VERTEX_T *pFound1 = NULL;
    VERTEX_T *pFound2 = NULL;
    VERTEX_T *pVertexPrev1 = NULL;
    VERTEX_T *pVertexPrev2 = NULL;
    EDGE_T *pEdge = NULL;
    EDGE_T *pPrevEdge = NULL;
    EDGE_T *pEdgeList = NULL;

    pFound1 = findVertexByKey(fromKey, &pVertexPrev1);
    pFound2 = findVertexByKey(toKey, &pVertexPrev2);
    if (pFound1 == NULL || pFound2 == NULL)
    {
        status = -1;
    }
    else
    {
        pEdge = findEdgeByStruct(pFound1, pFound2, &pPrevEdge);
        if (pEdge == NULL)
        {
            status = -2;
        }
        else
        {
            if (pEdge == pFound1->adjListHead)
            {
                pFound1->adjListHead = pEdge->pNext;
                if (pFound1->adjListHead == NULL)
                {
                    pFound1->adjListTail = NULL;
                }
            }
            else if (pEdge == pFound1->adjListTail)
            {
                //printf("here\n");
                pPrevEdge->pNext = NULL;
                pFound1->adjListTail = pPrevEdge;
            }
            else
            {

                pPrevEdge->pNext = pEdge->pNext;
            }
            free(pEdge);
        }
    }
    return status;
}

/*  Free memory of the network
 * */
void freeNetwork()
{
    VERTEX_T *pCurrentVertex = NULL;
    VERTEX_T *pPrevVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    EDGE_T *pPrevEdge = NULL;

    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        pCurrentEdge = pCurrentVertex->adjListHead;
        while (pCurrentEdge != NULL)
        {
            pPrevEdge = pCurrentEdge;
            pCurrentEdge = pCurrentEdge->pNext;
            free(pPrevEdge);
            pPrevEdge = NULL;
        }
        pCurrentVertex = pCurrentVertex->pNext;
    }
    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        pPrevVertex = pCurrentVertex;
        pCurrentVertex = pCurrentVertex->pNext;
        free(pPrevVertex);
        pPrevVertex = NULL;
    }
    pCurrentVertex = startVertex;
    pCurrentEdge = pCurrentVertex->adjListHead;
    while (pCurrentEdge != NULL)
    {
        pPrevEdge = pCurrentEdge;
        pCurrentEdge = pCurrentEdge->pNext;
        free(pPrevEdge);
        pPrevEdge = NULL;
    }
    free(startVertex);
    free(endVertex);
    vertexListHead = NULL;
    vertexListTail = NULL;
    startVertex = NULL;
    endVertex = NULL;
}