#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define DATAFILE "bad_apple.txt"
#define MUSICFILE "bad_apple.mp3"
#define MSTODELAY 30000
#define BUFLEN 10000
#define CHARPERLINE 161
#define LINEPERFRAME 61
int main(void) {
	FILE *fp;
	char buffer[BUFLEN] = { 0 };
	if ((fp = fopen(DATAFILE, "r")) == 0) {
		return 1;
	}
	pid_t pid=fork();
	if(pid==0)
	{
		execlp("mpg123", "mpg123", "-q", MUSICFILE, (char *)0);
	}
	while (1) {
		for (int i = 0; i < LINEPERFRAME; i++) {
			if (fgets(buffer + i * CHARPERLINE, CHARPERLINE + 1, fp) == NULL) {
				fclose(fp);
				exit(0);
			}
		}
      printf("[H%s", buffer);
      buffer[0] = 0;
      usleep(MSTODELAY);
	}
}
