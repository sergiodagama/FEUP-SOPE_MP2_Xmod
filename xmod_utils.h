/**
 * @file xmod_utils.h
 * @author N0il
 * @brief Header file of xmod_utils.c
 * @version 0.1
 * @date 2021-03-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef XMOD_UTILS
#define XMOD_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>

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

char* mode_resolver(char mode_argument[]);

bool octal_permissions_changer(char file_name[], char octals[]);

int verbal_permissions_changer(char file_name[], char verbals[], char action[], char user_type[], char option[]);

int verbal_permissions_changer_encapsulated(char file_name[], char option[], char mode[]);

#endif //XMOD_UTILS
