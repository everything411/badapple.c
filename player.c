#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define DATA_FILE "bad_apple.txt"
#define MUSIC_FILE "bad_apple.mp3"
#define US_TO_DELAY 30000
#define BUF_LEN 10000
#define CHAR_PER_LINE 161
#define LINE_PER_FRAME 61
int main(void)
{
	FILE *fp;
	char buffer[BUF_LEN] = {0};
	if ((fp = fopen(DATA_FILE, "r")) == 0)
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
		for (int i = 0; i < LINE_PER_FRAME; i++)
		{
			if (fgets(buffer + i * CHAR_PER_LINE, CHAR_PER_LINE + 1, fp) == NULL)
			{
				fclose(fp);
				exit(0);
			}
		}
		printf("[H%s", buffer);
		buffer[0] = 0;
		usleep(US_TO_DELAY);
	}
}
