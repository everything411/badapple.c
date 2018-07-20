#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define DATAFILE "bad_apple.txt"
#define MUSICFILE "bad_apple.mp3"
#define USTODELAY 30000
#define BUFLEN 10000
#define CHARPERFRAME 9662
int main(void) {
	int fd;
	char buffer[BUFLEN]={0};
	if ((fd = open(DATAFILE,O_RDONLY))==-1)
	{
		return 1;
	}
	pid_t pid=fork();
	if(pid==0)
	{
		execlp("mpg123", "mpg123", "-q", MUSICFILE, (char *)0);
	}
	while (1)
	{
		if(read(fd,buffer,CHARPERFRAME)!=CHARPERFRAME)
		{
			close(fd);
			return 0;
		}
		printf("[H%s",buffer);
		usleep(USTODELAY);
	}
}
