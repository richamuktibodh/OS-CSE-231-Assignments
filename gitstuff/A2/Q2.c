#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <errno.h>
#define SYS_kernel_2d_memcpy 336



int main(){
    float arr1[3][3] = {{1.1, 1.2, 1.3},{2.1,2.2,2.3},{3.1,3.2,3.3}};
    float arr2[3][3];
    long val = syscall(SYS_kernel_2d_memcpy,arr1,arr2,3);
    printf("System call returned %ld.\n", val);
    if (val != EFAULT)
    {
        printf("Message : System Call 336 successfuly invoked \n");
    }
    int i,j;
    // printing elements of arr2[]
    for (i=0; i<3; i++){
        for (j=0; j<3; j++){
            printf("%f ", arr2[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}
