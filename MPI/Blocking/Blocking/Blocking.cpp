#include "pch.h"
#include <iostream>
#include <mpi.h>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	int id, nproc;
	MPI_Status status;
	double x, y, Pi, error;
	long long allsum;
	const long long n = 1000000;
	const double Piiii = 3.1415926;

	// Initialize MPI:
	MPI_Init(&argc, &argv);
	// Get my rank:
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	// Get the total number of processors:
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	printf("I am process %d of %d.\n", id, nproc);

	srand((unsigned)time(0));
	cout.precision(12);

	long long sum = 0;
	for (long long i = 0; i < n; ++i) {
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;
		if (x*x + y * y < 1) ++sum;
	}

	//Slave:
	if (id != 0) {
		MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD);
	}

	//Master:
	else {
		allsum = sum;
		for (int i = 1; i < nproc; ++i) {
			MPI_Recv(&sum, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			allsum += sum;
		}

		//calculate Pi, compare to the Pi to 3.1415926
		Pi = (4.0*allsum) / (n*nproc);
		error = fabs(Pi - Piiii);
		cout << " Const  Pi: " << Piiii << endl;
		cout << "\tPi: " << Pi << endl;
		cout << "Error: " << fixed << error << endl;
	}

	// Terminate MPI:
	MPI_Finalize();
	return 0;
}