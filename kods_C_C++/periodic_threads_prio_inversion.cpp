/* 
 * Copyright (C) 2009 Chris Simmonds (chris@2net.co.uk)
 *
 * This is a demonstration of periodic threads using the Linux timerfd
 * interface which was introduced in GNU libc 2.8 and kernel 2.6.25.
 */

//Thread 3 basically has to wait before the other ones are done

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <time.h>
#include <iostream>
#include <fstream>  
using namespace std;

// 89000000 is 1 sec:
#define N_CPU_BURN 87000000 

//Create thread handles.
pthread_t t_0;
pthread_t t_1;
pthread_t t_2;
pthread_t t_3;

pthread_mutex_t Mutex;

struct periodic_info { //User defined data type with members int timer_fd and unsigned long long wakeups_missed.
	int timer_fd;
	unsigned long long wakeups_missed;
};


static int make_periodic(unsigned int period, struct periodic_info *info) //Function with static int return type and input arguments unsigned int period and members from the struct periodic_info.
{
	int ret;
	unsigned int ns;
	unsigned int sec;
	int fd;
	struct itimerspec itval;

	/* Create the timer */
	fd = timerfd_create(CLOCK_MONOTONIC, 0); //Create a new timer object with the arguments: clockid that has the type CLOCK_MONOTONIC which is a nonsettable monotonically increasing clock measuring time from an unspecific point.
	info->wakeups_missed = 0; //Pointer pointing to the structure member wakeups_missed.
	info->timer_fd = fd; //Pointer pointing to the structure member timer_fd.
	if (fd == -1) //return fd integer if fd == -1.
		return fd;

	/* Make the timer periodic */
	sec = period / 1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(fd, 0, &itval, NULL);
	return ret;
}


static void wait_period(struct periodic_info *info)
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read(info->timer_fd, &missed, sizeof(missed)); //Read function with arguments: pointer to structure member timer_fd which is where to read the input from, a pointer to where the read content will be stored, numbers of bytes to read before 
														 //truncating data and if all data to be read is smaller, it is all stored in the buffer.
	if (ret == -1) {
		perror("read timer");
		return;
	}

	info->wakeups_missed += missed; //Use pointer to set value of structure member wakeups_missed to be wakeups_missed + missed.
}


static void CPUburn(unsigned long n)
{
	for (unsigned long i = 0; i<= n;i++) 
	{
	  // do nothing  
	}
}

void work(int time_units, int task_no, char output) 
{ 
  static unsigned int globalTime = 0;
  for(int j = 0; j < time_units; j++) //For loop iterating as long as variable j is below value of time_units.
  {
		printf("%d:",globalTime); //Print globaltime value.
		globalTime++; //Increase globaltime value.
		for(int j = 0; j < task_no ; j++) printf("\t"); //For loop iterating as long as j is below value of task_no and print a comma.
		printf("%d%c\n",task_no,output); //Print task_no and output value.
		fflush(stdout);
		CPUburn(N_CPU_BURN); //Burn the CPU for around 1 second.
  }
}

static void* idle_thread(void *vptr) //Idle thread function.
{
 int thread_no = (int) vptr; //Void pointer pointing to an integer value. 
 while(1)
 {
   work(1,thread_no,' '); //Pass parameters to work function.
 }
 return NULL;
}

static void *thread_1(void *vptr) //Thread1 function.
{
	
        int thread_no = (int) vptr;	//void pointer pointing to an integer value.
	struct periodic_info info;

	//printf("Thread 1 period 20 s\n");
	make_periodic(20000000, &info); //Pass parameters to make_periodic function.
	while (1) 
	{
		work(1, thread_no, 'C'); //Pass parameters to work function.
		pthread_mutex_lock(&Mutex); //lock mutex while work is being done.
        work(4, thread_no,' '); //Pass parameters to work function.
		work(1, thread_no, 'R');
		pthread_mutex_unlock(&Mutex); //Release mutex and let other threads access.
		work(1, thread_no, ' ');
		wait_period(&info); //Pass parameter referenced to by &info to wait_periodic function.
	}
	return NULL;
}

static void *thread_2(void *vptr) //Function for thread2.
{

        int thread_no = (int) vptr;	
	struct periodic_info info;

	//printf("Thread 2 period 20 s\n");
	make_periodic(20000000, &info);
	while (1) {
		   sleep(2); //Sleep for 2 s.
           work(6,thread_no,' '); //Pass parameters to work function.	
		wait_period(&info); //Pass parameter referenced to by &info to wait_periodic function.
	}
	return NULL;
}

