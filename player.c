#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>

#define DATA_FILE "bad_apple.txt"
#define MUSIC_FILE "bad_apple.mp3"
#define US_TO_DELAY 300
#define US_PRE_FRAME 33333
#define BUF_LEN 10000
#define CHAR_PER_LINE 161
#define LINE_PER_FRAME 60
#define CLEAR_SCR "\x1b[H"

// return a microsecond timestamp
long long timestamp(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000LL + tv.tv_usec;
}

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
		exit(0); // if fail, exit
	}
	int size_read = 0;
	long long start_time = timestamp();
	long long frame = 1;
	long long now_time = 0;
	while (1)
	{
		size_read = fread(buffer, 1, CHAR_PER_LINE * LINE_PER_FRAME + 2, fp);
		if(size_read == 0)
		{
			// EOF, stop
			break;
		}
		// CLEAR_SCR is a magic ASCII escape sequence \x1b[H
		// when pressing HOME on keyboard it produces \x1b[H in tty and return the cursor to line 1 col 1
		// we print the sequence to return to line 1 col 1
		printf(CLEAR_SCR "%s\n", buffer);
		while(1)
		{
			// if now - start > framecount * frametime
			// then we reach the next frame
			now_time = timestamp();
			if(now_time - start_time > frame * US_PRE_FRAME)
			{
				printf("frame %lld\n", frame);
				frame++;
				break;
			}
			// sleep for a while instead of busy waiting to reduce cpu usage
			usleep(US_TO_DELAY);
		}
	}
	wait(NULL); // wait for mpg123
	return 0;
}

