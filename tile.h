
#define TILE_WATER 0
#define TILE_LAND 1
typedef struct Tile {
	int type;
	int up, down, left, right;
} Tile;

void *Tile_create(int type) {
	Tile *tile = (Tile *) malloc(sizeof(Tile));
	tile->type = type;
	return tile;
}

void Tile_set_neighbours(Tile *tile, int up, int down, int left, int right) {
	tile->up = up;
	tile->down = down;
	tile->left = left;
	tile->right = right;
}
