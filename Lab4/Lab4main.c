/*  Corey Rondeau and Caroline Hester
 *  ECE 4680 Lab 4
 *  File Created: Feb. 13 2018
 *  File Name: Lab4main.c
 * 
 */


#include "functions.h"

int main(int argc, char *argv[])
{   // These will be the file pointes used for compression and decompression
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
    // Position in buffer, set initially to max
    int bufferIndex = BUFFERSIZE;


    // Array to store character frequencies, codes and lengths of codes
    int frequencies[256];
    unsigned short int codes[256];
    unsigned short int lengths[256];
    
    for(i = 0; i < 256; i++)
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
    for(i = 0; i < 256; i++)
    {   // initialize code and length to 0
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
            // insert new node in the sorted list
            listInsertSorted(list,node);
        }
    }
    // Turn the list into the Huffman tree
    while(list->listSize > 1)
    {   // combine and remove first two nodes and insert back in the sorted list
        combineFirstTwo(list);
    }
    
    // Compression -----------------------------------------------------------

    // write size of filedata, then frequencies table

    // write size of original file to the compression output file as part of the header
    fwrite(&fileSize,sizeof(int),1,outptr1);
    // write the freqencies of each index (ASCII value) and find the codes for indices 
    // that have a frequency > 0 
    for (i=0;i<256;i++)
    {   
        fwrite(&frequencies[i],sizeof(int),1,outptr1);
        if (frequencies[i]>0)
        {
            findLeaf(list->head,0,&codes[i],i,&lengths[i]);
        }
    }

    
    unsigned char fileChar = 0;
    unsigned short int code;
    int codeLen, bitsLen;
    // for each character in the original file
    for (i = 0; i < fileSize; i++)
    {
        fileChar = fileData[i]; // character in file
        codeLen = lengths[fileChar]; // length of character code
        code = codes[fileChar]; // code for character
        // if there is enough room in buffer to write entire character code
        if(bufferIndex >= codeLen - 1)
        {   // shift character code to the left to start at the bufferIndex
            buffer |= code << ((bufferIndex - codeLen) + 1);
            bufferIndex -= codeLen; // update new bufferIndex
            // if the buffer is full, write, reset
            if(bufferIndex == -1)
            {  
                fwrite(&buffer,sizeof(unsigned char),1,outptr1); // write 8 bits at a time
                buffer = 0;
                bufferIndex = BUFFERSIZE;
            }
        }
        else
        {
            // find the number of bits of the code that will fit in the buffer
            bitsLen = bufferIndex + 1;
            // save the bits that fit in the buffer
            buffer |= code >> (codeLen - bitsLen);
            // write the full buffer
            fwrite(&buffer,sizeof(unsigned char),1,outptr1);
            // reset buffer
            buffer = 0;
            bufferIndex = BUFFERSIZE;
            // update codeLen with length of remaining code
            codeLen -= bitsLen;
            // if there is enough room now in buffer to write the rest of the character code
            if(bufferIndex >= codeLen - 1)
            {   // shift character code to the left to start at the beginning of the buffer
                buffer |= code << ((bufferIndex - codeLen) + 1);
                bufferIndex -= codeLen; // update buffer index
                // if the buffer is full, write, reset
                if(bufferIndex == -1)
                {
                    fwrite(&buffer,sizeof(unsigned char),1,outptr1); // write 16 bits at a time
                    buffer = 0;
                    bufferIndex = BUFFERSIZE;
                }
            }
            else
            {   // if remaining character code will not fit in buffer
                // find the number of bits of the code that will fit in the buffer
                bitsLen = bufferIndex + 1;
                // save the bits that fit in the buffer
                buffer |= code >> (codeLen - bitsLen);
                // write the full buffer
                fwrite(&buffer,sizeof(unsigned char),1,outptr1);
                // reset buffer
                buffer = 0;
                bufferIndex = BUFFERSIZE;
                // add remaining bits to the buffer
                buffer |= code << ((bufferIndex + 1) - (codeLen - bitsLen));
                bufferIndex -= codeLen - bitsLen; // update buffer index
            }
        }
    }
    // if there is anything in the buffer, write remaining bits
    if (bufferIndex != BUFFERSIZE) fwrite(&buffer,sizeof(unsigned char),1,outptr1);

    // close compressed output file and original input file
    fclose(outptr1);
    fclose(inptr1);

    // Decompression ---------------------------------------------------------

    // initialize variables for decompression
    int fileSize2, origFileSize, dataSize;
    int frequencies2[256];
    int byteCount = 0;
    unsigned char *fileData2;

    // create and initialize list for decompression
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

    // Getting the size in bytes of the compressed file
    fseek(inptr2, 0, SEEK_END);
    fileSize2 = ftell(inptr2);
    rewind(inptr2);

    // read header of compressed file
    // read the size of the original file
    fread(&origFileSize,sizeof(int),1,inptr2);

    for(i = 0; i < 256; i++)
    {   // read the frequencies for each character in the dictionary (indices)
        fread(&frequencies2[i],sizeof(int),1,inptr2);
    }

    // Now we will remake the tree as we did during compression
    for(i = 0; i < 256; i++)
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
    // Turn the list into the Huffman tree like before compression
    while(list2->listSize > 1)
    {
        combineFirstTwo(list2);
    }


    dataSize = (fileSize2 - FILE_HEADER_SIZE); // size of compressed data in file
    // calloc for compressed file data and read from file
    fileData2 = (unsigned char *)calloc(dataSize,sizeof(unsigned char));
    fread(fileData2,sizeof(unsigned char),dataSize,inptr2);
    unsigned char currentByte;
    int shiftIndex;
    listNode *rover;
    rover = list2->head;
    // for each byte in the compressed file
    for(i = 0; i < dataSize; i++)
    {   // for each bit that byte
        for(shiftIndex=BUFFERSIZE;shiftIndex>=0;shiftIndex--)
        {   // shift current byte to look at one bit at a time
            currentByte = fileData2[i];
            currentByte = currentByte >> shiftIndex;
            currentByte &= 0x01;
            // the path given from each character code is followed through the Huffman tree
            // to find the given character, each code is unique and the shortest valid code
            // found will be the next character

            // if current bit is a 1, the next node to get to the character is to the right
            if (currentByte == 0x01)
            {
                rover = rover->right;
            }
            else // the next node to get to the character is to the left
            {
                rover = rover->left;
            }
            // when the right and left nodes are null, a leaf node was found and it will have a symbol value
            if (rover->right == NULL && rover->right == NULL)
            {   // write value of symbol to the decompressed file
                fwrite(&rover->symbol,sizeof(unsigned char),1,outptr2);
                rover = list2->head; // set rover back to head to start code path over again
                byteCount++;  // increase number of bytes written
                if (byteCount==origFileSize)
                {   // only write the number of bytes that were in the original file
                    break;
                }  
            }
            
        }
        if (byteCount==origFileSize)
        {
            break;
        }

    }
    // free data structures and file data
    dataDestruct(list);
    dataDestruct(list2);
    free(fileData);
    free(fileData2);
    // close file pointers
    fclose(outptr2);
    fclose(inptr2);

    return 0;
}