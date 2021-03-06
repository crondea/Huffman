/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: list.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

/*  The purpose of this function is to initialize the header block that will contain
 *  metadata as well as the pointer to the first node in our data structure.
 */
listRoot *listConstruct()
{
    listRoot *listPtr;
    listPtr = (listRoot *)malloc(sizeof(listRoot));
    listPtr->head = NULL;
    listPtr->listSize = 0;
    return listPtr;
}

/*  The prupose of this function is to insert a given node into the given list in a
 *  sorted manner. The sorting os based upon the frequency value of the nodes. That is,
 *  the first node will always have the lowest frequency, and the last node will always
 *  have the largest frequency.
 */
void listInsertSorted(listRoot *treeHeader, listNode *newNode)
{
    // Initializing a roving pointer to traverse the list
    listNode *currPtr = NULL;
    // Checking if the list is empty so we can simply add the new node
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
        // Starting at the first node in the list
        currPtr = treeHeader->head;
        // Checking if we only have one item in the list
        if(currPtr->next == NULL)
        {
            // Only one item, check the frequency of the item and insert new node
            // according to that
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

        // There must be multiple list items, traverse until we hit the end (NULL)
        while(currPtr != NULL)
        {
            // If the current frequency is greater than the frequency of the new node,
            // we want to insert the node in front of the current node, else we want to
            // insert it after the current node
            if(currPtr->frequency > newNode->frequency)
            {
                // If the current node is the first node in the list and its frequency
                // is greater than the new node's, then we will insert the new node in
                // front of the current node, thus we will have a new first node
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
                    // Simply insert after the current node
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
                // Current node frequency is less than the new node, but the current
                // node is the last node, so we can just add the new node to the end.
                newNode->prev = currPtr;
                newNode->next = NULL;
                currPtr->next = newNode;
                treeHeader->listSize++;
                return;
            }
            // Increment roving pointer to next node
            currPtr = currPtr->next;
        }
    }
}

/*  The purpose of this function is to remove the first two nodes (2 smallest) from the
 *  list, combine their frequencies and create a new node, set the removed nodes as the
 *  left and right children of the new node, and re-insert the new node (now the head of
 *  a tree, but still a list node) into the list.
 */
void combineFirstTwo(listRoot *treeHeader)
{
    // Initializing the new parent node for the two nodes being removed
    listNode *sumNode = (listNode *)malloc(sizeof(listNode));
    sumNode->next = NULL;
    sumNode->prev = NULL;
    sumNode->left = NULL;
    sumNode->right = NULL;
    sumNode->parent = NULL;
    sumNode->frequency = 0;
    sumNode->symbol = '\0';
    // Initializing pointers to the nodes we are removing from the list
    listNode *firstNode;
    listNode *secondNode;
    // Remove first two nodes from the tree
    // We will have at least one node left in the list
    firstNode = listRemove(treeHeader);
    secondNode = listRemove(treeHeader);
    sumNode->frequency = (firstNode->frequency) + (secondNode->frequency);
    // Setting tree pointers
    sumNode->left = firstNode;
    sumNode->right = secondNode;
    firstNode->parent = sumNode;
    secondNode->parent = sumNode;
    // Adding the new parent node to the list, it is now the head of a tree
    listInsertSorted(treeHeader,sumNode);
}

/*  The purpose of this funtion is to remove the first node in the list and return
 *  a pointer to it.
 */
listNode *listRemove(listRoot *treeHeader)
{
    // Setting the pointer to the node being removed
    listNode *removedNode = treeHeader->head;
    // Setting the pointers and the new head of the list
    removedNode->prev = NULL;
    treeHeader->head = removedNode->next;
    treeHeader->listSize--;
    removedNode->next = NULL;
    // If we just took out the last node, set the head to null
    if(treeHeader->listSize == 0)
    {
        treeHeader->head = NULL;
    }
    return removedNode;
}

