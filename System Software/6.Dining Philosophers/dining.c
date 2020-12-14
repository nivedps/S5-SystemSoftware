#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define N 5

typedef enum {
	THINKING,
	HUNGRY,
	EATING
} Actions;

#define LEFT (phil_num + 4) % N
#define RIGHT (phil_num + 1) % N

sem_t mutex;
sem_t S[N];

Actions state[N];
int philosophers[N] = {0, 1, 2, 3, 4}; 

void * philosopher(void *);
void start_eating(int);
void end_eating(int);
void test(int);

void * philosopher(void * num)
{
	while(1)
	{
		int * i = num;
		sleep(1);
		start_eating(*i);
		sleep(0);
		end_eating(*i);
	}
}

void start_eating(int phil_num)
{
	sem_wait(&mutex);
	state[phil_num] = HUNGRY;
	printf("\nPhilosopher%d is hungry", phil_num+1);
	test(phil_num);
	sem_post(&mutex);
	sem_wait(&S[phil_num]);
	sleep(1);
}

void end_eating(int phil_num)
{
	sem_wait(&mutex);
	state[phil_num] = THINKING;
	printf(
		"\nPhilosopher%d stops eating and puts fork %d and %d down",
		phil_num+1,
		LEFT+1,
		phil_num+1
	);
	printf("\nPhilosopher%d is thinking", phil_num+1);
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void test(int phil_num)
{
	if(state[phil_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
	{
		state[phil_num] = EATING;
		sleep(2);
		printf(
			"\nPhilosopher%d takes fork %d and %d",
			phil_num+1,
			LEFT+1,
			phil_num+1
		);
		printf("\nPhilosopher%d is eating", phil_num+1);
		test(LEFT);
		test(RIGHT);
		sem_post(&S[phil_num]);
	}
}

int main()
{
	int i;
	pthread_t thread_id[N];
	sem_init(&mutex, 0, 1);

	for(i = 0; i < N; i++) sem_init(&S[i], 0, 0);

	for(i = 0; i < N; i++)
	{
		pthread_create(&thread_id[i], NULL, philosopher, &philosophers[i]);
		printf("\nPhilosopher%d is thinking", i+1);
	}

	for(i = 0; i < N; i++) pthread_join(thread_id[i], NULL);
}
