#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>

pthread_mutex_t customer_mutex = PTHREAD_MUTEX_INITIALIZER; //Initialize mutex with macro.
sem_t waiting_customers; //Semaphore handle.
sem_t awaits_shaving;

int NuOfCustomers = 0;
#define CUSTOMERS 6
#define BARBERSHOP_CAPACITY 4
//pthread_t - simply identifies thread
pthread_t customer[CUSTOMERS]; //Thread handle array with size equal to CUSTOMERS.
pthread_t barber;

void* customer_body(void* vptr)
{
   int customer_id = (int) vptr; //Void pointer poiting to integere value from thread creation.
  while(1)
  { 
      //Customer arrives
      pthread_mutex_lock(&customer_mutex); //Use customer mutex while work is being done.
       if ( NuOfCustomers < BARBERSHOP_CAPACITY ) //Check if the number of customers are below the capacity of the barbershop.    
       {
        NuOfCustomers++; //Increase number of customers.
        //Wakes up the Barber
        sem_post(&waiting_customers); //Unlock semaphore and increment the value. If value becomes greater than 0, another process or thread blocked in a sem_wait call will be woken up and proceed to lock the semaphore.
        printf(" number of custoners %d customer id %d \n", NuOfCustomers, customer_id);
        pthread_mutex_unlock(&customer_mutex); //
        sem_wait(&awaits_shaving); //Lock semaphore and decrement value. If the value is greater than 0 the function returns immediately and if the value is 0 the call blocks until its incremented or a signal handler interrupts the call.
       }
       else  pthread_mutex_unlock(&customer_mutex); //Unlock the customer mutex if the barbershop doesnt have room for customers.

      sleep(customer_id); //Sleep value is corresponding to the customer_id from the thread creation value.
  }
 
return NULL;
   
}

void* barber_body(void* vptr)
{

   while(1) //Simple forever
   {
        //BARBER IS SLEEPING
        sem_wait(&waiting_customers); //Lock the semaphore and decrement the value.
        //Woken by the customer
        pthread_mutex_lock(&customer_mutex); //Lock the customer mutex while work is being done.
        NuOfCustomers--; //Decrement customer number.
        printf(" shaved one - number of custoners is now %d \n", NuOfCustomers); 
        sem_post(&awaits_shaving); //Unlock the semaphore and increment value.
        pthread_mutex_unlock(&customer_mutex); //Unlock the customer mutex.
    
        sleep(2);

   }

return NULL;

}

int main(void)
{   
   int i;
   //Initlize 2 semaphores
   sem_init(&waiting_customers,0,0);    //Initialize semaphore with the arguments: semamphore handle
   sem_init(&awaits_shaving             // semamphore handle
            ,0                          // a zero value meaning its shared between threads and a non zero value means its shared between processes
            ,0                          // The initial value of the semaphore.
   );
   
   pthread_create(&barber, NULL, &barber_body, NULL); //Create thread with the arguments: thread handle, thread attributes set to NULL creating the thread with default attributes, the main function used by the thread, the argument passed to start.
   
   for(i=0;i < CUSTOMERS; i++) //For loop creating the amount of threads according to the value of CUSTOMERS.
    {
       pthread_create( &customer[i], NULL , &customer_body, (void*) (i+1) ); //Create thread with the arguments: thread handle, thread attributes set to NULL creating the thread with default attributes, the main function used by the thread, 
                                                                             //the argument passed to start.
    } 

   pthread_join(barber, NULL); //Waits for the specified thread to terminate with no exit status.

   for(i=0;i < CUSTOMERS; i++) //for loop waiting for all the customer threads to terminate with no exit status.
    {
       pthread_join( customer[i], NULL);
    } 

return 0;

   

}

