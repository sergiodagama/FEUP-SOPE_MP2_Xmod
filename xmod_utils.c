/**
 * @file xmod_utils.c
 * @brief Set of functions used to make xmod program, all functions have their respective tests on the last section of code commented
 * @copyright Copyright (c) 2021
 * 
 */

#include "xmod_utils.h"
#include <limits.h>

/**
 * @brief Converts string with file all permissions in verbal mode to octal mode
 *
 * @param verbal the string in verbal mode
 *  return the string with permissions in octal mode
 */
char* verbal_to_octal(char verbal[]){  //array with nine characters representing file permissions (rwx)

    char *octals = malloc(sizeof(char) * 3 + 1);

    octals[4] = '\0';

    for(int i = 0; i < 3; i++){
        int j = 3 * i;

        if(verbal[j] == '-' && verbal[j + 1] == '-' && verbal[j + 2] == '-')
            octals[i] = '0';
        else if(verbal[j] == '-' && verbal[j + 1] == '-' && verbal[j + 2] == 'x')
            octals[i] = '1';
        else if(verbal[j] == '-' && verbal[j + 1] == 'w' && verbal[j + 2] == '-')
            octals[i] = '2';
        else if(verbal[j] == '-' && verbal[j + 1] == 'w' && verbal[j + 2] == 'x')
            octals[i] = '3';
        else if(verbal[j] == 'r' && verbal[j + 1] == '-' && verbal[j + 2] == '-')
            octals[i] = '4';
        else if(verbal[j] == 'r' && verbal[j + 1] == '-' && verbal[j + 2] == 'x')
            octals[i] = '5';
        else if(verbal[j] == 'r' && verbal[j + 1] == 'w' && verbal[j + 2] == '-')
            octals[i] = '6';
        else if(verbal[j] == 'r' && verbal[j + 1] == 'w' && verbal[j + 2] == 'x')
            octals[i] = '7';
        else
            printf("\nERROR: verbal_to_octal\n");
    }

    return octals;
}

/**
 * @brief Converts string with file all permissions in verbal mode to octal mode, but returns int value, instead of string
 *
 * @param verbal the string in verbal mode
 *  return integer with permissions in octal mode
 */
int verbal_to_octal_int(char verbal[]){
    
    char *octal_string = malloc(sizeof(char) * 3 + 1);

    octal_string = verbal_to_octal(verbal);

    int octal_int = strtol(octal_string, NULL, 8);

    return octal_int;
}

/**
 * @brief Converts string with file permissions of one user type in verbal mode to octal mode
 *
 * @param verbal the string in verbal mode
 *  return the string with permissions in octal mode
 */
char* verbal_to_octal_by_user(char verbal[]){

    char *octals = malloc(sizeof(char) * 1 + 1);

    octals[1] = '\0';

    if(verbal[0] == '-' && verbal[1] == '-' && verbal[2] == '-')
        octals[0] = '0';
    else if(verbal[0] == '-' && verbal[1] == '-' && verbal[2] == 'x')
        octals[0] = '1';
    else if(verbal[0] == '-' && verbal[1] == 'w' && verbal[2] == '-')
        octals[0] = '2';
    else if(verbal[0] == '-' && verbal[1] == 'w' && verbal[2] == 'x')
        octals[0] = '3';
    else if(verbal[0] == 'r' && verbal[1] == '-' && verbal[2] == '-')
        octals[0] = '4';
    else if(verbal[0] == 'r' && verbal[1] == '-' && verbal[2] == 'x')
        octals[0] = '5';
    else if(verbal[0] == 'r' && verbal[1] == 'w' && verbal[2] == '-')
        octals[0] = '6';
    else if(verbal[0] == 'r' && verbal[1] == 'w' && verbal[2] == 'x')
        octals[0] = '7';
    else{
        printf("\nERROR: verbal_to_octal_by_user!\n");
    }

    return octals;
}

/**
 * @brief Converts permissions string from octal mode to verbal mode
 *
 * @param octal_string the string with permissions to be converted
 * @return the string with permissions converted to verbal (rwx)
 */
char* octal_to_verbal(char octal_string[]){

    int octal_int = strtol(octal_string, NULL, 8);

    char *verbal = malloc(sizeof(char) * 9 + 1);

    verbal[0] = (octal_int & S_IRUSR) ? 'r' : '-';
    verbal[1] = (octal_int & S_IWUSR) ? 'w' : '-';
    verbal[2] = (octal_int & S_IXUSR) ? 'x' : '-';
    verbal[3] = (octal_int & S_IRGRP) ? 'r' : '-';
    verbal[4] = (octal_int & S_IWGRP) ? 'w' : '-';
    verbal[5] = (octal_int & S_IXGRP) ? 'x' : '-';
    verbal[6] = (octal_int & S_IROTH) ? 'r' : '-';
    verbal[7] = (octal_int & S_IWOTH) ? 'w' : '-';
    verbal[8] = (octal_int & S_IXOTH) ? 'x' : '-';
    verbal[9] = '\0';

    return verbal;
}

/**
 * @brief Displays user messages when using xmod program
 *
 * @param changed true if the permissions on the file where changed, false otherwise
 * @param file_name the file name
 * @param old_octal the permissions in octal mode, before permissions changed
 * @param old_verbal the permissions in verbal mode, before permissions changed
 * @param new_octal the permissions in octal mode, after permissions changed
 * @param new_verbal the permissions in verbal mode, after permissions changed
 */
void message_displayer(bool changed, char file_name[], char old_octal[], char old_verbal[], char new_octal[], char new_verbal[]){
    if(!changed){
        printf("mode of '%s' changed from 0%s (%s) to 0%s (%s)\n", file_name, old_octal, old_verbal, new_octal, new_verbal);
    }
    else{
        printf("mode of '%s' retained as 0%s (%s)\n", file_name, old_octal, old_verbal);
    }
}

/**
 * @brief Get the permissions from file and output in octal mode
 *
 * @param file_name the file to get the permissions from
 * @return string with verbal mode permissions, or error in case of error
 */
char* get_permissions(char *file_name){
    struct stat st;

    char *perms_octal = malloc(sizeof(char) * 3 + 1); //could be 5 or 4 ?

    if(stat(file_name, &st) == 0){
        mode_t permissions = st.st_mode;

        int mask = 0777;

        int perms = permissions & mask;

        snprintf(perms_octal, sizeof(perms_octal), "%o", perms);

        return perms_octal;
    }
    else{
        return strerror(errno);
    }
}

/**
 * @brief Get the permissions from file and output in octal mode
 *
 * @param file_name the file to get the permissions from
 * @return string with verbal mode permissions, or error in case of error
 */
