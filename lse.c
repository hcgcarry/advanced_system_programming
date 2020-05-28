#include <unistd.h>  
#include <stdio.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <stdlib.h>
#include <sys/stat.h>  
#include <string.h>
#include <dirent.h>  
#include <time.h>  
  
void printDirInfo(struct stat statbuf,char *filename);
void printFileInfo(struct stat statbuf,char *filename);
static int getInfo(char *filename,int printContent) {  
    struct stat statbuf;  
    if (stat(filename, &statbuf) == -1) {  
        if(printContent){
            printf("Get stat on %s ",
                    filename );
        }
        return (-1);  
    }  
    if (S_ISDIR(statbuf.st_mode)){
        if(printContent)
            printDirInfo(statbuf,filename);
        return (1);  
    }
    if (S_ISREG(statbuf.st_mode)){
        if(printContent)
            printFileInfo(statbuf,filename);
        return (0);  
    }
}  
void printFileInfo(struct stat statbuf,char *filename){
    printf("file:%s  size:%ld bytes  UID:%d  GID:%d  modified at %s",  
        filename, statbuf.st_size, statbuf.st_uid,statbuf.st_gid,ctime(&statbuf.st_mtime));  
}
void printDirInfo(struct stat statbuf,char *dirName){
    DIR *dirp;  
    char buf[1000];  
    struct dirent *direntp;  

    int number_of_file = 0 ,number_of_subDirectory=0,size=0;
    

    if ((dirp = opendir(dirName)) == NULL) {  
        printf("error");
    }  
    
    while ((direntp = readdir(dirp)) != NULL){  
        sprintf(buf,"%s/%s",dirName,direntp->d_name);  
        struct stat statbuf;  
        if (stat(buf, &statbuf) == -1) {  
            printf("error");
        }  
        else if (S_ISDIR(statbuf.st_mode)){
            number_of_subDirectory++;
            size+=statbuf.st_size;
        }
        else if (S_ISREG(statbuf.st_mode)){
            number_of_file++;
            size+=statbuf.st_size;
        }
            //dir
        //if (getInfo(buf) == -1)break;  
    }  
    number_of_subDirectory-=2;
    
    printf("Dierectory:%s total space storage size of directory:%d bytes,number of file: %d\
    ,number of subdirectory:%d\n",dirName,size,number_of_file,number_of_subDirectory);
}
int programMan(){
        printf("使用說明：輸入 lse --help ,lse /? , lse 可以開啟使用說明\n");
        printf("lse {directoryName} 可以顯示出directoryName這個資料夾和下面的檔案的資訊\n");
}

  
int main(int argc, char **argv) {  
    DIR *dirp;  
    struct dirent *direntp;  
    int stats;  
    char buf[1000];  
    char* dirName=argv[1];

    if(argc ==1){
        programMan();
        return 0;
    }
    

    else if(!strcmp(argv[1],"/?")  || !strcmp(argv[1],"--help")){
        programMan();
        return 0;

    }
    else if ((stats = getInfo(argv[1],0) == 0) || (stats == -1))exit(1);  
    //is directory

    if ((dirp = opendir(argv[1])) == NULL) {  
        printf("error");
        exit(1);  
    }  
    while ((direntp = readdir(dirp)) != NULL){  
        //if (strcmp(direntp->d_name,".") ==0 || strcmp(direntp->d_name,"..")==0)continue;
        sprintf(buf,"%s/%s",argv[1],direntp->d_name);  
        if (getInfo(buf,1) == -1)break;  
    }  
    closedir(dirp);  
    exit(1);  
}  
