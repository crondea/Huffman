/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Jan. 31 2018
 *  File Name: list.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"


listRoot *listConstruct()
{
    listRoot *listPtr;
    listPtr = (listRoot *)malloc(sizeof(listRoot));
    listPtr->head = NULL;
    listPtr->listSize = 0;
    return listPtr;
}

void listInsertSorted(listRoot *treeHeader, unsigned short int frequency, unsigned char symbol)
{
    listNode *newNode = (listNode *)malloc(sizeof(listNode));
    newNode->frequency = frequency;
    newNode->symbol = symbol;
    newNode->next = NULL;
    newNode->prev = NULL;

    listNode *currPtr = NULL;
    printf("Adding: %d\n",frequency);
    

    if(treeHeader->head == NULL)
    {
        treeHeader->head = newNode;
        newNode->prev = NULL;
        newNode->next = NULL;
        treeHeader->listSize++;
        return;
    }
    else
    {
        currPtr = treeHeader->head;
        // Only one item in the list
        if(currPtr->next == NULL)
        {
            if(currPtr->frequency < newNode->frequency)
            {
                // Insert in front of
                currPtr->next = newNode;
                newNode->next = NULL;
                newNode->prev = currPtr;
                treeHeader->listSize++;
                return;
            }
            else
            {
                // Insert after
                newNode->next = currPtr;
                currPtr->prev = newNode;
                currPtr->next = NULL;
                treeHeader->head = newNode;
                treeHeader->listSize++;
                return;
            }
        }

        // Must be multiple list items
        while(currPtr != NULL)
        {
            // If the next node exists, check if it's frequency is higher than the new node
            // and insert it after the current node.
            if(currPtr->frequency > newNode->frequency)
            {
                if(currPtr == treeHeader->head)
                {
                    newNode->next = currPtr;
                    newNode->prev = NULL;
                    currPtr->prev = newNode;
                    treeHeader->head = newNode;
                    treeHeader->listSize++;
                    return;
                }
                else
                {
                    newNode->prev = currPtr->prev;
                    newNode->next = currPtr;
                    currPtr->prev->next = newNode;
                    treeHeader->listSize++;
                    return;
                }

            }
            else if(currPtr->next == NULL)
            {
                newNode->prev = currPtr;
                newNode->next = NULL;
                currPtr->next = newNode;
                treeHeader->listSize++;
                return;
            }

            currPtr = currPtr->next;
        }
    }
}

void listRemoveFirstTwo(listRoot *treeHeader)
{
    listNode *sumNode = (listNode *)malloc(sizeof(listNode));
    sumNode->next = NULL;
    sumNode->prev = NULL;
    sumNode->left = NULL;
    sumNode->right = NULL;
    sumNode->frequency = 0;

    listNode *firstNode;
    listNode *secondNode;

    // Remove first two nodes from the tree
    if(treeHeader->listSize > 2)
    {
        // We will have at least one node left in the list

    }
    else if(treeHeader->listSize == 2)
    {
        // We will be emptying the list, then adding in the final node
        
    }


}

void listDestruct(listRoot *treeHeader)
{


}

/*  The purpose of this recursive function is to print the list nodes' symbols
 *  and their frequencies.
 */
void printList(listNode *node)
{
    if(node == NULL)
    {
        printf("(NULL)\n");
        return;
    }
    listNode *ptr = node;
    while(ptr != NULL)
    {
        printf("(%d %c)->",ptr->frequency,ptr->symbol);
        ptr = ptr->next;
    }
    printf("\n");
    return;
}