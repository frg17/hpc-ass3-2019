#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "./tile.h"
#include "./ship.h"
#define SIZE 16

#define MAX_ITERATIONS 20

int main (int argc, char** argv) {
	int numtasks, rank;
	int cycle = 0;

	MPI_Comm cartcomm, carts, boats;
	Tile *tile = Tile_create();
	Ship *ship;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank < 36) {
		MPI_Comm_split(MPI_COMM_WORLD, 1, rank, &carts);
	} else {
		MPI_Comm_split(MPI_COMM_WORLD, 2, rank, &boats);
	}

	if (rank < 36) {
		Tile_setup(tile, carts, &cartcomm);
		Tile_getNeighbourTypes(tile, cartcomm);
	} else {
		ship = Ship_create();
		Ship_setup(ship, boats);
	}	
	
	srand(rank);

	while (cycle < MAX_ITERATIONS) {
		if (rank < 36) {
			Tile_iterate(tile, cartcomm);
		} else {
			Ship_iterate(ship);
		}
	
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);

		cycle++;
		if (rank == 0) {
			printf("Cycle %d finished\n\n", cycle);
		}
		
		MPI_Barrier(MPI_COMM_WORLD);
		

	}

	MPI_Finalize();

	return 0;
}


