#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

int 	Max_Sleep_Secs ;
int 	Total_Items2Produce ;
int 	Ring_Buffer_Size ;
int 	Empty_Count ;
int 	mutex 		= 1 ;
int 	Full_Count	= 0 ;
int 	Count_Consumed 	= 0 ;
int	Count_Produced 	= 0 ;
int 	global_id	= 0 ;


// Data Sturctue to store the items produced 
struct RingBuffer
{
	int *buffer;
	int head;
	int tail;
}*rbuf;


// Random number generator
int random_num(int n)
{
	return ( ((double)rand()/(double)RAND_MAX) * n) ; 
}

/**
Semaphore Implementation with Pthread mutex, Condition Variable, P() & V()
**/

// Intializing the locks and condition variables
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER; 

int P(int *S)
{

	pthread_mutex_lock(&lock);
	
	while( (*S) <= 0)
	{
		//printf("Waiting\n");
		pthread_cond_wait(&cv, &lock);		//Waiting Signal
	}
	(*S)--;
	pthread_mutex_unlock(&lock);

}

int V(int *S)
{

	pthread_mutex_lock(&lock);
	(*S)++;
	pthread_cond_signal(&cv);			//Wakeup Signal
	pthread_mutex_unlock(&lock);
	//printf("done Waiting\n");
}

/**
Semaphore Implementation ends
**/


// Producer Do Thread function.
void* Producer(void *id)
{
	int 	next_produced, tm;
	int* 	id1 = (int*)id;
	int 	idp = -1;

	srand(time(NULL));				//To differ the seed


	while(1)
	{
		// producing data 
		next_produced = random_num(50);

		
		//Acquiring Locks
		P(&Empty_Count);
		P(&mutex);

		// Critical Section - Begin 

		// Unique Ids for Each Thread
		if(idp == -1)
		{
			
			idp = global_id++;
			// UnComment the print statement to check the Id for Producer Thread	
			// printf("Producer: Id = %d \n ",id);
		}

		// Release the locks when all the item are produced
		if(Count_Produced >= Total_Items2Produce )
		{
			V(&mutex);
			V(&Full_Count);
			break;		
		}
	
		//Writing the produced data to ring buffer
		rbuf->buffer[rbuf->head] = next_produced ;
		rbuf->head = (rbuf->head + 1) % Ring_Buffer_Size;
		
		// Maintain the count of the items produced!
		Count_Produced++;


		// Critical Section - End

		// Releasing Locks
		V(&mutex);		
		V(&Full_Count);

		// Sleep for random seconds to make other threads execute
		tm = random_num(Max_Sleep_Secs);
		sleep(tm);

	}

//	printf("Id = %d \n ",*id1);
//	printf("Id = %d,prod _Count = %d \n ",id,Count_Produced);

	return NULL;
}


// Consumer Do Thread function.
void* Consumer(void *id)
{
	int 	next_consumed, tm;
	int* 	id2 = (int*)id;
	int 	idc = -1;


	srand(time(NULL));				//To differ the seed


//	printf("Total Items to consume:%d\n",Total_Items2Produce);	
	while(1)
	{
		
//		printf("Consumer: Full Count = %d \t Empty Count = %d\n",  Full_Count, Empty_Count);

		// Sleep for random seconds to make other threads execute
		tm = random_num(Max_Sleep_Secs);
		sleep(tm);

		//Acquiring Locks
		P(&Full_Count);
		P(&mutex);
		
		// Critical Section - Begin 

		// Unique Ids for Each Thread
		if(idc == -1)
		{
			idc = global_id++;
			// UnComment the print statement to check the Id for Consumer Thread	
			// printf("Consumer: Id = %d \n ",id);
		}
		if(Count_Consumed >= Total_Items2Produce )
		{
			V(&mutex);
			V(&Empty_Count);			
			break;
		}
	
		//reading the produced data out of ring buffer	
		next_consumed = rbuf->buffer[rbuf->tail];
		rbuf->tail = (rbuf->tail + 1) % Ring_Buffer_Size;
		

		// Maintain the count of the items consumed!
		Count_Consumed++;

		// Critical Section - End


		// Releasing Locks
		V(&mutex);
		V(&Empty_Count);		


	}

//	printf("CId = %d \n ", *id2);
//	printf("Id = %d,con _Count = %d \n ",id,Count_Consumed);
	
	return NULL;

}

