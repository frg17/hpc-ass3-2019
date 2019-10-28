#include <mpi.h>
#include <stdio.h>

#include "grid.h"
#include "tile.h"


void *Tile_create() {
	Tile *tile = (Tile *) malloc(sizeof(Tile));
	tile->type = 0;
	return tile;
}


void *Tile_debug_print(Tile *tile) {
	printf("Rank %d, has neighbours (u, d, l ,r) : (%d, %d, %d, %d)\n",  tile->rank,
		tile->up, tile->down, tile->left, tile->right);
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
