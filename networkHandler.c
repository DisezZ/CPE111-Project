/*
 *  networkHandler.c
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abstractNetwork.h"
#include "abstractQueue.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

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

VERTEX_T *endVertex = NULL;
VERTEX_T *startVertex = NULL;
VERTEX_T *vertexListHead = NULL;
VERTEX_T *vertexListTail = NULL;
VERTEX_T **topSortVertices = NULL;
int totalVertex = 0;
int traverseIndex;

/******************************/
/** Private function section **/
/******************************/

int vertexCompareMax(void *data1, void *data2)
{
    VERTEX_T *pVertex1 = (VERTEX_T *)data1;
    VERTEX_T *pVertex2 = (VERTEX_T *)data1;
    int status = 0;

    if (pVertex1->dayWork < pVertex2->dayWork)
    {
        return -1;
    }
    else if (pVertex1->dayWork > pVertex2->dayWork)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

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

void deleteEdgesOfVertex(VERTEX_T *pTargetVertex)
{
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
            }
        }
        else
        {
            pCurrentEdge = pCurrentVertex->adjListHead;
            while (pCurrentEdge != NULL)
            {
                if (pCurrentEdge->pVertex == pTargetVertex)
                {
                    if (pCurrentEdge == pCurrentVertex->adjListHead)
                    {
                        pCurrentVertex->adjListHead = pCurrentEdge->pNext;
                        if (pCurrentVertex->adjListHead == NULL)
                        {
                            pCurrentVertex->adjListTail = NULL;
                        }
                        free(pCurrentEdge);
                    }
                    else if (pCurrentEdge == pCurrentVertex->adjListTail)
                    {
                        pCurrentVertex->adjListTail = pPrevEdge;
                        pPrevEdge->pNext = NULL;
                        free(pCurrentEdge);
                    }
                    else
                    {
                        pPrevEdge->pNext = pCurrentEdge->pNext;
                        free(pCurrentEdge);
                    }
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

void traverseBreadthFirst(VERTEX_T *pVertex, void (*function)(VERTEX_T *))
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

void indexAssign(VERTEX_T *pCurrentVertex)
{
    topSortVertices[traverseIndex] = pCurrentVertex;
    --traverseIndex;
}

int sortTopoligcal()
{
    int status = 1;
    topSortVertices = calloc(totalVertex, sizeof(VERTEX_T *));
    if (topSortVertices == NULL)
    {
        status = 0;
    }
    else
    {
        traverseIndex = totalVertex - 1;
        colorAllVertex(WHITE);
        traverseDepthFirst(startVertex, &indexAssign);
    }
    vertexListHead = topSortVertices[1];
    vertexListTail = topSortVertices[totalVertex - 1];
    for (int i = 1; i < totalVertex; i++)
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
    free(topSortVertices);
    return status;
}

void LongestPath()
{
    VERTEX_T *pCurrentVertex = NULL;
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;
    int weight = 0;

    sortTopoligcal();
    resetAllVertex();
    /*for (int i = 0; i < totalVertex; i++)
    {
        pCurrentVertex = topSortVertices[i];
        printf("%s ", pCurrentVertex->name);
    }*/
    pCurrentVertex = vertexListHead;
    while (pCurrentVertex != NULL)
    {
        //printf("%s\n", pCurrentVertex->name);
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
    printf("Min total weight of shortest path: %d\n", endVertex->totalDay);
    printf("Path:\n");
    pCurrentVertex = endVertex;
    while (pCurrentVertex != NULL)
    {
        printf("==> '%s' ", pCurrentVertex->name);
        pCurrentVertex = pCurrentVertex->pParentVertex;
    }
    printf("\n");
}

/*****************************/
/** Public function section **/
/*****************************/

void initNetwork()
{
    EDGE_T *pTemptEdge = NULL;
    char endName[] = "end";

    //endName = calloc(4, sizeof(char));
    startVertex = calloc(1, sizeof(VERTEX_T));
    endVertex = calloc(1, sizeof(VERTEX_T));

    startVertex->name = "start";
    startVertex->description = "start vertex as dependent of all vertex";
    endVertex->name = "end";
    endVertex->description = "start vertex as depend on all vertex";

    pTemptEdge = calloc(1, sizeof(EDGE_T));
    pTemptEdge->pVertex = endVertex;
    startVertex->adjListHead = startVertex->adjListTail = pTemptEdge;
    totalVertex = 2;
}

int addVertex(char *key, char *description, int weight, char *autoSave)
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
            pKey = strdup(key);
            pDescription = strdup(description);
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
                if (strcasecmp(autoSave, "ON") == 0)
                {
                    // call file handler
                }
            }
        }
    }

    return status;
}

