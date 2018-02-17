/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Jan. 31 2018
 *  File Name: tree.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"


// treeRoot *treeConstruct()
// {
//     treeRoot *treePtr;
//     treePtr = (treeRoot *)malloc(sizeof(treeRoot));
//     treePtr->treeHead = NULL;
//     treePtr->treeSize = 0;
//     return treePtr;
// }

// void insertTree(treeRoot *head, treeNode *newNode)
// {
//     treeNode *currPtr = NULL;

//     if(head->treeHead == NULL)
//     {
//         head->treeHead = newNode;
//     }
//     else
//     {
//         currPtr = head->treeHead;
//         while(currPtr != NULL)
//         {
//             if(currPtr->frequency < newNode->frequency)
//             {
//                 currPtr = currPtr->leftNode;
//             }
//             else
//             {


//             }
//         }
//     }
// }

// void removeTree(treeRoot *head)
// {
    
// }

// void freeTree(treeRoot *head)
// {

// }

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

// void nodeCreate(unsigned short int frequency, unsigned char symbol, treeRoot *head)
// {
//     treeNode *newNode = (treeNode *)malloc(sizeof(treeNode));
//     newNode->rightNode = NULL;
//     newNode->leftNode = NULL;
//     newNode->symbol = symbol;
//     newNode->parent = NULL;
//     newNode->frequency = frequency;
//     newNode->leafFlag = 1;
//     insertTree(head,newNode);
// }



// void printTree(treeNode *head)
// {
//     //print left
//     printTreeAux(head->leftNode);
//     //print right
//     printTreeAux(head->rightNode);
// }

// void printTreeAux(treeNode *currNode)
// {
//     if((currNode->leftNode == NULL) && (currNode->rightNode == NULL))
//     {
//         printf("%d\n",currNode->frequency);
//         return;
//     }
//     printTreeAux(currNode->leftNode);
//     printTreeAux(currNode->rightNode);
// }