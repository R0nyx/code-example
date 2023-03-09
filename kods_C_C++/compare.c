<<<<<<< HEAD
#include <cs50.h> 
#include <stdio.h> 
=======
#include <cs50.h>
#include <stdio.h>
>>>>>>> 9cc4a729c7a9e09b5522ad4ba779a0f6056fb1e5

int main(){
    int x = get_int("What's x?");
    int y = get_int("What's y?");
<<<<<<< HEAD
=======

    if (x > y)
    {
        printf("%d is larger than %d\n", x,y);
    }

    else if (x < y)
    {
        printf("%d is larger than %d\n", y,x);
    }
    
    // or just say (else {})
    if (x == y)
    {
        printf("The values are equal\n");
    }
>>>>>>> 9cc4a729c7a9e09b5522ad4ba779a0f6056fb1e5
}