
#include <iostream>
#include <cstdlib>
//#include <cstdio>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <typeinfo>
#include <mpi.h>
#include <sstream>
#include "./../include/Bunny.hpp"
#include "./../include/Animal.hpp"
#include "./../include/Teddy.hpp"
#include "./../include/Meadow.hpp"
#include "./../include/Constants.hpp"
#include "./../include/PartyRequest.hpp"

using namespace std;

int main(int argc, char **argv)
{
    const string wrongInitializationMsg = " mpirun -np [number of processes] ./forest [number of bunnies] [number of teddies] [number of processes] \n \
    Number of processes must equal number of bunnies + number of teddies";
    Animal* me;
    vector <Meadow*> meadows;
    int numOfThreads,tid;
	
    MPI_Status status;
    MPI_Init(&argc, &argv); 

    MPI_Comm_size( MPI_COMM_WORLD, &numOfThreads );
    MPI_Comm_rank( MPI_COMM_WORLD, &tid );

    int bunnyCount = 10;
    int teddyCount = 5;
    int meadowCount = 5;
    
    bool startProgram = true;
    
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
		cerr << "Argv conversion unsuccessful" << endl;
		//MPI_Abort(MPI_COMM_WORLD, -1);
	    }
	    startProgram = false;
	}
    }

    if ( (numOfThreads != bunnyCount + teddyCount) || (bunnyCount < 0) ||
	(teddyCount < 0) || (meadowCount <1) )
    {
	if (tid == 0)
	{
	    cerr << wrongInitializationMsg << endl;
	    //MPI_Abort(MPI_COMM_WORLD, -1);
	}
	startProgram = false;
    }
	
    //if no errors were made during initialization
    if (startProgram)
    {
	//if first bunny
	if (tid == 0)
	{
		srand(time(NULL));

		for (int i = 0; i < meadowCount; ++i)
		{
		  meadows.push_back(new Meadow(i, rand() % maxMeadowCapacity));
	
		}
		
		//broadcasting meadows
		for (int thNum = 1; thNum < numOfThreads; thNum++)
		{
		  for (int i=0; i< meadowCount; i++)
		  {
		    
		    int toSend[2];
		    toSend[0] = meadows.at(i)->getId();
		    toSend[1] = meadows.at(i)->getCapacity();
		    MPI_Send(&toSend, 2, MPI_INT, thNum, 100, MPI_COMM_WORLD);
		  }
		}
		  
	  }
	else //if not tid 0
	{
	    int toRecieve[2] = {0,0};
	    for (int i=0; i< meadowCount; i++)
	    {
		MPI_Recv(&toRecieve, 2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	  
		//cout << "tid = " <<  tid << ",recieved " << toRecieve[0] <<  "+" << toRecieve[1] << endl;
		meadows.push_back(new Meadow(toRecieve[0], toRecieve[1]));
	    }
	}
	
	if (tid < bunnyCount)
	{
	    me = new Bunny(tid);
	}
	else
	{
	    me = new Teddy(tid);
	}	
	me->setMeadows(meadows);
	me->setCounts(numOfThreads, bunnyCount, teddyCount, meadowCount);
	me->party();
	
	//me -> broadcastPartyMsg();
	//me -> handleRecievingMessages();
	
    }

    MPI_Finalize();
}