static void *thread_3(void *vptr) //Function for thread3.
{

        int thread_no = (int) vptr;	
	struct periodic_info info;

	//printf("Thread 3 period 20 s\n");
	make_periodic(20000000, &info);
	while (1) {
		        sleep(3); //Sleep for 3 s.
                work(2, thread_no,' ');
				work(1, thread_no, 'C');
				pthread_mutex_lock(&Mutex); //Lock mutex while work is being done.
				work(2, thread_no, ' ');
				work(1, thread_no, 'R');
				pthread_mutex_unlock(&Mutex); //Release mutex again.
				work(1, thread_no, ' ');
				
			
		wait_period(&info);
	}
	return NULL;
}



int main(int argc, char *argv[])
{

// work calibration of N_CPU_BURN to 1 sec.:
//int s,e;s=clock();work(1,0,'C');e=clock();cout<<endl<<e-s<<"us"<<endl;	
	
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr); //Initialize the mutex attributes object. 
	//pthread_mutexattr_setprotocol(&attr,PTHREAD_PRIO_NONE);
	pthread_mutexattr_setprotocol(&attr,PTHREAD_PRIO_INHERIT); //Set the inheritscheduler attribute of the mutex attributes object referred to by &attr using PTHREAD_PRIO_INHERIT which means when a thread is blocking a higher priority thread because of 
															   //owning one or more mutexes, it executes at the higher of its priority or the priority of the highest priority thread waiting on any mutexes owned by this thread.
	//pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);
	pthread_mutexattr_setprioceiling(&attr, 60); //Set the priority ceiling attribute of a mutex attributes object pointed to by attr.
	pthread_mutex_init(&Mutex, &attr); //Initialize mutex referenced by mutex with the attributes specified by attr.

	// Scheduling parameters
	
	struct sched_param params_t0;
	params_t0.sched_priority = 19; //Set scheduling priority to 19 as scheduling parameter for thread 1.

	struct sched_param params_t1;
	params_t1.sched_priority = 23;

	struct sched_param params_t2;
	params_t2.sched_priority = 24;
	
	struct sched_param params_t3;
	params_t3.sched_priority = 25;
	
	//thread handle for attributes for all threads.
    pthread_attr_t attr_t0;
	pthread_attr_t attr_t1;
    pthread_attr_t attr_t2;
	pthread_attr_t attr_t3;
	
	//Initialize the thread attributes objects for all threads.
	pthread_attr_init(&attr_t0);
	pthread_attr_init(&attr_t1);
	pthread_attr_init(&attr_t2);
    pthread_attr_init(&attr_t3);

	//Set the inheritscheduler attribute of the thread attributes object referred to by all threads using PTHREAD_EXPLICIT_SCHED which takes the scheduling attributes from the values specified by the attributes objects.
	pthread_attr_setinheritsched(&attr_t0, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t1, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t2, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t3, PTHREAD_EXPLICIT_SCHED);
	
	//Set the policy of the scheduling to be FIFO(first in first out), or RR(round-robin) for all threads.
	pthread_attr_setschedpolicy(&attr_t0, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t1, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t2, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t3, SCHED_FIFO);

	//Set the scheduling parameters of the threads to use the attributes objects for all threads and parameters for all threads.
	pthread_attr_setschedparam(&attr_t0, &params_t0);
	pthread_attr_setschedparam(&attr_t1, &params_t1);
	pthread_attr_setschedparam(&attr_t2, &params_t2);
	pthread_attr_setschedparam(&attr_t3, &params_t3);	

	//Create the threads by using the arguments: thread handle, the thread attributes object specifying the attributes for the thread, the function for the thread and starting argument to pass on.
	pthread_create(&t_3, &attr_t3, thread_3, (void*)3 );
	pthread_create(&t_2, &attr_t2, thread_2, (void*)2 );
	pthread_create(&t_1, &attr_t1, thread_1, (void*)1 );
    pthread_create(&t_0, &attr_t0, idle_thread, (void*)0 );

	//Waits for the specified thread to terminate with no exit status.
	pthread_join(t_0,NULL);
	pthread_join(t_1,NULL);
	pthread_join(t_2,NULL);
	pthread_join(t_3,NULL);

	sleep(200);
	return 0;
}
