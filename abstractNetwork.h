/*
 *  abstractNetwork.h
 * 
 * 
 * 
 * 
 * */

int addNode(char *key, char *description);

int modifyNode(char *key, char *keyNew, char *newdescription);

int deleteNode(char *key);

void *findNode(char *key);

int addLink(char *fromKey, char *toKey);

int deleteLink(char *fromKey, char *toKey);

void printBreadthFirst(char *startKey);

void printDepthFirst(char *startKey);
