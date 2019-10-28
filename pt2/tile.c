#include <stdio.h>

#include "grid.h"
#include "tile.h"


void *Tile_create() {
	Tile *tile = (Tile *) malloc(sizeof(Tile));
	tile->type = TILE_WATER;
	return tile;
}


void *Tile_debugPrint(Tile *tile) {
	printf("Rank %d, has neighbours (u, d, l ,r) : (%d, %d, %d, %d)\n",  tile->rank,
		tile->up, tile->down, tile->left, tile->right);
}



void Tile_makeLand(Tile *tile) {
	tile->type = TILE_LAND;
}

void Tile_setup(Tile *tile, int num_dimensions, MPI_Comm *cartcomm) {
	//MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, &cartcomm);
	int periods[2] = {1, 1}, reorder = 1;
	
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

	

	MPI_Cart_shift(*cartcomm, 0, 1, &tile->up, &tile->down);
	MPI_Cart_shift(*cartcomm, 1, 1, &tile->left, &tile->right);
}


void Tile_iterate(Tile *tile) {
	int tag = 0;
	MPI_Request reqs[8];
	MPI_Status stats[8];
	int inbuf[4];
	int outbuf = 5;

	for (int i=0; i<4;i++) {
		int nbrs[4] = { tile->up, tile->down, tile->left, tile->right };

		int dest=nbrs[i];
		int source=nbrs[i];

		// perform non-blocking communication
		MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &reqs[i]);
		MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &reqs[i+4]); // 4 as a kind of offset
	}

	// wait for non-blocking communication to be completed for output  
	MPI_Waitall(8, reqs, stats);
	printf("Rank %d finished iteration.\n", tile->rank);
}
