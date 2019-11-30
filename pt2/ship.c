#include <stdlib.h>
#include <stdio.h>
#include "./ship.h"

Ship *Ship_create() {
	return (Ship *) malloc(sizeof(Ship));
}

void Ship_setup(Ship *ship, MPI_Comm communicator) {
	ship->communicator = communicator;
	MPI_Comm_rank(communicator, &ship->rank);
	if (ship->rank == 0) {
		ship->coords[0] = 0; ship->coords[1] = 1;
	} else if (ship->rank == 1) {
		ship->coords[0] = 1; ship->coords[1] = 0;
	}
	
	int x, y;	
	for (int i = 0; i < 36; i++) {
		x = i / 6;
		y = i % 6;	
		ship->map[x][y] = i;
	}
}

void Ship_iterate(Ship *ship) {
	//Send other ship location.
	MPI_Status status;	
	int x, y, rx, ry;
	int nx, ny;
	if (ship->rank == 0) {
		MPI_Send(&ship->coords[0], 2, MPI_INT, 1, 0, ship->communicator);
		MPI_Recv(&ship->otherCoords[0], 2, MPI_INT, 1, 0, ship->communicator, &status);

		x = ship->coords[0]; y = ship->coords[1];
		while(1) {
			rx = (rand() % 3) - 1;	
			nx = x + rx;	
			if (nx < 0 || nx > 5) continue;
			
			ry = (rand() % 3) - 1;	
			ny = y + ry;
			if (ny < 0 || ny > 5) continue;
			

			if (nx == 0 && ny == 0) continue;	
			
			
			if (nx == ship->otherCoords[0] && ny == ship->otherCoords[1]) continue;
			ship->coords[0] = nx; ship->coords[1] = ny;
			break;
		}

	} else if  (ship->rank == 1) {
		MPI_Recv(&ship->otherCoords[0], 2, MPI_INT, 0, 0, ship->communicator, &status);
		x = ship->coords[0]; y = ship->coords[1];
		
		//Next x coord
		while(1) {
			rx = (rand() % 3) - 1;	
			nx = x + rx;	
			if (nx < 0 || nx > 5) continue;
			
			ry = (rand() % 3) - 1;	
			ny = y + ry;
			if (ny < 0 || ny > 5) continue;

			if (nx == 0 && ny == 0) continue;	
			
			if (nx == ship->otherCoords[0] && ny == ship->otherCoords[1]) continue;

			ship->coords[0] = nx; ship->coords[1] = ny;
			break;
		}

		MPI_Send(&ship->coords[0], 2, MPI_INT, 0, 0, ship->communicator);
		
	}

	printf("Ship %d at %d, %d\n", ship->rank, ship->coords[0], ship->coords[1]);
	
	MPI_Barrier(MPI_COMM_WORLD); //Sync world
	
	Ship_net(ship);
	
}

void Ship_net(Ship *ship) {
	int fishing = 0;
	int x = ship->coords[0]; int y = ship->coords[1];
	MPI_Request req;
	MPI_Status status;
	int dest = ship->map[x][y];

	MPI_Isend(&fishing, 1, MPI_INT, dest, 3, MPI_COMM_WORLD, &req);

	MPI_Wait(&req, NULL);

	MPI_Barrier(MPI_COMM_WORLD); //Sync world

	MPI_Recv(&fishing, 1, MPI_INT, dest, 3, MPI_COMM_WORLD, &status);
	printf("%d fish netted\n", fishing);
}




