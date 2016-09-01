

#include "ArrayList.h"
#include <stdlib.h>

Node* createNode(void* data) 
{
	Node* toReturn = (Node*)malloc(sizeof(Node));
	toReturn->data = data;
	toReturn->next = toReturn->pervius = NULL;
}

Node* freeNode(Node* node)
{
	free(node);
}

ArrayList* createArrayList()
{
	ArrayList* toReturn = (ArrayList*)malloc(sizeof(ArrayList));
	toReturn->elementInList = 0;
	toReturn->startOfArray = toReturn->endOfArray = NULL;
	return toReturn;
}

int doOperationOnArrayListIfPosible(ArrayList* list, int(*function)(void*))
{
	int i;
	int managed = 1;
	Node* tempNode = list->startOfArray;

	for (i = 0 ; i < list->elementInList; i++)
	{
		if (function(tempNode->data) == 0)
			managed = 0;
		tempNode = tempNode->next;
	}
	
	return managed;
}

void doOperationOnArrayList(ArrayList* list, void(*function)(void*)) 
{
	int i;
	Node* tempNode = list->startOfArray;

	for (i = 0; i < list->elementInList; i++)
	{
		function(tempNode->data);
		tempNode = tempNode->next;
	}

}

void* popNextItemFromStartArrayList(ArrayList* list) // delete the item
{
	if (list->elementInList > 0)
	{
		Node* firstNode = list->startOfArray;
		Node* newFirstNode = firstNode->next;
		void* data = firstNode->data;
		freeNode(firstNode);
		list->startOfArray = newFirstNode;
		list->elementInList--;
		if(list->elementInList > 0)
			newFirstNode->pervius = NULL;
		return data;
	}

	return NULL;
}

void* popNextItemFromEndArrayList(ArrayList* list) // delete the item
{
	if (list->elementInList > 0)
	{
		Node* lastNode = list->endOfArray; 
		Node* newLastNode = lastNode->pervius;
		void* data = lastNode->data;
		freeNode(lastNode);
		list->endOfArray = newLastNode;
		list->elementInList--;
		if (list->elementInList > 0)
			newLastNode->next = NULL;
		return data;
	}

	return NULL;
}

void* getNextItemFromStartArrayList(ArrayList* list) // not Deleting the item
{
	if (list->elementInList > 0)
	{
		return list->startOfArray->data;
	}
	return NULL;
}

void* getNextItemFromEndArrayList(ArrayList* list) // not Delete the item
{
	if (list->elementInList > 0)
	{
		return list->endOfArray->data;
	}
	return NULL;
}

void pushItemToStartArrayList(ArrayList* list, void* data)
{
	if (list->elementInList == 0)
	{
		Node* newNode = createNode(data);
		list->elementInList = 1;
		list->endOfArray = list->startOfArray = newNode;
	}
	else 
	{
		Node* currFirst = list->startOfArray;
		Node* newFirst = createNode(data);
		newFirst->next = currFirst;
		currFirst->pervius = newFirst;
		list->startOfArray = newFirst;
	}
}

void pushItemToEndArrayList(ArrayList* list, void* data)
{
	if (list->elementInList == 0)
	{
		Node* newNode = createNode(data);
		list->elementInList = 1;
		list->endOfArray = list->startOfArray = newNode;
	}
	else
	{
		Node* currLast = list->endOfArray;
		Node* newLast = createNode(data);
		newLast->pervius = currLast;
		currLast->next = newLast;
		list->endOfArray = newLast;
		list->elementInList++;
	}
}

void freeArrayList(ArrayList* list)
{

	int i;
	Node* tempNode = NULL;
	Node* operateNode = list->startOfArray;

	for (i = 0; i < list->elementInList; i++)
	{
		tempNode = operateNode->next;
		free(operateNode);
		operateNode = tempNode;
	}

	free(list);

}