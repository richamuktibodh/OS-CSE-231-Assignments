#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../sem.h"
#include<time.h>
#include<string.h>

#define SHM_SIZE 6 * sizeof(int)


char payload[50];

int main()
{
    srand(time(0));
    int i,j,m,max,r;
    clock_t start, end;	
    for (i=0; i < 50; i++)
    {
        payload[i] = 'a' + rand() % 26;
        // payload[i][1] = '\0';
    }

    semaphore_t *semshm,*semsync;

    semshm = semaphore_create("/tmp/semshm"); // for shared memory
    if (semshm == NULL){
      printf("Sender : Not Able to create semshm semaphore \n");
      exit(1);
    }
    semshm = semaphore_create("/tmp/semsync"); // to signal when each program should read/write 
    if (semshm == NULL){
      printf("Sender : Not Able to create semsync semaphore \n");
      exit(1);
    }
    semshm = semaphore_open("/tmp/semshm");
    if (semshm == NULL){
	printf("Sender : Not Able to open semaphore \n");
        exit(1);
    }
    semsync = semaphore_open("/tmp/semsync"); 
    if (semsync == NULL){
	printf("Sender : Not Able to open semaphore \n");
        exit(1);
    }

    	int fd = shm_open("/meow", O_CREAT | O_EXCL | O_RDWR, 0600);
    	// printf("fd: %d\n", fd);
    	if (fd < 0) {
        	printf("Error creating shm object\n");
        	return EXIT_FAILURE;
    	}

    	/*Resize the shm*/
    	ftruncate(fd, SHM_SIZE );

    		char* data = (char*) mmap(0, 5* sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            int* ind = (int*) mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	start = clock();
        for ( i = 0 ; i < 10  ; i++ ){
            data[0] = payload[5*i+0] ;
    		data[1] = payload[5*i+1] ;
    		data[2] = payload[5*i+2] ;
    		data[3] = payload[5*i+3] ;
    		data[4] = payload[5*i+4] ;
            ind[0] = 5*i+0 ;
    		ind[1] = 5*i+1 ;
    		ind[2] = 5*i+2 ;
    		ind[3] = 5*i+3 ;
    		ind[4] = 5*i+4 ;
    		ind[5] = -99 ;
            printf("Values Sent : %c %c %c %c %c\n",payload[5*i+0],payload[5*i+1],payload[5*i+2],payload[5*i+3],payload[5*i+4]);
            printf("Indexes Sent : %d %d %d %d %d\n",5*i+0,5*i+1,5*i+2,5*i+3,5*i+4);
            // printf("data[5]             : %d \n", data[5]);
            // printf("ind[5]             : %d \n", ind[5]);
            semaphore_post(semshm);
            semaphore_wait(semsync);
		
		    // printf("Receive.... data[5] : %d \n", data[5]);
            printf("Received ind[5] : %d \n", ind[5]);
	    }
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // in seconds
    printf("Time taken for transfer 50 times in seconds: %f\n", time_taken);
 
    // semaphore_wait(semshm);
    if (munmap(data,SHM_SIZE) < 0) {
       	printf("Error unmap shm object\n");
       	return EXIT_FAILURE;
    }
    if (close(fd) < 0) {
       	printf("Error closing fd \n");
       	return EXIT_FAILURE;
    }
    if (unlink("/tmp/semshm") < 0) {
        printf("Error unlink semshm \n");
        return EXIT_FAILURE;
    }
    if (unlink("/tmp/semsync") < 0) {
        printf("Error unlink semsync \n");
        return EXIT_FAILURE;
    }
    if (shm_unlink("/meow") < 0) {
        printf("Error unlink meow \n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
