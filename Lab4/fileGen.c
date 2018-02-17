/*  Corey Rondeau
 *  ECE 4680 Lab 2
 *  File Created: Jan. 31 2018
 *  File Name: fileGen.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char outputFilename[60];
    FILE *ofptr = NULL;

    printf("Please provide an output file name: ");
    scanf("%s",outputFilename);


    if(!(ofptr = fopen(outputFilename,"wb")))
    {
        printf("Error opening file %s",outputFilename);
        exit(-1);
    }

    int aCount = 5, xCount = 4, bCount = 3, lCount = 7, mCount = 2;
    int i,k;

    // Writing the As
    k = 0;
    for(i = 0; i < aCount; i++)
    {
        k = fwrite("A",sizeof(unsigned char),1,ofptr);
    }
    if(k == 0)
    {
        printf("fwrite failed!\n");
    }

    // Writing the Xs
    k = 0;
    for(i = 0; i < xCount; i++)
    {
        k = fwrite("X",sizeof(unsigned char),1,ofptr);
    }
    if(k == 0)
    {
        printf("fwrite failed!\n");
    }

    // Writing the Bs
    k = 0;
    for(i = 0; i < bCount; i++)
    {
        k = fwrite("b",sizeof(unsigned char),1,ofptr);
    }
    if(k == 0)
    {
        printf("fwrite failed!\n");
    }

    // Writing the Ls
    k = 0;
    for(i = 0; i < lCount; i++)
    {
        k = fwrite("L",sizeof(unsigned char),1,ofptr);
    }
    if(k == 0)
    {
        printf("fwrite failed!\n");
    }

    // Writing the Ms
    k = 0;
    for(i = 0; i < mCount; i++)
    {
        k = fwrite("M",sizeof(unsigned char),1,ofptr);
    }
    if(k == 0)
    {
        printf("fwrite failed!\n");
    }

    fclose(ofptr);

    return 0;
}