#ifndef __TILE_H__
#define __TILE_H__

#include <mpi.h>

typedef struct Tile Tile;
#include "fish.h"

#define TILE_WATER 0
#define TILE_LAND 1

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

typedef struct Tile {
	MPI_Comm cartcomm;
	int type;
	int nbr[4]; //u, d, l, r
	int nbrType[4]; //u, d, l, r
	int rank;
	Fish *fish[3];
} Tile;


void *Tile_create();


void *Tile_debugPrint(Tile *tile);

void Tile_setup(Tile *tile, int num_dimensions, MPI_Comm *cartcomm);

void Tile_getNeighbourTypes(Tile *tile);

void Tile_iterate(Tile *tile);

void Tile_makeLand(Tile *tile);

#endif
