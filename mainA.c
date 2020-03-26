#include <pthread.h> 
#include <semaphore.h> 
#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>

void createThread4();

int accounts[3];
pthread_t threadId[4];

/*  Semapore descriptions     */
sem_t mutex;
sem_t t1;
sem_t t2;
sem_t t3;
sem_t t4;



/* 
 *     Below are the functions we call to execute on each of the newly created threads. 
 *
 *         @param args Void pointer for run time arguments
 *             @return void pointer
 *  
 */

void *thread1(void *args) {

    int temp, i = 0;
    printf("Created thread %d\n", 1);

    createThread4();


   sem_post(&t1);   
   sem_post(&t1);
   sem_post(&t1);

   sem_wait(&t4);
   sem_wait(&t2);
   sem_wait(&t3);


    for (i = 0; i < 10000; i++) {
        /* Enter CS */
        sem_wait(&mutex);

        temp = accounts[0];
        accounts[0] = temp - 200;

        temp = accounts[1];
        accounts[1] = temp + 200;

        sem_post(&mutex);
        /* Exit CS */

	if (i % 3000 == 0) {
	    usleep(200000);
	}
    }
    

    pthread_exit(NULL);
}

void *thread2(void *args) {

    int temp, i = 0;
    printf("Created thread %d\n", 2);


   sem_post(&t2);   
   sem_post(&t2);
   sem_post(&t2);

   sem_wait(&t1);
   sem_wait(&t4);
   sem_wait(&t3);


    for (i = 0; i < 10000; i++) {
        /* Enter CS */
        sem_wait(&mutex);

        temp = accounts[1];
        accounts[1] = temp - 200;

        temp = accounts[2];
        accounts[2] = temp + 200;

        sem_post(&mutex);
        /* Exit CS */

	if (i == 2000 || i == 4000 || i == 6000 || i == 9000) {
		usleep(200000);
	}
    }

    pthread_exit(NULL);
}
void *thread3(void *args) {

    int temp, i = 0;
    printf("Created thread %d\n", 3);



   sem_post(&t3);   
   sem_post(&t3);
   sem_post(&t3);

   sem_wait(&t1);
   sem_wait(&t2);
   sem_wait(&t4);

    for (i = 0; i < 10000; i++) {
        /* Enter CS */
        sem_wait(&mutex);

        temp = accounts[2];
        accounts[2] = temp - 200;

        temp = accounts[0];
        accounts[0] = temp + 200;

        sem_post(&mutex);
        /* Exit CS */

	if (i == 2000 || i == 7500) {
		usleep(300000);
	}
    }

    pthread_exit(NULL);
}
void *thread4(void *args) {


    int timesChecked, timesNotEqual = 0;

    printf("Created thread %d\n", 4);


   sem_post(&t4);   
   sem_post(&t4);
   sem_post(&t4);

   sem_wait(&t1);
   sem_wait(&t2);
   sem_wait(&t3);

    for (timesChecked = 0; timesChecked < 50000; timesChecked++) {
        sem_wait(&mutex);

        int sum = accounts[0] + accounts[1] + accounts[2];

        if (sum != 300000) timesNotEqual = timesNotEqual + 1;

        sem_post(&mutex);

	if (timesChecked % 10000 == 0) {
		usleep(50000);
	}        
    }

    printf("Checked: %d  Times not equal to 300000: %d \n", timesChecked, timesNotEqual);

    exit(1);
}


/* 
 *     Below are the functions we call to create each of the threads.
 *
 *
 *         @return PID for thread 
 *         */

void createThread1() {
    pthread_create(&threadId[0], NULL, thread1, NULL);
}

void createThread2() {
    pthread_create(&threadId[1], NULL, thread2, NULL);
}

void createThread3() {
    pthread_create(&threadId[2], NULL, thread3, NULL);
}

void createThread4() {
    pthread_create(&threadId[3], NULL, thread4, NULL);
}

int main(int argc, char **argv) {

    int i, mutexReturn, sem1Return, sem2Return, sem3Return, sem4Return = 0;
    /* Initialize Accounts */
    accounts[0] = 100000;
    accounts[1] = 100000;
    accounts[2] = 100000;

    /* Initialize our mutex to a value of 1 */
    mutexReturn = sem_init(&mutex, 0, 1);
    sem1Return = sem_init(&t1, 0, 0);
    sem2Return = sem_init(&t2, 0, 0);
    sem3Return = sem_init(&t3, 0, 0);
    sem4Return = sem_init(&t4, 0, 0);

    if (mutexReturn != 0 || sem1Return != 0 || sem2Return != 0 || sem3Return != 0 || sem4Return != 0) {
        printf("Failed to create mutex");
        exit(EXIT_FAILURE);
    }


    /* Spawn all the the threads with respective functions */
    createThread1();
    createThread2();
    createThread3();



    /* Wait for the second thread to complete then exit */
    for (i = 0; i < 4; i++) pthread_join(threadId[i], NULL); 
    return 0;
}





