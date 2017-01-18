/*=============================================================================

CS 50300 Operating Systems Lab4 - Multi-Threaded program (Multi-Producer Multi-Consumer problem)

MP_MC.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/


There are following functionalities that are supported by the program:

random_num(int n)
P(int *S)
V(int *S)
Producer(void *id)
Consumer(void *id)
main(int argc, char* argv[])


random_num(int n):
~~~~~~~~~~~~~~~~~
This function is used to randomly generate the numbers.

P(int *S):
~~~~~~~~~
This function locks the mutex on a condition variable due to which no resources are held for the other running processes i.e., to interleave multi-thread processing.

V(int *S):
~~~~~~~~~
This function unlocks the condition variable by broadcasting pthread_cond_signal.

Producer(void *id):
~~~~~~~~~~~~~~~~~~
Using the above semaphore implemented, the producer function generates random items as per the limit and appends to the head of the ring buffer. The function also produces only the no of items requested by the user.

Consumer(void *id):
~~~~~~~~~~~~~~~~~~
Using the above semaphore implemented, the consumer function reads the items from the tail of the ring buffer. The function also consumes only the no of items produced accordingly. Each consumer thread sleeps for random time which is in between maximum sleeps seconds.

main(int argc, char* argv[]):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-This function reads and validates the user inputs.
-Creates the memory for ring buffer for the threads to operate on.
-Create the requested no of producer and consumer threads.
-Join all the producer and consumer threads with the return values.
-Free all the producer and consumer threads variables.
-Provides with error handling.


Exceution Steps :

1) Change the current working directory to the source code location
2) Make sure there is a ".c" files
3) Type make and hit enter
4) Provide appropriate inputs as prompted
5) Once the program is done run make clean 
