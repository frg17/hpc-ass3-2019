#ifndef __FISH_H__
#define __FISH_H__

#include <mpi.h>

typedef struct Fish {
	int count;
} Fish;

#include "tile.h"

Fish *Fish_create(int x, int y);

// Tími á að fjölga sér
void Fish_propagate(Fish *fish);

// Hvert eiga fiskar að fara?
int Fish_swim(Fish *fish, Tile *tile);

void Fish_typeInit(MPI_Datatype *Fishtype);	

#endif
