#include<pthread.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include <sched.h>
using namespace std;

pthread_t primes_id; //thread handle for prime numbers.
pthread_t seconds_clock_id; //thread handle for clock.


//Function that prints prime numbers.
void* primes(void *vptr)
{
	unsigned int i, n = 2;
	while (n < 1000000000)
	{
		for (i = 2; i <= n; i++) if (n % i == 0) break;
		if (i == n) cout << " " << n << flush;
		n++;
	}
        return NULL;

}

//Function that increments variable s after 1 second and prints the value of s.//
//What happens if you have equal priority??
//What happens if Prime has Highest priority??
//What happens if you use Sched_RR? This is round robin, so tasks with the same priority get some time to work, and then 

void* seconds_clock(void *vptr)
{ 
  int s=0;
  while(1)
  { 
    cout << endl << s << endl << flush;
    sleep(1);
    s++;
  }
  return NULL;
}

int main(void)
{   
  cout << "hello world" << flush;

   //First block of statements for the task
  sched_param primes_params; //Create scheduling parameters.
  primes_params.sched_priority = 19; //Set the scheduling priority.
  pthread_attr_t primes_attr; //thread handle for attributes for primes_params.
  pthread_attr_init(&primes_attr); //Initialize the thread attributes object.
  pthread_attr_setinheritsched(&primes_attr,PTHREAD_EXPLICIT_SCHED); //Set the inheritscheduler attribute of the thread attributes object referred to by primes_attr using PTHREAD_EXPLICIT_SCHED which takes the scheduling attributes from the values
																	 //specified by the attributes object.
  //PTHREAD_EXPLICIT_SCHED - means we dont inherit schedulin policy from main, but set it ourselves
  pthread_attr_setschedpolicy(&primes_attr,SCHED_FIFO); //Set the policy of the scheduling to be FIFO(first in first out), or RR(round-robin).
  //
  pthread_attr_setschedparam(&primes_attr,&primes_params); //Set the scheduling parameters of the thread to use the attributes objects primes_attr and primes_params.
  pthread_create(&primes_id, &primes_attr, &primes, NULL); //Create the thread by using the arguments: 
  //thread handle, the thread attributes object specifying the attributes for the thread, the function for the thread and starting argument to pass on.


 //Second block for Clock tasks
  sched_param seconds_clock_params;
  seconds_clock_params.sched_priority = 20;
  pthread_attr_t seconds_clock_attr;
  pthread_attr_init(&seconds_clock_attr);
  pthread_attr_setinheritsched(&seconds_clock_attr,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&seconds_clock_attr,SCHED_FIFO);
  pthread_attr_setschedparam(&seconds_clock_attr,&seconds_clock_params);
  pthread_create(&seconds_clock_id, &seconds_clock_attr, &seconds_clock, NULL);

  pthread_join(primes_id,NULL); //Waits for the specified thread to terminate with no exit status.
  pthread_join(seconds_clock_id,NULL); //Waits for the specified thread to terminate with no exit status.
  cout << "bye cruel world";
 return 0;
}


