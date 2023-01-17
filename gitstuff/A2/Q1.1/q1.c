#include <sched.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>

FILE *fptr;

void writePrios(int prio1, int prio2, int prio3){
    fprintf(fptr,"%d %d %d\n ", prio1, prio2, prio3);
}

void *countA(void *arg) {
    long int i, count = 0;
    struct timespec before, after;
    clock_gettime(CLOCK_MONOTONIC,&before); // sets time in before
    for (i=1; i<=pow(2,32); i++){
        count += 1;
    }
    clock_gettime(CLOCK_MONOTONIC,&after); // sets time in after
    uint64_t before_ns = (before.tv_sec * 1000000000) + before.tv_nsec;
    uint64_t after_ns = (after.tv_sec * 1000000000) + after.tv_nsec;
    int64_t elapsed = after_ns - before_ns;
    int s = elapsed/1000000000;
    printf("OTHER: ");	
    printf("%d seconds, ", s);
    int64_t ns = elapsed - (int64_t)s* 1000000000;
    printf("%ld nanoseconds elapsed\n", ns);
    fprintf(fptr,"OTHER %d %d\n ",s, ns);
    printf("completed thread1\n");
    return NULL;
}

void *countB(void *arg) {
    long int i, count = 0;
    struct timespec before, after;

    clock_gettime(CLOCK_MONOTONIC,&before); // sets time in before
    for (i=1; i<=pow(2,32); i++){
        count += 1;
    }
    clock_gettime(CLOCK_MONOTONIC,&after); // sets time in after
    uint64_t before_ns = (before.tv_sec * 1000000000) + before.tv_nsec;
    uint64_t after_ns = (after.tv_sec * 1000000000) + after.tv_nsec;
    int64_t elapsed = after_ns - before_ns;
    int s = elapsed/1000000000;
    printf("RR: ");	
    printf("%d seconds, ", s);
    int64_t ns = elapsed - (int64_t)s* 1000000000;
    printf("%ld nanoseconds elapsed\n", ns);
    fprintf(fptr,"RR %d %d \n",s, ns);
    printf("completed thread2\n");
    return NULL;
}

void *countC(void *arg) {
    long int i, count = 0;
    struct timespec before, after;

    clock_gettime(CLOCK_MONOTONIC,&before); // sets time in before
    for (i=1; i<=pow(2,32); i++){
        count += 1;
    }
    clock_gettime(CLOCK_MONOTONIC,&after); // sets time in after
    uint64_t before_ns = (before.tv_sec * 1000000000) + before.tv_nsec;
    uint64_t after_ns = (after.tv_sec * 1000000000) + after.tv_nsec;
    int64_t elapsed = after_ns - before_ns;
    int s = elapsed/1000000000;
    printf("FIFO: ");
    printf("%d seconds, ", s);
    int64_t ns = elapsed - (int64_t)s* 1000000000;
    printf("%ld nanoseconds elapsed\n", ns);
    fprintf(fptr,"FIFO %d %d\n",s, ns);
    printf("completed thread3\n");
    return NULL;
}

int main(){
    struct sched_param param1, param2, param3;
    pthread_t thread1, thread2, thread3;
    int prio1, prio2,prio3, ret1, ret2, ret3;
    int rc1, rc2, rc3;
    pthread_attr_t attr1, attr2, attr3;
    fptr = fopen("vals.txt","a");

    rc1 = pthread_attr_init(&attr1);
    rc2 = pthread_attr_init(&attr2);
    rc3 = pthread_attr_init(&attr3);

    // 1st iteration
    // thread 1
    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 1;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 1;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration1\n");

// 2nd iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 10;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 10;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration2\n");

// 3rd iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 20;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 20;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration3\n");
	
// 4th iteration
    // thread 1
    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 30;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 30;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration4\n");

// 5th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 40;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 40;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration5\n");

// 6th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 50;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 50;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration6\n");

// 7th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 60;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 60;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration7\n");

// 8th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 70;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 70;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration8\n");

// 9th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 80;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 80;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration9\n");

// 10th iteration
    // thread 1

    pthread_create( &thread1, NULL, countA, "hello1" );
    pthread_create( &thread2, NULL, countB, "hello2" );
    pthread_create( &thread3, NULL, countC, "hello3" );

    prio1 = 0;
    param1.sched_priority = prio1;
    rc1 = pthread_attr_setinheritsched(&attr1, PTHREAD_EXPLICIT_SCHED);
    ret1 = pthread_setschedparam (thread1, SCHED_OTHER, &param1);
    if (ret1 != 0){
        printf("Error in calling pthread_setschedparam for thread1\n");
        printf("ret1: %d\n",ret1);
    }


    //thread 2
    prio2 = 99;
    param2.sched_priority = prio2;
    rc2 = pthread_attr_setinheritsched(&attr2, PTHREAD_EXPLICIT_SCHED);
    ret2 = pthread_setschedparam (thread2, SCHED_RR, &param2);

    if (ret2 != 0){
        printf("Error in calling pthread_setschedparam for thread2\n");
        printf("ret2: %d\n",ret2);
    }
    
    
    // thread 3
    prio3 = 99;
    param3.sched_priority = prio3;
    rc3 = pthread_attr_setinheritsched(&attr3, PTHREAD_EXPLICIT_SCHED);
    ret3 = pthread_setschedparam (thread3, SCHED_FIFO, &param3);

    if (ret3 != 0){
        printf("Error in calling pthread_setschedparam for thread3\n");
        printf("ret3: %d\n",ret3);
    }
  
// join
    pthread_join(thread1,  NULL);
    pthread_join(thread2,  NULL);
    pthread_join(thread3,  NULL);

    writePrios(prio1, prio2, prio3);

printf("Finished iteration10\n");

// destroy
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2); 
    pthread_attr_destroy(&attr3);

    fclose(fptr);
    return 0;
}