int modifyVertex(char *key, char *option, void *newInfo)
{
    int status = 1;
    VERTEX_T *pFound = NULL;
    VERTEX_T *pPrev = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pFound = findVertexByKey(key, &pPrev);
    if (pFound == NULL)
    {
        status = -2;
    }
    else
    {
        if (strcasecmp(option, "key") == 0)
        {
            strcpy(pFound->name, newInfo);
        }
        else if (strcasecmp(option, "description") == 0)
        {
            strcpy(pFound->description, newInfo);
        }
        else if (strcasecmp(option, "weight") == 0)
        {
            pFound->dayWork = (__intptr_t)newInfo;
            pCurrentEdge = pFound->adjListHead;
            while (pCurrentEdge != NULL)
            {
                pCurrentEdge->weight = (__intptr_t)newInfo;
                pCurrentEdge = pCurrentEdge->pNext;
            }
        }
        else
        {
            status = -3;
        }
    }

    return status;
}

int deleteVertex(char *key)
{
    int status = 1;
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
        }
        else
        {
            pPrev->pNext = pFound->pNext;
        }
        free(pFound);
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
            resultList[i] = strdup(pCurrentVertex->name);
            //printf("%s\n", resultList[i]);
            ++i;
        }
        pCurrentVertex = pCurrentVertex->pNext;
    }
    return resultList;
}

int addEdge(char *fromKey, char *toKey, char *autoSave)
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
        //printf("before\n");
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

                    if (strcasecmp(autoSave, "ON") == 0)
                    {
                        // call file handler
                    }
                }
            }
        }
    }

    return status;
}

int modifyEdge(char *fromKey, char *oldToKey, char *newToKey)
{
    int status = 0;
    VERTEX_T *pFound1 = NULL;
    VERTEX_T *pFound2 = NULL;
    VERTEX_T *pFound3 = NULL;
    VERTEX_T *pAdjVertex = NULL;
    EDGE_T *pCurrentEdge = NULL;

    pFound1 = findVertex(fromKey);
    pFound2 = findVertex(oldToKey);
    pFound3 = findVertex(newToKey);
    if (pFound1 != NULL || pFound2 != NULL || pFound3 != NULL)
    {
        status = -1;
    }
    else if (pFound1 == pFound2 || pFound1 == pFound3 || pFound2 == pFound3)
    {
        status = -2;
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

/*  Debuging main */

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
                returnStatus = addVertex(key, description, weight, "OFF");
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
            memset(terminalInput, 0, sizeof(terminalInput));
            printf("Enter from key value for vertex: ");
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", fromKey);
            printf("Enter to key for vertex: ");
            memset(terminalInput, 0, sizeof(terminalInput));
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", toKey);
            if (strlen(key) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                pCurrentVertex = findVertex(key);
                if (pCurrentVertex != NULL)
                {
                    traverseBreadthFirst(pCurrentVertex, &printBreadthFirst);
                }
                else
                {
                    printf(">>> Vertex with key |%s| didn't exists in graph\n", key);
                }
            }
            fgets(terminalInput, sizeof(terminalInput), stdin);
            terminalInput[strlen(terminalInput) - 1] = '\0';
            sscanf(terminalInput, "%[^\n]", toKey);
            if (strlen(fromKey) == 0 || strlen(toKey) == 0)
            {
                printf(">>> Input error - key or data empty!\n");
            }
            else
            {
                returnStatus = addEdge(fromKey, toKey, "OFF");
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
                pCurrentVertex = findVertex(key);
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
            pCurrentVertex = findVertex(key);
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