char* get_permissions_with_zero(char *file_name){
    struct stat st;

    char *perms_octal = malloc(sizeof(char) * 4 + 1); //could be 5 or 4 ?

    if(stat(file_name, &st) == 0){
        mode_t permissions = st.st_mode;

        int mask = 0777;

        //printf("perms: %o\n", permissions);

        int perms = permissions & mask;

        snprintf(perms_octal, sizeof(perms_octal), "0%03o", perms);

        

        return perms_octal;
    }
    else{
        return strerror(errno);
    }
}

/**
 * @brief Accepts a string with all permissions and retrieves only the user ones
 *
 * @param permissions all the file permissions string
 * @param is_octal states if the permissions are in octal mode or verbal
 * @return string with user permissions
 */
char* get_user_permissions(char permissions[], bool is_octal){

    if(!is_octal){
        char *user_perms = malloc(sizeof(char) * 3 + 1);

        for(int i = 0; i < 3; i++){
            user_perms[i] = permissions[i];
        }

        user_perms[3] = '\0';

        return user_perms;
    }
    else
    {
        char *user_perms = malloc(sizeof(char) * 1 + 1);

        user_perms[0] = permissions[0];

        user_perms[1] = '\0';

        return user_perms;
    }
}

/**
 * @brief Accepts a string with all permissions and retrieves only the group ones
 *
 * @param permissions all the file permissions string
 * @param is_octal states if the permissions are in octal mode or verbal
 * @return string with group permissions
 */
char* get_group_permissions(char permissions[], bool is_octal){

    if(!is_octal){
        char *group_perms = malloc(sizeof(char) * 3 + 1);

        for(int i = 0; i < 3; i++){
            group_perms[i] = permissions[i + 3];
        }

        group_perms[3] = '\0';

        return group_perms;
    }
    else
    {
        char *group_perms = malloc(sizeof(char) * 1 + 1);

        group_perms[0] = permissions[1];

        group_perms[1] = '\0';

        return group_perms;
    }
}

/**
 * @brief Accepts a string with all permissions and retrieves only the other ones
 *
 * @param permissions all the file permissions string
 * @param is_octal states if the permissions are in octal mode or verbal
 * @return string with others permissions
 */
char* get_other_permissions(char permissions[], bool is_octal){
    if(!is_octal){
        char *other_perms = malloc(sizeof(char) * 3 + 1);

        for(int i = 0; i < 3; i++){
            other_perms[i] = permissions[i + 6];
        }

        other_perms[3] = '\0';

        return other_perms;
    }
    else
    {
        char *other_perms = malloc(sizeof(char) * 1 + 1);

        other_perms[0] = permissions[2];

        other_perms[1] = '\0';

        return other_perms;
    }
}

/**
 * @brief Checks if file permissions are the same as the ones passed, passing the whole permissions
 *
 * @param file_name the file to check the permissions with
 * @param perms the permissions to check with
 * @param is_octal states if the permissions passed are in octal or not
 * @return true if file permissions are the same as the arguments, false otherwise
 */
bool file_equal_permission_all(char* file_name, char perms[], bool is_octal){
    char *old_perms;

    old_perms = get_permissions(file_name);

    if(!is_octal){  //if not octal convert it to compare to output of get_permissions(), which is octal

        char *perms_octal;

        perms_octal = verbal_to_octal(perms);

        if(strcmp(old_perms, perms_octal) == 0) return true;
        else return false;
    }
    else
    {
        if(strcmp(old_perms, perms) == 0) return true;
        else return false;
    }
}

/**
 * @brief Checks if file permissions are the same as the ones passed, passing just permissions of user_type
 *
 * @param file_name the file to check the permissions with
 * @param perms the permissions to check with
 * @param is_octal states if the permissions passed are in octal or not
 * @param user_type the correspondant user of the permissions passed
 * @return true if file permissions are the same as the arguments, false otherwise
 */
bool file_equal_permission_by_user(char* file_name, char perms[], bool is_octal, char user_type[]){
    char *old_perms = malloc(sizeof(char) * 4 + 1);

    old_perms = get_permissions(file_name);

    char *old_perms_by_user = malloc(sizeof(char) * 1 + 1);

    char *octal_perms = perms;

    if(!is_octal){ //as get_permissions always returns an octal string, then we always have to convert perms to octal
        octal_perms = verbal_to_octal_by_user(perms); //TODO
    }

    if(strcmp(user_type, "u") == 0){
        old_perms_by_user = get_user_permissions(old_perms, true);

        if(strcmp(old_perms_by_user, octal_perms) == 0) return true;
        else return false;

    }
    else if(strcmp(user_type, "g") == 0){
        old_perms_by_user = get_group_permissions(old_perms, true);

        if(strcmp(old_perms_by_user, octal_perms) == 0) return true;
        else return false;
    }
    else if(strcmp(user_type, "o") == 0){
        old_perms_by_user = get_other_permissions(old_perms, true);

        if(strcmp(old_perms_by_user, octal_perms) == 0) return true;
        else return false;
    }
    else{
        return false;
    }


    if(!is_octal){  //if not octal convert it to compare to output of get_permissions(), which is octal

        char *perms_octal;

        perms_octal = verbal_to_octal(perms);

        if(strcmp(old_perms, perms_octal) == 0) return true;
        else return false;
    }
    else
    {
        if(strcmp(old_perms, perms) == 0) return true;
        else return false;
    }
}

/**
 * @brief Sees if a permission (solo one) is already in other permission
 *
 * @param perm, the permission to check
 * @param permission, the permission to check with
 * @return true if it has, false otherwise
 */
bool one_perm_in(char perm[], char permission[], int offset){
    int size = strlen(permission);

    char permi = perm[offset];

    for(int i = 0; i < size; i++){
        if(permi == permission[i]){
            return true;
        }
    }
    return false;
}

/**
 * @brief Sees if a permission (multiple ones) are already in other permission
 *
 * @param perms, the permissions to check
 * @param permission, the permission to check with
 * @return true if it has all perms, false otherwise
 */
bool multi_perm_in(char perms[], char permission[]){

    int n_perms = strlen(perms);


    for(int k = 0; k < n_perms; k++){
        if(perms[k] == 'r' || perms[k] == 'w' || perms[k] == 'x'){
            if(!one_perm_in(perms, permission, k)) return false;
        }
    }

    return true;
}

/**
 * @brief Sees if multiple permissions aren't all at once present in other permission
 *
 * @param perms, the permissions to check
 * @param permission, the permission to check with
 * @return true if it has all perms aren't present, false otherwise
 */
bool no_multi_perm_in(char perms[], char permission[]){

    int n_perms = strlen(perms);

    for(int k = 0; k < n_perms; k++){
        if(perms[k] == 'r' || perms[k] == 'w' || perms[k] == 'x'){
            if(one_perm_in(perms, permission, k)) return false;
        }
    }

    return true;
}

