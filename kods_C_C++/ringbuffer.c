#include "ringbuffer.h"

//STATIC means private
static void* QueueArray[QUEUE_SIZE]; //pointer to queue array.
//*void means we can put anything
static int queue_start;
static int queue_end;
static int actual_size;

static pthread_mutex_t ringbuffer_mutex; //handle for mutex
static pthread_cond_t full_cond, empty_cond; //handle for condition variable. 2 different condition variables, full_cond and empty_cond.

extern void initialize_ringbuffer(void) //Public function initialize ringbuffer.
{
	pthread_mutex_init(&ringbuffer_mutex,NULL);		//Mutex initialization. Arguments: mutex handle, attributes set to NULL which is default attributes.
	pthread_cond_init(&full_cond,NULL);				//Condition variable initialization. Arguments: Condition variable handle, attributes set to NULL which is default attributes.
	pthread_cond_init(&empty_cond,NULL);			//Condition variable initialization. Arguments: Condition variable handle, attributes set to NULL which is default attributes.
	queue_start = 0; 
	queue_end = QUEUE_SIZE - 1;
	actual_size = 0; //Queue_start starts at 0, queue_end ends at the value corresponding to queue_size - 1(4-1 = 3), actual size = 0.
}

extern void put_mail(void* a) //Put mail function. input argument is a void pointer pointing to the value used when using the function call.
   pthread_mutex_lock(&ringbuffer_mutex); //Lock the ringbuffer mutex while work is being done by setting the signal to non-signaled(owned).
        while (actual_size == QUEUE_SIZE) pthread_cond_wait(&full_cond,&ringbuffer_mutex); //While the actual size value is the same as the queue size, pthread_cond_wait atomically releases the mutex pointed to by ringbuffer mutex and causes the 
																						   //the calling thread to block on the condition variable pointed to by full_cond. Atomic type objects are the only ones that can be accessed and modified by
																						   //multiple threads concurrently without occuring race conditions. This feature is essential for global and static variables accessed from different threads
																						   //and program correctness will be preserved.
																						   //If the queue is full the mutex ring buffer is atomically released which causes the thread calling the function to block on the condition variable pointed
																						   //to by full_cond. the full_cond condition variable pointed to is the address of the condition variable that the function waits for.
																						   //This means that if the queue is full the calling thread is blocked on the condition variable full_cond and the mutex ring buffer is unlocked by changing the
																						   //the state to signaled. 
        QueueArray[(++queue_end) % QUEUE_SIZE] = a; //At spot 4 % 4 = 0, the value of variable a is assigned to.
	    actual_size++; //Actual size of queue is incremented by 1.
        pthread_cond_signal(&empty_cond); //Unblock thread that is blocked on the condition variable pointed to by empty_cond.
   pthread_mutex_unlock(&ringbuffer_mutex); //Unlock mutex that is being pointed to by ringbuffer mutex and change its state to be signaled.
}

extern int try_put_mail(void* a) //Try to put mail function.
{
   pthread_mutex_lock(&ringbuffer_mutex); //Lock the ring buffer mutex while work is being done.

        if (actual_size == QUEUE_SIZE) //Check if the actual_size of the queue is equal to the queue_size.
		{
			pthread_mutex_unlock(&ringbuffer_mutex); //If the actual size of the queue is the same as the queue size meaning the queue is full, the mutex ring buffer is unlocked and state is change back to signaled.
			return 0;
		}

        QueueArray[(++queue_end) % QUEUE_SIZE] = a;		//If there is room in the queue, put the value of variable a into the position at spot 4 % 4 = 0.
	    
		actual_size++;									//Increment the actual queue size after mail has been put into the queue.
        pthread_cond_signal(&empty_cond);				//Unblock thread that is blocked on the condition variable pointed to by empty_cond. If a thread tried to put mail into the queue and the queue wasnt full, any thread that was blocked on the conditional variable
														//empty_cond is now unblocked since the queue is no longer empty. A thread would be blocked on the condition variable if it tried to get mail from the queue, but the queue was empty.
   pthread_mutex_unlock(&ringbuffer_mutex);				//Unlock the mutex ring buffer by changing the state to signaled.
   return 1;
}

extern void* get_mail(void) //Get mail function.
{
   pthread_mutex_lock(&ringbuffer_mutex); //Lock the mutex ring buffer while work is being done.
       while (actual_size == 0) pthread_cond_wait(&empty_cond,&ringbuffer_mutex); //While the actual size of the queue is 0, the thread is blocked on the empty condition variable and the ring buffer mutex is released.
       actual_size--; //If there is something in the queue, the actual size of the queue is decremented by 1 when someone is trying to get mail from it.
       void* temp = QueueArray[(queue_start++) % QUEUE_SIZE]; //A pointer to a temporary variable is created and the value stored in it is the value in the queuearray at position: 0 % 3 = 0.
       pthread_cond_signal(&full_cond); //Unblock thread that is blocked on the condition variable pointed to by full_cond. If a thread tryed putting mail into the queue and it was full, the thread was then blocked on the condition variable full_cond
										//but if the function get_mail was called and mail was removed from the queue, that thread is then unblocked since the queue is no longer full.
   pthread_mutex_unlock(&ringbuffer_mutex); //Unlock the mutex ring buffer by changing the state to signaled.
   return temp; //Return the value stored from the queuearray in the temporary variable.
}

extern void* try_get_mail(void) //try to get mail function.
{
   pthread_mutex_lock(&ringbuffer_mutex); //Lock the mutex ring buffer while work is being done by changing the state to be non-signaled.
       if (actual_size == 0) //Checking if the actual size of the queue is 0 meaning the queue is empty.
	   {
			pthread_mutex_unlock(&ringbuffer_mutex); //If the queue is empty, unlock the mutex ring buffer by changing the signal to be signaled.
			return NULL;
		}
       actual_size--; //If the queue is not full, decrement the queue size by 1.
       void* temp = QueueArray[(queue_start++) % QUEUE_SIZE]; //A pointer to a temporary variable called temp is created and the value stored in it is the value in the queuearray at position: 0 % 3 = 0.
       pthread_cond_signal(&full_cond); //Unblock any thread blocked on the condition variable full_cond because they previously tried to get mail from an empty queue.
   pthread_mutex_unlock(&ringbuffer_mutex); //Unlock the mutex ring buffer by changing its state to be signaled.
   return temp; //return the temporary value stored from the queuearray.
}

