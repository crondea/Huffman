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
    int k;
    k = fwrite("A",sizeof(unsigned char),1,ofptr);
    k = fwrite("B",sizeof(unsigned char),1,ofptr);
    k = fwrite("C",sizeof(unsigned char),1,ofptr);
    // k = fwrite(" ",sizeof(unsigned char),1,ofptr);
    // k = fwrite("R",sizeof(unsigned char),1,ofptr);
    // k = fwrite("A",sizeof(unsigned char),1,ofptr);
    // k = fwrite("T",sizeof(unsigned char),1,ofptr);
    // k = fwrite(" ",sizeof(unsigned char),1,ofptr);
    // k = fwrite("S",sizeof(unsigned char),1,ofptr);
    // //k = fwrite("\0",sizeof(unsigned char),1,ofptr);
    // k = fwrite("A",sizeof(unsigned char),1,ofptr);
    // //k = fwrite("\0",sizeof(unsigned char),1,ofptr);
    // k = fwrite("T",sizeof(unsigned char),1,ofptr);
    // k = fwrite(" ",sizeof(unsigned char),1,ofptr);
    // k = fwrite("O",sizeof(unsigned char),1,ofptr);
    // k = fwrite("N",sizeof(unsigned char),1,ofptr);
    // k = fwrite(" ",sizeof(unsigned char),1,ofptr);
    // k = fwrite("A",sizeof(unsigned char),1,ofptr);
    // k = fwrite(" ",sizeof(unsigned char),1,ofptr);
    // k = fwrite("C",sizeof(unsigned char),1,ofptr);
    // k = fwrite("A",sizeof(unsigned char),1,ofptr);
    // k = fwrite("R",sizeof(unsigned char),1,ofptr);


    // int aCount = 5, xCount = 4, bCount = 3, lCount = 7, mCount = 2;
    // int i,k;

    // // Writing the As
    // k = 0;
    // for(i = 0; i < aCount; i++)
    // {
    //     k = fwrite("A",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Xs
    // k = 0;
    // for(i = 0; i < xCount; i++)
    // {
    //     k = fwrite("X",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Bs
    // k = 0;
    // for(i = 0; i < bCount; i++)
    // {
    //     k = fwrite("b",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Ls
    // k = 0;
    // for(i = 0; i < lCount; i++)
    // {
    //     k = fwrite("L",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Ms
    // k = 0;
    // for(i = 0; i < mCount; i++)
    // {
    //     k = fwrite("M",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }




    // int qCount = 5, wCount = 4, eCount = 3, rCount = 7, tCount = 2;

    // // Writing the As
    // k = 0;
    // for(i = 0; i < qCount; i++)
    // {
    //     k = fwrite("Q",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Xs
    // k = 0;
    // for(i = 0; i < wCount; i++)
    // {
    //     k = fwrite("W",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Bs
    // k = 0;
    // for(i = 0; i < eCount; i++)
    // {
    //     k = fwrite("E",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Ls
    // k = 0;
    // for(i = 0; i < rCount; i++)
    // {
    //     k = fwrite("R",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    // // Writing the Ms
    // k = 0;
    // for(i = 0; i < tCount; i++)
    // {
    //     k = fwrite("T",sizeof(unsigned char),1,ofptr);
    // }
    // if(k == 0)
    // {
    //     printf("fwrite failed!\n");
    // }

    fclose(ofptr);

    return 0;
}