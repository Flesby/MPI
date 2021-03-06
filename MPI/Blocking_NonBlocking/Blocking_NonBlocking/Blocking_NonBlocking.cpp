#include "pch.h"
#include "mpi.h"
#include <iostream>
#include <math.h>

using namespace std;

int main( int argc, char **argv)
{
	int done = 0, n, myid, numprocs, I, rc;
	double PI25DT = 3.141595653589793238462643;
	double mypi, pi, h, sum, x, a;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (myid == 0) {
		n = 1000000;
	}

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	h = 1.0 / double(n);
	sum = 0.0;

	for (int i = myid + 1; i <= n; i += numprocs) {
		x = h * (double(i) - 0.5);
		sum += 4.0 / (1.0 + x * x);
	}
	myid = h * sum;
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myid == 0) {
		cout << "pi is approximately: " << pi << endl;
		cout << "error is :" << fabs(pi-PI25DT) << endl;
	}
	MPI_Finalize();
	return 0;
}
