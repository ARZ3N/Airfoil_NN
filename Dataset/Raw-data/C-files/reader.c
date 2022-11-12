#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){
    printf(" > cmd line args entered: %d \n", argc);
    printf(" > program file: %s \n", argv[0]);
    printf(" > Airfoil file selected: %s \n", argv[1]);

    char *file_name = (char*) malloc( sizeof(*(argv[1])) );
    file_name = argv[1];
    

    char OGfilepath[] = "./";
    //char NWfilepath[] = "../csv_coord_seligFmt/";

    char *file_path = (char*) malloc(sizeof(char)*(strlen(OGfilepath) + strlen(file_name)));
    //char *nwfile_path = (char*) malloc(sizeof(char)*(strlen(NWfilepath) + strlen(file_name) + 2));

    strcat(file_path, OGfilepath);
    strcat(file_path, file_name);
    /*
    strcat(nwfile_path, NWfilepath);
    strcat(nwfile_path, "nw");
    strcat(nwfile_path, file_name);
    */
    
    FILE *OGrdr;
    
    if(OGrdr = fopen(file_path, "r")){
        printf("|| input file opened successfully! \n");
        fflush(stdout);
    }
    else{
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    

    //- variable declarations:-------------------
    size_t n = 0;
    ssize_t nread;
    char *lineptr = NULL;
    int line = 1;
    //===========================================
    while(feof(OGrdr) == 0){
        nread = getline(&lineptr, &n, OGrdr);
        fflush(OGrdr);
        int it = 0;
        printf("Line: %d > ", line);
        for(it = 0; it < strlen(lineptr); ++it){
            printf(" %d", lineptr[it]);
        }
        printf("\n");
        line += 1;
    }

}