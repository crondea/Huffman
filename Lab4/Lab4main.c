/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: Lab4main.c
 * 
 */


#include "functions.h"

int main(int argc, char *argv[])
{
    FILE *inptr1 = NULL;
    FILE *inptr2 = NULL;
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
        }
    }
    // Turn the list into the Huffman tree
    while(list->listSize > 1)
    {
        combineFirstTwo(list);
    }
    
    fwrite(&fileSize,sizeof(int),1,outptr1);
    for (i=0;i<256;i++)
    {   
        fwrite(&frequencies[i],sizeof(int),1,outptr1);
        if (frequencies[i]>0)
        {
            findLeaf(list->head,0,&codes[i],i,&lengths[i]);
        }
    }

    // Compression -----------------------------------------------------------
    // write size of filedata, then frequencies table
    unsigned char fileChar = 0;
    unsigned short int code;
    int codeLen, bitsLen;
    for (i = 0; i < fileSize; i++)
    {
        fileChar = fileData[i]; // character in file
        codeLen = lengths[fileChar];
        code = codes[fileChar];

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
                buffer |= code << ((bufferIndex + 1) - (codeLen - bitsLen));
                bufferIndex -= codeLen - bitsLen;
            }
        }
    }

    if (bufferIndex != BUFFERSIZE) fwrite(&buffer,sizeof(unsigned char),1,outptr1);

    fclose(outptr1);
    fclose(inptr1);


    // Decompression ---------------------------------------------------------

    int fileSize2, origFileSize, dataSize;
    int frequencies2[256];
    int byteCount = 0;
    unsigned char *fileData2;

    listRoot *list2 = listConstruct();

    if(!(inptr2 = fopen("compressed","rb")))
    {
        printf("Error opening compressed input file\n");
        exit(-1);
    }
    if(!(outptr2 = fopen("decompressed","wb")))
    {
        printf("Error opening decompressed output file\n");
        exit(-1);
    }

    // Getting the size in bytes of the file
    fseek(inptr2, 0, SEEK_END);
    fileSize2 = ftell(inptr2);
    rewind(inptr2);

    fread(&origFileSize,sizeof(int),1,inptr2);

    for(i = 0; i < 256; i++)
    {
        fread(&frequencies2[i],sizeof(int),1,inptr2);
    }

    // Now we will remake the tree as we did during compression
    for(i = 0; i < 255; i++)
    {
        if(frequencies2[i] > 0)
        {
            listNode *node = (listNode *)malloc(sizeof(listNode));
            node->left = NULL;
            node->right = NULL;
            node->parent = NULL;
            node->next = NULL;
            node->prev = NULL;
            node->frequency = frequencies2[i];
            node->symbol = (unsigned char)i;
            listInsertSorted(list2,node);
        }
    }
    // Turn the list into the Huffman tree
    while(list2->listSize > 1)
    {
        combineFirstTwo(list2);
    }

    dataSize = (fileSize2 - FILE_HEADER_SIZE);
    fileData2 = (unsigned char *)calloc(dataSize,sizeof(unsigned char));
    fread(fileData2,sizeof(unsigned char),dataSize,inptr2);

    for(i = 0; i < dataSize; i++)
    {


    }

    return 0;
}