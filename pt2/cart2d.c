#include <stdio.h>
#include <mpi.h>
#include "./tile.h"
#define SIZE 16
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


#define MAX_ITERATIONS 1000

int main (int argc, char** argv) {
	int numtasks, rank, source, dest, outbuf, i, tag=1;
	int inbuf[4]={MPI_PROC_NULL,MPI_PROC_NULL, MPI_PROC_NULL,  
		MPI_PROC_NULL};
	int nbrs[4];
	int dims4x4[2] = {4,4}, dims5x5[2] = {5,5}, dims6x6[2] = {6,6}, periods[2] = {1,1}, reorder=1;
	int coords[2]; int x, y;

	int cycle = 0;
	MPI_Comm cartcomm;

	Tile *tile = Tile_create();

	MPI_Request reqs[8];
	MPI_Status stats[8];

	// starting with MPI program
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	if (numtasks == 36)
		Tile_setup(tile, 6, &cartcomm);
	

	//Tile_debug_print(tile);
	while (cycle < MAX_ITERATIONS) {
			
		Tile_iterate(tile);
		cycle++;
	}
	//MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods,reorder, &cartcomm);
/*
	if(numtasks == 25)
	{
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims5x5, periods,reorder, &cartcomm);
		MPI_Comm_rank(cartcomm, &rank);
		MPI_Cart_coords(cartcomm, rank, 2, coords);
		x = coords[0]; y = coords[1];
		tile = Tile_create(grid5x5[x][y]);
	}
	else if(numtasks == 36)
	{
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims6x6, periods,reorder, &cartcomm);
                MPI_Comm_rank(cartcomm, &rank);
                MPI_Cart_coords(cartcomm, rank, 2, coords);
                x = coords[0]; y = coords[1];
                tile = Tile_create(grid6x6[x][y]);
	}
	else
	{
		MPI_Cart_create(MPI_COMM_WORLD, 2, dims4x4, periods,reorder, &cartcomm);
                MPI_Comm_rank(cartcomm, &rank);
                MPI_Cart_coords(cartcomm, rank, 2, coords);
                x = coords[0]; y = coords[1];
                tile = Tile_create(grid4x4[x][y]);
	}

	MPI_Cart_shift(cartcomm, 0, 1, &nbrs[UP], &nbrs[DOWN] );
	MPI_Cart_shift(cartcomm, 1, 1, &nbrs[LEFT], &nbrs[RIGHT] );


	printf("rank= %d coords= %d %d having neighbours(u,d,l,r)=%d %d %d %d \n", rank, 
			coords[0], coords[1], nbrs[UP], nbrs[DOWN], nbrs[LEFT], nbrs[RIGHT]);

	// do some work with MPI communication operations...
	// e.g. exchanging simple data with all neighbours

	outbuf = tile->type;

	for (i=0; i<4;i++) {
		dest=nbrs[i];
		source=nbrs[i];

		// perform non-blocking communication
		MPI_Isend(&outbuf, 1, MPI_INT, dest, tag, MPI_COMM_WORLD, &reqs[i]);
		MPI_Irecv(&inbuf[i], 1, MPI_INT, source, tag, MPI_COMM_WORLD, &reqs[i+4]); // 4 as a kind of offset
	}

	// wait for non-blocking communication to be completed for output  
	MPI_Waitall(8, reqs, stats);
*/

//	Tile_set_neighbours(tile, inbuf[UP], inbuf[DOWN], inbuf[LEFT], inbuf[RIGHT] );


	MPI_Finalize();

	return 0;
}


