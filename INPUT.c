if(argc < 2) printf("missing operands");
else{
    if(strcmmp(argv[0],"-v") == 0 || strcmp(argv[0], "--verbose") == 0){

    }
    else if(strcmp(argv[0], "-c") == 0 || strcmp(argv[0], "--changes") == 0){
        
    }
    else if(strcmp(argv[0], "-R") == 0){
        
    }
    else if(mode_resolver(argv[0]) != NULL){  //these two must be exclusive
    //check if argv[1] has a real file name
    //use the ouput of mode resolver
    //separate in all the strings needed to use verbal_changer
    //invoke verbal changer
    }
    else if(octal_resolver(argv[0]) != NULL){
    //check if argv[1] has a real file name
    //use the ouput of octal resolver
    //separate in all the strings needed to use octal_changer
    //invoke octal changer
    }
}