#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "fish.h"

Fish *Fish_create(int x, int y) {
	Fish *fish = (Fish *) malloc(sizeof(Fish));
	fish->count = 1000;
	fish->x = x;
	fish->y = y;

	return fish;
}


// Býr til MPI derived týpu fyrir struct Fish
void  Fish_typeInit(MPI_Datatype *Fishtype) {
	int count = 3;
	int blocklengths[3] = { sizeof(int) };
	MPI_Aint displ[3];
	MPI_Datatype types[3] = { MPI_INT, MPI_INT, MPI_INT };
	
	MPI_Type_create_struct(3, blocklengths, displ, types, Fishtype);
	MPI_Type_commit(Fishtype);
}


// TODO
int Fish_swim(Fish *fish, Tile *tile) {
	int r = 0;

	while (1) {
		r = rand() % 5;

		if (r == 4) return r;

		if (tile->nbr[r] < 0)
			continue;

		if (tile->nbrType[r] == TILE_WATER)
			return tile->nbr[r];
	}
}


void Fish_propagate(Fish *fish) {
	fish->count = (int) ((float) fish->count *  1.1f);
}
