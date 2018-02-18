/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Jan. 31 2018
 *  File Name: binaryTree.c
 * 
 */

// Below are the structure definitions as well as the function definitions
// for the linked list
typedef struct listNodeTag
{
    struct listNodeTag *prev;
    struct listNodeTag *next;
    struct listNodeTag *left;
    struct listNodeTag *right;
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
void treeDestruct(listRoot *treeHeader);
void printList(listNode *node);
void printTree(listRoot *tree);
void printTreeAux(listNode *node, int level);
void printCode(listNode *node, int level, int binary, char*code);



// Below are the structure definitions as well as the function definitions
// for the binary tree
// typedef struct treeNodeTag
// {
//     struct treeNodeTag *parent;
//     struct treeNodeTag *leftNode;
//     struct treeNodeTag *rightNode;
//     unsigned short int frequency;
//     unsigned char symbol;
//     unsigned short int leafFlag;
// } treeNode;

// typedef struct treeRootTag
// {
//     treeNode *treeHead;
//     unsigned short int treeSize;
// } treeRoot;

// treeRoot *treeConstruct();
// void insertTree(treeRoot *head, treeNode *newNode);
// void removeTree(treeRoot *head);
// void freeTree(treeRoot *head);
unsigned short int findFrequencies(unsigned char *data, int size ,int index);
// void nodeCreate(unsigned short int frequency, unsigned char symbol, treeRoot *head);
// void printTree(treeNode *head);
// void printTreeAux(treeNode *currNode);