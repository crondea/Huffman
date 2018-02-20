/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Jan. 31 2018
 *  File Name: list.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"


listRoot *listConstruct()
{
    listRoot *listPtr;
    listPtr = (listRoot *)malloc(sizeof(listRoot));
    listPtr->head = NULL;
    listPtr->listSize = 0;
    return listPtr;
}


void listInsertSorted(listRoot *treeHeader, listNode *newNode)
{

    listNode *currPtr = NULL;
    //printf("Adding: %d\n",newNode->frequency);
    

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
                    currPtr->prev = newNode;
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


void combineFirstTwo(listRoot *treeHeader)
{
    listNode *sumNode = (listNode *)malloc(sizeof(listNode));
    sumNode->next = NULL;
    sumNode->prev = NULL;
    sumNode->left = NULL;
    sumNode->right = NULL;
    sumNode->frequency = 0;
    sumNode->symbol = '\0';

    listNode *firstNode;
    listNode *secondNode;

    // Remove first two nodes from the tree
    // We will have at least one node left in the list
    firstNode = listRemove(treeHeader);
    secondNode = listRemove(treeHeader);
    sumNode->frequency = (firstNode->frequency) + (secondNode->frequency);
    sumNode->left = firstNode;
    sumNode->right = secondNode;
    firstNode->parent = sumNode;
    secondNode->parent = sumNode;
    listInsertSorted(treeHeader,sumNode);
}


listNode *listRemove(listRoot *treeHeader)
{
    listNode *removedNode = treeHeader->head;
    removedNode->prev = NULL;
    treeHeader->head = removedNode->next;
    treeHeader->listSize--;
    removedNode->next = NULL;
    if(treeHeader->listSize == 0)
    {
        treeHeader->head = NULL;
    }
    return removedNode;
}

void treeDestruct(listRoot *treeHeader)
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


void printTree(listRoot *tree)
{
    printTreeAux(tree->head,0);
}


void printTreeAux(listNode *node, int level)
{
    int i;
    if(node == NULL) return;
    printTreeAux(node->right,level+1);
    for(i = 0; i < level; i++) printf("        ");
    printf("(%d %c)\n",node->frequency,node->symbol);
    printTreeAux(node->left,level+1);
    
}


void printCode(listNode *node, int level, int binary, char*code)
{
    int i;
    if(node == NULL) return;
    code = (char *)realloc(code,(level+2)*sizeof(char));
    printCode(node->right,level+1,1,strcat(code,"1"));
    for(i = 0; i < level; i++) printf("    ");
    if(level != 0)
    {
        //printf("(%d %c)\n",node->frequency,node->symbol);
        printf("%d\n",binary);
    }
    char *newcode;
    newcode = (char *)calloc(1,(level+2)*sizeof(char));
    free(code);
    code = newcode;
    printCode(node->left,level+1,0,strcat(code,"0"));
    if (node->symbol)
    {
        printf("%c: %s\n",node->symbol,code);
    }
}


unsigned short int findFrequencies(unsigned char *data, int size, int index)
{
    int count = 0;
    int i;

    for(i = 0; i < size; i++)
    {
        if((int)data[i] == index) count++;
    }
    return count;
}