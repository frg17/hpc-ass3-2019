#include "fish.h"

void  Fish_typeInit(MPI_Datatype *Fishtype) {
	int count = 3;
	int blocklengths[3] = { sizeof(int) };
	MPI_Aint displ[3];
	MPI_Datatype types[3] = { MPI_INT, MPI_INT, MPI_INT };
	
	MPI_Type_create_struct(3, blocklengths, displ, types, Fishtype);
	MPI_Type_commit(Fishtype);
}
