#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

/*
int main(int argc, char const *argv[])
{
	int fd;
	if((fd = open("/dev/ttyUU0", O_RDWR | O_NOCTTY | O_SYNC) < 0))
	{
		printf("Error Opening Port\n");
		return -1;
	}
	printf("The value of fd: %d\n",fd);
    write(fd, "Abel", 1);
	return 0;
}
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    
int main() {
    char byte;
    int fd = open("/dev/ttyUU0", O_RDWR | O_NOCTTY | O_SYNC);
    printf("value of fd:%d\n",fd);
    write(fd, "X", 1);
    ssize_t size = read(fd, &byte, 1);
    printf("Read byte %c\n", byte);
    return 0;
}