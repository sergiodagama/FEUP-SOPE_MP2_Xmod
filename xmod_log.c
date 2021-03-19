/**
 * @file xmod_log.c
 * @brief Contains all functions used to created and manage the log files done by xmod
 * @copyright Copyright (c) 2021
 * 
 */
#include "xmod_log.h"

/**
 * @brief Check the existence of LOG_FILENAME as an environment variable
 * @param str - Path indicated by environment variable LOG_FILENAME
 * @return 0 upon success, 1 otherwise
 */
int check_envvar_set(const char* str){
    if(str != NULL) return 0;
    return 1;
}

/**
 * @brief Close the file that was opened before with the int that was created by open()
 * @param file - created by open()
 * @return 0 upon success, 1 otherwise
 */
int close_file(int file){
    if( close(file) < 0 ){
        //printf("xmod: cannot access the log file: ");
        return 1;
    }
    return 0;
}

/**
 * @brief Create a file with the path indicated by LOG_FILENAME environmental variable
 * @param str - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int create_log_file(char* str){
    int file = open(str, O_TRUNC | O_CREAT | O_APPEND, S_IRWXU);
    if( file == -1 )
    {
        printf("xmod: cannot acess the log file\n");
        return 1;
    }
    if(close_file(file) != 0) return 1;
    return 0;
}

/**
 * @brief Open a file
 * @param str - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int open_file(char* str){
    int file = open(str, O_APPEND | O_WRONLY );
    if ( file == -1 ){
        printf("xmod: cannot access the log file: \n");
        return 1;
    }
    return file;
}

/**
 * @brief Output the PROG_CREAT event to the file
 * @param instant - time in ms
 * @param pid - process id
 * @param argv - argv[]
 * @param argc - argc
 * @param path - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int prog_create(double instant, int pid, char** argv , int argc, char* path){
    int file = open_file(path);
    if(file == 1) return 1;
    double time_elapsed = instant;
    char* output = malloc(sizeof(char) * 50);

    snprintf(output, 50, "%f", time_elapsed);
    write(file,output,strlen(output));

    write(file, " ; ",3);
    snprintf(output,50,"%d",pid);

    write(file,output,strlen(output));

    write(file," ; ",3);

    write(file,"PROC_CREAT",strlen("PROC_CREAT"));

    write(file, " ; ",3);
    for(int i = 1; i < argc;i++){
        write(file,argv[i],strlen(argv[i]));
        write(file, " ",1);
    }

    write(file,"\n",1);
    free(output);
    return close_file(file);
}

/**
 * @brief Output to the log file the PROG_EXIT event
 * @param instant - time in ms
 * @param exit_status - 1 if something went wrong during the program execution
 * @param pid - process id
 * @param path - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int send_proc_exit(double instant, int exit_status, int pid, char* path){
    int file = open_file(path);
    if(file == 1) return 1;
    char output[50];
    double time_elapsed = instant;
    snprintf(output, 50, "%f", time_elapsed);

    write(file,output,strlen(output));

    write(file, " ; ",3);

    snprintf(output,50,"%d", pid);

    write(file,output,strlen(output));

    write(file," ; ",3);

    write(file,"PROC_EXIT",strlen("PROC_EXIT"));

    write(file, " ; ",3);

    snprintf(output, 50, "%d", exit_status);

    write(file,output,strlen(output));

    write(file,"\n",1);
    return close_file(file);
}

/**
 * @brief Output to the log file the SIGNAL_RECV event
 * @param instant - time in ms
 * @param signal - signal that was received
 * @param pid - process id
 * @param path - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int signal_recv(double instant,int signal, int pid, char* path){
    int file = open_file(path);
    if(file == -1) return -1;
    char *str = "SIGINT";
    double time_elapsed = instant;
    char output[50];
    snprintf(output, 50, "%f", time_elapsed);
    write(file,output,strlen(output));
    write(file, " ; ",3);
    snprintf(output,50,"%d",pid);
    write(file,output,strlen(output));
    write(file, " ; ",3);
    write(file,"SIGNAL_RECV",strlen("SIGNAL_RECV"));
    write(file, " ; ",3);
    write(file,str,strlen(str));
    write(file,"\n",1);
    return close_file(file);
}

/**
 * @brief Output to the log file the FILE_MODF event
 * @param instant - time in ms
 * @param oldPerms - old file permissions
 * @param newPerms - new file permissions
 * @param path - the current process path
 * @param pid - process id
 * @param open_path - LOG_FILENAME value
 * @return 0 upon success, 1 otherwise
 */
int file_modf(double instant,char* oldPerms, char* newPerms,char* path, int pid, char* open_path){
    int file = open_file(open_path);
    if(file == 1) return 1;
    double time_elapsed = instant;
    char output[200];
    snprintf(output, 50, "%f", time_elapsed);
    write(file,output,strlen(output));
    write(file, " ; ",3);
    snprintf(output,50,"%d",pid);
    write(file,output,strlen(output));
    write(file, " ; ",3);
    write(file,"FILE_MODF",strlen("FILE_MODF"));
    write(file, " ; ",3);
    //char real_path[4096];
    write(file,path,strlen(path));
    snprintf(output,50," : %s : %s", oldPerms,newPerms);
    write(file,output,strlen(output));
    write(file,"\n",1);
    return close_file(file);
}
