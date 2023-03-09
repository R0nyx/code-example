
/* Standard includes. */
#include <stdint.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"


/* TODO Add any manufacture supplied header files necessary for CMSIS functions
to be available here. */
//#include "stm32f4xx.h"

/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the portTICK_RATE_MS constant. */
#define mainQUEUE_SEND_PERIOD_MS			( 200 / portTICK_RATE_MS )

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the portTICK_RATE_MS constant. */
#define mainSOFTWARE_TIMER_PERIOD_MS		( 1000 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/*-----------------------------------------------------------*/

/*
 * TODO: Implement this function for any hardware specific clock configuration
 * that was not already performed before main() was called.
 */
static void prvSetupHardware( void );
/*
 * The callback function assigned to the example software timer as described at
 * the top of this file.
 */
//static void vExampleTimerCallback( xTimerHandle xTimer );

/*-----------------------------------------------------------*/

/* The queue used by the queue send and queue receive tasks. */
static xQueueHandle xQueue = NULL;

/* The semaphore (in this case binary) that is used by the FreeRTOS tick hook
 * function and the event semaphore task.
 */
static xSemaphoreHandle xEventSemaphore = NULL;

/* The counters used by the various examples.  The usage is described in the
 * comments at the top of this file.
 */
static volatile uint32_t ulCountOfTimerCallbackExecutions = 0;
static volatile uint32_t ulCountOfItemsReceivedOnQueue = 0;
static volatile uint32_t ulCountOfReceivedSemaphores = 0;
/*
 * The event semaphore task as described at the top of this file.
 */
//static void prvEventSemaphoreTask( void *pvParameters );


/*end of freeRTOS stuff-----------------------------------------------------------*/

enum uniqueReq  { INT_WANTED, CHAR_WANTED }; //User defined data type. INT_WANTED and CHAR_WANTED being the 2 enumerators.
struct Req { 
	enum uniqueReq req_kind;
	void* unique_ptr; }; //User defined data type. The members of the structure is the enumerators from uniqueReq and a void ptr.

static xQueueHandle uniqueReqQueue; //Create a handle for the queue.

static void uniqueIDserver(void *vptr ); //Function prototypes
static void intClient(void *vptr );
static void char1Client(void *vptr );
static void char2Client(void *vptr );

static xSemaphoreHandle screenMutex; //Create a handle for the semaphore.

static int uniqueInt;
static char unique1Char;
static char unique2Char;


int main(void)
{
 // graphical initialization begin
	  LCD_Init();
	  LCD_LayerInit();
	  LTDC_Cmd(ENABLE);
	  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	  LCD_Clear(LCD_COLOR_WHITE);
 // graphical initialization end

 // freeRtos stuff begin
   //   xTimerHandle xExampleSoftwareTimer = NULL;
      prvSetupHardware();
	  xQueue = xQueueCreate(mainQUEUE_LENGTH,sizeof( uint32_t ) );
	  vQueueAddToRegistry( xQueue, ( signed char * ) "MainQueue" );
      vSemaphoreCreateBinary( xEventSemaphore );
      vQueueAddToRegistry( xEventSemaphore, ( signed char * ) "xEventSemaphore" );
 // freeRtos stuff end

      LCD_DisplayStringLine(LINE(1), (uint8_t*)" hello unique 1 ");
      screenMutex = xSemaphoreCreateMutex(); //Creates a mutex.
      uniqueReqQueue =  xQueueCreate(1, sizeof(struct Req)); //Create queue. First parameter being the maximum number of items the queue can hold. Second parameter is the size in bytes required to hold each item in the queue.
		// have a queue of 1
															 //--------------------
	  //Unique ID has priority higher than the rest of them
	  xTaskCreate( uniqueIDserver,
				 ( signed char * ) "uniqueIDserver",
				configMINIMAL_STACK_SIZE,
				NULL,
				2,
				NULL ); //Create task for uniqueidserver. First argument being a pointer to the function. Second argument being a descriptive name for the task usually used for debugging. Third argument being the number of words to allocate for use as
						//the tasks stack. Fourth argument being a value that is passed as the parameter to the created task. Fifth argument being the priority at which the task will execute. Last argument being used to pass a handle to the created task
						//out of the argument, but is optional and can be set to NULL.

	xTaskCreate( intClient,
				( signed char * ) "intClient",
				configMINIMAL_STACK_SIZE,     //Stack depth
				NULL,
				1,
				NULL );
	xTaskCreate( char1Client,
				( signed char * ) "char1Client",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );
	xTaskCreate( char2Client,
				( signed char * ) "char2Client",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );

	vTaskStartScheduler(); //Starts the kernel. this will execute the threads. Started them here

	for( ;; );
}

