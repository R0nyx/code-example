#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

//Change the definitions of variables/methods
#define EVER ;; //Just means it run forever, using wile(true) might be more reasonable solution
#define PARKINGPLACES 3 // number of parking places pre-deifned	
#define CARS 5 // number of Cars predifned 

HANDLE parkSem, road1Mutex, road2Mutex, screenMutex; //Some pre-defined generic references

void car_body(void* vptr) //Function creation with no return type and input parameter of type void ptr. This vptr parameter gives us the car number.
{
	unsigned int car_no = (unsigned int)vptr, i;

	for (EVER) //Infinite loop
	{	
		WaitForSingleObject(screenMutex, INFINITE); //Waitforsingleobject function is a syncronization object which can either be signal(available) or non-signaled(owned). This specific one has an inifinite timeout value. if the syncronization object
													//is signaled, the function returns 0 indicating succes, and if its non-signaled the function blocks until the object is available or the timeout interval expires. If the timeout interval expires
													//the status word WAIT_TIMEOUT is returned. WAIT_ABANDONED keyword can be returned if the object is a mutex owned by a terminated thread which never released ownership.
													//In this case the function is set to be non-signaled(owned) with an infinite timeout interval meaning the only way to change it to be signaled(available) again is releasing the screenmutex.

		 for (i = 0; i < car_no - 1; i++) printf("            "); 
		 printf(" car %d traffic\n",car_no);

		ReleaseMutex(screenMutex); //Releases the screenmutex changing the state of waitforsingleobject changes to be signaled(available).
		WaitForSingleObject(parkSem, INFINITE); //Wait operation is called on the parking semaphore making it non-signaled with an infinite timeout interval causing the semaphore to be non-signaled(owned) until release. 5 threads can now own this.
		
		if (WaitForSingleObject(road1Mutex, 0) == WAIT_TIMEOUT ) //If statement checking if the timeout interval(0 ms) of wait operation on road1mutex expires and if it is the condition is true. 
		{
			WaitForSingleObject(road2Mutex, INFINITE); //Wait operation is called on the road2mutex making it non-signaled with an infinite timeout interval. 
			  WaitForSingleObject(screenMutex, INFINITE); //Wait operation is called on the screenmutex making it non-signaled with an infinite timeout interval.
			    for (i = 0; i < car_no - 1; i++) printf("            "); 
			    printf(" car %d parking road 2\n", car_no); //Prints that the car is taking road2.
			  ReleaseMutex(screenMutex); //Releases the screenmutex making it signaled.
			ReleaseMutex(road2Mutex); //Releases the road2mutex making it signaled.
		}
		else
		{
			WaitForSingleObject(screenMutex, INFINITE); //If the timeout interval of road1mutex hasnt expired, wait operation is called on screenmutex making it non-signaled.
			  for (i = 0; i < car_no - 1; i++) printf("            ");
			  printf(" car %d parking road 1\n", car_no); //Prints that the car is taking road1.
			ReleaseMutex(screenMutex); //Releases the screenmutex making it signaled.
		  ReleaseMutex(road1Mutex); //Releases the road1mutex making it signaled since road1 was available.
		}
		  WaitForSingleObject(screenMutex, INFINITE); //Wait operation is called on screenmutex making it non-signaled.
		    for (i = 0; i < car_no - 1; i++) printf("            ");
		    printf(" car %d parked\n", car_no); //Prints that the car is parked
		  ReleaseMutex(screenMutex); //Releases the screenmutex Making it signaled.

		Sleep(1000 * car_no); //Sleep for 1000*car number´, which in case of car 5 would be 5 seconds wait after car 5 is parked.
		if ( car_no % 2 == 0) //Checking if the remainder of car_no divided by 2 is 0, if it is the condition is true and the car takes road1.
		{ 
		  WaitForSingleObject(road1Mutex, INFINITE); //Wait operation called on road1mutex making it non-signaled. 
			WaitForSingleObject(screenMutex, INFINITE); //Wait operation called on screenmutex making it signaled.
			   for (i = 0; i < car_no - 1; i++) printf("            ");
			   printf(" car %d parking road 1\n", car_no); //Prints thath the car is taking road1.
			ReleaseMutex(screenMutex); //Releases the screenmutex making it signaled.
		  ReleaseMutex(road1Mutex); //Releases the road1mutex making it signaled.
		}
		else
		{
			WaitForSingleObject(road2Mutex, INFINITE); //Wait operation called on road2mutex making it non-signaled.
			  WaitForSingleObject(screenMutex, INFINITE); //Wait operation called on screenmutex making it non-signaled.
			    for (i = 0; i < car_no - 1; i++) printf("            ");
			    printf(" car %d parking road 2\n", car_no); //Prints thath the car is taking road2.
			  ReleaseMutex(screenMutex); //Releases the screenmutex making it signaled.
			ReleaseMutex(road2Mutex); //Releases the road2mutex making it signaled.
		}
		ReleaseSemaphore(parkSem, 1, NULL); //Releases the parking semaphore and adds 1 to the current semaphore count.
		
		Sleep( car_no); //Sleeps for ms according to car number.
	}
} //

