#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>

// p1 --> reads then writes
int main()
{
    srand(time(0));
    int fd1, i;

    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char str1[50][2];
    int index = 0;
    while (index + 4 < 50)
    {
        // First open in read only and read
        fd1 = open(myfifo,O_RDONLY);
        
        for (i = index; i < index + 5; i++)
        {
            read(fd1, &str1[i], sizeof(str1[i]));
            printf("Data: %s , index: %d\n", str1[i],i);
        }
        index = i;

        close(fd1);

        // Now open in write mode and write
        // string taken from user.
        fd1 = open(myfifo,O_WRONLY);
        write(fd1,&index, sizeof(int));
        close(fd1);
    }
    return 0;
}