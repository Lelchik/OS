#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
static void print_string(char * str, int length,int link) {
    printf("%s",str );
    int i;
    for ( i=0; i<40-length; i++) {printf(" " );}
    printf("links: %d\n", link);
}
static void list_dir (const char * dir_name) {
    DIR * d;
    struct stat sb;
    d = opendir (dir_name);

    if (!d) {
        printf ("Cannot open directory '%s': %s\n", dir_name, strerror (errno));
        exit (0);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;
        entry = readdir (d);
        if (! entry) {
            break;
        }
        d_name = entry->d_name;

        int path_length;
        int link;          
        char path[PATH_MAX];
        path_length = snprintf (path, PATH_MAX,"%s/%s", dir_name, d_name);
        if (stat(path, &sb) == -1) {
           exit(0);
        }
        link = sb.st_nlink;
          print_string(path, path_length, link);
              
        if (entry->d_type == 4 ) {
            if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
                print_string(path, path_length, link);
                if (path_length >= PATH_MAX) {
                    printf ("long path\n");
                    exit (0);
                }
                list_dir (path);
            }
	   }
    }
    if (closedir (d)) {
        printf ("Could not close '%s': %s\n",dir_name, strerror (errno));
        exit (0);
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf( "name of dir in first argument\n");
        exit(1);
    }
    list_dir (argv[1]);
    return 0;
}