#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
//https://pubs.opengroup.org/onlinepubs/7908799/xsh/dirent.h.html dirent.h documentation
void recurse_chmod(char* path, int mode)
{
    struct dirent *dp; // Directory handler
    DIR *dir = opendir(path);
    if (dir == NULL) {
        // not a directory, carry on
        return;
    }
    char *subpath = malloc(sizeof(char)*PATH_MAX);
    int pathlen = strlen(path);
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 ||
            strcmp(dp->d_name, "..") == 0) continue;
        if (strlen(dp->d_name) + pathlen + 2/*NUL and slash*/ > PATH_MAX) {
            fprintf(stderr, "Invalid path specified: too long\n");
            exit(1);
        }
        strcpy(subpath, path);
        strcat(subpath, "/");
        strcat(subpath, dp->d_name);
        if (chmod(subpath, mode) < 0) {
            fprintf(stderr, "Unable to chmod %s: %s\n", subpath, strerror(errno));
            exit(1);
        }
        recurse_chmod(subpath, mode);
    }
    free(subpath);
    closedir(dir);
}
