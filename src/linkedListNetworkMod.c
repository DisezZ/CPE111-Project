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
 *  Made the following changes:
 *  - Modify findVertexByKey function.
 *  - Delete EdgeExists function.
 *  - Modify freeAdjacencyList and removeReferences into deleteEdgesOfVertex function
 *  - Delete countAdjacent function.
 *  - Modify colorAll into colorAllVertex.
 *  - Delete initAll function.
 *  - 
 *  - 
 *  - 
 *  - 
 *  - 
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
}

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

void printBreadthFirst(VERTEX_T *pCurrentVertex)
{
    printf(">>> Key: %s\n", pCurrentVertex->name);
    printf(">>> Info: %s\n", pCurrentVertex->description);
    printf(">>> Weight: %d\n", pCurrentVertex->dayWork);
    printf("\n");
}

void printDepthFirst(char *startKey)
{
}

void printfAdjVertex(VERTEX_T *pCurrentVertex)
{
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pCurrentEdge = pCurrentVertex->adjListHead;
    printf("Adjacent:\n");
    while (pCurrentEdge != NULL)
    {
        pAdjVertex = pCurrentEdge->pVertex;
        printf(">>> %s\n", pAdjVertex->name);
        pCurrentEdge = pCurrentEdge->pNext;
    }
}

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
    }
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
    return status;
}

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
    /*for (int i = 0; i < totalVertex; i++)
    {
        pCurrentVertex = topSortVertices[i];
        printf("%s ", pCurrentVertex->name);
    }
    printf("\n");*/
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
    /*printf("Max total weight of longest path: %d\n", endVertex->totalDay);
    printf("Path:\n");
    pCurrentVertex = endVertex;
    while (pCurrentVertex != NULL)
    {
        printf("==> '%s' ", pCurrentVertex->name);
        pCurrentVertex = pCurrentVertex->pParentVertex;
    }
    printf("\n");*/
    free(topSortVertices);
}

/*****************************/
/** Public function section **/
/*****************************/

VERTEX_T *getVertexListHead()
{
    return vertexListHead;
}

VERTEX_T *getStartVertex()
{
    return startVertex;
}

int getTotalVertex()
{
    return totalVertex - 2;
}

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

int modifyVertex(char *key, char *newKey, char *newDescription, int newWeight)
{
    int status = 1;
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
        printf(":%s:%s:%d", newKey, newDescription, newWeight);
        if (strlen(newKey) != 0)
        {
            //pFound->name = strdup(newKey);
            free(pFound->name);
            pFound->name = calloc(strlen(newKey), sizeof(char));
            strcpy(pFound->name, newKey);
        }
        if (strlen(newDescription) != 0)
        {
            //pFound->description = strdup(newDescription);
            free(pFound->description);
            pFound->description = calloc(strlen(newDescription), sizeof(char));
            strcpy(pFound->description, newDescription);
        }
        if (newWeight > 0)
        {
            pFound->dayWork = newWeight;
        }
    }

    return status;
}

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
            printf("head1\n");
            vertexListHead = pFound->pNext;
            if (vertexListHead == NULL)
            {
                vertexListTail = NULL;
            }
            printf("head1\n");
        }
        else if (pFound == vertexListTail)
        {
            printf("tail1\n");
            if (pPrev)
                printf("Found:\n");
            vertexListTail = pPrev;
            vertexListTail->pNext = NULL;
            printf("tail2\n");
        }
        else
        {
            printf("body1\n");
            if (pPrev)
                printf("Found:\n");
            else
                printf("Notfound:\n");
            pPrev->pNext = pFound->pNext;
            printf("body2\n");
        }
        deleteEdgeByStruct(startVertex, startVertex);
        /*pCurrentEdge = startVertex->adjListHead;
        while (pCurrentEdge)
        {
            pAdjVertex = pCurrentEdge->pVertex;
            if (pFound == pAdjVertex)
            {
                if (pCurrentEdge == startVertex->adjListHead)
                {
                    printf("-head1\n");
                    startVertex->adjListHead = pCurrentEdge->pNext;
                    if (startVertex->adjListHead == NULL)
                    {
                        startVertex->adjListTail = NULL;
                    }
                    printf("-head1\n");
                }
                else if (pCurrentEdge == startVertex->adjListTail)
                {
                    printf("-tail1\n");
                    if (pPrev)
                        printf("-Found:\n");
                    startVertex->adjListTail = pPrevEdge;
                    startVertex->adjListTail->pNext = NULL;
                    printf("-tail2\n");
                }
                else
                {
                    printf("-body1\n");
                    if (pPrev)
                        printf("-Found:\n");
                    else
                        printf("-Notfound:\n");
                    pPrevEdge->pNext = pCurrentEdge->pNext;
                    printf("-body2\n");
                }
                free(pCurrentEdge);
                pCurrentEdge = NULL;
                break;
            }
            pPrevEdge = pCurrentEdge;
            pCurrentEdge = pCurrentEdge->pNext;
        }*/
        free(pFound);
        --totalVertex;
        pFound = NULL;
    }
    return status;
}

