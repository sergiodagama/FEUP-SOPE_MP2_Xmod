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
//status: to be solved
//
//________________________________________________________________________

#include "xmod_utils.c"

int main(int argc, char *argv[]){

    if(argc < 3)  //minimum of two arguments the mode, file_name and the xmod.out, wich is considered an argument
    { 
        printf("missing operands\n");
    }
    else{
        //the case where options are used
        if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0 || strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--changes") == 0){  
            //checks for octal input first, if it is, convert it to verbal
            if(octal_checker(argv[2])){
                verbal_permissions_changer_encapsulated(argv[3], argv[1], octal_to_verbal(argv[2]));
            }
            else{

                printf8
                verbal_permissions_changer_encapsulated(argv[3], argv[1], argv[2]);
            }
        }
        //recursive option 
        else if(strcmp(argv[1], "-R") == 0){
            //TODO
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

    return 0;
}
