/**
 * @file xmod.c
 * @author N0il
 * @brief Program that replicates the unix command chmod, also with chmod system calls
 * @version 1.12
 * @date 2021-03-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//________________________________________________________________________
//
//                        ATTENTION BUG REPORTS:
//
//-> found bug in mode a+rwx when already has rwxrwxrwx it adds rwx------
//status: solved!
//
//-> found bug in commands of type xmod (-v/-c/etc) 777 file
//status: solved!
//________________________________________________________________________

//header files

#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include "xmod_utils.h"

//--------------GLOBAL VARIABLES IN ORDER TO HANDLE SIGNALS---------------

int current_pid;
char *current_file_name;
int nftot = 0; //number of total fils
int nfmod = 0; //number of files modified

//-----------------------END OF GLOBAL VARIABLES--------------------------

/**
 * @brief Function to handle the interrupt signal in xmod program
 * 
 * @param signal the signal to be handled
 */
void handle_sigint(int signal){
    printf("\n%d\t %s\t %d\t %d\t \n", current_pid, current_file_name, nftot, nfmod);
}

/**
 * @brief main xmod function
 * 
 * @param argc the number of arguments
 * @param argv the arguments
 * @return the number of files modified
 */
int xmod(int argc, char *argv[]){

    if(argc < 3)  //minimum of two arguments the mode, file_name and the xmod.out, wich is considered an argument
    { 
        printf("missing operands\n");
    }
    else{
        //the case where options are used
        if((strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--changes") == 0) && strcmp(argv[2], "-R") != 0){  
            //octal mode given, but with option
            if(octal_checker(argv[2])){
                return octal_permissions_changer_with_display(argv[3], argv[2], argv[1]);
            }
            else{
                return verbal_permissions_changer_encapsulated(argv[3], argv[1], argv[2]);
            }

            current_file_name = argv[3];  //assigning global variables to handle signals
        }
        //recursion
        else if(strcmp(argv[1],"-R") == 0 || strcmp(argv[2], "-R") == 0){ 
            //when user inputs "-R" option first
            if(strcmp(argv[1], "-R") == 0){
                //the case where options are used
                if(strcmp(argv[2], "-v") == 0 || strcmp(argv[2], "--verbose") == 0 || strcmp(argv[2], "-c") == 0 || strcmp(argv[2], "--changes") == 0){  
                    //octal mode given, but with option
                    if(octal_checker(argv[3])){
                        return octal_permissions_changer_with_display(argv[4], argv[3], argv[2]);
                    }
                    else{
                        return verbal_permissions_changer_encapsulated(argv[4], argv[2], argv[3]);
                    }

                    current_file_name = argv[4];  //assigning global variables to handle signals

                }
                //octal mode given, but with no option
                else if(octal_checker(argv[2])){
                    return octal_permissions_changer(argv[3], argv[2]);

                    current_file_name = argv[3];
                }
                //last case, verbal mode given, but without options
                else{ 
                    current_file_name = argv[3];

                    return verbal_permissions_changer_encapsulated(argv[3], "", argv[2]);
                }
            }
        }
        //octal mode given, but with no option
        else if(octal_checker(argv[1])){
            return octal_permissions_changer(argv[2], argv[1]);

            current_file_name = argv[2];
        }
        //last case, verbal mode given, but without options
        else{ 
            current_file_name = argv[2];

            return verbal_permissions_changer_encapsulated(argv[2], "", argv[1]);
        }
    }

    return 0;
}

/**
 * @brief fucntion to print all files and folders recursivly in a tree structure
 *
 * @param basePath the base path, or file, where the function is going to be called
 * @param root the value of lines to print
 */
void print_tree_structure(char *basePath, int root)
{
    char path[1000];
    struct dirent *dp;
    int st, i = 0;

    DIR *dir = opendir(basePath);

    //in case of error with opendir
    if (dir == NULL){
        return;
    }

    while ((dp = readdir(dir)) != NULL) //reads next element of directory stream
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            for(i = 0; i < root; i++){
                if(i % 2 == 0 || i == 0)
                    printf("%s", "║");
                else
                    printf(" ");
            }

            printf("%s%s%s\n", "╠", "═", dp->d_name);

            print_tree_structure(path, root + 2);
        }
    } 

    closedir(dir);            
}

