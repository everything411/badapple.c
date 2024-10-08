#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#include "zlib.h"
#define TIMESTAMP(t) (long long)((t).QuadPart)
#define FRAME_COUNT 5479
#define TIME_PRE_PRAME 40
#define FRAME_TIME(x, freq) (((x) + 1) * TIME_PRE_PRAME * ((freq) / 1000))
#define CHUNK 16384
unsigned long long frame[5500][142][5];
void GotoXY(int x, int y)
{
    COORD c;
    HANDLE hConsoleOut;
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(hConsoleOut, c);
}
int inf(FILE *source, void *dest)
{
    int offset = 0;
    int ret;
    unsigned have;
    z_stream strm;
    static unsigned char in[CHUNK];
    static unsigned char out[CHUNK];
    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;
    /* decompress until deflate stream ends or end of file */
    do
    {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source))
        {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        /* run inflate() on input until output buffer not full */
        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR); /* state not clobbered */
            switch (ret)
            {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            memcpy(dest + offset, out, have);
            offset += have;
        } while (strm.avail_out == 0);
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
void output_frame(int i)
{
    unsigned char frame_buf[45200];
    int cnt = 0;
    for (int j = 0; j < 140; j++)
    {
        for (int k = 0; k < 5; k++)
        {
            for (int l = 0; l < 64; l++)
            {
                frame_buf[cnt++] = (frame[i][j][k] & (1ULL << l)) ? '*' : ' ';
            }
        }
        frame_buf[cnt++] = '\n';
    }
    puts(frame_buf);
}

int main(void)
{
    FILE *fp = fopen("deflate.dat", "rb");
    if (!fp)
    {
        return 1;
    }
    int zlibret = inf(fp, frame);
    fclose(fp);
    if (zlibret != Z_OK)
    {
        return zlibret;
    }
    LARGE_INTEGER qpc;
    long long freq = 0;
    if(!QueryPerformanceFrequency(&qpc))
    {
         printf("QueryPerformanceFrequency fail!\n");
         return 0;
    }
    else
    {
        freq = TIMESTAMP(qpc);
    }
    WinExec("mpg123.exe -q bad_apple.mp3", SW_HIDE);

    long long start, end;
    long long print_start, print_end;
    QueryPerformanceCounter(&qpc);
    start = TIMESTAMP(qpc);
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        QueryPerformanceCounter(&qpc);
        print_start = TIMESTAMP(qpc);
        GotoXY(0, 0);
        output_frame(i);
        QueryPerformanceCounter(&qpc);
        print_end = TIMESTAMP(qpc);
        while (1)
        {
            QueryPerformanceCounter(&qpc);
            end = TIMESTAMP(qpc);
            if(end - start < FRAME_TIME(i, freq))
                Sleep(1);
            else
                break;
        }
        printf("frame%04d, tick per-second %lld, tick %010lld, frame tick %08lld, print tick %08lld\n",
               i, freq, end - start, end - print_start, print_end - print_start);
    }
    return 0;
}
