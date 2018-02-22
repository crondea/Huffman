/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: Lab4main.c
 * 
 */

#include "functions.h"
#define BUFFERSIZE 7

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
    int bufferIndex = BUFFERSIZE;


    // Array to store character frequencies
    int frequencies[256];
    //unsigned char *codes[256];
    unsigned short int codes[256];
    unsigned short int lengths[256];
    
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
    if(!(outptr2 = fopen("Broke_compressed","wb")))
    {
        printf("Error opening broke output file\n");
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
        codes[i] = 0;
        lengths[i] = 0;
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

    //fwrite(&fileSize,sizeof(int),1,outptr1);

    for (i=0;i<256;i++)
    {   //fwrite(&frequencies[i],sizeof(int),1,outptr1);
        if (frequencies[i]>0)
        {
            findLeaf(list->head,0,&codes[i],i,&lengths[i]);
            //findLeaf(list->head,0,&codes[i],i);
            //printf("%c: %X\tfreq: %d\tlen: %d\n",i,codes[i],frequencies[i],lengths[i]);
        }
    }


    // Compression -----------------------------------------------------------
    // write size of filedata, then frequencies table
    


    unsigned char fileChar = 0;
    //unsigned char *code;
    //unsigned char *extraBits;
    unsigned short int code;
    int codeLen, bitsLen;
    for (i = 0; i < fileSize; i++)
    {
        //fwrite(codes[fileData[i]],lengths[fileData[i]],1,outptr2);
        fileChar = fileData[i]; // character in file
        //codeLen = strlen((const char *)codes[fileChar]);
        codeLen = lengths[fileChar];
        //code = (unsigned char *)calloc(1,codeLen);
        //code = (unsigned char *)strcpy((char *)code,(char *)codes[fileChar]);
        code = codes[fileChar];
        //printf("%s, %X\n",codes[fileChar],(unsigned int)*codes[fileChar]);

        if(bufferIndex >= codeLen - 1)
        {
            buffer |= code << ((bufferIndex - codeLen) + 1);
            bufferIndex -= codeLen;
            // if the buffer is full, write, reset
            if(bufferIndex == -1)
            {
                fwrite(&buffer,sizeof(unsigned char),1,outptr1); // write 16 bits at a time
                buffer = 0;
                bufferIndex = BUFFERSIZE;
            }
        }
        else
        {
            // save what we can in the buffer
            bitsLen = bufferIndex + 1;
            // get the bits that we can't fit
            buffer |= code >> (codeLen - bitsLen);
            // write the full buffer
            fwrite(&buffer,sizeof(unsigned char),1,outptr1);
            // reset buffer
            buffer = 0;
            bufferIndex = BUFFERSIZE;
            // add remaining bits to the buffer

            //code = codes[fileChar]; // reset code for character
            //buffer |= code << ((bufferIndex + 1) - (codeLen - bitsLen));
            //bufferIndex -= codeLen - bitsLen;
            codeLen -= bitsLen;
            if(bufferIndex >= codeLen - 1)
            {
                buffer |= code << ((bufferIndex - codeLen) + 1);
                bufferIndex -= codeLen;
                // if the buffer is full, write, reset
                if(bufferIndex == -1)
                {
                    fwrite(&buffer,sizeof(unsigned char),1,outptr1); // write 16 bits at a time
                    buffer = 0;
                    bufferIndex = BUFFERSIZE;
                }
            }
            else
            {
                // save what we can in the buffer
                bitsLen = bufferIndex + 1;
                // get the bits that we can't fit
                buffer |= code >> (codeLen - bitsLen);
                // write the full buffer
                fwrite(&buffer,sizeof(unsigned char),1,outptr1);
                // reset buffer
                buffer = 0;
                bufferIndex = BUFFERSIZE;
                // add remaining bits to the buffer
                //code = codes[fileChar];
                buffer |= code << ((bufferIndex + 1) - (codeLen - bitsLen));
                bufferIndex -= codeLen - bitsLen;
            }

        }

    }

    if (bufferIndex != BUFFERSIZE) fwrite(&buffer,sizeof(unsigned short int),1,outptr1);

    // Decompression ---------------------------------------------------------

    return 0;
}