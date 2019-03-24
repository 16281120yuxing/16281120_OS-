#include "stdio.h"
#include "sys/types.h"
#include "sys/time.h"
#include "unistd.h"
#include "stdlib.h"

#define HASPRO -10

int main()
{
        pid_t p1,p2,p3,p4,p5;

            int cnt=0;

                while((p1=fork()) == -1)
                        sleep(0.1);

                    if(!p1)
                    {
                                while((p2=fork()) == -1)
                                            sleep(0.1);

                                        if(!p2)
                                        {
                                                        while ((p4=fork())==-1);
                                                                    if (!p4)
                                                                    {
                                                                                        while(1)
                                                                                        {
                                                                                                                printf("Node p4 is p2's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
                                                                                                                                    sleep(0.1);
                                                                                                                                                    
                                                                                        }
                                                                                                    
                                                                    }
                                                                    else
                                                                    {
                                                                                        while ((p5=fork())==-1)
                                                                                                        sleep(0.1);

                                                                                                        if (!p5)
                                                                                                        {
                                                                                                                                while(1)
                                                                                                                                {
                                                                                                                                                            printf("Node p5 is p2's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
                                                                                                                                                                                    sleep(0.1);
                                                                                                                                                                                                        
                                                                                                                                }

                                                                                                                                                
                                                                                                        }

                                                                                                                    
                                                                    }

                                                                    while(1)
                                                                    {
                                                                                        printf("Node p2 is p1's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
                                                                                                        sleep(0.1);
                                                                                                                //exit(0);
                                                                                                                //            
                                                                    }
                                                                            while(1)
                                                                            {
                                                                                            printf("Node p1 is parent with pid %d, it's parent pid %d.\n",getpid(),getppid());
                                                                                                        sleep(0.1);
                                                                                                                
                                                                            }
                                                                                    
                                        }
                                                else
                                                {
                                                                ;
                                                                        
                                                }

                                                        
                                                    
                    }
                        else
                        {
                                    while ((p3=fork())==-1)
                                                    sleep(0.1);

                                                    if (!p3)
                                                    {
                                                                            while(1)
                                                                            {
                                                                                                        printf("Node p3 is p1's child with pid %d, it's parent pid %d.\n",getpid(),getppid());
                                                                                                                                sleep(0.1);
                                                                                                                                                    
                                                                            }

                                                                                            
                                                    }
                                                        
                        }

                            return 0;

}
