/*  Corey Rondeau and Caroline Hester
*  ECE 4680 Lab 4
*  File Created: Feb. 13 2018
*  File Name: Lab4main.c
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void printList(listNode *node);
void printTree(listRoot *tree);
void printTreeAux(listNode *node, int level);
//void printCode(listNode *node, int level, int binary, char*code);
unsigned short int findFrequencies(unsigned char *data, int size, int index);
void findLeaf(listNode *node, int level, unsigned char **code, unsigned char symbol);
void findCode(listNode *node, int level, unsigned char **code);



int main(int argc, char *argv[])
{
	FILE *inptr1 = NULL;
	//FILE *inptr2 = NULL;
	FILE *outptr1 = NULL;
	FILE *outptr2 = NULL;
	int fileSize;
	int i, count;

	// This will be a pointer to the head of the tree
	listRoot *list = listConstruct();

	// This will be our buffer for writing the codes to the compressed file
	unsigned char buffer = 0;
	// Position in buffer set initially to max
	int bufferIndex = 7;


	// Array to store character frequencies
	unsigned short int frequencies[256];
	unsigned char *codes[256];
	for (i = 0; i < 255; i++)
	{
		frequencies[i] = 0;
	}

	if (!(inptr1 = fopen(argv[1], "rb")))
	{
		printf("Error opening %s\n", argv[1]);
		exit(-1);
	}
	if (!(outptr1 = fopen("compressed", "wb")))
	{
		printf("Error opening compression output file\n");
		exit(-1);
	}
	if (!(outptr2 = fopen("Broke_compressed", "wb")))
	{
		printf("Error opening broke output file\n");
		exit(-1);
	}

	// Getting the size in bytes of the file
	fseek(inptr1, 0, SEEK_END);
	fileSize = ftell(inptr1);
	rewind(inptr1);

	// Allocating for the amount of the data in the file, then storing the file data
	unsigned char *fileData = (unsigned char *)calloc(1, fileSize * sizeof(unsigned char));
	fread(fileData, sizeof(unsigned char), fileSize, inptr1);

	// We send the data and the index to a function to count the number of times this index
	// (or ASCI value) occurs in the data.
	// We also need to allocate a new node for every entry in the frequencies array that is > 0
	for (i = 0; i < 255; i++)
	{
		codes[i] = NULL;
		count = findFrequencies(fileData, fileSize, i);
		if (count > 0)
		{
			frequencies[i] = count;
			listNode *node = (listNode *)malloc(sizeof(listNode));
			node->left = NULL;
			node->right = NULL;
			node->parent = NULL;
			node->next = NULL;
			node->prev = NULL;
			node->frequency = frequencies[i];
			node->symbol = (unsigned char)i;
			listInsertSorted(list, node);
			//printList(list->head);
		}
	}
	// Turn the list into the Huffman tree
	while (list->listSize > 1)
	{
		combineFirstTwo(list);
		//printList(list->head);
	}

	//printf("\n\n");
	//printTree(list);

	for (i = 0; i<256; i++)
	{
		if (frequencies[i]>0)
		{
			findLeaf(list->head, 0, &codes[i], i);
			//printf("%d %c: %s f:%d\n",i,i,codes[i],frequencies[i]);
		}
	}


	// Compression -----------------------------------------------------------
	unsigned char fileChar = 0;
	unsigned char *code;
	unsigned char *extraBits;
	int codeLen, bitsLen;
	for (i = 0; i < fileSize; i++)
	{
		fwrite(codes[fileData[i]], strlen((const char *)codes[fileData[i]]) * sizeof(unsigned char), 1, outptr2);
		fileChar = fileData[i];
		codeLen = strlen((const char *)codes[fileChar]);
		code = (unsigned char *)calloc(1, codeLen);
		code = (unsigned char *)strcpy((char *)code, (char *)codes[fileChar]);
		//printf("%s, %X\n",codes[fileChar],(unsigned int)*codes[fileChar]);

		if (bufferIndex >= codeLen - 1)
		{
			//printf("%d\n",atoi((char *)code));
			*code = *code << ((bufferIndex - codeLen) + 1);
			//printf("%X\n\n",*code);
			buffer |= *code;
			bufferIndex -= codeLen;

			// if the buffer is full, write, reset
			if (bufferIndex == -1)
			{
				fwrite(&buffer, sizeof(unsigned char), 1, outptr1);
				buffer = 0;
				bufferIndex = 7;
			}
		}
		else
		{
			// save what we can in the buffer
			extraBits = (unsigned char *)calloc(1, codeLen);
			extraBits = (unsigned char *)strcpy((char *)extraBits, (char *)codes[fileChar]);
			bitsLen = bufferIndex + 1;
			// get the bits that we can't fit
			*extraBits = *extraBits >> (codeLen - bitsLen);
			buffer |= *extraBits;
			// write the full buffer
			fwrite(&buffer, sizeof(unsigned char), 1, outptr1);
			// reset buffer
			buffer = 0;
			bufferIndex = 7;
			// add remaining bits to the buffer
			free(extraBits);
			buffer |= *code << ((bufferIndex + 1) - (codeLen - bitsLen));
			bufferIndex -= codeLen - bitsLen;
		}

		//free(code);
		//
	}
	fwrite(&buffer, sizeof(unsigned char), 1, outptr1);

	// Decompression ---------------------------------------------------------

	return 0;
}




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
	if (treeHeader->head == NULL)
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
		if (currPtr->next == NULL)
		{
			// Only one item, check the frequency of the item and insert new node
			// according to that
			if (currPtr->frequency < newNode->frequency)
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
		while (currPtr != NULL)
		{
			// If the current frequency is greater than the frequency of the new node,
			// we want to insert the node in front of the current node, else we want to
			// insert it after the current node
			if (currPtr->frequency > newNode->frequency)
			{
				// If the current node is the first node in the list and its frequency
				// is greater than the new node's, then we will insert the new node in
				// front of the current node, thus we will have a new first node
				if (currPtr == treeHeader->head)
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
			else if (currPtr->next == NULL)
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
	listInsertSorted(treeHeader, sumNode);
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
	if (treeHeader->listSize == 0)
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

}

/*  The purpose of this function is to print the list nodes' symbols and their frequencies.
*/
void printList(listNode *node)
{
	if (node == NULL)
	{
		printf("(NULL)\n");
		return;
	}
	// Starting at the node (should be the first node in the list from the function call)
	listNode *ptr = node;
	// Looping and printing until we hit the end (NULL)
	while (ptr != NULL)
	{
		if (ptr->next == NULL)
		{
			printf("(%d %c)\n", ptr->frequency, ptr->symbol);
			return;
		}
		else
		{
			printf("(%d %c)->", ptr->frequency, ptr->symbol);
			ptr = ptr->next;
		}
	}
}


