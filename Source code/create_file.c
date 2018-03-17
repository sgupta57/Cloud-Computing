#include<stdio.h>
#include<stdlib.h>

main()
{
        FILE *fh;
        long int i;
        fh=fopen("file.txt","w+");
        for(i=0;i<5368709120;i++)
        {
                fprintf(fh,"%s","c");
        }
        fclose(fh);
}
