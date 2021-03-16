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

#include "xmod_utils.h"
#include "xmod_signals.h"

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
        }
        //octal mode given, but with no option
        else if(octal_checker(argv[1])){
            octal_permissions_changer(argv[2], argv[1]);
        }
        //last case, verbal mode given, but without options
        else{ 
            if(verbal_permissions_changer_encapsulated(argv[2], "", argv[1]) == -1){
                printf("Invalid or missing operands!\n");
            }  
        }
    }
}

#include <unistd.h>

int main(int argc, char *argv[]){
    
    if(argc > 2){ //to avoid segmentation error, when testing argv[1]
        //recursive option 
        if(strcmp(argv[1], "-R") == 0){
            //TODO
        }
        //aall the other options
        else{
            xmod(argc, argv);
        }
    }

    //handle interrupt signal
    signal(SIGINT, handle_sigint);

    int n = 0;

    while(n < 10){
        sleep(2);
    }

    return 0;
}
