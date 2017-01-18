/*=============================================================================

CS 50300 Operating Systems Lab4 - Multi-Threaded program (Matrix Multiplication)

matmul.c Source code		

Kartik Sooji (ksooji)

=============================================================================*/

There are following functionalities that are supported by the program:

get_rand(int n)
Init_Mat() 
Sequential(double ***Mat)
do_Thread(void * id)
MultiThread()
main(int argc, char* argv[])


get_rand(int n):
This function is used to randomly generate the numbers.

Init_Mat():
~~~~~~~~~~~
This function dynamically allocates the memory for the matrix and initialises the matrix elements that are generated randomly.

Sequential(double ***Mat):
~~~~~~~~~~~~~~~~~~~~~~~~
This function multiplies two matrices in a traditional approach which calculates c[i][j] consists with three loops.

do_Thread(void * id):
~~~~~~~~~~~~~~~~~~~
This function is applicable for the no of threads created to perform matrix multiplication. Here each thread computes only one value of c[i][j] at a time. This is achieved by applying lock on the row and column variables i.e., each thread reads only one value of row and column and increments the same for successive threads to overcome the race condition among threads.

MultiThread():
~~~~~~~~~~~~~
-Create the requested no of threads.
-Join all the threads with the return values.
-Free all the threads variables.
-Provides with error handling.

main(int argc, char* argv[]):
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-This function reads and validates the user inputs.
-Creates the memory for ring buffer for the threads to operate on.
-Calls both the Sequential() and Multithread() functions.
-Both the resultant matrices are compared and expected output is printed.



Exceution Steps :

1) Change the current working directory to the source code location
2) Make sure there is a ".c" files
3) Type make and hit enter
4) Provide appropriate inputs as prompted
5) Once the program is done run make clean 
