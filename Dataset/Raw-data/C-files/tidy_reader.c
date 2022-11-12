#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define BUF_SIZE 1024

int main(int argc, char* argv[]){
   
    //printf(" > cmd line args entered: %d \n", argc);
    //printf(" > program file: %s \n", argv[0]);
    printf(" > Airfoil file selected: %s \n", argv[1]);

    char *file_name = (char*) malloc( sizeof(*(argv[1])) );
    file_name = argv[1];
    

    char OGfilepath[] = "../";
    char NWfilepath[] = "../../ss_coord_Fmt/";
    

    char *file_path = (char*) malloc(sizeof(char)*(strlen(OGfilepath) + strlen(file_name)));
    char *nwfile_path = (char*) malloc(sizeof(char)*(strlen(NWfilepath) + strlen(file_name) + 2));
    strcat(file_path, OGfilepath);
    strcat(file_path, file_name);
    
    strcat(nwfile_path, NWfilepath);
    strcat(nwfile_path, "NW_");
    strcat(nwfile_path, file_name);

    
    FILE *OGrdr, *NWwrtr, *logger;
    
    if(OGrdr = fopen(file_path, "r")){
        //printf("|| input file opened successfully! \n");
        
    }
    else{
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    if(NWwrtr = fopen(nwfile_path, "w")){
        //printf("|| output file opened succesfully! \n");
        
    }

    //- variable declarations:-------------------
    size_t n = 0;
    ssize_t nread;
    char *lineptr = NULL;
    int line = 1;
    //===========================================
    //printf("all clear till here: point 1\n");

    //- Read the first line from file: contains airfoil name
    nread = getline(&lineptr, &n, OGrdr);
    //printf("first line: %s\n", lineptr);
    //======================================================
    
    //-Parse through the rest of the file: X, Y coordinates
    while(feof(OGrdr) == 0){
        printf(" all clear till here, point 2 \n");
        nread = getline(&lineptr, &n, OGrdr);
        //printf(" pass: %d \n", line);
        int index = 0, indent1 = 0, indent2 = 0, ch1_len = 0, ch2_len = 0;
        while(lineptr[index] == ' '){
            ++indent1;
            index += 1;
        }
        while(lineptr[index] != ' '){
            index += 1;
        }

        ch1_len = index - indent1;
        char *chX = malloc(sizeof(char)*ch1_len);
        if(!chX){
            fprintf(stderr, "malloc error: can't allocate for var: 'chX' \n");
        }

        printf(" all clear, point 3 \n");

        while(lineptr[index] == ' '){
            ++indent2;
            index += 1;
        }
        while(lineptr[index] != '\r'){
            index += 1;
        }

        int ch2_start = (indent1 + ch1_len + indent2);
        ch2_len = index - ch2_start;

        char *chY = malloc(sizeof(char)*ch2_len);

        if(!chY){
            fprintf(stderr, "malloc error: can't allocate for var: 'chY' \n");
            //printf("No chY !!! \n");
        }

        printf(" all clear, point 4 \n");

        int it = 0, jt = 0;

        printf(" indent1: %d, ch1_len: %d, indent2: %d, ch2_len: %d, index: %d \n", indent1, ch1_len, indent2, ch2_len, index);

        for(it = indent1, jt = 0; it < ch1_len + indent1; ++it, ++jt){
            chX[jt] = lineptr[it];
        }

        printf(" all clear, point 5 \n");

        for(it = ch2_start, jt = 0; it < ch2_len + ch2_start; ++it, ++jt){
            //printf(" %d", lineptr[it]);
            chY[jt] = lineptr[it];
        }
        
        //printf("\n");
        printf(" all clear, point 6");
        //printf(" chY: %s \n", chY);
        fprintf(NWwrtr, "%s %s\n", chX, chY);
        //printf(" line: %d > X: %s, Y: %s \n", line, chX, chY);      
        
        
        line += 1;
        free(chX);
        free(chY);

        
    }
    //printf(" >> Closed %s <<", file_name);
    //printf(">> debug probe ... \n");
    fclose(OGrdr);
    //printf(">> debug probe ... \n");
    fclose(NWwrtr);
    //printf(">> debug probe ... \n");
    free(lineptr);
    //printf(">> debug probe ... \n");
    //free(file_name);
    //printf(">> debug probe ... \n");
    free(file_path);
    //printf(">> debug probe ... \n");
    //free(nwfile_path);
    //printf(">> debug probe ... \n");

    if(!(logger = fopen("../coord_seligFmt/exit_success.txt", "a"))){
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(logger, "%s\n", argv[1]);
    fclose(logger);

    return 0;
}