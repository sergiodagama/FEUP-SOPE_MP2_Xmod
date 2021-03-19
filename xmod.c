/**
 * @file xmod.c
 * @brief Program that replicates the unix command chmod, also with chmod system calls
 * @copyright Copyright (c) 2021
 * 
 */

//header files
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "xmod_utils.h"
#include "xmod_log.h"

//--------------------GLOBAL VARIABLES IN ORDER TO HANDLE SIGNALS---------------------

int current_pid;  //current process id
char *current_file_name;  //current file name being treated
int nftot; //number of total fils
int nfmod; //number of files modified
clock_t start_time, end_time; //running start time of program 
bool active_log_file = false; //states if the environment variable is active or not
char *env_var; //environment variable
extern char* old_perms_global;
extern char* new_perms_global;

//-----------------------------END OF GLOBAL VARIABLES--------------------------------

/**
 * @brief Function to handle the interrupt signal in xmod program
 * 
 * @param signal the signal to be handled
 */
void handle_sigint(int signal){
    if(active_log_file){
        end_time = clock();
        signal_recv((double) (end_time - start_time) / 1000, signal, current_pid, env_var);
    }

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
    int modified = 0;

    if(argc < 3)  //minimum of two arguments the mode, file_name and the xmod.out, wich is considered an argument
    { 
        printf("xmod: missing operand\n");
    }
    else{
        //the case where options are used
        if((strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--changes") == 0) && strcmp(argv[2], "-R") != 0){  
            current_file_name = argv[3];  //assigning global variables to handle signals

            //octal mode given, but with option
            modified = octal_permissions_changer_with_display(argv[3], argv[2], argv[1]);
        }
        //recursion
        else if(strcmp(argv[1],"-R") == 0 || strcmp(argv[2], "-R") == 0){ 
            //when user inputs "-R" option first
            if(strcmp(argv[1], "-R") == 0){
                //the case where options are used
                if(strcmp(argv[2], "-v") == 0 || strcmp(argv[2], "--verbose") == 0 || strcmp(argv[2], "-c") == 0 || strcmp(argv[2], "--changes") == 0){  
                    current_file_name = argv[4];  //assigning global variables to handle signals

                    //octal mode given, but with option
                   modified = octal_permissions_changer_with_display(argv[4], argv[3], argv[2]);
                }
                //octal mode given, but with no option
                else{
                    current_file_name = argv[3];
                    
                    modified = octal_permissions_changer_with_display(argv[3], argv[2], "-n");
                }
            }
        }
        //octal mode given, but with no options
        else{
            current_file_name = argv[2];

            modified = octal_permissions_changer_with_display(argv[2], argv[1], "-n");
        }
    }

    if(modified){
        if(active_log_file){
            end_time = clock();
            file_modf((double) (end_time - start_time) / 1000, old_perms_global, new_perms_global, env_var, current_pid, current_file_name);
        }
    }

    return modified;
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
    char *command;



    DIR *dir = opendir(basePath);

    //in case of error with opendir
    if (dir == NULL){
        return;
    }

    while ((dp = readdir(dir)) != NULL) //reads next element of directory stream
    {
        nftot++; //updates number of total files

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            current_pid = getpid(); //updated current process id to handling signal

            //current_file_name = basePath; //update current file name to handling signal

            if(dp->d_type == DT_DIR){ //if path is directory

                pid = fork();

                switch (pid){
                    case -1:{
                        perror("ERROR: fork error\n");
                        exit(1);
                    }
                    case 0:{
                        //creates child process 
                        if(active_log_file){
                            end_time = clock();
                            prog_create((double) (end_time - start_time) / 1000, current_pid, argv , argc, env_var);
                        }

                        strcpy(path, basePath);
                        strcat(path, "/");
                        strcat(path, dp->d_name);

                        if(file_position == 4) argv[4] = path;
                        else if(file_position == 3) argv[3] = path;
                        else printf("ERROR: file position error\n");

                        xmod(argc, argv);

                        //execvp("./xmod", argv);
                        xmod_recursion(argc, argv, path, file_position);

                        //ends child process
                        if(active_log_file){
                            end_time = clock();
                            send_proc_exit((double) (end_time - start_time) / 1000, 0, current_pid, env_var);
                        }
                        exit(0);
                        break;
                    }
                    default:{
                        //parent process
                        if(active_log_file){
                            end_time = clock();
                            prog_create((double) (end_time - start_time) / 1000, current_pid, argv , argc, env_var);
                        }
                        
                        pid = wait(&st);

                        if(active_log_file){
                            end_time = clock();
                            send_proc_exit((double) (end_time - start_time) / 1000, 0, current_pid, env_var);
                        }
                        break;
                    }
                }  
            }
            else if(dp->d_type == DT_REG){
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);

                if(file_position == 4) argv[4] = path;
                else if(file_position == 3) argv[3] = path;
                else printf("ERROR: file postion error\n");

                int n = xmod(argc, argv); 

                nfmod += n;  //updates number of files modified

                xmod_recursion(argc, argv, path, file_position);
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
    //first process
    if(active_log_file){
        end_time = clock();
        prog_create((double) (end_time - start_time) / 1000, current_pid, argv , argc, env_var);
    }
    
    //code for first path
    nftot++;
        
    int n = xmod(argc, argv);
    
    nfmod += n;
    
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

    //init clock
    start_time = clock();

    //checking environment variable
    env_var = getenv("LOG_FILENAME");

    if(check_envvar_set(env_var) != 1){
        //printf("xmod: no LOGFILENAME\n");
        active_log_file = true;
    }

    //create log file
    if(active_log_file){
        if(create_log_file(env_var) != 0){
            return 0;
        }
    }

    //handle interrupt signal
    signal(SIGINT, handle_sigint);

    nfmod = 0;
    nftot = 0;

    uint8_t flag = 0x00; //flag for handler
 
    if(handler(&flag, argv, &argc)){
        //printf("error on handler\n");
        return 0;
    }

    bool not_recursive = true; //to control wether it is a recursive call or not

    current_pid = getpid();

    if(argc > 2){ //to avoid segmentation error, when testing argv[1]
        //recursive option 
        if(strcmp(argv[1], "-R") == 0){
            //char *path; //DEBUGGING PURPOSE TO USE TREE

            not_recursive = false;
            
            //two options
            if(argc == 5){ 
                //path = argv[4]; //DEBUGGIND PURPOSE TO USE TREE

                xmod_recursion_encapsulator(argc, argv, argv[4], 4);  //TODO check if it is directory on basepath given to recursive mode

                //TREE STRUCTURE
                //print_tree_structure_encapsulator(path, 2);
            }
            //one option
            else if (argc == 4){
                //path = argv[3]; //DEBUGGIND PURPOSE TO USE TREE
            
                xmod_recursion_encapsulator(argc, argv, argv[3], 3);

                //TREE STRUCTURE
                //print_tree_structure_encapsulator(path, 2);
            }
        }
    }

    //all the other options, besides recursive one
    if(not_recursive){
        nfmod += xmod(argc, argv);
    }

    //DEBUGGING PURPOSE
    //printf("\nNFTOT: %d\n", nftot);
    //printf("NFMOD: %d\n", nfmod);

    //final process exit
    if(active_log_file){
        end_time = clock();
        send_proc_exit((double) (end_time - start_time) / 1000, 0, current_pid, env_var);
    }

    return 0;
}
