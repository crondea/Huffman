/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: functions.h
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This is the size of the buffer we are using to write the codes to the compressed file
#define BUFFERSIZE 7

// This is the header of the compressed file containing 4 bytes (int) of the original file size
// followed by the 256 symbol frequencies (ints)
#define FILE_HEADER_SIZE 1028

// Below are the structure definitions as well as the function definitions
// for the linked list
typedef struct listNodeTag
{
    struct listNodeTag *prev;
    struct listNodeTag *next;
    struct listNodeTag *left;
    struct listNodeTag *right;
    struct listNodeTag *parent;
    unsigned short int frequency;
    unsigned char symbol;
} listNode;

typedef struct listRootTag
{
    listNode *head;
    unsigned short int listSize;
} listRoot;

listRoot *listConstruct();
void listInsertSorted(listRoot *treeHeader, listNode *newNode);
void combineFirstTwo(listRoot *treeHeader);
listNode *listRemove(listRoot *treeHeader);
void dataDestruct(listRoot *treeHeader);
void dataDestructAux(listNode *node, int level);
void printList(listNode *node);
void printTree(listRoot *tree);
void printTreeAux(listNode *node, int level);
int findFrequencies(unsigned char *data, int size ,int index);
void findLeaf(listNode *node, int level, unsigned short int *code, unsigned char symbol, unsigned short int *lengths);
void findCode(listNode *node, int level, unsigned short int *code, unsigned char symbol, unsigned short int *lengths);
