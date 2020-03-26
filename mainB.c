#include <pthread.h> 
#include <semaphore.h> 
#include <stdlib.h> 
#include <stdio.h>


typedef struct {

	char chars[5];
	int itemNumber;



} BufferAItem;

typedef struct {

	int itemNumber;
	char chars[3];

} BufferBItem;



BufferAItem bufferA[20];
BufferBItem bufferB[40];


pthread_t threads[3];

sem_t emptyA;
sem_t fullA;

sem_t emptyB;
sem_t fullB;


sem_t mutexA;
sem_t mutexB;


void *threadA(void *args) {


	int i = 0;


	for (i = 0; i < 3000; i++) {



		BufferAItem newItem;
		
		newItem.itemNumber = i;
		

		if (i % 2 == 0) {
			
			newItem.chars[0] = 'A';
			newItem.chars[1] = 'B';
			newItem.chars[2] = 'C';
			newItem.chars[3] = 'a';
			newItem.chars[4] = 'a';

		} else {
			
			newItem.chars[0] = 'X';
			newItem.chars[1] = 'Y';
			newItem.chars[2] = 'Z';
			newItem.chars[3] = 'x';
			newItem.chars[4] = 'x';
			
		}

		sem_wait(&emptyA);
		sem_wait(&mutexA);

		bufferA[i % 20] = newItem;

		sem_post(&mutexA);
		sem_post(&fullA);
	
	}














}

void *threadB(void *args) {}

void *threadC(void *args) {}


void createThreadA() {

	pthread_create(&threads[0], NULL, threadA, NULL);

}

void createThreadB() {

	pthread_create(&threads[1], NULL, threadB, NULL);

}

void createThreadC() {

	pthread_create(&threads[2], NULL, threadC, NULL);

}








int main(int argc, char **argv) {



	sem_init(&emptyA, 0, 20);
	sem_init(&fullA, 0, 0);

	sem_init(&emptyB, 0, 40);
	sem_init(&fullB, 0, 0);


























	createThreadA();
	createThreadB();
	createThreadC();

}
