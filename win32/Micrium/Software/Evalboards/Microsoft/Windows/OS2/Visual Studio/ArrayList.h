#ifndef __ARRAY_LIST_H
#define __ARRAY_LIST_H

typedef struct node_t {
	void* data;
	struct node_t* next;
	struct node_t* pervius;
}Node;

Node* createNode(void* data);
Node* freeNode(Node* node);


typedef struct {

	int elementInList;
	Node* startOfArray;
	Node* endOfArray;

} ArrayList;


ArrayList* createArrayList();
int doOperationOnArrayListIfPosible(ArrayList* list, int(*function)(void*));
void doOperationOnArrayList(ArrayList* list, void(*function)(void*));
void* popNextItemFromStartArrayList(ArrayList* list); // delete the item
void* popNextItemFromEndArrayList(ArrayList* list); // delete the item
void* getNextItemFromStartArrayList(ArrayList* list); // not Deleting the item
void* getNextItemFromEndArrayList(ArrayList* list); // not Delete the item
void pushItemToStartArrayList(ArrayList* list , void* data);
void pushItemToEndArrayList(ArrayList* list , void* data);
void freeArrayList(ArrayList* list);


#endif