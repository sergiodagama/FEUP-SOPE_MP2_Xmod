/**
 * @file xmod_utils.h
 * @brief Header file of xmod_utils.c
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef XMOD_UTILS
#define XMOD_UTILS

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>

#define BIT(n)  ( 1 << (n) ) //Bit toggle
#define BITC(n) ~(1 << (n) ) //Bit clear

char* old_perms_global;
char* new_perms_global;


char* verbal_to_octal(char verbal[]);

int verbal_to_octal_int(char verbal[]);

char* verbal_to_octal_by_user(char verbal[]);

char* octal_to_verbal(char octal_string[]);

void message_displayer(bool changed, char file_name[], char old_octal[], char old_verbal[], char new_octal[], char new_verbal[]);

char* get_permissions(char *file_name);

char* get_user_permissions(char permissions[], bool is_octal);

char* get_group_permissions(char permissions[], bool is_octal);

char* get_other_permissions(char permissions[], bool is_octal);

bool file_equal_permission_all(char* file_name, char perms[], bool is_octal);

bool file_equal_permission_by_user(char* file_name, char perms[], bool is_octal, char user_type[]);

bool one_perm_in(char perm[], char permission[], int offset);

bool multi_perm_in(char perms[], char permission[]);

bool no_multi_perm_in(char perms[], char permission[]);

bool file_and_dir_checker(char file_name[]);

bool octal_checker(char octals[]);

bool octal_permissions_changer(char file_name[], char octals[]);

bool octal_permissions_changer_with_display(char file_name[], char octals[], char option[]);

void equal_flag_updater(int bit, uint8_t equal_flag, uint8_t *user_perms, uint8_t *group_perms, uint8_t *other_perms);

int perms_updater_handler(uint8_t *user_perms, uint8_t *group_perms , uint8_t *other_perms,const char *amb);

int file_perm_convert(char macro,uint8_t *flag);

int octal_converter(char result[4], const uint8_t *perms, int place);

int mode_treatment(char *amb, uint8_t *flag);

int perms_updater( const uint8_t *flag, uint8_t *user_perms, uint8_t *group_perms, uint8_t *other_perms, char* mode );

char * output_treatment(const uint8_t *flag, char **argv, int *argc, char *mode, char* file_path);

int newfile_perms(int var, char** argv, int *argc, uint8_t *flag, char* perms, char* mode);

int getopt_flag_updater( uint8_t *flag, char **argv, int argc);

int handler(uint8_t *flag, char *argv[], int *argc);

char* get_permissions_with_zero(char *file_name);

/*

//DEPRECATED FUNCTIONS

int verbal_permissions_changer(char file_name[], char verbals[], char action[], char user_type[], char option[]);

int verbal_permissions_changer_encapsulated(char file_name[], char option[], char mode[]);

char* mode_resolver(char mode_argument[]);
*/

#endif //XMOD_UTILS
