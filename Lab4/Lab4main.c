/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 3
 *  File Created: Jan. 31 2018
 *  File Name: fileGen.c
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

int main(int argc, char *argv[])
{
    FILE *inptr1 = NULL;
    //FILE *inptr2 = NULL;
    FILE *outptr1 = NULL;
    //FILE *outptr2 = NULL;
    int fileSize;
    int i;

    // This will be a pointer to the head of the tree
    //treeRoot *tree = treeConstruct();
    listRoot *list = listConstruct();

    // Array to store character frequencies
    unsigned short int frequencies[256];
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

    unsigned char *fileData = (unsigned char *)calloc(1,fileSize*sizeof(unsigned char));
    fread(fileData,sizeof(unsigned char),fileSize,inptr1);

    // We send the data and the index to a function to count the number of times this index
    // (or ASCI value) occurs in the data.
    // We also need to allocate a new node for every entry in the frequencies array that is > 0
    for(i = 0; i < 255; i++)
    {
        frequencies[i] = findFrequencies(fileData,fileSize,i);
        if(frequencies[i] > 0)
        {

            listInsertSorted(list,frequencies[i],(unsigned char)i);
            //nodeCreate(frequencies[i],(unsigned char)i,treeHead);
        }
    }

    //printTree(tree->treeHead);
    // Compression -----------------------------------------------------------
    

    // Decompression ---------------------------------------------------------

    return 0;
}