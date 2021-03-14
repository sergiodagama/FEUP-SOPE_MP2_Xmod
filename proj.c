#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#define BIT(n)  ( 1 << (n) ) //Bit toggle
#define BITC(n) ~(1 << (n) ) //Bit clear

int option_treatment(char *amb, uint16_t *flag )
{
    for( char *t = amb ; *t != '\0'; t++){
        switch (*t) {
            case 'u':
                *flag |= BIT(3);
                break;
            case 'g':
                *flag |= BIT(4);
                break;
            case 'o':
                *flag |= BIT(5);
                break;
            case 'a':
                *flag |= BIT(6);
                break;
            default:
                printf((const char *) stderr, "Error");
                break;
        }
    }
    char *k = amb;
    while ( *k != '\0'){
        switch (*k) {
            case '+':
                *flag |= BIT(7);
                while(*k != '\0') {
                    printf("%c\n",*k);
                    if( *k == '=' || *k == '-') break;
                    else if (*k == 'r') *flag |= BIT(10);
                    else if (*k == 'w') *flag |= BIT(11);
                    else if (*k == 'x') *flag |= BIT(12);
                    k++;
                }
                break;
            case '-':
                k++;
                *flag |= BIT(8);
                while( *k != '\0' ) {
                    if( *k == '=' || *k == '-') break;
                    else if (*k == 'r') *flag &= BITC(10);
                    else if (*k == 'w') *flag &= BITC(11);
                    else if (*k == 'x') *flag &= BITC(12);
                    k++;
                }
                break;
            case '=':
                k++;
                *flag |= BIT(9);
                while( *k != '\0' ) {
                    if( *k == '=' || *k == '-') break;
                    else if (*k == 'r') *flag |= BIT(10);
                    else if (*k == 'w') *flag |= BIT(11);
                    else if (*k == 'x') *flag |= BIT(12);
                    k++;
                }
                break;
            default:
                k++;
                printf((const char *) stderr, "Error");
                break;
        }
    }
    return 0;

}

int main (int argc, char **argv)
{
    uint16_t option_flag = 0;
    /* BIT(0) Verbose_flag
     * Bit(1) Change_flag
     * Bit(2) Recursive_flag
     * Bit(3) User_flag
     * Bit(4) Group_flag
     * Bit(5) OtherUsers_flag
     * Bit(6) AllUsers_flag
     * Bit(7) Plus_flag
     * Bit(8) Minus_flag
     * Bit(9) Equal_flag
     * Bit(10) Read_flag
     * Bit(11) Write_flag
     * Bit(12) Execution_flag
     * */
    int c;
    opterr = 0;
    int index;

    while ((c = getopt (argc, argv, ":vcR?:")) != -1) {
        switch (c) {
            case 'v':
                option_flag |= BIT(0);
                break;
            case 'c':
                option_flag |= BIT(1);
                break;
            case 'R':
                option_flag |= BIT(2);
                break;
            case '?':
                fprintf(stderr, "Unknown option character `\\x%x'.\n",
                        optopt);
                return 1;
            default:
                abort();
        }
    }

    printf("Verbose_flag = %d\nChange_flag = %d\nRecursive_flag = %d\n###########\n",
            option_flag & BIT(0), option_flag & BIT(1), option_flag & BIT(2));

    for (index = optind; index < argc; index++) {
        printf ("Non-option argument %s\n", argv[index]);
        if ( argv[index][0] != 'u' && argv[index][0] != 'g' && argv[index][0] != 'a' && argv[index][0] != 'o')
        {
            printf("Here !\n");
        }
        else{
            printf("Treatment loop!\n");
            if(option_treatment(argv[index], &option_flag) != 0) printf((const char *) stderr, "Error in option_treatment func()");
            //printf("%d\n", option_flag & BIT(6));

        }
    }
    printf ("user_flag = %d\ngroup_flag = %d\nother_user = %d\nall_flag = %d\nplus_flag= %d\n",
            option_flag & BIT(3), option_flag & BIT(4), option_flag & BIT(5), option_flag & BIT(6)
            , option_flag & BIT(7));
    printf( "minus_flag = %d\nequal_flag = %d\nread_flag = %d\nwrite_flag = %d\nexecution_flag = %d\n",
            option_flag & BIT(8), option_flag & BIT(9), option_flag & BIT(10), option_flag & BIT(11),
            option_flag & BIT(12));
    /*return 0;
    int fileCount = 0;

    char isFileError = 0;
    uid_t myUid = getuid();

    struct stat fileStat;
    mode_t addPermissions = 0;
    mode_t removePermissions = 0;

    while ((c = getopt (argc, argv, "u:g:o:U:G:O:")) != -1) { // Parse cli args
        switch (c) {

            case 'u':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        addPermissions |= S_IRUSR;
                    else if (optarg[i] == 'w')
                        addPermissions |= S_IWUSR;
                    else if (optarg[i] == 'x')
                        addPermissions |= S_IXUSR;
                }
                break;

            case 'g':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        addPermissions |= S_IRGRP;
                    else if (optarg[i] == 'w')
                        addPermissions |= S_IWGRP;
                    else if (optarg[i] == 'x')
                        addPermissions |= S_IXGRP;
                }
                break;

            case 'o':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        addPermissions |= S_IROTH;
                    else if (optarg[i] == 'w')
                        addPermissions |= S_IWOTH;
                    else if (optarg[i] == 'x')
                        addPermissions |= S_IXOTH;
                }
                break;

            case 'U':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        removePermissions |= S_IRUSR;
                    else if (optarg[i] == 'w')
                        removePermissions |= S_IWUSR;
                    else if (optarg[i] == 'x')
                        removePermissions |= S_IXUSR;
                }
                break;

            case 'G':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        removePermissions |= S_IRGRP;
                    else if (optarg[i] == 'w')
                        removePermissions |= S_IWGRP;
                    else if (optarg[i] == 'x')
                        removePermissions |= S_IXGRP;
                }
                break;

            case 'O':
                for (i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'r')
                        removePermissions |= S_IROTH;
                    else if (optarg[i] == 'w')
                        removePermissions |= S_IWOTH;
                    else if (optarg[i] == 'x')
                        removePermissions |= S_IXOTH;
                }
            default:
                break;
        }
    }*/
}

