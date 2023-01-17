#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../sem.h"

#define SHM_SIZE 6 * sizeof(int)

int main()
{
    int i,j,max,r;
    semaphore_t *semshm,*semsync;
    semshm = semaphore_open("/tmp/semshm");
    if (semshm == NULL){
        printf("Receiver : Not Able to open semaphore \n");
        exit(1);
    }
    semsync = semaphore_open("/tmp/semsync");
    if (semsync == NULL){
        printf("Receiver : Not Able to open semaphore \n");
        exit(1);
    }
    int fd = shm_open("/meow", O_CREAT | O_RDWR, 0600);
    if (fd < 0 )
    {
       	printf("Error opening shm\n");
       	return EXIT_FAILURE;
    }
    char* data = (char*) mmap(0, 5* sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    int* ind = (int*) mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    for(i = 0; i < 10 ; i++)
    {
        semaphore_wait(semshm);
        printf("Received indexes ");
        max = ind[0];
        for ( j = 0 ; j < 5 ; j++ ){
            if ( ind[j] > max )
                max = ind[j];
            printf("%d ",ind[j]);
        }
        ind[5] = max;
        printf("\nSent Max index: %d\n",max);
        semaphore_post(semsync);
    }
    // semaphore_post(semshm);
    if (munmap(data,SHM_SIZE) < 0) {
        printf("Error unmap shm object\n");
        return EXIT_FAILURE;
    }
    if (close(fd) < 0) {
       printf("Error closing fd \n");
       return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
