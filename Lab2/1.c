#include <unistd.h> 
#include <stdio.h>
int main (){
    pid_t fpid; 
    int count=0;
    fpid=fork(); 
    if (fpid < 0)
        printf("error in fork!"); 
    else if (fpid == 0) {
        execl("/usr/bin/vi","vi",NULL); 

    }
    else { for(;;){}
    }
    return 0;

}
