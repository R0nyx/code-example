#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#define EVER ;; 

//Global variables.
int mul_number_1, mul_number_2, mul_result ;
int div_number_1, div_number_2, div_result ;
int add_number_1, add_number_2, add_result ;
int sub_number_1, sub_number_2, sub_result ;

//Handles for events.
//EVENT variabbles
HANDLE mul_operands_ready, mul_result_ready ;
HANDLE div_operands_ready, div_result_ready ;
HANDLE add_operands_ready, add_result_ready ;
HANDLE sub_operands_ready, sub_result_ready ;

void multiplicator( void ) //Multiplication function.
{ 
	for( EVER )
	{
		WaitForSingleObject( mul_operands_ready, INFINITE ) ; //Use the multiplication operator ready event while multiplication is being done. If the event is in the signaled state the wait function returns and the thread continues. If in the non-signaled
															  //state the thread is blocked until the event is signaled.
		mul_result = mul_number_1 * mul_number_2 ; //Multiply results.
		ResetEvent( mul_operands_ready ) ; //Call reset event to set the event multiplication operator ready to non-signaled state.
		SetEvent( mul_result_ready ) ; //Set the multiplication result event to signaled.
	}
}

void divider( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( div_operands_ready, INFINITE ) ; //Use the div operator ready event while division is being done.
		div_result = div_number_1 / div_number_2 ; //Division result.
		ResetEvent( div_operands_ready ) ; //Change the state of the div operator ready event to be signaled from non-signaled.
		SetEvent( div_result_ready ) ; //set the division result to be signaled.
	}	
}

void adder( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( add_operands_ready, INFINITE ) ; //Use the add operator ready while addition is being done.
		add_result = add_number_1 + add_number_2 ; //Addition result.
		ResetEvent( add_operands_ready ) ; //Change the state of the add operator ready event to be signaled from non-signaled.
		SetEvent( add_result_ready ) ; //Set the addition result to be signaled.
	}
}

void subtractor( void )
{ 
	for( EVER )
	{
		WaitForSingleObject( sub_operands_ready, INFINITE ) ; //Use the sub operator ready while subtraction is being done.
		sub_result = sub_number_1 - sub_number_2 ; //Subtraction result.
		ResetEvent( sub_operands_ready ) ; //Change the state of the sub operator ready event to be signaled from non-signaled.
		SetEvent( sub_result_ready ) ; //Set the sub result to be signaled.
	}
}

void calculator( void ) //Calculator function.
{ 
	for( EVER )
	{    
		mul_number_1 = 4;
		mul_number_2 = 3;
        SetEvent( mul_operands_ready) ; //Set the mul operator ready event to be signaled.
		// When even is signaled, particular thread should execute

		div_number_1 = 8;
		div_number_2 = 2;
        SetEvent( div_operands_ready) ; //Set the div operator ready event to be signaled.


		WaitForSingleObject( mul_result_ready, INFINITE ) ; //Use the mul result ready event by setting it to be non-signaled.
		ResetEvent( mul_result_ready ) ; //Change the state of the mul result ready event to be signaled from non-signaled.

		WaitForSingleObject( div_result_ready, INFINITE ) ; //Use the div result ready event by setting it to be non-signaled.
		ResetEvent( div_result_ready ) ; //Sets the specified event object to the nonsignaled state.

		sub_number_1 = mul_result;
		sub_number_2 = 2;
        SetEvent( sub_operands_ready) ; //Set the sub operator ready event to be signaled.
        WaitForSingleObject( sub_result_ready, INFINITE ) ; //Use the sub result ready event by setting it to be non-signaled.
	
		ResetEvent( sub_result_ready ) ; //Change the state of the sub result ready event to be signaled from non-signaled.

        add_number_1 = sub_result;
		add_number_2 = div_result;
		SetEvent( add_operands_ready) ; //Set the add operator ready event to be signaled.
        WaitForSingleObject( add_result_ready, INFINITE ) ; //Use the add result ready event by setting it to be non-signaled.
		ResetEvent( add_result_ready ) ; //Sets the specified event object to the nonsignaled state.

		printf("%d ",add_result); //Print the add_result.
		
	}
}

void main( void )
{   
   
   //Create event objects with the arguments: pointer to security attributes set to NULL meaning it cannot be inherited by child processes, boolean deciding the type of event object to create and while its TRUE the type is manual-reset which requires
   //resetevent function call to set the event state to non-signaled, boolean deciding the initial state of the event and being set to FALSE means its initially non-signaled, the name of the event object that a different process can us to refer to
   //the event object.
   mul_operands_ready = CreateEvent(
	   NULL,	//	Defautl Security attribute
	   TRUE,	//	Manual-reset event
	   FALSE,	//	Initial state is NON signaled
	   NULL		//	Object name
   );
   mul_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

   div_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   div_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);  
  
   add_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   add_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

   sub_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
   sub_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);


	HANDLE calc_thread[5]; //Handle for array with 5 threads.
	//Create threads using different functions with the arguments: pointer to security handle set to NULL meaning it cant be inherited by child processes, initial stack size in bytes, a pointer to the function executed by the thread, a pointer to a 
	//variable to be passed to the thread, argument determining whether the thread is ran immediately and since the value is 0 the thread is ran immediately, a pointer to a variable that receives the thread identifier and since its NULL the thread 
    //identifier is not returned.
	calc_thread[0] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)multiplicator, NULL,	0, NULL) ;
	calc_thread[1] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)divider,		NULL,	0, NULL) ;
    calc_thread[2] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)adder,			NULL,	0, NULL) ;
	calc_thread[3] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)subtractor,	NULL,	0, NULL) ;
	calc_thread[4] = CreateThread( NULL, 0 , (LPTHREAD_START_ROUTINE)calculator,	NULL,	0, NULL) ;
	
    WaitForMultipleObjects(5,calc_thread,true,INFINITE); //Waitformultipleobjects waiting for all threads to be created. Arguments: size of array, name of array, boolean set to TRUE meaning it waits for all objects to be in the signaled state, the timeout
														 //interval

}
