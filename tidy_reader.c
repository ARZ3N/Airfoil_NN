#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUF_SIZE 1024


int main(int argc, char* argv[]){
    
    printf(" > cmd line args entered: %d \n", argc);
    printf(" > program file: %s \n", argv[0]);
    printf(" > Airfoil file selected: %s \n", argv[1]);

    char *file_name = (char*) malloc( sizeof(*(argv[1])) );
    file_name = argv[1];
    

    char OGfilepath[] = "./coord_seligFmt/";
    char NWfilepath[] = "./csv_coord_seligFmt/";

    char *file_path = (char*) malloc(sizeof(char)*(strlen(OGfilepath) + strlen(file_name)));
    char *nwfile_path = (char*) malloc(sizeof(char)*(strlen(NWfilepath) + strlen(file_name) + 2));

    strcat(file_path, OGfilepath);
    strcat(file_path, file_name);
    
    strcat(nwfile_path, NWfilepath);
    strcat(nwfile_path, "nw");
    strcat(nwfile_path, file_name);

    
    FILE *OGrdr, *NWwrtr;
    
    if(OGrdr = fopen(file_path, "r")){
        printf("|| input file opened successfully! \n");
        fflush(stdout);
    }
    else{
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    if(NWwrtr = fopen(nwfile_path, "w")){
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
        fflush(stdout);
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