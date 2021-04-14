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
    int weight;
    struct _adjNode *pNext;
} LINK_T;

typedef struct _node
{
    char *name;
    char *description;
    int color;
    int dayWork;
    struct _node *pNext;
    LINK_T *adjListHead;
    LINK_T *adjListTail;
} NODE_T;

NODE_T *nodeListHead = NULL;
NODE_T *nodeListTail = NULL;

/******************************/
/** Private function section **/
/******************************/

NODE_T *findNodeByName(char *key, NODE_T **pPrev)
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
        *pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }
    return pFound;
}

LINK_T *findLinkByName(NODE_T *pFrom, NODE_T *pTo, LINK_T **pPrev)
{
    LINK_T *pFound = NULL;
    LINK_T *pCurrent = NULL;

    pCurrent = pFrom->adjListHead;
    while (pCurrent != NULL)
    {
        if (pCurrent->pNode == pTo)
        {
            pFound = pCurrent;
            break;
        }
        *pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }
}

void deleteLinks(NODE_T *pTargetNode)
{
    NODE_T *pCurrentNode = NULL;
    LINK_T *pCurrentLink = NULL;
    LINK_T *pPrevLink = NULL;

    pCurrentNode = nodeListHead;
    while (pCurrentNode != NULL)
    {
        if (pCurrentNode == pTargetNode)
        {
            pCurrentLink = pTargetNode->adjListHead;
            while (pCurrentLink != NULL)
            {
                pPrevLink = pCurrentLink;
                pCurrentLink = pCurrentLink->pNext;
                free(pPrevLink);
            }
        }
        else
        {
            pCurrentLink = pTargetNode->adjListHead;
            while (pCurrentLink != NULL)
            {
                if (pCurrentLink->pNode == pTargetNode)
                {
                    if (pCurrentLink == pCurrentNode->adjListHead)
                    {
                        pCurrentNode->adjListHead = pCurrentLink->pNext;
                        if (pCurrentNode->adjListHead == NULL)
                        {
                            pCurrentNode->adjListTail = NULL;
                        }
                        free(pCurrentLink);
                    }
                    else if (pCurrentLink == pCurrentNode->adjListTail)
                    {
                        pCurrentNode->adjListTail = pPrevLink;
                        pPrevLink->pNext = NULL;
                        free(pCurrentLink);
                    }
                    else
                    {
                        pPrevLink->pNext = pCurrentLink->pNext;
                        free(pCurrentLink);
                    }
                }
                pPrevLink = pCurrentLink;
                pCurrentLink = pCurrentLink->pNext;
            }
        }
    }
}

void printNodeData(char *key)
{
    NODE_T *pFound = NULL;
    NODE_T *pPrev = NULL;
    pFound = findNodeByName(key, &pPrev);
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
    char *pKey;
    char *pDescription;
    NODE_T *pFound = NULL;
    NODE_T *pNewNode = NULL;
    NODE_T *pPrev = NULL;

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

int modifyNode(char *key, char *keyNew, char *descriptionNew)
{
}

int deleteNode(char *key)
{
    int status = 1;
    NODE_T *pFound = NULL;
    NODE_T *pPrev = NULL;
    pFound = findNodeByName(key, &pPrev);
    if (pFound == NULL)
    {
        status = -1;
    }
    else
    {
        deleteLinks(pFound);
        if (pFound == nodeListHead)
        {
            nodeListHead = pFound->pNext;
            if (nodeListHead == NULL)
            {
                nodeListTail = NULL;
            }
        }
        else if (pFound == nodeListTail)
        {
            nodeListTail = pPrev;
        }
        else
        {
            pPrev->pNext = pFound->pNext;
        }
        free(pFound);
    }
    return status;
}

void *findNode(char *key)
{
}

int addLink(char *fromKey, char *toKey)
{
    int status = 1;
    NODE_T *pFound1 = NULL;
    NODE_T *pFound2 = NULL;
    NODE_T *pPrev1 = NULL;
    NODE_T *pPrev2 = NULL;
    LINK_T *pLink = NULL;
    LINK_T *pLinkList = NULL;

    pFound1 = findNodeByName(fromKey, &pPrev1);
    pFound2 = findNodeByName(toKey, &pPrev2);
    if (pFound1 == NULL || pFound2 == NULL)
    {
        status = -1;
    }
    else
    {
        pLink = calloc(sizeof(LINK_T), 1);
        if (pLink == NULL)
        {
            status = 0;
        }

        pLink->weight = pFound1->dayWork;
        pLink->pNode = pFound2;
        if (pFound1->adjListHead == NULL)
        {
            pFound1->adjListHead = pFound1->adjListTail = pLink;
        }
        else
        {
            pFound1->adjListTail->pNext = pLink;
            pFound1->adjListTail = pLink;
        }
    }
    return status;
}

int deleteLink(char *fromKey, char *toKey)
{
    int status = 1;
    NODE_T *pFound1 = NULL;
    NODE_T *pFound2 = NULL;
    NODE_T *pNodePrev1 = NULL;
    NODE_T *pNodePrev2 = NULL;
    LINK_T *pLink = NULL;
    LINK_T *pLinkPrev = NULL;
    LINK_T *pLinkList = NULL;

    pFound1 = findNodeByName(fromKey, &pNodePrev1);
    pFound2 = findNodeByName(toKey, &pNodePrev2);
    if (pFound1 == NULL || pFound2 == NULL)
    {
        status = -1;
    }
    else
    {
        pLink = findLinkByNode(pFound1, pFound2, &pLinkPrev);
        if (pLink == NULL)
        {
            status = -2;
        }
        else
        {
            if (pLink == pFound1->adjListHead)
            {
                pFound1->adjListHead = pLink->pNext;
                if (pFound1->adjListHead == NULL)
                {
                    pFound1->adjListTail = NULL;
                }
            }
            else if (pLink == nodeListTail)
            {
                nodeListTail = pLinkPrev;
            }
            else
            {
                pLinkPrev->pNext = pLink->pNext;
            }
            free(pLink);
        }
    }
}

void *findLink(char *fromKey, char *toKey)
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