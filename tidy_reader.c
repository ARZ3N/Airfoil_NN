#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

#define BUF_SIZE 1024


int main(){
    FILE *OGrdr, *NWwrtr;
    
    char *OGfilepath = "./coord_seligFmt/2032c.dat";
    char *NWfilepath = "./csv_coord_seligFmt/nw2032c.dat";
    
    
    if(OGrdr = fopen(OGfilepath, "r")){
        printf("|| input file opened successfully! \n");
        fflush(stdout);
    }
    else{
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    if(NWwrtr = fopen(NWfilepath, "w")){
        printf("|| output file opened succesfully! \n");
        fflush(stdout);
    }

    //- variable declarations:-------------------
    size_t n = 0;
    ssize_t nread;
    char *lineptr = NULL;
    int line = 1;
    //===========================================
    printf("all clear till hear: point 1\n");

    //- Read the first line from file: contains airfoil name
    nread = getline(&lineptr, &n, OGrdr);
    printf("first line: %s\n", lineptr);
    //======================================================
    
    //-Parse through the rest of the file: X, Y coordinates
    while(feof(OGrdr) == 0){
        nread = getline(&lineptr, &n, OGrdr);
        int it=0;
        while(lineptr[it] != ' '){
            it += 1;
        }
        char *chX = malloc(sizeof(char[it]));
        if(!chX){
            fprintf(stderr, "malloc error: can't allocate for var: 'chX' \n");
        }
        int jt = it;
        while(lineptr[jt] != '\r'){
            jt += 1;
        }
        char *chY = malloc(sizeof(char[jt - it - 1]));
        if(!chY){
            fprintf(stderr, "malloc error: can't allocate for var: 'chY' \n");
        }
        int index=0;
        for(index=0; index<it; index++){
            chX[index] = lineptr[index];
        }
        for(index=0; index<jt - it; index++){
            chY[index] = lineptr[index + it + 1];
            fflush(OGrdr);
        }
        
        printf("Line: %d> chX: %s \t ", line, chX);
        fflush(stdout);
        printf("chY: %s \n", chY);
        fprintf(NWwrtr, "%s,%s\n", chX, chY);      
        line += 1;

        
    }
    free(lineptr);
    fclose(OGrdr);
    fclose(NWwrtr);
    return 0;
}