/**
 * @brief Checks if the given string is a file or a directory that exists
 *
 * @param file_name, the string representing the directory or the file to be checked
 * @return true if it is exists, false otherwise
 */
bool file_and_dir_checker(char file_name[]){

    bool file_exists = false;

    struct stat buffer;

    if(stat(file_name, &buffer) == 0) file_exists = true;

    return file_exists;
}

/**
 * @brief Checks if the given string is a valid octal
 *
 * @param octals, the octals to be checked
 * @return true if it is valid, false otherwise
 */
bool octal_checker(char octals[]){
    int size = strlen(octals);

    for(int i = 0; i < size; i++){
        if(!(octals[i] >= '0' && octals[i] <= '7')) return false;
    }
    return true;
}

/**
 * @brief Changes file permissions in octal mode
 *
 * @param file_name the file whose permissions are going to be changed
 * @param octals the octals of the permissions to be changed
 * @return 1 if no erros, 0 otherwise
 */
bool octal_permissions_changer(char file_name[], char octals[]){
    errno = 0;
    char *endptr = NULL; 

    long int mode = strtol(octals, &endptr, 8);

    /*
    //test return to number and errno values 
    if (octals == endptr)
        printf (" number : %ld  invalid  (no digits found, 0 returned)\n", mode);
    else if (errno == ERANGE && mode == LONG_MIN)
        printf (" number : %ld  invalid  (underflow occurred)\n", mode);
    else if (errno == ERANGE && mode == LONG_MAX)
        printf (" number : %ld  invalid  (overflow occurred)\n", mode);
    else if (errno == EINVAL)  // not in all c99 implementations - gcc OK 
        printf (" number : %ld  invalid  (base contains unsupported value)\n", mode);
    else if (errno != 0 && mode == 0)
        printf (" number : %ld  invalid  (unspecified error occurred)\n", mode);
    else if (errno == 0 && octals && !*endptr)
        printf (" number : %ld   valid  (and represents all characters read)\n", mode);
    else if (errno == 0 && octals && *endptr != 0)
        printf (" number : %ld    valid  (but additional characters remain)\n", mode);

    printf ("\n"); */

    if(chmod(file_name, mode) == 0) return 1;
    else return 0; 
}

/**
 * @brief Changes file permissions in octal mode and displays
 *
 * @param file_name the file whose permissions are going to be changed
 * @param octals the octals of the permissions to be changed
 * @param option the option, one of these: -v | --verbose | -c | --changes
 * @return 1 if modified file, 0 otherwise
 */
 bool octal_permissions_changer_with_display(char file_name[], char octals[], char option[]){
    
    char* old_file_permissions = malloc(sizeof(char) * 3 + 1);

    old_file_permissions = get_permissions(file_name);
     
    if(strcmp(option, "-c") == 0 || strcmp(option, "--changes") == 0){
        if(octal_permissions_changer(file_name, octals)){

            char* new_file_permissions = malloc(sizeof(char) * 3 + 1);

            new_file_permissions = get_permissions(file_name);

            if(!(strcmp(new_file_permissions, old_file_permissions) == 0)){ //only if the permissions have changed
                message_displayer(false, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), new_file_permissions, octal_to_verbal(new_file_permissions));

                free(new_file_permissions);
                free(old_file_permissions);
                return 1;
            }
        }
        else{
            return 0;
        }
    }
    else if(strcmp(option, "-v") == 0 || strcmp(option, "--verbose") == 0){ 
         if(octal_permissions_changer(file_name, octals)){
             
            char* new_file_permissions = malloc(sizeof(char) * 3 + 1);

            new_file_permissions = get_permissions(file_name);

            if(strcmp(new_file_permissions, old_file_permissions) == 0){
                message_displayer(true, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), old_file_permissions, octal_to_verbal(old_file_permissions));

                free(new_file_permissions);
                free(old_file_permissions);
                return 0;
            }else{
                message_displayer(false, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), new_file_permissions, octal_to_verbal(new_file_permissions));
                return 1;
            }
         }else{
             return 0;
         }
    }
    else if(strcmp(option, "-n") == 0){
        if(octal_permissions_changer(file_name, octals)){
            return 1;
        }
        else return 0;
    }
    else{
        printf("Not a valid option!\n");
        return 0;
    }
 }

/**
 * @brief perms_updater_handler helper function to handle the equal case
 * @param bit - the bit in the rwx permission that has to be changed (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param equal_flag - equal_flag variable from the perm_updater_handler function
 * @param user_perms - User permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param group_perms - Group permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param other_perms - Other permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 */
void equal_flag_updater(int bit, uint8_t equal_flag, uint8_t *user_perms, uint8_t *group_perms, uint8_t *other_perms){
    if( equal_flag & BIT(bit) ){
        *user_perms |= BIT(bit); *group_perms |= BIT(bit); *other_perms |= BIT(bit);
    }
    else{
        *user_perms &= BITC(bit); *group_perms &= BITC(bit); *other_perms &= BITC(bit);
    }
}

/**
 * @brief Updates the user_perms, group_perms and other_perms even the flags weren't enabled
 * @param user_perms - User permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param group_perms - Group permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param other_perms - Other permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3/4/5/6/7) -> always set to 0
 * @param amb - Mode in normal form ex.'ugo=rwx+r'
 * @return 0 upon success, 1 otherwise
 */