int main(int argc, char* argv[])
{
	int		i, j;
	int 		No_of_Producers, No_of_Consumers;
	int             *ptids, *ctids;  
  	pthread_t       *pthrds, *cthrds;
	pthread_attr_t  *pattrs, *cattrs;
	void            *pretval, *cretval;
	
	if(argc != 6)
	{
		perror("Invalid Number of arguemnts\n");
		exit(1);
	}

	No_of_Producers		= atoi(argv[1]);
	No_of_Consumers 	= atoi(argv[2]);
	Max_Sleep_Secs 		= atoi(argv[3]);
	Total_Items2Produce 	= atoi(argv[4]);
	Ring_Buffer_Size 	= atoi(argv[5]);
	
	

	// Validating Inputs
	if(No_of_Producers < 2)
	    {
	      fprintf(stderr, "arg 2 : Atleast 2 producer processes for multithread program \n");
	      exit(1);
	    }

	if(No_of_Consumers < 2)
	    {
	      fprintf(stderr, "arg 3 : Atleast 2  consumer processes for multithread program \n");
	      exit(1);
	    }
	if(Max_Sleep_Secs < 1)
	    {
	      fprintf(stderr, "arg 4 : Sleep cycle should atleast be 1 Sec \n");
	      exit(1);
	    }
	if(Total_Items2Produce <= 0)
	    {
	      fprintf(stderr, "arg 5 : Atleast 1 item needs to be produced and consumed\n");
	      exit(1);
	    }
	if(Ring_Buffer_Size <= 0)
	    {
	      fprintf(stderr, "arg 6 : Buffer Memory cant be Zero or less\n");
	      exit(1);
	    }


	Empty_Count 		= Ring_Buffer_Size;
	

	// Allocating Memory for the Ring Buffer
	rbuf 		= malloc(sizeof(struct RingBuffer));
	rbuf->buffer 	= calloc(Ring_Buffer_Size,sizeof(int));
	rbuf->head 	= 0;
	rbuf->tail 	= 0;	
	
	pthrds 			= (pthread_t*) 	    malloc(sizeof (pthread_t)* No_of_Producers);
	pattrs			= (pthread_attr_t*) malloc(sizeof (pthread_attr_t)* No_of_Producers);
	ptids  			= (int*) 	    malloc(sizeof (int)* No_of_Producers);

	cthrds 			= (pthread_t*) 	    malloc(sizeof (pthread_t)* No_of_Consumers);
	cattrs			= (pthread_attr_t*) malloc(sizeof (pthread_attr_t)* No_of_Consumers);
	ctids  			= (int*) 	    malloc(sizeof (int)* No_of_Consumers);


	//To differ the seed
	srand(time(NULL));				


	// Create Producer threads
	for(i = 0; i < No_of_Producers; i++) 
	{
		if(pthread_attr_init(pattrs+i)) 
			perror("Producer attr_init()");
		
		ptids[i] = i;
		
		if(pthread_create(pthrds+i, pattrs+i, Producer, ptids+i) != 0) 
			perror("Producer pthread_create()");

	}
	
	// Create Consumer threads
	for(j = 0; j < No_of_Consumers; j++) 
	{
		if(pthread_attr_init(cattrs+j)) 
			perror("Consumer attr_init()");
		
		ctids[j] = j;
		
		if(pthread_create(cthrds+j, cattrs+j, Consumer, ctids+j) != 0) 
			perror("Consumer pthread_create()");
	}
 	
	// join threads
	for(i = 0; i < No_of_Producers; i++)
		pthread_join(pthrds[i], &pretval);

	for(j = 0; j < No_of_Consumers; j++)
		pthread_join(cthrds[j], &cretval);

       	fflush(stdout);

	printf("Number of Items Produced : %d\n",  Count_Produced);
	printf("Number of Items Consumed : %d\n",  Count_Consumed);
	
	free(pattrs);
	free(pthrds);
	free(ptids);
	
	free(cattrs);
	free(cthrds);
	free(ctids);
	
	return 0;
	
}
