#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

// Global Variables
int Mat_Size;
int No_of_Threads;
int rw = 0, cl = 0;

double **I_Matrix;		// Initial/Input matrix
double **Pt_Matrix;		// Multithread matrix

// mutex lock for one resource i.e., mat[rw][cl] 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Random number generator
double get_rand(int n) 
{ 
  return  ( ((double) rand()) / RAND_MAX ) * n;
}


// Matrix Initialization 

void Init_Mat() 
{

    int i,j,k;

    // Dynamic allocation of the memory for the Matrix
    I_Matrix = (double **) malloc(sizeof(double *) * Mat_Size);
    for (k = 0; k < Mat_Size; k++) 
        I_Matrix[k] = (double *) malloc(sizeof(double) * Mat_Size);
    

    //Initializing the values of the matrix randomly
    for (i = 0; i < Mat_Size; i++) 
    {
        for (j = 0; j < Mat_Size; j++)
            I_Matrix[i][j] = get_rand(100);
    }   
  
}

// Sequential Matrix Multiplication function
void Sequential(double ***Mat)
{
	
	int i,j,k;
	double sum;
		
	// Dynamic allocation of the memory for the Matrix
    	(*Mat) = (double **) malloc(sizeof(double *) * Mat_Size);
    	for (k = 0; k < Mat_Size; k++) 
        	(*Mat)[k] = (double *) malloc(sizeof(double) * Mat_Size);	
	
	// Sequential Multiplication Operation
	for(i = 0; i < Mat_Size ; i++)
	{
		for(j = 0; j < Mat_Size ; j++)
		{
			sum = 0.0;			
			for (k = 0; k < Mat_Size ; k++)
				sum += I_Matrix[i][k] * I_Matrix[k][j];
			(*Mat)[i][j] = sum;
		}
	}	
} 


void* do_Thread(void * id)
{

	int i,r,c ;
	int* id1 = (int*)id;
	double sum;

	while (1){


		//Acquiring Locks
		pthread_mutex_lock(&lock);

		//Store the i,j values to calculate C[i][j]
		r = rw;
		c = cl;

		// Increment row value of the matrix and set column to zero for the 
		if(cl >= Mat_Size ){	
			rw++;
			cl = 0;
		}
		// Increment column value of the matrix
		else
			cl++;

		pthread_mutex_unlock(&lock);
		//Releasing Locks


		// Exit Condtion
		if (r >= Mat_Size  )			
			break;

		//calculating C[i][j]
		sum = 0.0;
		for( i = 0 ; i < Mat_Size ; i++)
			sum += I_Matrix[r][i] * I_Matrix[i][c];
		Pt_Matrix[r][c] = sum;
	}

	//sleep(2);
	//pthread_exit();
	return NULL;

}


void MultiThread()
{
	int		i ;
	int             *tids;  
  	pthread_t       *thrds;
	pthread_attr_t  *attrs;
	void            *retval = NULL;
	
	thrds 			= (pthread_t*) 	    malloc(sizeof (pthread_t)* No_of_Threads);
	attrs			= (pthread_attr_t*) malloc(sizeof (pthread_attr_t)* No_of_Threads);
	tids  			= (int*) 	    malloc(sizeof (int)* No_of_Threads);

	// Create threads
	for(i = 0; i < No_of_Threads; i++) 
	{
		if(pthread_attr_init(attrs+i)) 
		{
			perror(" attr_init()");
		}
		
		tids[i] = i;
		
		if(pthread_create(thrds+i, attrs+i, do_Thread, tids+i) != 0) 
		{ 
			perror(" pthread_create()");
			exit(1);
		}
		
	}
	
 	//printf("threads created\n");
	// join threads

	
	for(i = 0; i < No_of_Threads; i++)
		pthread_join(thrds[i], &retval);
		
	//sleep(1);
	//printf("task completed\n");
	free(attrs);
	free(thrds);
	free(tids);

	return;
	
}


int main(int argc, char* argv[])
{

	double 	**Seq_Res, err = 0.0000001;
	int 	i, j, flag = 0; 
	
	if(argc != 3)
	{
		fprintf(stderr, "argc : Invalid number of arguements \n");
		exit(1);
	}

	Mat_Size		= atoi(argv[1]);
	No_of_Threads	 	= atoi(argv[2]);	
	
	if(Mat_Size < 2)
	{
	      fprintf(stderr, "arg 2 : Matrix size be alteast 2X2 \n");
	      exit(1);
	}

	if(No_of_Threads < 2)
	{
	      fprintf(stderr, "arg 3 : Atleast 2 threads for multithread program \n");
	      exit(1);
	}

    	Pt_Matrix = (double **) malloc(sizeof(double *) * Mat_Size);
    	for (i = 0; i < Mat_Size; i++) 
        	Pt_Matrix[i] = (double *) malloc(sizeof(double) * Mat_Size);
	
	Init_Mat();					
	
	MultiThread();
	Sequential(&Seq_Res);


	fflush(stdout);
	//sleep(2);

	for(i=0;i<Mat_Size;i++){
        	for(j=0;j<Mat_Size;j++){
			//if(fabs(Seq_Res[i][j] - Pt_Matrix[i][j]) > err){
			if(Seq_Res[i][j] != Pt_Matrix[i][j]){				
				flag = 1;
			        //printf("%d\t%d\t%f\t%f\n", i,j, Seq_Res[i][j], Pt_Matrix[i][j]);
				break;
			}
		}
		//if(flag) break;
	} 
	
	if (!flag)
		printf("\nSUCCESS : Sequential and Threaded resultant matrices are same\n\n");
	else
		printf("\nOOPS!!!!....Sequential and Threaded resultant matrices are Different\n\n");

	return 0;
	
}
