/*
 *  abstractNetwork.h
 * 
 * 
 * 
 * 
 * */

#ifndef ABSTRACTNETWORK_H
#define ABSTRACTNETWORK_H

void initNetwork();

int addVertex(char *key, char *description, int weight, char *autoSave);

int modifyVertex(char *key, char *option, void *newInfo);

int deleteVertex(char *key);

void *findVertex(char *key);

char **searchVertex(char *key, int *status);

int addEdge(char *fromKey, char *toKey, char *autoSave);

int modifyEdge(char *fromKey, char *oldToKey, char *newToKey);

int deleteEdge(char *fromKey, char *toKey);

#endif // !ABSTRACTNETWORK_H