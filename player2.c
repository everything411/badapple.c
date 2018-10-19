#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define DATA_FILE "bad_apple.txt"
#define MUSIC_FILE "bad_apple.mp3"
#define US_TO_DELAY 25500
#define BUF_LEN 10000
#define CHAR_PER_FRAME 9662
int main(void)
{
	int fd;
	char buffer[BUF_LEN] = {0};
	if ((fd = open(DATA_FILE, O_RDONLY)) == -1)
	{
		return 1;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		execlp("mpg123", "mpg123", "-q", MUSIC_FILE, (char *)0);
	}
	while (1)
	{
		if (read(fd, buffer, CHAR_PER_FRAME) != CHAR_PER_FRAME)
		{
			close(fd);
			return 0;
		}
		printf("[H%s", buffer);
		usleep(US_TO_DELAY);
	}
}
