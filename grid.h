#include <stdlib.h>

int grid4x4[4][4] = 
{
	0, 0, 0, 0,
	0, 1, 1, 0,
	0, 1, 1, 0,
	0, 0, 0, 0
};

#define TILE_WATER 0
#define TILE_LAND 1
typedef struct Tile {
	int type;
	int UP, DOWN, LEFT, RIGHT;
} Tile;

void *Tile_create(int type) {
	Tile *tile = (Tile *) malloc(sizeof(Tile));
	tile->type = type;
	return tile;
}
	