/**
 * @brief encapsulator to recursive function in order to handle the basepath
 *
 * @param basePath the base path, or file, where the function is going to be called
 * @param root the value of lines to print
 */
void print_tree_structure_encapsulator(char *basePath, int root){

    printf("\nTree Structure Representation\n\n");

    printf("%s\n", basePath);

    print_tree_structure(basePath, root);
}

/**
 * @brief function that completes xmod recursive option
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param basePath the base path, or file, where the function is going to be called
 * @param file_position the position of the file name in the string array
 */
void xmod_recursion(int argc, char *argv[], char *basePath, int file_position)
{
    char path[1000];
    struct dirent *dp;
    int st;
    int i = 0, pid;

    DIR *dir = opendir(basePath);

    //in case of error with opendir
    if (dir == NULL){
        return;
    }

    while ((dp = readdir(dir)) != NULL) //reads next element of directory stream
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            sleep(1);

            nftot++; //updates number of files modified

            current_pid = getpid(); //updated current process id to handling signal

            current_file_name = basePath; //update current file name to handling signal

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if(file_position == 4) argv[4] = path;
            else if(file_position == 3) argv[3] = path;
            else printf("ERROR: file postion error\n");

            nfmod += xmod(argc, argv);  //updates number of files modified

            pid = fork();

            switch (pid){
                case -1:{
                    perror("ERROR: fork error\n");
                    exit(1);
                }
                case 0:{
                    xmod_recursion(argc, argv, path, file_position);
                    
                    exit(0);
    
                    break;
                }
                default:{
                    pid = wait(&st);

                    break;
                }
            }     
        }
    }

    closedir(dir);
}

/**
 * @brief xmod recursive encapsulator function in order to handle the basepath
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param basePath the base path, or file, where the function is going to be called
 * @param file_position the position of the file name in the string array
 */
void xmod_recursion_encapsulator(int argc, char *argv[], char *basePath, int file_position){
    //code for first path
    if(file_and_dir_checker(basePath)){
        nftot++;
        nfmod += xmod(argc, argv);
    }

    xmod_recursion(argc, argv, basePath, file_position);
}

/**
 * @brief main function of xmod program
 * 
 * @param argc number of arguments inputted
 * @param argv main arguments inputted
 * @return 0 if no erros, else otherwise
 */
int main(int argc, char *argv[]){

    //handle interrupt signal
    signal(SIGINT, handle_sigint);

    bool not_recursive = true;

    current_pid = getpid();

    if(argc > 2){ //to avoid segmentation error, when testing argv[1]
        //recursive option 
        if(strcmp(argv[1], "-R") == 0 || strcmp(argv[2], "-R") == 0){
            
            char *path;

            not_recursive = false;
            if(argc == 5){ //got to be change to number of args of the output from Rui

                path = argv[4]; //DEBUGGIND PURPOSE TO USE tree

                xmod_recursion_encapsulator(argc, argv, argv[4], 4);  //TODO check if it is directory on basepath given to recursive mode

                //DEBUGGING PURPOSE
                print_tree_structure_encapsulator(path, 2);
            }
            else if (argc == 4){
                path = argv[3]; //DEBUGGIND PURPOSE TO USE tree

                xmod_recursion_encapsulator(argc, argv, argv[3], 3);

                //DEBUGGING PURPOSE
                print_tree_structure_encapsulator(path, 2);
            }
        }
    }

    //all the other options
    if(not_recursive){
        nfmod += xmod(argc, argv);
    }

    //DEBUGGING PURPOSE
    printf("\nNFTOT: %d\n", nftot);
    printf("NFMOD: %d\n", nfmod);

    return 0;
}