int perms_updater_handler(uint8_t *user_perms, uint8_t *group_perms , uint8_t *other_perms,const char *amb){
    const char *k = amb;
    uint8_t error_flag = 0x00; //To check if either = - + where enabled
    uint8_t equal_flag = 0x00;
    while ( *k != '\0'){
        switch (*k) {
            case '+':
                error_flag |= BIT(0);
                k++;
                while( true ) {
                    if( *k == '=' || *k == '-' || *k == '\0') break;
                    else if (*k == 'r') {
                        *user_perms |= BIT(2); *group_perms |= BIT(2); *other_perms |= BIT(2);
                    }
                    else if (*k == 'w') {
                        *user_perms |= BIT(1); *group_perms |= BIT(1); *other_perms |= BIT(1);
                    }
                    else if (*k == 'x') {
                        *user_perms |= BIT(0); *group_perms |= BIT(0); *other_perms |= BIT(0);
                    }
                    else{
                        return 1;
                    }
                    k++;
                }
                break;
            case '-':
                error_flag |= BIT(1);
                k++;
                while( true ){
                    if( *k == '=' || *k == '+' || *k == '\0') break;
                    else if (*k == 'r') {
                        *user_perms &= BITC(2); *group_perms &= BITC(2); *other_perms &= BITC(2);
                    }
                    else if (*k == 'w') {
                        *user_perms &= BITC(1); *group_perms &= BITC(1); *other_perms &= BITC(1);
                    }
                    else if (*k == 'x') {
                        *user_perms &= BITC(0); *group_perms &= BITC(0); *other_perms &= BITC(0);
                    }
                    else{
                        return 1;
                    }
                    k++;
                }
                break;
            case '=':
                error_flag |= BIT(2);
                k++;
                equal_flag = 0x00;
                while( true ) {
                    if( *k == '+' || *k == '-' || *k == '\0') {
                        equal_flag_updater(2,equal_flag,user_perms,group_perms,other_perms);
                        equal_flag_updater(1,equal_flag,user_perms,group_perms,other_perms);
                        equal_flag_updater(0,equal_flag,user_perms,group_perms,other_perms);
                        break;
                    }
                    if (*k == 'r') equal_flag |= BIT(2);
                    else if (*k == 'w') equal_flag |= BIT(1);
                    else if (*k == 'x') equal_flag |= BIT(0);
                    else{
                        return 1;
                    }
                    k++;
                }
                break;
            default:
                k++;
                break;
        }
    }
    if( !( (error_flag & BIT(0) ) || (error_flag & BIT(1)) || (error_flag & BIT(2)) ) )
    {
        printf("+-= not found\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Transform the char give by the get_permissions to a uint8_t
 * @param macro - number from 0 up to 7 ( 0 - --- 7 - rwx)
 * @param *flag - User/Group/Other (rwx) BIT(0) - x BIT(1) - w BIT(2) - r BIT(3) - Always set to zero
 * @return 0 upon success, 1 otherwise
 */
int file_perm_convert(char macro,uint8_t *flag){
    switch (macro) {
        case '0':
            *flag &= BITC(0); *flag &= BITC(1); *flag &= BITC(2);
            break;
        case '1':
            *flag |= BIT(0); *flag &= BITC(1); *flag &= BITC(2);
            break;
        case '2':
            *flag &= BITC(0); *flag |= BIT(1); *flag &= BITC(2);
            break;
        case '3':
            *flag |= BIT(0); *flag |= BIT(1); *flag &= BITC(2);
            break;
        case '4':
            *flag &= BITC(0); *flag &= BITC(1); *flag |= BIT(2);
            break;
        case '5':
            *flag |= BIT(0); *flag &= BITC(1); *flag |= BIT(2);
            break;
        case '7':
            *flag |= BIT(0); *flag |= BIT(1); *flag |= BIT(2);
            break;
        default:
            printf("Error! file_perm_convert()\n");
            return 1;
    }
    return 0;
}

/**
 * @brief uint8_t convert to char
 * @param result - octal mode result after uint_t to char[] conversion
 * @param perms - user_perms/group_perms/other_perms
 * @param place - User_octal [0] Group_octal[1] Other_octal[2]
 * @return 0 in case of sucess, 1 otherwise
 */
int octal_converter(char result[4], const uint8_t *perms, int place){
    switch (*perms) {
        case 0x00:
            result[place] = '0';
            break;
        case 0x01:
            result[place] = '1';
            break;
        case 0x02:
            result[place] = '2';
            break;
        case 0x03:
            result[place] = '3';
            break;
        case 0x04:
            result[place] = '4';
            break;
        case 0x05:
            result[place] = '5';
            break;
        case 0x06:
            result[place] = '6';
            break;
        case 0x07:
            result[place] = '7';
            break;
        default:
            printf("Bad perm given! Erro in octal_converter()\n");
            return 1;
    }
    return 0;
}



/**
 * @brief Function used to enable user group other all flags
 * @param amb - string with the mode
 * @param flag - BIT(0) -> user_flag BIT(1) -> group_flag BIT(2) -> other_flag
 * @return 0 in case of success, 1 otherwise
 */
int mode_treatment(char *amb, uint8_t *flag){
    for( char *t = amb ; *t != '\0'; t++){
        switch (*t) {
            case 'u':
                *flag |= BIT(0);
                break;
            case 'g':
                *flag |= BIT(1);
                break;
            case 'o':
                *flag |= BIT(2);
                break;
            case 'a':
                *flag |= BIT(0); *flag |= BIT(1); *flag |= BIT(2);
                break;
            default:
                break;
        }
    }
    return 0;

}

/**
 * @brief Function used to save the initial file permissions and to update them only if xmod activates user, group or other permissions
 * @param flag - Whose permission will change (BIT(0) -> user BIT(1) -> group BIT(2) -> other
 * @param user_perms - User permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3) -> always set to 0
 * @param group_perms - Group permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3) -> always set to 0
 * @param other_perms - Other users permissions (0x0000) BIT(0) -> execute BIT(1) -> write BIT(2) -> read BIT(3) -> always set to 0
 * @param mode - mode given by xmod
 * @return 0 in case of success, 1 otherwise
 */
int perms_updater( const uint8_t *flag, uint8_t *user_perms, uint8_t *group_perms, uint8_t *other_perms, char* mode ){
    uint8_t up_copy = *user_perms, gp_copy = *group_perms, op_copy = *other_perms;
    //printf("OP_COPY->%02x\n",op_copy);
    if( perms_updater_handler(user_perms,group_perms,other_perms,mode) != 0) return 1;
    if(!(*flag & BIT(0))) *user_perms = up_copy;
    if(!(*flag & BIT(1))) *group_perms = gp_copy;
    if(!(*flag & BIT(2))) *other_perms = op_copy;
    return 0;
}
/**
 * @brief Function used to modify argv[] and argc depending on the options given by xmod
 * @param flag - xmod OPTION flag (BIT(0) -> -v BIT(1) -> -c BIT(2) -> -R
 * @param argv - argv[]
 * @param argc - argc
 */
char * output_treatment(const uint8_t *flag, char **argv, int *argc, char *mode, char* file_path){
    if( *flag & BIT(2) ){
        argv[1] = "-R";
        if( *flag & BIT(1) ) {
            argv[2] = "-c"; argv[3] = mode; argv[4] = file_path;
            *argc = 5;
        }
        else if( *flag & BIT(0) ) {
            argv[2] = "-v"; argv[3] = mode; argv[4] = file_path;
            *argc = 5;
        }
        else {
            argv[2] = mode; argv[3] = file_path;
            *argc = 4;
        }
    }
    else if( (*flag & BIT(0)) || (*flag & BIT(1)) )
    {
        if( *flag & BIT(1) ) argv[1] = "-c";
        else if( *flag & BIT(0) ) argv[1] = "-v";
        argv[2] = mode;
        argv[3] = file_path;
        *argc = 4;
    }
    else {
        argv[1] = mode;
        argv[2] = file_path;
        *argc = 3;
    }
    return NULL;

}
/**
 * @brief Change file permissions, get the file initial permissions
 * @param var - File path index in argv
 * @param argv - argv[]
 * @param argc - argc
 * @param flag - xmod OPTION flag (BIT(0) -> -v BIT(1) -> -c BIT(2) -> -R
 * @param perms - File permissions
 * @param mode - Mode given by xmod
 * @return 0 in case of success and 1 otherwise
 */
int newfile_perms(int var, char** argv, int *argc, uint8_t *flag, char* perms, char* mode){

    if( file_and_dir_checker(argv[var]) ) perms = get_permissions_with_zero(argv[var]);
    else{
        printf("cannot access '%s': No such file or directory\n", argv[var]);  
        return 1;
    }

    old_perms_global = perms; //atuallizing old perms

    //printf("File_Perms->%s\n",perms);
    uint8_t user_perms = 0x00, group_perms = 0x00, other_perms = 0x00;
    if( perms[0] == '0') {
        if(file_perm_convert(perms[1], &user_perms) != 0) printf("Error setting user perms!");
        if(file_perm_convert(perms[2], &group_perms) != 0) printf("Error setting group perms!");
        if(file_perm_convert(perms[3], &other_perms) != 0) printf("Error setting other_perms!");
    }
    else{
        printf("Error in octal_mode!");
        return 1;
    }
    /*
    printf("UserPerms->%01x\n",user_perms);
    printf("GroupPerms->%01x\n",group_perms);
    printf("OtherPerms->%01x\n",other_perms);*/
    uint8_t ugo_flag = 0x00;
    if(mode != NULL) {
        mode_treatment(mode, &ugo_flag);
        //printf("Uflag = %d\nGflag = %d\nOFlag = %d\n", ugo_flag&BIT(0), ugo_flag &BIT(1), ugo_flag & BIT(2));
    }
    if(perms_updater(&ugo_flag,&user_perms,&group_perms,&other_perms,mode) != 0) return 1;
    /*
    printf("UserPerms_Updated->%01x\n",user_perms);
    printf("GroupPerms_Updated->%01x\n",group_perms);
    printf("OtherPerms_Updated->%01x\n",other_perms); */
    char test_var[4];
    octal_converter(test_var, &user_perms,0);
    octal_converter(test_var, &group_perms,1);
    octal_converter(test_var, &other_perms,2);
    test_var[3] = '\0';

    new_perms_global = test_var; //atuallizing new perms

    //printf("FinalPerms->%s\n",test_var);
    output_treatment(flag, argv, argc, (char *) test_var,argv[var]);

    return 0;
}

/**
 * @brief Function that uses get_opt() to check if xmod has any option enabled
 * @param flag - BIT(0)-> -v BIT(1) -> -c BIT(2) -> -R BIT(3/4/5/6/7) -> 0 by default
 * @param argv - argv[]
 * @param argc - argc[]
 * @return returns the rest of the xmod arguments that weren't preceded by '-'
 */
int getopt_flag_updater( uint8_t *flag, char **argv, int argc){
    int c;
    while ((c = getopt (argc, argv, ":vcR?:")) != -1) {
        switch (c) {
            case 'v':
                *flag |= BIT(0);
                if(*flag & BIT(1)) *flag &= BITC(1); //-c and -v compete, therefore the one choosen by unix is the last one
                break;
            case 'c':
                *flag |= BIT(1);
                if(*flag & BIT(0)) *flag &= BITC(0);
                break;
            case 'R':
                *flag |= BIT(2);
                break;
            case '?':
                fprintf(stderr, "Unknown option character `\\x%x'.\n",
                        optopt);
                abort();
            default:
                abort();
        }
    }
    return optind;
}

/**
 * @brief Handle xmod arguments
 * @param flag - BIT(0)-> -v BIT(1) -> -c BIT(2) -> -R BIT(3/4/5/6/7) -> 0 by default
 * @param argv - argv[]
 * @param argc - argc
 * @return 0 upon success, 1 otherwise
 */
int handler(uint8_t *flag, char *argv[], int *argc){

    //in case of not enough args
    if(*argc < 2) return 1;

    int index;
    char* mode = NULL;
    int opt_index = getopt_flag_updater(flag, argv, *argc);
    //printf("Verbose_flag = %d\nChange_flag = %d\nRecursive_flag = %d\n",*flag & BIT(0), *flag & BIT(1), *flag & BIT(2));
    bool mode_found = false, mode_octal = false;
    for (index = opt_index; index < *argc; index++) {
        if ( argv[index][0] != 'u' && argv[index][0] != 'g' && argv[index][0] != 'a' && argv[index][0] != 'o' && argv[index][0] != '0') continue;
        else{
            mode_found = true;
            mode = argv[index];
            if(octal_checker(mode))  {
                //printf("Octal mode found\n");
                mode_octal = true;
            }
            //printf("Mode->%s\n",mode);
            //printf("Treatment loop!\n");
        }
    }
    if(!mode_found) {
        printf("xmod: invalid mode: %s\n", mode);
        return 1;
    }
    int var = --index; //Get the last argument before the loop ended to get the file path
    char *perms = NULL;
    if( !mode_octal ) {
        //printf("Not octal\n");
        if( newfile_perms(var, argv, argc, flag, perms, mode) != 0 ) {
            printf("xmod: invalid mode: %s\n", mode);
            return 1;
        }
    }
    else{
        if( file_and_dir_checker(argv[var]) ) perms = get_permissions_with_zero(argv[var]);
        else{
            printf("xmod: cannot access '%s': No such file or directory\n", argv[var]);  
            return 1;
    }
        char *new_mode = mode + 1;
        output_treatment(flag, argv, argc, new_mode, argv[var]);
    }
    for( int i = 0; i < *argc; i++){
        //printf("ARGV[%d] = %s\n", i, argv[i]);
    }
    return 0;

}

//-----------------------------------------------------------DEPRECATED FUNCTIONS-----------------------------------------------------------------
//Initial tries of resolving the problems we had, that are now deprecated functions,that we decide to 
//mantain here in order to see the progress the program had through his diferent versions

/*
 * @brief Resolves the user input mode argument
 *
 * @param mode_argument, the whole argument containing the user (u | g | o | a), action (+ | - | = ) and permissions (rwx)
 * @return the string in format (<user><action><permissions>), NULL in case of error
 *

char* mode_resolver(char mode_argument[]){
    int size = strlen(mode_argument);

    //checks to see if it has any character not accepted
    for(int i = 0; i < size; i++){
        if(mode_argument[i] != 'u' && mode_argument[i] != 'g' && mode_argument[i] != 'o' && mode_argument[i] != 'a' && mode_argument[i] != '+' && mode_argument[i] != '-' && mode_argument[i] != '=' && mode_argument[i] != 'r' && mode_argument[i] != 'w' && mode_argument[i] != 'x'){
            return NULL;
        }
    }

    char *resolved = malloc(sizeof(char) * 11 + 1);

    for(int i = 0; i < 11; i++){
        resolved[i] = '-';
    }

    resolved[11] = '\0';

    int perms_first_indice = 0;

    //to get the last inserted user type and action
    for(int i = 0; i < size - 1; i++){
        if(mode_argument[i + 1] == '+' || mode_argument[i + 1] == '-' || mode_argument[i + 1] == '='){ //if none of this are inputted then it will be invalid, because resolve[0] wont have a valid value
            if(mode_argument[i] == 'u' || mode_argument[i] == 'g' || mode_argument[i] == 'o' || mode_argument[i] == 'a'){
                resolved[0] = mode_argument[i];
            }
        }
        if(mode_argument[i + 1] == 'r' || mode_argument[i + 1] == 'w' || mode_argument[i + 1] == 'x'){
            resolved[1] = mode_argument[i];
            perms_first_indice = i + 1;
            break;
        }
    }

    //to get unsorted perms into sorted (sorted is only useful in 'a' option and when there is multiple equal permissions
    for(int i = perms_first_indice; i < size; i++){
        if(mode_argument[i] == 'r'){
            if(resolved[2] != 'r'){
                resolved[2] = 'r';
                continue;
            }
            if(resolved[5] != 'r'){
                resolved[5] = 'r';
                continue;
            }
            if(resolved[8] != 'r'){
                resolved[8] = 'r';
            }
        }
        else if(mode_argument[i] == 'w'){
            if(resolved[3] != 'w'){
                resolved[3] = 'w';
                continue;
            }
            if(resolved[6] != 'w'){
                resolved[6] = 'w';
                continue;
            }
            if(resolved[9] != 'w'){
                resolved[9] = 'w';
            }
        }
        else if(mode_argument[i] == 'x'){
            if(resolved[4] != 'x'){
                resolved[4] = 'x';
                continue;
            }
            if(resolved[7] != 'x'){
                resolved[7] = 'x';
                continue;
            }
            if(resolved[10] != 'x'){
                resolved[10] = 'x';
            }
        }
    }

    //final check
    if(resolved[0] != 'u' && resolved[0] != 'g' && resolved[0] != 'o' && resolved[0] != 'a'){
            return NULL;
    }
    if(resolved[1] != '+' && resolved[1] != '-' && resolved[1] != '='){
            return NULL;
    }

    //cut the string in case of not being 'a' option (all)
    char *resolved_not_a = malloc(sizeof(char) * 5 + 1);

    resolved_not_a[5] = '\0';

    for(int i = 0; i < 5; i++){
        resolved_not_a[i] = resolved[i];
    }

    if(resolved[0] == 'a'){
        return resolved;
    }
    else{
        return resolved_not_a;
    }
}


/*
 * @brief Changes file permissions in verbal mode
 *
 * @param file_name the file whose permissions are going to be changed
 * @param verbals the verbals of the permissions to be changed
 * @param action, one of this three: + | - | =
 * @param user_type, one of these three: u | g | o | a
 * @param option, one of these values: -c | -v | --verbose | --changes 
 * @return 1 if modified file, 0 otherwise
 
int verbal_permissions_changer(char file_name[], char verbals[], char action[], char user_type[], char option[]){
    char *file_permissions = malloc(sizeof(char) * 3 + 1);

    file_permissions = get_permissions(file_name);

    char *file_permissions_verbal = malloc(sizeof(char) * 9 + 1);

    file_permissions_verbal = octal_to_verbal(file_permissions);

    char perms_array[10];

    for(int h = 0; h < 9; h++){
        perms_array[h] = file_permissions_verbal[h];
    }

    perms_array[9] = '\0';

    int res = 0;  //used to check erros along the function execution
    int display = 0;  //to control the messages that are displayed
    int modified = 0; //the value that is going to be return

    //mode 'a'
    if(strcmp(user_type, "a") == 0){ 
        if(strcmp(action, "+") == 0){
            if(one_perm_in("r", verbals, 0)){
                perms_array[0] = 'r';
                perms_array[3] = 'r';
                perms_array[6] = 'r';
            }
            if(one_perm_in("w", verbals, 0)){
                perms_array[1] = 'w';
                perms_array[4] = 'w';
                perms_array[7] = 'w';
            }
            if(one_perm_in("x", verbals, 0)){
                perms_array[2] = 'x';
                perms_array[5] = 'x';
                perms_array[8] = 'x';
            }
        }
        else if(strcmp(action, "-") == 0){
            if(one_perm_in("r", verbals, 0)){
                perms_array[0] = '-';
                perms_array[3] = '-';
                perms_array[6] = '-';
            }
            if(one_perm_in("w", verbals, 0)){
                perms_array[1] = '-';
                perms_array[4] = '-';
                perms_array[7] = '-';
            }
            if(one_perm_in("x", verbals, 0)){
                perms_array[2] = '-';
                perms_array[5] = '-';
                perms_array[8] = '-';
            }
        }
        else if(strcmp(action, "=") == 0){
            if(one_perm_in("r", verbals, 0)){
                perms_array[0] = 'r';
                perms_array[3] = 'r';
                perms_array[6] = 'r';
            }
            else{
                perms_array[0] = '-';
                perms_array[3] = '-';
                perms_array[6] = '-';
            }
            if(one_perm_in("w", verbals, 0)){
                perms_array[1] = 'w';
                perms_array[4] = 'w';
                perms_array[7] = 'w';
            }
            else{
                perms_array[1] = '-';
                perms_array[4] = '-';
                perms_array[7] = '-';  
            }
            if(one_perm_in("x", verbals, 0)){
                perms_array[2] = 'x';
                perms_array[5] = 'x';
                perms_array[8] = 'x';
            }
            else{
                perms_array[2] = '-';
                perms_array[5] = '-';
                perms_array[8] = '-'; 
            }
        }
        else{
            printf("ERROR: mode 'a'\n");
            res = -1;
        }

        if(res == 0){
            if(file_equal_permission_all(file_name, verbal_to_octal(perms_array), true)){
                display = 1;
            }
            else{
                octal_permissions_changer(file_name, verbal_to_octal(perms_array));
                display = 2;
            }
        }
    }
    else{
        //mode 'u'
        if(strcmp(user_type, "u") == 0){
            if(strcmp(action, "+") == 0){
                    
                if(multi_perm_in(verbals, get_user_permissions(file_permissions_verbal, false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_user_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }                    
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){

                char *initial_perms = malloc(sizeof(char) * 5 + 1);

                initial_perms = get_user_permissions(octal_to_verbal(file_permissions), false);

                if(one_perm_in("r", verbals, 0)){
                    perms_array[0] = 'r';
                }
                else{
                    perms_array[0] = '-';
                }
                if(one_perm_in("w", verbals, 0)){
                    perms_array[1] = 'w';
                }
                else{
                    perms_array[1] = '-';
                }
                if(one_perm_in("x", verbals, 0)){
                    perms_array[2] = 'x';
                }
                else{
                    perms_array[2] = '-';
                }

                if(strcmp(perms_array, file_permissions_verbal) == 0){
                    display = 1;
                }
                else{
                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else{
                printf("\nERROR: mode 'u'\n");
            }
        }
        //mode 'g'
        else if(strcmp(user_type, "g") == 0){
            if(strcmp(action, "+") == 0){
                if(multi_perm_in(verbals, get_group_permissions(octal_to_verbal(file_permissions), false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[3] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[4] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[5] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_group_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[3] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[4] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[5] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){
                char *initial_perms = get_group_permissions(octal_to_verbal(file_permissions), false);

                if(one_perm_in("r", verbals, 0)){
                    perms_array[3] = 'r';
                }
                else{
                    perms_array[3] = '-';
                }
                if(one_perm_in("w", verbals, 0)){
                    perms_array[4] = 'w';
                }
                else{
                    perms_array[4] = '-';
                }
                if(one_perm_in("x", verbals, 0)){
                    perms_array[5] = 'x';
                }
                else{
                    perms_array[5] = '-';
                }

                if(strcmp(perms_array, file_permissions_verbal) == 0){
                    display = 1;
                }
                else{
                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }else{
                printf("\nERROR: mode 'g'\n");
            }
        }
        //mode 'o'
        else if(strcmp(user_type, "o") == 0){
            if(strcmp(action, "+") == 0){
                if(multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[6] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[7] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[8] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[8] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){
                    char *initial_perms = get_other_permissions(octal_to_verbal(file_permissions), false);

                    if(one_perm_in("r", verbals, 0)){
                        perms_array[6] = 'r';
                    }
                    else{
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[7] = 'w';
                    }
                    else{
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[8] = 'x';
                    }
                    else{
                        perms_array[8] = '-';
                    }

                    if(strcmp(perms_array, file_permissions_verbal) == 0){
                        display = 1;
                    }
                    else{
                        res = chmod(file_name, verbal_to_octal_int(perms_array));

                        display = 2;
                    }
            }
            else{
                printf("\nERROR: mode 'uo'\n");
            }
        }
        //mode 'ug'
        else if(strcmp(user_type, "e") == 0){  
            if(strcmp(action, "+") == 0){
                if(multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = 'r';
                        perms_array[3] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = 'w';
                        perms_array[4] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = 'x';
                        perms_array[5] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = '-';
                        perms_array[3] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = '-';
                        perms_array[4] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = '-';
                        perms_array[5] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){
                    char *initial_perms = get_other_permissions(octal_to_verbal(file_permissions), false);

                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = 'r';
                        perms_array[3] = 'r';
                    }
                    else{
                        perms_array[0] = '-';
                        perms_array[3] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = 'w';
                        perms_array[4] = 'w';
                    }
                    else{
                        perms_array[1] = '-';
                        perms_array[4] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = 'x';
                        perms_array[5] = 'x';
                    }
                    else{
                        perms_array[2] = '-';
                        perms_array[5] = '-';
                    }

                    if(strcmp(perms_array, file_permissions_verbal) == 0){
                        display = 1;
                    }
                    else{
                        res = chmod(file_name, verbal_to_octal_int(perms_array));

                        display = 2;
                    }
            }
            else{
                printf("\nERROR: mode 'ug'\n");
            }
        }
        //mode 'uo'
        else if(strcmp(user_type, "s") == 0){
            if(strcmp(action, "+") == 0){
                if(multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = 'r';
                        perms_array[6] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = 'w';
                        perms_array[7] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = 'x';
                        perms_array[8] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = '-';
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = '-';
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = '-';
                        perms_array[8] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){
                    char *initial_perms = get_other_permissions(octal_to_verbal(file_permissions), false);

                    if(one_perm_in("r", verbals, 0)){
                        perms_array[0] = 'r';
                        perms_array[6] = 'r';
                    }
                    else{
                        perms_array[0] = '-';
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[1] = 'w';
                        perms_array[7] = 'w';
                    }
                    else{
                        perms_array[1] = '-';
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[2] = 'x';
                        perms_array[8] = 'x';
                    }
                    else{
                        perms_array[2] = '-';
                        perms_array[8] = '-';
                    }

                    if(strcmp(perms_array, file_permissions_verbal) == 0){
                        display = 1;
                    }
                    else{
                        res = chmod(file_name, verbal_to_octal_int(perms_array));

                        display = 2;
                    }
            }
            else{
                printf("\nERROR: mode 'uo'\n");
            }
        }
        //mode 'go'
        else if(strcmp(user_type, "d") == 0){
            if(strcmp(action, "+") == 0){
                if(multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){
                    display = 1; //it has already all the permissions
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[3] = 'r';
                        perms_array[6] = 'r';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[4] = 'w';
                        perms_array[7] = 'w';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[5] = 'x';
                        perms_array[8] = 'x';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "-") == 0){
                if(no_multi_perm_in(verbals, get_other_permissions(octal_to_verbal(file_permissions), false))){ //nothing to remove
                    display = 1;
                }
                else{
                    if(one_perm_in("r", verbals, 0)){
                        perms_array[3] = '-';
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[4] = '-';
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[5] = '-';
                        perms_array[8] = '-';
                    }

                    res = chmod(file_name, verbal_to_octal_int(perms_array));

                    display = 2;
                }
            }
            else if(strcmp(action, "=") == 0){
                    char *initial_perms = get_other_permissions(octal_to_verbal(file_permissions), false);

                    if(one_perm_in("r", verbals, 0)){
                        perms_array[3] = 'r';
                        perms_array[6] = 'r';
                    }
                    else{
                        perms_array[3] = '-';
                        perms_array[6] = '-';
                    }
                    if(one_perm_in("w", verbals, 0)){
                        perms_array[4] = 'w';
                        perms_array[7] = 'w';
                    }
                    else{
                        perms_array[4] = '-';
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[5] = 'x';
                        perms_array[8] = 'x';
                    }
                    else{
                        perms_array[5] = '-';
                        perms_array[8] = '-';
                    }

                    if(strcmp(perms_array, file_permissions_verbal) == 0){
                        display = 1;
                    }
                    else{
                        res = chmod(file_name, verbal_to_octal_int(perms_array));

                        display = 2;
                    }
            }
            else{
                printf("\nERROR: mode 'go'\n");
            }
        }
        else{
            printf("ERROR: user type not correct!\n");
            res = -1;
        }
    }

    //to control the returned value, as display is only set to two when the file is modified
    if(display == 2){
        modified = 1;
    }

    //options
    if(strcmp(option, "") == 0){
        display = 3;
    }
    else if(strcmp(option, "-c") == 0 || strcmp(option, "--changes") == 0){
        if(display == 1) display = 3; //only prints info when changes occur (when display equals to two)
    }
    else if(!(strcmp(option, "-v") == 0 || strcmp(option, "--verbose") == 0)){ //to only allow the options listed
        printf("ERROR: no valid option inputted!\n");
        display = 3;
        res = -1;
    }

    if(res == 0){
        switch(display){
            case 0:{
                printf("\nERROR: display wasn't set!\n");
                break;
            }
            case 1:{
                message_displayer(true, file_name, file_permissions, octal_to_verbal(file_permissions), file_permissions, octal_to_verbal(file_permissions));
                break;
            }
            case 2:{
                char* new_file_permissions = malloc(sizeof(char) * 9 + 1);

                new_file_permissions = get_permissions(file_name);

                message_displayer(false, file_name, file_permissions, octal_to_verbal(file_permissions), new_file_permissions, octal_to_verbal(new_file_permissions));
                break;
            }
        }
    }

    return modified;
}

/*
 * @brief Encapsulates verbal_permissions_changer to treat all the preparations and only receive minimal input
 * 
 * @param file_name the directory or file name
 * @param option one of these values: "-v" | "--verbose" | "-c" | "--changes" | ""
 * @param mode the rough mode string from arguments that will be treated accordingly
 * @return 1 if modified file, 0 otherwise
 
int verbal_permissions_changer_encapsulated(char file_name[], char option[], char mode[]){

    char *resolved_mode;

    //checks if file or directory exists or not
    if(file_and_dir_checker(file_name)){
        resolved_mode = mode_resolver(mode);

        if(resolved_mode != NULL){

            //needed strings to invoke verbal_permissions changer
            char *user_type = malloc(sizeof(char) * 1 + 1);

            user_type[0] = resolved_mode[0];
            user_type[1] = '\0';

            char *action = malloc(sizeof(char) * 1 + 1);

            action[0] = resolved_mode[1];
            action[1] = '\0';

            char *perms;

            if(user_type[0] == 'u' || user_type[0] == 'g' || user_type[0] == 'o'){  //in case of changing solo user permissions
                perms = malloc(sizeof(char) * 3 + 1);

                for(int i = 0; i < 3; i++){
                    perms[i] = resolved_mode[i + 2];
                }
            }
            else{  //in case of changing all permissions
                perms = malloc(sizeof(char) * 9 + 1);

                for(int i = 0; i < 9; i++){
                    perms[i] = resolved_mode[i + 2];
                }
            }

            //calls verbal permissions changer
            return verbal_permissions_changer(file_name, perms, action, user_type, option);
        }
        else{
            printf("Invalid mode given!\n");
            return 0;
        }
    }
    else{
        printf("cannot access '%s': No such file or directory\n", file_name);
        return 0;
    }
}

*/

//--------------------------------------------------------END DEPRECATED FUNCTIONS-----------------------------------------------------------------

//UNCOMMENT THE NEXT LINE AND THE RESPECTIVE CLOSER TO TEST THE UTILS ONE BY ONE
/*
int main()
{
	//test to verbal_to_octal converter
	char *octals;

    octals = verbal_to_octal("rwxrwx---");

    printf("OCTAL CONVERTER TEST: %s\n", octals);

    //test to octal_to_verbal converter

    char *verbal;

    verbal = octal_to_verbal("200");

	printf("VERBAL CONVERTER TEST: %s\n", verbal);

	//test to messsage_displayer

	message_displayer(false, "file.txt", "777", "rwxrwxrwx", "770", "rwxrwx---");

	//test to get_user_permissions, get_group_permissions and get_other_permissions

	char *perms;

	perms = get_user_permissions("rwx------", false);

	printf("\nTEST TO GET USER PERMS: %s\n", perms);

	perms = get_group_permissions("---r-x---", false);

	printf("TEST TO GET GROUP PERMS: %s\n", perms);

	perms = get_other_permissions("rwx----wx", false);

	printf("TEST TO GET OTHER PERMS: %s\n", perms);

	perms = get_other_permissions("763", true);

	printf("TEST TO GET PERMS OCTAL: %s\n", perms);

	//test to get_permissions done in linux [fully working]

	//test to one_perm_in

	printf("ONE PERMS IN TEST: %d\n", one_perm_in("r", "rwx", 0));

	printf("ONE PERMS IN TEST: %d\n", one_perm_in("w", "--x", 0));

	//test to multi_perm__in

	printf("MULTI PERM IN TEST: %d\n", multi_perm_in("rw", "rwx"));

	printf("MULTI PERM IN TEST: %d\n", multi_perm_in("wx", "--x"));

	//test to no_multi_perm_in

	printf("NO MULTI PERM TEST: %d\n", no_multi_perm_in("rw", "--x"));

	//test to mode resolver

	char *res = mode_resolver("oooou-++---=wx");

	printf("\n-------- TEST TO MODE RESOLVER ---------\n");

	if(res == NULL){
        printf("Not a valid mode!\n");
	}
	else{
        printf("MODE: %s\n\n", res);
	}

	//test verbal_to_octal_int

	printf("VERBAL TO OCTAL INT: %o\n\n", verbal_to_octal_int("rwxrwxrwx"));

    //test octal_permissions_changer

    char *file_name = "t.txt";

    printf("PREVIOUS PERMISSIONS: %s\n", get_permissions(file_name));

    //printf("OCTAL PERMISSION CHANGER: %s %d\n", file_name, octal_permissions_changer(file_name, "7"));

    //test verbal_permission_changer

    printf("VERBAL PERMISSION CHANGER: %d\n", verbal_permissions_changer("test", "rwx------", "+", "a", "--verbose"));
    
    //test to octal_checker

    printf("OCTAL CHECKER TEST: %d\n", octal_checker("9"));

    //test to file_and_dir_checker

    printf("FILE AND DIR CHECKER TEST: %d\n", file_and_dir_checker("t.txt"));

    //test to verbal_permissions_changer_encapsulated

    printf("VERBAL PERMISSIONS CHANGER ENCAPSULATED TEST: %d\n", verbal_permissions_changer_encapsulated("t.txt", "--verbose", "oou=-rrx"));

	return 0;
}

*/
