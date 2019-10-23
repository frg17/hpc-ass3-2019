#include <stdio.h>
#include <mpi.h>
#define SIZE 16
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int main (int argc, char** argv) {
	int numtasks, rank, source, dest, outbuf, i, tag=1;
	int inbuf[16]={MPI_PROC_NULL};
	int world_rank; int color;
	int nbrs[4];
	int dims[2] = {4,4}, periods[2] = {1,1}, reorder=1;
	int coords[2];
	MPI_Comm cartcomm;
	MPI_Comm boat;
	MPI_Comm tiles;

	MPI_Request reqs[16];
	MPI_Status stats[16];

	// starting with MPI program
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	
	if (world_rank != 0) {
		MPI_Comm_split(MPI_COMM_WORLD, 0, world_rank, &tiles);
		MPI_Cart_create(tiles, 2, dims, periods, reorder, &cartcomm);
		MPI_Comm_rank(cartcomm, &rank);
		MPI_Cart_coords(cartcomm, rank, 2, coords);
	
	}
	else {
		MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, 0, &tiles);
	}

	

	// do some work with MPI communication operations...
	// e.g. exchanging simple data with all neighbours

	outbuf = rank;
	if (world_rank != 0) {
		MPI_Isend(&outbuf, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &reqs[i]);
	}

	// wait for non-blocking communication to be completed for output  
	if (world_rank == 0) {
		for (int i = 1; i < 17; i++) {
			MPI_Irecv(&inbuf[i - 1], 1, MPI_INT, i, tag, MPI_COMM_WORLD, &reqs[i - 1]);
		}

		MPI_Waitall(16, reqs, stats);

		for (int i = 0; i < 16; i++) {
			printf("Got meesage from %d\n", inbuf[i]);
		}
	}	

	MPI_Finalize();

	return 0;
}
