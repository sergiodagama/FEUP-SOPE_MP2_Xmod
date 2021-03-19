#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>


#define BIT(n)  ( 1 << (n) ) //Bit toggle
#define BITC(n) ~(1 << (n) ) //Bit clear

bool octal_checker(char octals[]){
    int size = strlen(octals);

    for(int i = 0; i < size; i++){
        if(!(octals[i] >= '0' && octals[i] <= '7')) return false;
    }
    return true;
}

bool file_and_dir_checker(char file_name[]){

    bool file_exists = false;

    struct stat buffer;

    if(stat(file_name, &buffer) == 0) file_exists = true;

    return file_exists;
}

char* get_permissions(char *file_name){
    struct stat st;

    char *perms_octal = malloc(sizeof(char) * 3 + 1); //could be 5 or 4 ?

    if(stat(file_name, &st) == 0){
        mode_t permissions = st.st_mode;

        int mask = 0777;

        int perms = permissions & mask;

        snprintf(perms_octal, sizeof(perms_octal), "0%o", perms);

        return perms_octal;
    }
    else{
        return NULL;
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
    printf("OP_COPY->%02x\n",op_copy);
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
    if( file_and_dir_checker(argv[var]) ) perms = get_permissions(argv[var]);
    else return 1;
    printf("File_Perms->%s\n",perms);
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
    printf("UserPerms->%01x\n",user_perms);
    printf("GroupPerms->%01x\n",group_perms);
    printf("OtherPerms->%01x\n",other_perms);
    uint8_t ugo_flag = 0x00;
    if(mode != NULL) {
        mode_treatment(mode, &ugo_flag);
        printf("Uflag = %d\nGflag = %d\nOFlag = %d\n", ugo_flag&BIT(0), ugo_flag &BIT(1), ugo_flag & BIT(2));
    }
    if(perms_updater(&ugo_flag,&user_perms,&group_perms,&other_perms,mode) != 0) return 1;
    printf("UserPerms_Updated->%01x\n",user_perms);
    printf("GroupPerms_Updated->%01x\n",group_perms);
    printf("OtherPerms_Updated->%01x\n",other_perms);
    char test_var[4];
    octal_converter(test_var, &user_perms,0);
    octal_converter(test_var, &group_perms,1);
    octal_converter(test_var, &other_perms,2);
    printf("FinalPerms->%s\n",test_var);
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
int handler(uint8_t *flag, char **argv, int argc){
    int index;
    char* mode = NULL;
    int opt_index = getopt_flag_updater(flag, argv, argc);
    printf("Verbose_flag = %d\nChange_flag = %d\nRecursive_flag = %d\n",*flag & BIT(0), *flag & BIT(1), *flag & BIT(2));
    bool mode_found = false, mode_octal = false;
    for (index = opt_index; index < argc; index++) {
        if ( argv[index][0] != 'u' && argv[index][0] != 'g' && argv[index][0] != 'a' && argv[index][0] != 'o' && argv[index][0] != '0') continue;
        else{
            mode_found = true;
            mode = argv[index];
            if(octal_checker(mode))  {
                printf("Octal mode found\n");
                mode_octal = true;
            }
            printf("Mode->%s\n",mode);
            printf("Treatment loop!\n");
        }
    }
    if( !mode_found ) {
        printf("Error no new mode identified\n");
        return 1;
    }
    int var = --index; //Get the last argument before the loop ended to get the file path
    char *perms = NULL;
    if( !mode_octal ) {
        printf("Not octal\n");
        if( newfile_perms(var, argv, &argc, flag, perms, mode) != 0 ) {
            printf("Error!\n");
            return 1;
        }
    }
    else{
        char *new_mode = mode + 1;
        output_treatment(flag, argv, &argc, new_mode, argv[var]);
    }
    for( int i = 0; i < argc; i++){
        printf("ARGV[%d] = %s\n", i, argv[i]);
    }
    return 0;

}

int main (int argc, char **argv) {
    const char *s = getenv("hello");
    printf("?->%s\n", s);
    uint8_t option_flag = 0x00;
    if(handler(&option_flag, argv, argc) != 0){
        printf("Error in handler()!\n");
        return 1;
    }
    return 0;
}