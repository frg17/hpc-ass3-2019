#ifndef __ship_h__
#define __ship_h__

#include <mpi.h>


typedef struct Ship {
	MPI_Comm communicator;
	int coords[2];
	int otherCoords[2];
	int rank;
	int map[6][6];
} Ship;


Ship *Ship_create();
void Ship_setup(Ship *ship, MPI_Comm communicator);
void Ship_iterate(Ship *ship);
void Ship_net(Ship *ship);






#endif