static void uniqueIDserver( void *pvParameters )
{
	struct Req req;
	int* uniqueIntPtr;
	char* uniqueCharPtr;
    int i = 1;
    char ch ='a';
    char chi[2] = "1";
    char chch[2] = "a";
    LCD_DisplayStringLine(LINE(3), (uint8_t*)" Unique ID ");

	for( ;; )
	{
	   xQueueReceive(uniqueReqQueue, &req, portMAX_DELAY); //Call to the function that receives an item from the queue. arguments being: handle to the queue, pointer to the buffer holding a copy of the receive item, the maximum amount of time the task
														   //should block waiting for an item should the queue be empty at the time of the call.
	   if ( req.req_kind == INT_WANTED ) //Condition checking if the item is equal to the enumerator INT_WANTED.
		  {
		    uniqueIntPtr = (int*)req.unique_ptr; //Typecast value to an int and store in uniqueintptr variable.
		    *uniqueIntPtr = i; 
		    xSemaphoreTake(screenMutex,portMAX_DELAY); //Use screenmutex with portmax_delay as timeout interval.
		      LCD_DisplayStringLine(LINE(5), (uint8_t*) &chi); //Show the value of variable chi on the LCD.
		    xSemaphoreGive(screenMutex); //Release the screenmutex making it signaled(available).
		    i++; //Increment i by 1.
		    chi[0] = '0' + i; //Assign 0 + i to be variable chi.
		    if ( i == 10 ) //If i == 10.
		 	   {
		 		   vTaskDelay(portMAX_DELAY); //Delay task for ever
		 	   }
		  }
	   if ( req.req_kind == CHAR_WANTED ) //Condition checking if the item is equal to the enumerator CHAR_WANTED.
	  	  {
		    uniqueCharPtr = (char*)req.unique_ptr; //Typecast value to a char and store in uniquecharptr variable.
	  		*uniqueCharPtr = ch;
	  		xSemaphoreTake(screenMutex,portMAX_DELAY); //Use screenmutex with portmax_delay as timeout interval.
	  		   LCD_DisplayStringLine(LINE(4), (uint8_t*) &chch); //Show the value of variable chch on the LCD.
	  		xSemaphoreGive(screenMutex); //Release the screenmutex making it signaled(available).
	  		ch = ch + 1; 
	  		chch[0] = ch;
	  		if ( ch ==  'z'+1) //Check if letter is 1 beyond z
	     	   {
	  	    	   vTaskDelay(portMAX_DELAY); //Delay task by portmax_delay
	  		   }
	  	   }
	}
}

static void intClient( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType two_sec = ( 2000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();

	struct Req req;
	req.req_kind = INT_WANTED;
	req.unique_ptr = &uniqueInt; //Reference to static int uniqueint.
    char chi[2] = "0";
    LCD_DisplayStringLine(LINE(6), (uint8_t*)" intClient: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,	(void*)&req ,portMAX_DELAY); //Post an item on queue. The item is queued by copy not reference. Arguments: The handle to the queue, a pointer to the item that is to be placed on the queue, the maximum time the task
															  //should block waiting for space to become available on the queue, should it already be full.
		xSemaphoreTake(screenMutex,portMAX_DELAY); //Use screenmutex with portmax_delay as timeout interval.
		  chi[0] = '0' + uniqueInt; //store '0' + uniqueint value in chi variable.
		  LCD_DisplayStringLine(LINE(7), (uint8_t*) &chi); //Print value for chi on LCD.
		xSemaphoreGive(screenMutex); //Release screenmutex.
		vTaskDelayUntil( &xNextWakeTime, two_sec*3 ); //Delay task until specific time. First argument being a pointer to a variable that holds the time at which the task was last unblocked. Second argument being the cycle time period and the task
													  //will be unblocked at time: previouswaketime*timeincrement.
	}
}

static void char1Client( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType one_sec = ( 1000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();
	struct Req req;
	req.req_kind = CHAR_WANTED;
	req.unique_ptr = &unique1Char;
    char chi[2] = "a";
    LCD_DisplayStringLine(LINE(8), (uint8_t*)"char1Client: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,(void*)&req,portMAX_DELAY);
		xSemaphoreTake(screenMutex,portMAX_DELAY);
		  chi[0] = unique1Char;
		  LCD_DisplayStringLine(LINE(9), (uint8_t*) &chi);
		xSemaphoreGive(screenMutex);
		vTaskDelayUntil( &xNextWakeTime, one_sec );
	}
}

static void char2Client( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType two_sec = ( 2000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();
	struct Req req;
	req.req_kind = CHAR_WANTED;
	req.unique_ptr = &unique2Char;
    char chi[2] = "a";
    LCD_DisplayStringLine(LINE(10), (uint8_t*)"char2Client: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,(void*)&req,portMAX_DELAY);
		xSemaphoreTake(screenMutex,portMAX_DELAY);
		  chi[0] = unique2Char;
		  LCD_DisplayStringLine(LINE(11), (uint8_t*) &chi);
		xSemaphoreGive(screenMutex);
		vTaskDelayUntil( &xNextWakeTime, two_sec );
	}
}


/*-----------------------------------------------------------*/
// rest of freeRtos stuff begin
/*
static void vExampleTimerCallback( xTimerHandle xTimer )
{

	ulCountOfTimerCallbackExecutions++;
}
*/

void vApplicationTickHook( void )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
static uint32_t ulCount = 0;

	/* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
	1 in FreeRTOSConfig.h.

	"Give" the semaphore on every 500th tick interrupt. */
	ulCount++;
	if( ulCount >= 500UL )
	{
		/* This function is called from an interrupt context (the RTOS tick
		interrupt),	so only ISR safe API functions can be used (those that end
		in "FromISR()".

		xHigherPriorityTaskWoken was initialised to pdFALSE, and will be set to
		pdTRUE by xSemaphoreGiveFromISR() if giving the semaphore unblocked a
		task that has equal or higher priority than the interrupted task.
		http://www.freertos.org/a00124.html */
		xSemaphoreGiveFromISR( xEventSemaphore, &xHigherPriorityTaskWoken );
		ulCount = 0UL;
	}

	/* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
	normally be performed before leaving the interrupt (because during the
	execution of the interrupt a task of equal or higher priority than the
	running task was unblocked).  The syntax required to context switch from
	an interrupt is port dependent, so check the documentation of the port you
	are using.  http://www.freertos.org/a00090.html

	In this case, the function is running in the context of the tick interrupt,
	which will automatically check for the higher priority task to run anyway,
	so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}


/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* TODO, implement your code here */
  while (1)
  {
  }
}
