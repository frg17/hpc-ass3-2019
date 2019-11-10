#include <stdio.h>

#include "tile.h"
#include "grid.h"

MPI_Datatype FISHTYPE;

void *Tile_create() {
	Tile *tile = (Tile *) malloc(sizeof(Tile));
	tile->type = TILE_WATER;
	for (int i = 0; i < 3; i++) {
		tile->fish[i] = NULL;
	}
	return tile;
}



/*
 * Gerir @tile að landi (tile er venjuelga water)
 */
void Tile_makeLand(Tile *tile) {
	tile->type = TILE_LAND;
}

/**
 * Setur upp @tile og finnur nágranna þess.
 */
void Tile_setup(Tile *tile, int num_dimensions, MPI_Comm *cartcomm) {
	//MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, &cartcomm);
	int periods[2] = {0, 0}, reorder = 1;
	
	int coords[2], x, y, dims[2];

	if(num_dimensions == 5)
	{
		dims[0] = 5, dims[1] = 5;
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, cartcomm);
		MPI_Comm_rank(*cartcomm, &tile->rank);
		MPI_Cart_coords(*cartcomm, tile->rank, 2, coords);
		x = coords[0]; y = coords[1];
	}
	else if(num_dimensions == 6)
	{
		dims[0] = 6, dims[1] = 6;
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, cartcomm);
                MPI_Comm_rank(*cartcomm, &tile->rank);
                MPI_Cart_coords(*cartcomm, tile->rank, 2, coords);
                x = coords[0]; y = coords[1];
	}
	else
	{
		dims[0] = 4, dims[1] = 4;
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, cartcomm);
                MPI_Comm_rank(*cartcomm, &tile->rank);
                MPI_Cart_coords(*cartcomm, tile->rank, 2, coords);
                x = coords[0]; y = coords[1];
	}

	if (tile->rank == 0) {
		Tile_makeLand(tile);
	}

	if (tile->rank == 10 && tile->rank == 20 && tile->rank == 30) {
		tile->fish[0] = Fish_create(x, y);
	}
	
	MPI_Cart_shift(*cartcomm, 0, 1, &tile->nbr[UP], &tile->nbr[DOWN]);
	MPI_Cart_shift(*cartcomm, 1, 1, &tile->nbr[LEFT], &tile->nbr[RIGHT]);

	Fish_typeInit(&FISHTYPE);
}

/**
 * Finnur týpu nágranna í @tile og geymir í tile->upType etc.
 */
void Tile_getNeighbourTypes(Tile *tile) {
	int tag = 0;
	MPI_Request reqs[8];
	MPI_Status stats[8];
	int inbuf[4];
	int outbuf = tile->type;

	int numReqs = 0; //Has to wait for (2 * numNeighbours) requests.
	int reqIndex = 0; //Hvar í reqs samskipti eru geymd

	for (int i=0; i<4;i++) {
		int dest=tile->nbr[i];
		int source=tile->nbr[i];

		if (dest != -2) {
			numReqs += 2;
			// perform non-blocking communication
			MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &reqs[reqIndex]);
			MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &reqs[reqIndex+1]); // 4 as a kind of offset
			reqIndex += 2;
		}
	}

	// wait for non-blocking communication to be completed for output  
	MPI_Waitall(numReqs, reqs, stats);
	
	// Geymi upplýsingar um týpu hvers nágranna. Það skiptir ekki máli hvaða rusl fer
	// í týpu þeirra nágranna sem er ekki til.
	tile->nbrType[UP] = inbuf[0]; tile->nbrType[DOWN] = inbuf[1];
	tile->nbrType[LEFT] = inbuf[2]; tile->nbrType[RIGHT] = inbuf[3];

	printf("Nbr %d has type %d | ", tile->nbr[UP], tile->nbrType[UP]);
	printf("Nbr %d has type %d | ", tile->nbr[DOWN], tile->nbrType[DOWN]);
	printf("Nbr %d has type %d | ", tile->nbr[LEFT], tile->nbrType[LEFT]);
	printf("Nbr %d has type %d\n", tile->nbr[RIGHT], tile->nbrType[RIGHT]);

}


/**
 * Ítrunar aðgerð fyrir @tile
 *  TODO
 */
void Tile_iterate(Tile *tile) {
	MPI_Status stats;

	if (tile->rank == 0) {
		Fish fish = { 40, 50, 999 };
		
		MPI_Send(&fish, 1, FISHTYPE, 20, 1, MPI_COMM_WORLD);
	}
	if (tile->rank == 20) {
		Fish fish;
		MPI_Recv(&fish, 1, FISHTYPE, 0, 1, MPI_COMM_WORLD, &stats);
		printf("Rank %d received Fish. Count: %d. x %d, y%d\n",
			tile->rank, fish.count, fish.x, fish.y);
	}
		
}


