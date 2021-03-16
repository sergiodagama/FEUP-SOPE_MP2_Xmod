/**
 * @file xmod.c
 * @author N0il
 * @brief Program that replicates the unix command chmod, also with chmod system calls
 * @version 0.12
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
#include "xmod_utils.h"

//--------------GLOBAL VARIABLES IN ORDER TO HANDLE SIGNALS---------------

int current_pid;
char *current_file_name;
int nftot = 0; // alter verbal output etc to return this values
int nfmod = 0; //etc etc

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
 */
void xmod(int argc, char *argv[]){

    if(argc < 3)  //minimum of two arguments the mode, file_name and the xmod.out, wich is considered an argument
    { 
        printf("missing operands\n");
    }
    else{
        //the case where options are used
        if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--changes") == 0){  
            //octal mode given, but with option
            if(octal_checker(argv[2])){
                octal_permissions_changer_with_display(argv[3], argv[2], argv[1]);
            }
            else{
                verbal_permissions_changer_encapsulated(argv[3], argv[1], argv[2]);
            }

            current_file_name = argv[3];  //assigning global variables to handle signals
        }
        //octal mode given, but with no option
        else if(octal_checker(argv[1])){
            octal_permissions_changer(argv[2], argv[1]);

            current_file_name = argv[2];
        }
        //last case, verbal mode given, but without options
        else{ 
            current_file_name = argv[2];

            if(verbal_permissions_changer_encapsulated(argv[2], "", argv[1]) == -1){
                printf("Invalid or missing operands!\n");
            }  
        }
    }
}

/**
 * @brief xmod recursive encapsulator function
 *
 * @param argc the number of arguments
 * @param argv the arguments
 * @param basePath the base path, or file, where the function is going to be called
 */
void xmod_recursion_encapsulator(int argc, char *argv[], char *basePath)
{
    char path[1000];
    struct dirent *dp;

    DIR *dir = opendir(basePath);

   
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if(argc == 3){
                argv[2] = basePath;
            }
            else if(argc == 4){
                argv[3] == basePath;
            }
            else{
                printf("ERROR: recursion error\n");
            }

            //xmod(argc, argv);

            xmod_recursion_encapsulator(argc, argv, path);
        }
    }

    closedir(dir);
}

/**
 * @brief main function of xmod program
 * 
 * @param argc number of arguments inputted
 * @param argv main arguments inputted
 * @return 0 if no erros, else otherwise
 */
int main(int argc, char *argv[]){

    bool not_recursive = true;

    current_pid = getpid();

    if(argc > 2){ //to avoid segmentation error, when testing argv[1]
        //recursive option 
        if(strcmp(argv[1], "-R") == 0){

            not_recursive = false;

            xmod_recursion_encapsulator(argc, argv, NULL);
        }
    }

    //all the other options
    if(not_recursive) xmod(argc, argv);

    //handle interrupt signal
    signal(SIGINT, handle_sigint);

    int n = 0;

    while(n < 10){
        sleep(1);
        n++;
    }

    return 0;
}
