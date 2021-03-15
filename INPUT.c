if(argc < 2) printf("missing operands\n");
else{

    //needed strings to invoke verbal_permissions changer
    char *action = malloc(sizeof(char) * 1 + 1);

    action[1] = '\0';

     char *user_type = malloc(sizeof(char) * 1 + 1);

    user_type[1] = '\0';

    char *mode;

    char *perms;

    if(strcmmp(argv[0],"-v") == 0 || strcmp(argv[0], "--verbose") == 0){

    }
    else if(strcmp(argv[0], "-c") == 0 || strcmp(argv[0], "--changes") == 0){
        
    }
    else if(strcmp(argv[0], "-R") == 0){
        
    }
    else if(mode_resolver(argv[0]) != NULL){  //these two must be exclusive

        mode = mode_resolver(argv[0]);  //redundunte code here

        if(strlen(mode) == 5){
            perms = malloc(sizeof(char) * 3 + 1);

            for(int i = 0; i < 3; i++){
                perms[i] = mode[i + 2];
            }
        }
        else{
            perms = malloc(sizeof(char) * 9 + 1);

            for(int i = 0; i < 9; i++){
                perms[i] = mode[i + 2];
            }
        }

        if(file_and_dir_checker(argv[1])){

            user_type[0] = mode[0];

            action[0] = mode[1];

            verbal_permission_changer(argv[1], perms, action, user_type, argv[0]);
        }
        else{
            printf("cannot access '%s': No such file or directory\n", argv[1]);
        }
    }
    else if(octal_checker(argv[0])){
    //check if argv[1] has a real file name
    //use the ouput of octal resolver
    //separate in all the strings needed to use octal_changer
    //invoke octal changer
    }
    else{
        printf("Invalid or missing operands!\n");
    }
}