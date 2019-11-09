#include <mpi.h>

typedef struct Fish {
	int count;
	int x, y;
} Fish;

// Tími á að fjölga sér
void Fish_propagate(Fish *fish);

// Hvert eiga fiskar að fara?
void Fish_swim(Fish *fish);

void  Fish_typeInit(MPI_Datatype *Fishtype);
	
