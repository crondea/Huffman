/*  Corey Rondeau
 *  ECE 4680 Lab 2
 *  File Created: Jan. 31 2018
 *  File Name: byteDump.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *ifptr = NULL;
    int i;
    int fSize;
    unsigned char byte;
    //unsigned char byteSize;

    if(argc != 2)
    {
        printf("Usage: byteDump [filename]");
        exit(-1);
    }
    else
    {
        if(!(ifptr = fopen(argv[1],"rb")))
        {
            printf("Error opening %s",argv[1]);
            exit(-1);
        }
    }

    fseek(ifptr, 0, SEEK_END);
    fSize = ftell(ifptr);
    rewind(ifptr);

    printf("File size: %d\n", fSize);
    for(i = 0; i < fSize; i++)
    {
        fread(&byte,sizeof(unsigned char),1,ifptr);
        printf("%X \n",byte);
    }

    return 0;
}