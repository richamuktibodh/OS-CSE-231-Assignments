#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<time.h>

// p2 --> writes then reads
int main()
{
    srand(time(0));
    int fd, i;
    clock_t start, end;
    // FIFO file path
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char arr1[80];
    // char payload[10][3] = {"ab", "cd", "ef","gh","ij","kl","mn","op","qr","st"};
    char payload[50][2];
    ////////
    for (i = 0; i < 50; i++)
    {
        payload[i][0] = 'a' + rand() % 26;
        payload[i][1] = '\0';
    }
    //////////
    int index = 0;
    start = clock();
    while (index + 4 < 50)
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);

        
        for (i = index; i < index + 5; i++)
        {
            write(fd, &payload[i], sizeof(payload[i]));
        }

        close(fd);

        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);

        // Read from FIFO
        read(fd, &index, sizeof(int));

        // Print the read message
        printf("Index returned: %d\n", index - 1);
        close(fd);
    }
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken for tranfer 50 times in seconds: %f\n", time_taken); // in seconds
    return 0;
}