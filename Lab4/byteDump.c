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
    unsigned short int byte;
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
    fSize = (ftell(ifptr))/2;
    rewind(ifptr);

    printf("File size: %d\n", fSize);
    for(i = 0; i < fSize; i++)
    {
        fread(&byte,sizeof(unsigned short int),1,ifptr);
        printf("%d (%c)\n",byte,byte);
    }

    return 0;
}