void main( void ) 
{   
	
	HANDLE car_thread[5]; //Create a car thread array with a size of 5
	// Mutex is:  is a mutually exclusive flag. It acts as a gate keeper to a section of code allowing one thread in and blocking access to all others.
	// This ensures that the code being controled will only be hit by a single thread at a time. Just be sure to release the mutex when you are done.
	road1Mutex = CreateMutex(NULL,FALSE,NULL);	  // Create road1mutex with security descriptor set to NULL,
												  // ownership flag is set to FALSE meaning the mutex is available
												  // to the first caller of the wait operation
												  // and mutex name set to NULL creating a mutex without a name

	road2Mutex = CreateMutex(NULL, FALSE, NULL);  // Create road2mutex with security descriptor set to NULL,	
												  // ownership flag is set to FALSE meaning the mutex is available
												  // to the first caller of the wait operation
												  // and mutex name set to NULL creating a mutex without a name

	screenMutex = CreateMutex(NULL, FALSE, NULL); // Create screenmutex with security descriptor set to NULL,
												  // ownership flag is set to FALSE meaning the mutex is available
												  // to the first caller of the wait operation
												  // and mutex name set to NULL creating a mutex without a name


	//	Semaphore:  A semaphore object allows only a specified number of concurrent accesses among
	//	threads in a critical section. Each semaphore object has a count associated with it, specifying the maximum 
	//	number of further accesses it can allow.
	parkSem = CreateSemaphore(NULL,PARKINGPLACES,PARKINGPLACES,NULL); //	Create semaphore, First argument of the creation is the security descriptor set to NULL(the handle cannot be inherited by child processes.),
																	  //	second argument is the initial count set to the value of parkingplaces previously assigned the value 5, this value is reduced by 1 when a wait operation is called on the 
																	  //	semaphore. When the count reaches 0, any thread waiting for the semaphore is blocked until some thread releases the semaphore which increases the initial count again.
																	  //    Third atribute means Maximum count, so you cant start fro 3 filled out of 5 options.
																	  //	Last atribute is the name of sempahore 

	for (int i = 0; i < CARS; i++) //For loop creating the threads according to the number of cars.
	{
		car_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)car_body, (LPVOID)(i+1), 0, NULL); //	CreateThread 
																										 // First argument is a pointer to a LPSECURITY_ATTRIBUTE structure containing a security descriptor. In this case the argument is NULL meaning it has all access priviliges granted to it. 
																										 // Second argument specifies the size of the new thread's stack in bytes.
																										 // Third argument is a function address from which the new thread begins execution. This function address it the address of the car_body function. 
																										 //	Fourth argument is the data to be passed on to the function. (Specifies a single 32-bit parameter value passed to the thread.)
																										 // Fifth argument specifies whether the thread should begin execution immediately. This argument can have the value 0 or CREATE_SUSPENDED. In this case the argument is 0 meaning the thread runs immediately upon creation.
																										 //	If the CREATE_SUSPENDED is used, the thread is immediately suspended and not continued until resumethread is called on its handle.
																										 //	The last argument is given the thread ID when the thread is created. 
	}
		
	
    WaitForMultipleObjects(5,car_thread,true,INFINITE); //	WaitforMultipleobjects waits for multiple objects. 
														//	The first argument is the size of the array of object in this case being 5 which is passed as the second argument.
														//  The third argument is a boolean flag which in this case is TRUE meaning the function waits for all objects to be in the signaled state at the same time. If the boolean is FALSE the function returns when any object is in the array
														//	attains the signaled state. 
														//  Last argument The time-out interval - INFINITE, the function will return only when the specified objects are signaled. 

}