void *findVertex(char *key)
{
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;

    pFound = findVertexByKey(key, &pPrev);
    return pFound;
}

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

    vertexListHead = NULL;
    vertexListTail = NULL;
}

/*  Debuging main */
/*
int getMenuOption()
{
    char input[32];
    int option = -1;
    while (option < 0)
    {
        printf("\nNetwork Tester options:\n");
        printf("  1 - Insert a vertex into the graph\n");
        printf("  2 - Insert an edge into the graph\n");
        printf("  3 - Remove a vertex from the graph\n");
        printf("  4 - Remove an edge from the graph\n");
        printf("  5 - Search for vertex in graph\n");
        printf("  6 - Modify a edge in graph\n");
        printf("  7 - Print breadth first or depth first\n");
        printf("  8 - Print maximum path (Dijkstra's algorithm)\n");
        printf("  9 - Print vertex information\n");
        printf(" 10 - Exit\n");
        printf("Which option? ");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &option);
        if ((option > 9) || (option < 1))
        {
            printf("Invalid selection - choose 1 to 9\n");
            option = -1;
        }
    }
    printf("\n");
    return option;
}
int main(int argc, char *argv[])
{
    VERTEX_T *pCurrentVertex = NULL;
    VERTEX_T *pAdjVertex = NULL;
    VERTEX_T *pPrevVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    char terminalInput[128];
    char fromKey[128];
    char toKey[128];
    char key[128];
    char description[512];
    int weight;
    int choice;
    int returnStatus;
    char *string = NULL;
    char **resultList = NULL;

    initNetwork();
    while (1)
    {
        memset(key, 0, sizeof(key));
        memset(fromKey, 0, sizeof(fromKey));
        memset(toKey, 0, sizeof(toKey));
        memset(description, 0, sizeof(description));
        choice = getMenuOption();
        if (choice == 1)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", key);
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter description for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", description);
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter day work for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%d", &weight);
            if (strlen(key) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                returnStatus = addVertex(key, description, weight);
                if (returnStatus == 1)
                {
                    printf(">>> Vertex |%s| added\n", key);
                }
                else if (returnStatus == -1)
                {
                    printf(">>> Vertex with key |%s| already exists in graph\n", key);
                }
                else if (returnStatus == -2)
                {
                    printf(">>> Vertex with key |%s| has error assign start and end edge\n", key);
                }
                else
                {
                    printf(">>> Memory allocation error or graph full!\n");
                }
            }
        }
        else if (choice == 2)
        {
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", toKey);
            if (strlen(fromKey) == 0 || strlen(toKey) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                returnStatus = addEdge(fromKey, toKey);
                if (returnStatus == 1)
                {
                    printf(">>> Edge |%s| -> |%s| added\n", fromKey, toKey);
                }
                else if (returnStatus == -1)
                {
                    printf(">>> Vertex key |%s| or key |%s| not exists in graph\n", fromKey, toKey);
                }
                else if (returnStatus == -2)
                {
                    printf(">>> Edge from key |%s| to key |%s| already exists in graph\n", fromKey, toKey);
                }
                else if (returnStatus == -3)
                {
                    printf(">>> Error - Adding edge from |%s| to |%s| will create loop\n", fromKey, toKey);
                }
                else
                {
                    printf(">>> Memory allocation error or graph full!\n");
                }
            }
        }
        else if (choice == 3)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter key value for vertex A: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", key);
            if (strlen(key) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                returnStatus = deleteVertex(key);
                if (returnStatus == 1)
                {
                    printf(">>> Vertex |%s| deleted\n", key);
                }
                else
                {
                    printf(">>> Vertex with key |%s| didn't exists in graph\n", key);
                }
            }
        }
        else if (choice == 4)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter from key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", fromKey);
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter to key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", toKey);
            if (strlen(fromKey) == 0 || strlen(toKey) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                returnStatus = deleteEdge(fromKey, toKey);
                if (returnStatus == 1)
                {
                    printf(">>> Edge |%s| -> |%s| deleted\n", fromKey, toKey);
                }
                else if (returnStatus == -1)
                {
                    printf(">>> Vertex key |%s| or key |%s| not exists in graph\n", fromKey, toKey);
                }
                else if (returnStatus == -2)
                {
                    printf(">>> Edge from key |%s| to key |%s| didn't exists in graph\n", fromKey, toKey);
                }
            }
        }
        else if (choice == 5)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", key);
            if (strlen(key) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                resultList = searchVertex(key, &returnStatus);
                for (int i = 0; i < 2; i++)
                {
                    printf("%s\n", resultList[i]);
                }
            }
        }
        else if (choice == 6)
        {
        }
        else if (choice == 7)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", key);
            if (strlen(key) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                pCurrentVertex = findVertexByKey(key, &pPrevVertex);
                if (pCurrentVertex != NULL)
                {
                    traverseBreadthFirst(pCurrentVertex, &printBreadthFirst);
                }
                else
                {
                    printf(">>> Vertex with key |%s| didn't exists in graph\n", key);
                }
            }
        }
        else if (choice == 8)
        {
            LongestPath();
        }
        else if (choice == 9)
        {
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", key);
            pCurrentVertex = findVertexByKey(key, &pPrevVertex);
            if (pCurrentVertex == NULL && strcasecmp(key, "start") != 0 && strcasecmp(key, "end") != 0)
            {
                printf(">>> Vertex with key |%s| didn't exists in graph\n", key);
            }
            else
            {
                if (strcasecmp(key, "start") == 0)
                {
                    pCurrentVertex = startVertex;
                }
                else if (strcasecmp(key, "end") == 0)
                {
                    pCurrentVertex = endVertex;
                }

                pCurrentEdge = pCurrentVertex->adjListHead;
                printf(">>> Vertex with key |%s| exists in graph\n", key);
                printf(">>> Key: %s\n", pCurrentVertex->name);
                printf(">>> Info: %s\n", pCurrentVertex->description);
                printf(">>> Weight: %d\n", pCurrentVertex->dayWork);
                printf(">>> Parent: %s\n", pCurrentVertex->pParentVertex->name);
                printf(">>> Total: %d\n", pCurrentVertex->totalDay);
                printf(">>> Dependent of: \n");
                printf(">>> ");
                while (pCurrentEdge != NULL)
                {
                    pAdjVertex = (VERTEX_T *)pCurrentEdge->pVertex;
                    printf("%s ", pAdjVertex->name);
                    pCurrentEdge = pCurrentEdge->pNext;
                }
            }
        }
        else if (choice == 10)
        {
            break;
        }
    }
}
*/