// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include<stdio.h>
 #include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>


//
// Created by Marco Scilipoti on 04/09/2022.
//
//
// Created by Ippo on 04/09/2022.
//

int main(int argc, char * argv[]){



    
     if(argc < 2){
        printf("Usage: ./md5 <file1> <file2> ... <fileN>\n");
    }

    struct stat stats;
    for(int i = 1; i < argc; i++){
        if(stat(argv[i], &stats) != 0)
            printf("Error! : cannot access %s file\n", argv[i]);

        if(S_ISDIR(stats.st_mode))
            printf("Error! : %s is a directory\n", argv[i]);
    } 
   
}
