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
void Tile_setup(Tile *tile, MPI_Comm input_comm, MPI_Comm *cartcomm) {
	//MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, &cartcomm);
	int periods[2] = {0, 0}, reorder = 1;
	
	int coords[2], x, y, dims[2];

	dims[0] = 6, dims[1] = 6;
	MPI_Cart_create(input_comm, 2, dims, periods,reorder, cartcomm);
	MPI_Comm_rank(*cartcomm, &tile->rank);
	MPI_Cart_coords(*cartcomm, tile->rank, 2, coords);
	x = coords[0]; y = coords[1];

	if (tile->rank == 0) {
		Tile_makeLand(tile);
	}

	if (tile->rank == 10 || tile->rank == 20 || tile->rank == 30) {
		tile->fish[0] = Fish_create(x, y);
	}
	
	MPI_Cart_shift(*cartcomm, 0, 1, &tile->nbr[UP], &tile->nbr[DOWN]);
	MPI_Cart_shift(*cartcomm, 1, 1, &tile->nbr[LEFT], &tile->nbr[RIGHT]);

	Fish_typeInit(&FISHTYPE);
}

/**
 * Finnur týpu nágranna í @tile og geymir í tile->upType etc.
 */
void Tile_getNeighbourTypes(Tile *tile, MPI_Comm cartcomm) {
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
			MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, cartcomm, &reqs[reqIndex]);
			MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, cartcomm, &reqs[reqIndex+1]); // 4 as a kind of offset
			reqIndex += 2;
		}
	}

	// wait for non-blocking communication to be completed for output  
	MPI_Waitall(numReqs, reqs, stats);
	
	// Geymi upplýsingar um týpu hvers nágranna. Það skiptir ekki máli hvaða rusl fer
	// í týpu þeirra nágranna sem er ekki til.
	tile->nbrType[UP] = inbuf[0]; tile->nbrType[DOWN] = inbuf[1];
	tile->nbrType[LEFT] = inbuf[2]; tile->nbrType[RIGHT] = inbuf[3];
}


/**
 * Ítrunar aðgerð fyrir @tile
 *  TODO
 */
void Tile_iterate(Tile *tile, MPI_Comm cartcomm) {
	Tile_handleFish(tile, cartcomm);
	
	int fishCount = 0;
	for (int i = 0; i < 3; i++) {
		if (tile->fish[i] != NULL) {
			fishCount += tile->fish[i]->count;	
		}
	}
	
	if (fishCount > 0)
		printf("Tile %d. Fish count: %d\n", tile->rank, fishCount);
}


int checkFish(Tile *tile) {
	int count = 0;	
	Fish **fish = tile->fish;
	for (int i = 0; i < 3; i++) {
		if (fish[i] != NULL) {
			count += fish[i]->count / 3;
			fish[i]->count = 2 *fish[i]->count / 3;
		}
	}			
	return count;
}


void Tile_handleFish(Tile *tile, MPI_Comm cartcomm) {
	int reqNum = 0;
	int incomingCount = 0;
	int tag = 0;
	int flag = 1;
	MPI_Request req;
	MPI_Request reqs[3];
	MPI_Status status;
	MPI_Status stats[3];

	for (int i = 0; i < 3; i++) {
		if (tile->fish[i] != NULL) {
			Fish_propagate(tile->fish[i]);

			int next = Fish_swim(tile->fish[i], tile); //Hvert syndir fiskur

			if (next == 4) {
				//printf("Rank: %d Fish staying\n", tile->rank);
				 continue; //Ekki færa fisk
			}

			MPI_Isend(tile->fish[i], 1, FISHTYPE, next, tag, cartcomm, &reqs[reqNum]);
			reqNum++;
			
			free(tile->fish[i]);
			tile->fish[i] = NULL;
		}
	}
	
	MPI_Waitall(reqNum, reqs, NULL);

	MPI_Barrier(cartcomm);

	MPI_Message msg;

	while (flag) {

		MPI_Iprobe(MPI_ANY_SOURCE, tag, cartcomm, &flag, &status); 

		if (!flag) break;
		

		Fish fish;

		MPI_Recv(&fish, 1, FISHTYPE, MPI_ANY_SOURCE, tag, cartcomm,  &status);

		for (int j = 0; j < 3; j++) {
			Fish *f = (Fish *) malloc(sizeof(Fish));	
			f->count = fish.count;
			if (tile->fish[j] == NULL) {
				tile->fish[j] = f;
				break;
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD); //Sync with ships

	MPI_Barrier(MPI_COMM_WORLD);


	MPI_Iprobe(MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &flag, &status); 
	
	if (flag) {
		int recv = 0;
		MPI_Recv(&recv, 1, MPI_INT, status.MPI_SOURCE, 3, MPI_COMM_WORLD, &status);
		int nettedFish = checkFish(tile);
		MPI_Send(&nettedFish, 1, MPI_INT, status.MPI_SOURCE, 3, MPI_COMM_WORLD);
	}

}
	
