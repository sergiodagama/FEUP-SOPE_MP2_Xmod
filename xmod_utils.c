/**
 * @file xmod_utils.c
 * @author N0il
 * @brief Set of functions used to make xmod program, all functions have their respective tests on the last section of code commented
 * @version 0.21
 * @date 2021-03-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "xmod_utils.h"

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
 * @brief Resolves the user input mode argument
 *
 * @param mode_argument, the whole argument containing the user (u | g | o | a), action (+ | - | = ) and permissions (rwx)
 * @return the string in format (<user><action><permissions>), NULL in case of error
 */
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


/**
 * @brief Changes file permissions in octal mode
 *
 * @param file_name the file whose permissions are going to be changed
 * @param octals the octals of the permissions to be changed
 * @return true on success, false otherwise
 */
bool octal_permissions_changer(char file_name[], char octals[]){

    if(file_and_dir_checker(file_name)){

        int mode = strtol(octals, NULL, 8);

        if(chmod(file_name, mode) == 0) return true;
        else return false;
    }
    else{
        printf("cannot access '%s': No such file or directory\n", file_name);
        return false;
    }
}

/**
 * @brief Changes file permissions in octal mode and displays
 *
 * @param file_name the file whose permissions are going to be changed
 * @param octals the octals of the permissions to be changed
 * @param option the option, one of these: -v | --verbose | -c | --changes
 * @return true on success, false otherwise
 */
 bool octal_permissions_changer_with_display(char file_name[], char octals[], char option[]){
    char* old_file_permissions = malloc(sizeof(char) * 9 + 1);

    old_file_permissions = get_permissions(file_name);
     
    if(strcmp(option, "-c") == 0 || strcmp(option, "--changes") == 0){
        if(octal_permissions_changer(file_name, octals)){

            char* new_file_permissions = malloc(sizeof(char) * 9 + 1);

            new_file_permissions = get_permissions(file_name);

            if(!(strcmp(new_file_permissions, old_file_permissions) == 0)){ //only if the permissions have changed
                message_displayer(false, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), new_file_permissions, octal_to_verbal(new_file_permissions));
            }
        }
        else{
            return false;
        }
    }
    else if(strcmp(option, "-v") == 0 || strcmp(option, "--verbose") == 0){ 
         if(octal_permissions_changer(file_name, octals)){
             
            char* new_file_permissions = malloc(sizeof(char) * 9 + 1);

            new_file_permissions = get_permissions(file_name);

            if(strcmp(new_file_permissions, old_file_permissions) == 0){
                message_displayer(true, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), old_file_permissions, octal_to_verbal(old_file_permissions));
            }else{
                message_displayer(false, file_name, old_file_permissions, octal_to_verbal(old_file_permissions), new_file_permissions, octal_to_verbal(new_file_permissions));
            }
         }else{
             return false;
         }
    }
    else{
        printf("Not a valid option!\n");
        return false;
    }
 }

/**
 * @brief Changes file permissions in verbal mode
 *
 * @param file_name the file whose permissions are going to be changed
 * @param verbals the verbals of the permissions to be changed
 * @param action, one of this three: + | - | =
 * @param user_type, one of these three: u | g | o | a
 * @param option, one of these values: -c | -v | --verbose | --changes 
 * @return 0 if no erros, -1 otherwise
 */
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

    int res = 0;
    int display = 0;

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
                    perms_array[1] = 'r';
                }
                else{
                    perms_array[1] = '-';
                }
                if(one_perm_in("x", verbals, 0)){
                    perms_array[2] = 'r';
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
                    perms_array[4] = 'r';
                }
                else{
                    perms_array[4] = '-';
                }
                if(one_perm_in("x", verbals, 0)){
                    perms_array[5] = 'r';
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
                        perms_array[7] = 'r';
                    }
                    else{
                        perms_array[7] = '-';
                    }
                    if(one_perm_in("x", verbals, 0)){
                        perms_array[8] = 'r';
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
                printf("\nERROR: mode 'o'\n");
            }
        }
        else{
            printf("ERROR: user type not correct!\n");
            res = -1;
        }
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

    return res;
}

/**
 * @brief Encapsulates verbal_permissions_changer to treat all the preparations and only receive minimal input
 * 
 * @param file_name the directory or file name
 * @param option one of these values: "-v" | "--verbose" | "-c" | "--changes" | ""
 * @param mode the rough mode string from arguments that will be treated accordingly
 * @return returns 0 if no erros, else -1
 */
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
            return -1;
        }
    }
    else{
        printf("cannot access '%s': No such file or directory\n", file_name);
        return -1;
    }
}

//UNCOMMENT THE NEXT LINE AND THE RESPECTIVE CLOSER TO TEXT THE UTILS ONE BY ONE
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
