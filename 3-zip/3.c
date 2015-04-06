#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <unistd.h>
#include <errno.h>
#include <string.h>
int main(int argc, char *argv[]) {
	char buf;
    int count_sim=-1;
    int count_zero=0;
    char buffer[1024];
    int fd=open(argv[1], O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IXUSR);  
    if ( fd == -1 ) {
    	printf("Can`t open file: %s\n", strerror(errno));
        exit(1);
    }
    while (read(0, &buf, 1) == 1) {
    	if (buf == 0) {
    		if (count_sim!=-1) {
    			printf("Write %d\n", count_sim);
    			if (write(fd, buffer, count_sim+1)==-1) {printf("Can't write: %s %d %s\n", strerror(errno), count_sim, buffer);};
				count_sim=-1;
    		}
            count_zero++;
        }
        else {
			if (count_zero!=0) {
				printf("Seek %d\n",count_zero);
				lseek(fd, count_zero, SEEK_CUR);
				count_zero=0;
        	}
        	count_sim++;
        	buffer[count_sim]=buf;
        }
    }

    if (count_sim!=-1) {
    	printf("Write %d\n",count_sim);
    	if (write(fd, buffer, count_sim+1)==-1) {printf("Can't write: %s %d %s\n", strerror(errno), count_sim, buffer);};
		}
    int er = close(fd);
    if (er == -1) {
    	printf("Can`t close file: %s\n", strerror(errno));
        exit(1);
    }
    return 0;
}