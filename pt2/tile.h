#include <mpi.h>

#define TILE_WATER 0
#define TILE_LAND 1

typedef struct Tile {
	MPI_Comm cartcomm;
	int type;
	int up, down, left, right;
	int upType, downType, leftType, rightType;
	int rank;
} Tile;


void *Tile_create();


void *Tile_debugPrint(Tile *tile);

void Tile_setup(Tile *tile, int num_dimensions, MPI_Comm *cartcomm);

void Tile_getNeighbourTypes(Tile *tile);

void Tile_iterate(Tile *tile);

void Tile_makeLand(Tile *tile);

