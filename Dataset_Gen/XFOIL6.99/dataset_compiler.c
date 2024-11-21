#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUF_SIZE 1024
#define RE 13000000
#define MACH 0.3

double power(double n, double e){
    double ans = 1;
    int j=1;
    for(j=1; j<=e; ++j){
        ans = ans*n;
    }
    return ans;
}
int main(int argc, char* argv[]){
    
    FILE *xdr, *c32r, *afr, *fail;
    size_t n = 0, dn = 0;
    ssize_t nread, dread;
    char *lineptr = NULL, *dataline = NULL;
    int line = 0;
    int AOA_FLAG = 0;
    int GREEN = 0;
    char FAIL_FL_PATH[] = "../failed_dcomp.txt";
   
    if(fail = fopen(FAIL_FL_PATH, "a")){
        printf("fail file opened successfully. \n");
    }
    else{
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    //printf(" > cmd line args entered: %d \n", argc);
    //printf(" > program file: %s \n", argv[0]);
    printf(" > Airfoil file selected: %s \n", argv[1]);
    printf(" > Extracting Line: %s", argv[2]);

    char *file_name = (char*) malloc( sizeof(*(argv[1])) );
    file_name = argv[1];
    /*
    *   xfoil datafile: "xdata_airfoilName"
    *   
    */
    int target_line = 0;
    
    int it = 0, us_index = 0, ext_index = 0;
    for(it = 0; it < strlen(argv[1]); ++it){
        if(argv[1][it] == '_'){
            //printf("underscore at : %d", it);
            us_index = it;
            break;
        }
    }
    for(it = us_index + 1; it < strlen(argv[1]); ++it){
        if(argv[1][it] == '.'){
            //printf("file ext starting from: %d", it);
            ext_index = it;
            break;
        }
    }

    char *af_name = (char*) malloc(sizeof(char) * (ext_index - us_index - 1));
    for(it = us_index + 1; it < ext_index; ++it){
        af_name[it - (us_index + 1)] = argv[1][it];
    }
    // xdata_ah88k136.txt
    // us_index = 5, ext_index = 14
    // airfoil name: 14 - 5 - 1 = 8
    printf("\n");
    printf(" af_name: %s \n", af_name);
    for(it=0; it<strlen(argv[2]); ++it){
        target_line += ((argv[2][it]-48) * power(10, strlen(argv[2]) - (it+1)));
    }
    printf(" > target_line : %d \n", target_line);

    char CRD32_FL_PATH[] = "../../Dataset/Stage_2/N32/";
    char XFDAT_FL_PATH[] = "./";
    char AF_PATH[] = "../af_samples/";

    char *c32_name = (char*) malloc(sizeof(char)*(ext_index - us_index - 1) + 4*sizeof(char) + 4*sizeof(char) + strlen(CRD32_FL_PATH)*sizeof(char));
    //char *xdat_name = (char*) malloc(sizeof(char)*(ext_index - us_index - 1) + 5*sizeof(char) + 4*sizeof(char) + strlen(XFDAT_FL_PATH)*sizeof(char));
    char *xdat_name = (char*) malloc(strlen(argv[1])*sizeof(char) + strlen(XFDAT_FL_PATH)*sizeof(char));
    char *fname = (char*) malloc(sizeof(char)*(ext_index - us_index - 1) + strlen(AF_PATH)*sizeof(char) + 3*sizeof(char) + 4*sizeof(char));

    printf("af_name len: %d \n", ext_index - us_index - 1);
    strcat(c32_name, CRD32_FL_PATH);
    strcat(c32_name, "n32_");
    strcat(c32_name, af_name);
    strcat(c32_name, ".txt");
    
    strcat(xdat_name, XFDAT_FL_PATH);
    strcat(xdat_name, argv[1]);
    /*
    strcat(fname, AF_PATH);
    strcat(fname, "af_");
    strcat(fname, af_name);
    strncat(fname, ".txt", 4);
    */
    strcat(fname, AF_PATH);
    strcat(fname, "dataset.csv");
    //strcat(fname, "SUPERMARINE371i.csv");

    /*
    for(it=0; it<strlen(fname); ++it){
        printf("%d ", fname[it]);
    }
    printf("\n");
    */
    /*
    for(it=0; it<strlen(fname); ++it){
        printf(" %d", fname[it]);
    }
    */
    printf("\n");
    //printf(" fname len and size : %ld \n", strlen(fname)*sizeof(char));
    printf("\n");
    //printf(" > c32: %s \n", c32_name);
    //printf(" > xdat: %s \n", xdat_name);
    //printf(" > fname: %s \n", fname);

    if(c32r = fopen(c32_name, "r")){
        printf(" >> %s coord file opened successfully! \n", c32_name);
        
    }
    else{
        printf(" > %s coord file failed to open! < \n", c32_name);
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    if(xdr = fopen(xdat_name, "r")){
        printf(" >> %s data file opened successfully! \n", xdat_name);
        
    }
    else{
        printf(" > %s data file failed to open! < \n", xdat_name);
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    if(afr = fopen(fname, "a")){
        printf(" >> %s+ airfoil file opened successfully! \n", fname);
        
    }
    else{
        printf(" > %s+ airfoil file failed to open! < \n", fname);
        fprintf(fail, "%s ", af_name);
        fprintf(fail ,"\n");
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    for(it=1; it<=12; ++it){
        dread = getline(&dataline, &dn, xdr);
    }
    //printf(" xdr line 1: %s \n", dataline);

    // =========== XDATA Cl EXTRACTION BEGINS ===========================
    signed int AoA = (target_line - 14);
    //printf(" AoA: %d \n", AoA);
    int ele=0, pad=1;
    signed int sign = 1;
    int A = 0, CL = 0;
    char *arr, *cl;
    while(feof(xdr) == 0){
        dread = getline(&dataline, &dn, xdr);
        A = 0;
        sign = 1;
        ele = 0;
        it = 0;
        while(dataline[ele]!='.'){
            ++ele;
        }
        int dot=ele;
        arr = (char*)malloc(sizeof(char)*dot);
        for(it=0; it<dot; ++it){
            if(dataline[it] == ' '){
                A += 0;
            }
            else if(dataline[it] == '-'){
                sign = -1;
            }
            else{
                A += (dataline[it] - 48) * power(10, dot-it-1);
            }
        }
        A = A * sign;
 
        //printf(" A : %d :: ", A);
        //printf(" AoA: %d \n", AoA);
        //printf("line: %s", dataline);
        if( A == AoA){
            AOA_FLAG = 1;
            it = 0;
            while(dataline[it] == ' '){
                ++it;
            }
            while(dataline[it] != ' '){
                ++it;
            }
            while(dataline[it] == ' '){
                ++it;
            }
            ele = it;
            while(dataline[it] != ' '){
                ++it;
            }
            //printf(" it - ele: %d \n", it-ele);
            cl = (char*)malloc(sizeof(char)*(it - ele));
            for(it = ele; dataline[it] != ' '; ++it){
                cl[it - ele] = dataline[it];
                //printf(" > dl: %c", dataline[it]);
            }

          
            break;
        }
        else{
            AOA_FLAG = 0;
        }
    }
    if(AOA_FLAG == 1){
        printf(" > AoA Data Available for Extraction.! \n");
        printf(" > AoA = %d \n", A);
        //printf(" > cl= %s :: %s", cl, dataline);

    }
    else{
        printf(" > AoA Data Not Available for current value ! \n");
        printf(" > AoA = %d \n", A);
    }
    // ============= XDATA Cl Extraction Stuff Ends ==================

    // ============= coord32 stuff :: ================================
    nread = getline(&lineptr, &n, c32r);
    int c32_line_len = strlen(lineptr);
    char *crdArr[32];
    for(it=0; it<32; ++it){
        crdArr[it] = (char*)malloc(sizeof(char)*c32_line_len);
    }
    for(it=0; it<strlen(lineptr); ++it){
        crdArr[0][it] = lineptr[it];
        
    }
    //printf("first coord line: %s", crdArr[0]);
    for(it=0;it<strlen(crdArr[0]);++it){
        //printf(" %d", crdArr[0][it]);
    }
    //printf("\n");
    it=1;
    int index;
    while(feof(c32r) == 0){
        if(it>31){
            break;
        }
        
        nread = getline(&lineptr, &n, c32r);
        for(index=0; index < strlen(lineptr); ++index){
            //printf(" %d", index);
            crdArr[it][index] = lineptr[index];
        }
        
        //printf("\n");
        //printf(" pass %d : %s \n", it, crdArr[it]);
        ++it;
    }
    // ================== coord 32 stuff ends :: ===============================
    // ================== COORDINATE EXTRACTION BEGINS =========================
    int spaces[32];
    for(it=0; it<32; ++it){
        spaces[it] = 0;
    }
    for(it=0; it<32; ++it){
        int j = 0;
        for(j=0; j<strlen(crdArr[it]); ++j){
            if(crdArr[it][j] == ' ' || crdArr[it][j] == '\n' || crdArr[it][j] == '\t' || crdArr[it][j] == '\r'){
                spaces[it] += 1;
            }
        }
        //printf(" spaces[%d]= %d \n", it, spaces[it]);
    }
    int xpos, xlen, ypos, ylen;
    char *X[32], *Y[32];
    for(it=0; it<32; ++it){
        int j =0;
        xpos = 0; ypos = 0;
        xlen = 0; ylen = 0;
        while(crdArr[it][j] == ' '){
            ++j;
        }
        xpos = j;
        while(crdArr[it][j] != ' '){
            ++j;
        }
        xlen = j - xpos;
        while(crdArr[it][j] == ' '){
            ++j;
        }
        ypos = j;
        while(crdArr[it][j] != ' ' && crdArr[it][j] != '\r' && crdArr[it][j] != '\n'){
            ++j;
        }
        ylen = j - ypos;
        //printf("xpos: %d, xlen: %d, ypos: %d, ylen: %d \n", xpos, xlen, ypos, ylen);
        X[it] = (char*)malloc(sizeof(char)*xlen);
        Y[it] = (char*)malloc(sizeof(char)*ylen);
        for(j=xpos; j<xlen+xpos; ++j){
            X[it][j - xpos] = crdArr[it][j];
        }
        for(j=ypos; j<ylen+ypos; ++j){
            Y[it][j - ypos] = crdArr[it][j];
        }
        //printf(" > crdArr[%d] : %s", it, crdArr[it]);
        //printf(" > X[%d]: %s  Y[%d]: %s \n", it, X[it], it, Y[it]);
        //printf("\n");
    }
    // ======================= Coordinate Extraction Ends =======================================
    GREEN = 1;
    for(it=0; it<strlen(cl); ++it){
        if(cl[it] == 11){
            GREEN = 0;
        }
    }

    if(GREEN == 1){
        for(it = 0; it < 32; ++it){
            if(it == 31){
                fprintf(afr, "%s,%s,%s,%s,%s,", X[it], Y[it], X[0], Y[0], argv[3]);    
            }
            else{
                fprintf(afr, "%s,%s,%s,%s,%s,", X[it], Y[it], X[it+1], Y[it+1], argv[3]);
            }
        }
        fprintf(afr, "13000000,0.3,%s \n", cl);
    }
    else{
        exit(EXIT_SUCCESS);
    }
    
    fclose(c32r);
    fclose(xdr);
    fclose(afr);
    fclose(fail);
    free(af_name);
    free(fname);
    free(arr);
    free(cl);
    return 0;
}