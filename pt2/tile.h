#define TILE_WATER 0
#define TILE_LAND 1

typedef struct Tile {
	int type;
	int up, down, left, right;
	int rank;
} Tile;


void *Tile_create();

void *Tile_debug_print(Tile *tile);

void Tile_setup(Tile *tile, int num_dimensions, MPI_Comm *cartcomm);

void Tile_iterate(Tile *tile);
