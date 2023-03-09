#include "screen_monitor.h"

//One lines init mutex in linux 
pthread_mutex_t monMutex = PTHREAD_MUTEX_INITIALIZER; //Initialize mutex by using initialization macro.
// MonMutex is private, and functions are public. 

//FUNCTION DEFINITIONS
void write_string(char* str) //Function that prints string.
{
        //LOCSK THE MONITOR MUTEX
        pthread_mutex_lock(&monMutex); //Locks the mutex while work is done.
        printf("%s", str); //Print the string.
        pthread_mutex_unlock(&monMutex); //Release the mutex.
}

void write_number(int num) //Function that prints number.
{
        pthread_mutex_lock(&monMutex);
        printf("%d", num);
        pthread_mutex_unlock(&monMutex);
}

void new_line(void)
{
        pthread_mutex_lock(&monMutex);
        printf("\n");
        pthread_mutex_unlock(&monMutex);
}

void write_string_and_number(char* str, int num) //Function that prints both string and number.
{
        pthread_mutex_lock(&monMutex);
        printf("%s %d", str, num);
        pthread_mutex_unlock(&monMutex);
}
