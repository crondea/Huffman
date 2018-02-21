/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: Lab4main.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int main(int argc, char *argv[])
{
    FILE *inptr1 = NULL;
    //FILE *inptr2 = NULL;
    FILE *outptr1 = NULL;
    //FILE *outptr2 = NULL;
    int fileSize;
    int i, count;

    // This will be a pointer to the head of the tree
    listRoot *list = listConstruct();

    // Array to store character frequencies
    unsigned short int frequencies[256];
    unsigned char *codes[256];
    for(i = 0; i < 255; i++)
    {
        frequencies[i] = 0;
    }

    if(!(inptr1 = fopen(argv[1],"rb")))
    {
        printf("Error opening %s\n",argv[1]);
        exit(-1);
    }
    if(!(outptr1 = fopen("compressed","wb")))
    {
        printf("Error opening compression output file\n");
        exit(-1);
    }

    // Getting the size in bytes of the file
    fseek(inptr1, 0, SEEK_END);
    fileSize = ftell(inptr1);
    rewind(inptr1);

    // Allocating for the amount of the data in the file, then storing the file data
    unsigned char *fileData = (unsigned char *)calloc(1,fileSize*sizeof(unsigned char));
    fread(fileData,sizeof(unsigned char),fileSize,inptr1);

    // We send the data and the index to a function to count the number of times this index
    // (or ASCI value) occurs in the data.
    // We also need to allocate a new node for every entry in the frequencies array that is > 0
    for(i = 0; i < 255; i++)
    {
        codes[i] = NULL;
        count = findFrequencies(fileData,fileSize,i);
        if(count > 0)
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
            listInsertSorted(list,node);
            //printList(list->head);
        }
    }
    // Turn the list into the Huffman tree
    while(list->listSize > 1)
    {
        combineFirstTwo(list);
        //printList(list->head);
    }

    //printf("\n\n");
    //printTree(list);

    for (i=0;i<256;i++)
    {
        if (frequencies[i]>0)
        {
            findLeaf(list->head,0,&codes[i],i);
            //printf("%d %c: %s f:%d\n",i,i,codes[i],frequencies[i]);
        }
    }

    
    // Compression -----------------------------------------------------------

    for (i=0;i<fileSize;i++)
    {
        fwrite(codes[fileData[i]],strlen((const char *)codes[fileData[i]])*sizeof(unsigned char),1,outptr1);
    }    

    // Decompression ---------------------------------------------------------

    return 0;
}