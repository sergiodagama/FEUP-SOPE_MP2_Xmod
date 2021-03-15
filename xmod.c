/**
 * @file xmod.c
 * @author N0il
 * @brief Program that replicates the unix command chmod, also with chmod system calls
 * @version 0.1
 * @date 2021-03-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */
 
#include "xmod_utils.c"

int main(int argc, char *argv[]){

    if(argc < 2)  //minimum of two arguments the mode and file_name
    { 
        printf("missing operands\n");
    }
    else{
        //the case where options are used
        if(strcmmp(argv[0], "-v") == 0 || strcmp(argv[0], "--verbose") == 0 || strcmp(argv[0], "-c") == 0 || strcmp(argv[0], "--changes") == 0){  
            //checks for octal input first, if it is, convert it to verbal
            if(octal_checker(argv[1])){
                verbal_permissions_changer_encapsulated(argv[2], argv[0], octal_to_verbal(argv[1]));
            }
            else{
                verbal_permissions_changer_encapsulated(argv[2], argv[0], argv[1]);
            }
        }
        //recursive option 
        else if(strcmp(argv[0], "-R") == 0){
            //TODO
        }
        //octal mode given, but with no option
        else if(octal_checker(argv[0])){
            octal_permissions_changer(argv[0]);
        }
        //last case, verbal mode given, but without options
        else{ 
            if(verbal_permissions_changer_encapsulated(argv[1], "", argv[0]) == -1)){
                printf("Invalid or missing operands!\n");
            }  
        }
    }

    return 0;
}
