#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>

#include "sort.c"

#define maxMeadowCapacity 20

#define TAG_JOIN 1
#define TAG_ENTER 2
#define TAG_LEAVE 3
#define TAG_RESPONSE 4

#define MSG_RESPONSE 0
#define MSG_REQUEST 1

#define MIN_ANIMAL_NUM



pthread_mutex_t myDataMutex;
pthread_mutex_t recievedDataMutex;
pthread_mutex_t mpiMutex;
pthread_mutex_t queueMutex;
int bunnyCount = 10;
int teddyCount = 5;
int meadowCount = 5;

int numOfThreads,tid;
MPI_Status status;


int * meadows;
int * initMeadows;


int animal[5];
//0 - animal id;
//1 - animal size;
//2 - meadow id;
//3 - lampost
//4 - just in case

void broadcastRequests(){

	MPI_Status rcvStatus;
	int i = 0;
	for(i = 0; i < numOfThreads; i++) {
		if(i == tid)
			continue;		

		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
		MPI_Isend(animal, 5, MPI_INT, i, TAG_JOIN,
              MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
			int flag;
			MPI_Test(&request, &flag, &rcvStatus);
			if(flag > 0){
				pthread_mutex_unlock(&mpiMutex);
				break;
			}
			pthread_mutex_unlock(&mpiMutex);
			usleep(100000);
		}
		sleep(1);
	}

}

void sendConfirmation(int toWhom){

	MPI_Status rcvStatus;
	
	MPI_Request request;

	pthread_mutex_lock(&myDataMutex);
	animal[3] = animal[3] + 1;
	pthread_mutex_unlock(&myDataMutex);

	pthread_mutex_lock(&mpiMutex);
	int ret = MPI_Isend(animal, 5, MPI_INT, toWhom, TAG_RESPONSE, MPI_COMM_WORLD, &request);
	pthread_mutex_unlock(&mpiMutex);
	while(1) {
		pthread_mutex_lock(&mpiMutex);				
		int flag;
		MPI_Test(&request, &flag, &rcvStatus);
		if(flag > 0 && ret == MPI_SUCCESS ) {
			pthread_mutex_unlock(&mpiMutex);
			break;
		}
		pthread_mutex_unlock(&mpiMutex);
		usleep(100000);
	}
}

broadcastMeadowInOut(int tag){

	int i = 0;
	for(i = 0; i < numOfThreads; i++) {
		if(i == tid)
			continue;		

		MPI_Request request;
		//pthread_mutex_lock(&myDataMutex);
		pthread_mutex_lock(&mpiMutex);
		MPI_Isend(animal, 5, MPI_INT, i, tag,
              MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		//pthread_mutex_unlock(&myDataMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
			int flag;
			MPI_Test(&request, &flag, &status);
			if(flag > 0){
				pthread_mutex_unlock(&mpiMutex);
				break;
			}
			pthread_mutex_unlock(&mpiMutex);
			usleep(100000);
		}
	}


}


void tryParty(){
	int ** subArray;
	while (1){
		//subArray = getSubArray(animal[2], 5);
		/*
		 if ((sizeof(subArray) / ( 5 * sizeof(int)) ) >= MIN_ANIMAL_NUM){
		 	break;
		 }
		 */
	}
	usleep(3000000);
	//subArray = getSubArray(animal[2], 5);

}
void party() {

	printf("tid:%d: partying on meadow %d!\n", tid, animal[2]);
	broadcastMeadowInOut(TAG_ENTER);
	usleep(rand() % 6000000+2000000);
	printf("tid:%d: leaving meadow %d!\n", tid, animal[2]);
	broadcastMeadowInOut(TAG_LEAVE);
}

void iWannaParty(){
		srand(time(NULL));

	pthread_mutex_lock(&myDataMutex);
	animal[3] = animal[3] + 1;
	animal[2] = rand() % meadowCount;
	pthread_mutex_unlock(&myDataMutex);
	pthread_mutex_lock(&queueMutex);
	/////////////////////////////////////////////////TODO : IMPLEMENT FUNCTION!!!
	//addToQueue(animal);
	//////////////////////////////////////////////////////////////////////////////////////
	pthread_mutex_unlock(&queueMutex);
	broadcastRequests();

	//tryParty();

}