/*  The purpose of this function is to free the memory allocated for the data structures
 *  during the algorithm. At the end of compression, the data structure will be a binary
 *  tree containing the nodes that were originally in a linked list.
 */
void dataDestruct(listRoot *treeHeader)
{   
    dataDestructAux(treeHeader->head,0);
    free(treeHeader);
}


void dataDestructAux(listNode *node, int level)
{
    // This is our base case
    if(node == NULL) return;
    listNode *left;
    // Traverse to the right (go down a level)
    dataDestructAux(node->right,level+1);
    left = node->left;
    // free node data
    free(node);
    // Travers to the left (go down a level)
    dataDestructAux(left,level+1);
}

/*  The purpose of this function is to print the list nodes' symbols and their frequencies.
 */
void printList(listNode *node)
{
    if(node == NULL)
    {
        printf("(NULL)\n");
        return;
    }
    // Starting at the node (should be the first node in the list from the function call)
    listNode *ptr = node;
    // Looping and printing until we hit the end (NULL)
    while(ptr != NULL)
    {
        if(ptr->next == NULL)
        {
            printf("(%d %c)\n",ptr->frequency,ptr->symbol);
            return;
        }
        else
        {
            printf("(%d %c)->",ptr->frequency,ptr->symbol);
            ptr = ptr->next;
        }
    }
}


void printTree(listRoot *tree)
{
    printTreeAux(tree->head,0);
}

/*  The purpose of this function is to facilitate the printing of the nodes in the tree.
 *  This is the auxilliary function which is recursive for the printTree function.
 */
void printTreeAux(listNode *node, int level)
{
    int i;
    // This is our base case
    if(node == NULL) return;
    // Traverse to the right (go down a level)
    printTreeAux(node->right,level+1);
    // Print space based on the level to give the form of a tree
    for(i = 0; i < level; i++) printf("        ");
    // Print the values of the node
    printf("(%d %c)\n",node->frequency,node->symbol);
    // Travers to the left (go down a level)
    printTreeAux(node->left,level+1);
}

/*  The purpose of this function is to go to the right and left of each node until a leaf node
*   is found. This is a recursive function and calls a code function when a leaf node is found.
*/
void findLeaf(listNode *node, int level, unsigned short int *code, unsigned char symbol, unsigned short int *lengths)
{
    if (node)
    {   // ensure the desired leaf node was found
        if(node->symbol == symbol && node->left == NULL)
        {
            findCode(node,level,code,symbol,lengths);
            return;
        }
    }
    if (node)
    {   // if node is not NULL, go to the right and left
        findLeaf(node->right,level+1,code,symbol,lengths);
        findLeaf(node->left,level+1,code,symbol,lengths);
    }
    

}

/*  The purpose of this function is to start from the leaf node and step back up to the parent, recording the steps taken
*   in the code value. The step value is shifted to the left by the number of levels it has moved up from the leaf node.
*/
void findCode(listNode *node, int level, unsigned short int *code, unsigned char symbol, unsigned short int *lengths)
{
        int i;
        listNode *parent, *current;

        current = node; // start at leaf node
        *lengths = level; // length of code will be the same as the number of levels down the leaf node is
        for(i = 0;i<=level-1;i++)
        {   // find parent node
            parent = current->parent;
            if (parent != NULL)
            {   // if the leaf node came from the right of the parent
                if (parent->right == current)
                {
                    *code |= 0x01 << i;
                }
                else 
                {   // if the leaf node came from the left of the parent
                    *code |= 0x00 << i;
                }
            }
            // move up tree to the head to find full code for character (leaf node)
            current = current->parent;
        }


}

/*  The purpose of this function is to cycle through the array containing the file data
 *  and increment a counter every time the given index(character/symbol) is found. This
 *  count is returned as the frequency of the given symbol.
 */
int findFrequencies(unsigned char *data, int size, int index)
{
    int count = 0, i;
    for(i = 0; i < size; i++) if((int)data[i] == index) count++;
    return count;
}