void printTree(listRoot *tree)
{
	printTreeAux(tree->head, 0);
}

/*  The purpose of this function is to facilitate the printing of the nodes in the tree.
*  This is the auxilliary function which is recursive for the printTree function.
*/
void printTreeAux(listNode *node, int level)
{
	int i;
	// This is our base case
	if (node == NULL) return;
	// Traverse to the right (go down a level)
	printTreeAux(node->right, level + 1);
	// Print space based on the level to give the form of a tree
	for (i = 0; i < level; i++) printf("        ");
	// Print the values of the node
	printf("(%d %c)\n", node->frequency, node->symbol);
	// Travers to the left (go down a level)
	printTreeAux(node->left, level + 1);
}


void findLeaf(listNode *node, int level, unsigned char **code, unsigned char symbol)
{
	if (node)
	{
		if (node->symbol == symbol && node->left == NULL)
		{
			findCode(node, level, code);
			//printf("code: %s\n",*code);
			return;
		}
	}
	if (node)
	{
		findLeaf(node->right, level + 1, code, symbol);
		findLeaf(node->left, level + 1, code, symbol);
	}


}

void findCode(listNode *node, int level, unsigned char **code)
{
	int i;
	listNode *parent, *current;

	*code = (unsigned char *)calloc(1, (level + 1) * sizeof(unsigned char));
	current = node;
	for (i = level - 1; i >= 0; i--)
	{
		parent = current->parent;
		if (parent != NULL)
		{
			if (parent->right == current)
			{
				code[0][i] = '1';
			}
			else
			{
				code[0][i] = '0';
			}
		}
		current = current->parent;
	}

}

/*  The purpose of this function is to cycle through the array containing the file data
*  and increment a counter every time the given index(character/symbol) is found. This
*  count is returned as the frequency of the given symbol.
*/
unsigned short int findFrequencies(unsigned char *data, int size, int index)
{
	int count = 0, i;
	for (i = 0; i < size; i++) if ((int)data[i] == index) count++;
	return count;
}