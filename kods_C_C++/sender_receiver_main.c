
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include "ringbuffer.h"

//thread handles for sender and receiver threads.
pthread_t sender;
pthread_t receiver;

char* Message[7] = {" this "," is ", " a "," message ", " to "," a "," receiver "}; //pointer to character array with a size of 7 storing 7 different strings in it.

void* sender_body(void* vptr) //Sender_body function.
{ 
  //printf(" sender \n");
	
   while(1)
   {  
      printf(" put_mail called: \n"); //Print that the function put_mail was called.
      for( unsigned int i = 0; i < 7; i++ ) //For loop iterating through the loop 7 times.
      {
        sleep(2); //Every time the loop is ran, sleep for some time before putting in the mail.
		put_mail(Message[i]); //The string at position i in the character array message is used as input parameter to the put_mail function.
	  }
	  sleep(2);
	  printf(" try_put_mail called: \n"); //Print that the try_put_mail function was called.
	  for( unsigned int i = 0; i < 7; i++ ) //iterate through for loop 7 times.
      {
        sleep(2);
		while( !try_put_mail(Message[i]) ) sleep(1); //While trying to put mail into the queue through try_put_mail function doesnt work, sleep for some time and try again. Function doesnt work when queue is full.
	  }
   }
return NULL;
}

void* receiver_body(void* vptr)
{ char* str; //Pointer to character called str.
	//printf(" receiver \n");
   while(1)
   {  
      printf(" get_mail called: \n"); //Print that get_mail was called.
      for( unsigned int i = 0; i < 7; i++ ) //iterate through for loop 7 times.
      {
        sleep(1);
		str = (char*)get_mail(); //variable str get the value stored in it corresponding to the string stored at position 0 in queuearray from ringbuffer.c file.
		printf(" %s \n", str ); //Print the string received from position 0 in queuearray.
	  }
      sleep(2);
	  printf(" try_get_mail called: \n"); //Print that try_get_mail was called.
	  for( unsigned int i = 0; i < 7; i++ ) //Iterate through for loop 7 times.
      {
        sleep(1);
		str = (char*)try_get_mail(); //variable str get the value stored in it corresponding to the string stored at position 0 in queuearray from ringbuffer.c file.
		if ( str != NULL ) printf(" %s \n", str ); //If variable str is not empty, print the string stored in the variable.
	  }
	  
   }
return NULL;

}

int main(void)
{
	//printf(" main \n");
	initialize_ringbuffer(); //Call function to initialize the ringbuffer from ringbuffer.c file.
	
	
	 pthread_create(&sender, NULL, &sender_body, NULL); //Create thread with the arguments: thread handle, thread attributes set to NULL creating the thread with default attributes, main function for thread to use, the argument passed to start.
	 pthread_create(&receiver, NULL, &receiver_body, NULL); //Create thread with the arguments: thread handle, thread attributes set to NULL creating the thread with default attributes, main function for thread to use, the argument passed to start.
	 
	 pthread_join(sender, NULL); //Wait for the specified thread to terminate without exit status.
	 pthread_join(receiver, NULL); //Wait for the specified thread to terminate without exit status.
	

	return 0;
}

