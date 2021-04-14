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

#define WHITE 0
#define GRAY 1
#define BLACK 2

char *colorName[] = {"WHITE", "GRAY", "BLACK"};

typedef struct _adjNode
{
    void *pNode;
    struct _adjNode *pNext;
} LINK_T;

typedef struct _node
{
    char *name;
    char *description;
    int color;
    struct _node *pNext;
    LINK_T adjListHead;
    LINK_T adjListTail;
} NODE_T;

NODE_T *nodeListHead = NULL;
NODE_T *nodeListTail = NULL;

/******************************/
/** Private function section **/
/******************************/

NODE_T *findNodeByName(char *key)
{
    NODE_T *pFound = NULL;
    NODE_T *pCurrent = NULL;
    pCurrent = nodeListHead;

    while (pCurrent != NULL)
    {
        if (strcmp(pCurrent->name, key) == 0)
        {
            pFound = pCurrent;
            break;
        }
        pCurrent = pCurrent->pNext;
    }
    return pFound;
}

void printNodeData(char *key)
{
    NODE_T *pFound = NULL;
    pFound = findNodeByName(key);
    if (pFound != NULL)
    {
        printf("\tKey = %s\n\tDescription = %s\n", pFound->name, pFound->description);
    }
    else
    {
        printf("\tSorry - not found node according to provided key\n");
    }
}

/*****************************/
/** Public function section **/
/*****************************/

int addNode(char *key, char *description)
{
    int status = 1;
    char *pKey = NULL;
    char *pDescription;
    NODE_T *pFound = NULL;
    NODE_T *pNewNode = NULL;
    NODE_T *pPrev = NULL;
    pFound = findNodeByName(key);
    if (pFound != NULL)
    {
        status = -1;
    }
    else
    {
        pNewNode = calloc(1, sizeof(NODE_T));
        if (pNewNode == NULL)
        {
            status = 0;
        }
        else
        {
            pKey = strdup(key);
            pDescription = strdup(description);
            pNewNode->name = pKey;
            pNewNode->description = pDescription;
            if (nodeListHead == NULL)
            {
                nodeListHead = nodeListTail = pNewNode;
            }
            else
            {
                nodeListTail->pNext = pNewNode;
                nodeListTail = pNewNode;
            }
        }
    }

    return status;
}

int modifyNode(char *key, char *keyNew, char *newdescription)
{
}

int deleteNode(char *key)
{
}

void *findNode(char *key)
{
}

int addLink(char *fromKey, char *toKey)
{
}

int deleteLink(char *fromKey, char *toKey)
{
}

void printBreadthFirst(char *startKey)
{
}

void printDepthFirst(char *startKey)
{
}

/*  Debuging main */
int main()
{
    int status;
    char terminalInput[128];
    char key[128];
    char description[128];
    while (1)
    {
        printf("Enter node key: ");
        fgets(terminalInput, sizeof(terminalInput), stdin);
        sscanf(terminalInput, "%s", key);
        if (strcmp(key, "-1") == 0)
        {
            break;
        }
        printf("Enter node description: ");
        fgets(terminalInput, sizeof(terminalInput), stdin);
        sscanf(terminalInput, "%s", description);
        status = addNode(key, description);
        if (status == 1)
        {
            printf("\tAdd node succesfully\n");
        }
        else if (status == 0)
        {
            printf("\tError - While allocate memory for new node\n");
        }
        else
        {
            printf("\tError - Node already exist in the network\n");
        }
    }

    while (1)
    {
        printf("Enter node key for information");
        fgets(terminalInput, sizeof(terminalInput), stdin);
        sscanf(terminalInput, "%s", key);
        if (strcmp(key, "-1") == 0)
        {
            break;
        }
        printNodeData(key);
    }
}
//aaa