void *handleMsgRecieve() {

	while(1) {	
		MPI_Status rcvStatus;
		int *received = (int *) malloc(5 * sizeof(int));
		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
			MPI_Irecv(received , 5, MPI_INT, MPI_ANY_SOURCE, 
				MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
				int flag;
				MPI_Test(&request, &flag, &rcvStatus);
				if(flag > 0) {
					pthread_mutex_unlock(&mpiMutex);
					break;
				}
			pthread_mutex_unlock(&mpiMutex);
			usleep(1000000 + rand() % 1000);
		}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		
		//increment lamport clock
		animal[3] = animal[3] + 1;	

		printf("my tid = %d, recieved msg from %d with meadow %d tag %d and clock %d \n", animal[0], received[0], received[2], rcvStatus.MPI_TAG, received[3]);

		if (rcvStatus.MPI_TAG == TAG_JOIN){
			//addToQueue(received);
			sendConfirmation(received[0]);
		}
		else if (rcvStatus.MPI_TAG == TAG_ENTER){
			//handle meadow in
			//
		}
		else if (rcvStatus.MPI_TAG == TAG_LEAVE){
			//handle meadow out
			//
		}
		else if (rcvStatus.MPI_TAG == TAG_RESPONSE){
			
		}
	
	}
	pthread_exit(NULL);
}









int main(int argc, char **argv)
{

    int startProgram = 1;
	
    MPI_Status status;
    MPI_Init(&argc, &argv); 

    MPI_Comm_size( MPI_COMM_WORLD, &numOfThreads );
    MPI_Comm_rank( MPI_COMM_WORLD, &tid );
    

	if (argc >=4)
    { 
      	bunnyCount = atoi(argv[1]);
		teddyCount = atoi(argv[2]);
		meadowCount = atoi(argv[3]);
	
	if ( ((strcmp(argv[1], "0") != 0) && bunnyCount == 0) ||
	    ((strcmp(argv[2], "0") != 0) && teddyCount == 0) ||
	    ((strcmp(argv[3], "0") != 0) && meadowCount == 0) )
	{
	    if (tid == 0)
	    {
		printf("Argv conversion unsuccessful!!!!!");
		//MPI_Abort(MPI_COMM_WORLD, -1);
	    }
	    startProgram = 0;
	}
    }

    if ( (numOfThreads != bunnyCount + teddyCount) || (bunnyCount < 0) ||
	(teddyCount < 0) || (meadowCount <1) )
    {
		if (tid == 0)
		{
	    	printf("Argv conversion unsuccessful");
		}
	startProgram = 0;
    }

    
    //if no errors were made during initialization
    if (startProgram == 1)
    {
    	meadows = (int *) malloc(meadowCount * sizeof(int));
		//if first bunny
		if (tid == 0)
		{
			srand(time(NULL));
			int i;
			for (i = 0; i < meadowCount; ++i)
			{
			  meadows[i] = rand() % maxMeadowCapacity;
		
			}			

     		memcpy(&initMeadows, &meadows, meadowCount * sizeof(int));
/*
     		int c;
			for ( c = 0 ; c < meadowCount ; c++ )
     			printf("meadow %d size =  %d\n", c, meadows[c]);

     		for ( c = 0 ; c < meadowCount ; c++ )
     			printf("meadow %d size =  %d\n", c, initMeadows[c]);
*/

			int thNum;
			//broadcasting meadows to all but tid = 0
			for (thNum = 1; thNum < numOfThreads; thNum++)
			{

				int i=0;
				for (i=0; i< meadowCount; i++)
		  		{
		    
				    int toSend;
				    toSend = meadows[i];
				    MPI_Send(&toSend, 1, MPI_INT, thNum, 100, MPI_COMM_WORLD);
				}
				//MPI_Send(&meadows, meadowCount, MPI_INT, thNum, 100, MPI_COMM_WORLD);		  
			}
			  
		  }
		else //if not tid 0
		{

			int i=0;
			int toRecieve = 0;
		    for (i=0; i< meadowCount; i++)
		    {
				MPI_Recv(&toRecieve, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	  
				meadows[i] = toRecieve;
		    }
     		memcpy(&initMeadows, &meadows, meadowCount * sizeof(int));
		}

		if (tid < bunnyCount)
		{
		   animal[1] = 1;
		}
		else
		{
		   animal[1] = 4;
		}	
		animal[0] = tid;
		animal[3] = 1;
		animal[4] = 0;

		//create new thread with 
		pthread_t id;
	  	pthread_create(&id, NULL, handleMsgRecieve, 
	  		NULL);


		while(1) {
			printf("tid:%d: I'm sleeping\n", tid );
			usleep(rand() % 5000000 + 2000000);
			printf("tid:%d: I wanna party\n", tid);
			iWannaParty();
		}

    	pthread_exit(&id);
	
    }


    MPI_Finalize();
}



