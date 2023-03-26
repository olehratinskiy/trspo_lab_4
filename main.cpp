#include <iostream>
#include <mpi.h>
#include <cmath>

using namespace std;

float f(float x) {
    return x - 10 * sin(x) + abs(pow(x, 2) - x * exp(5));
}

int main(int argc, char* argv[]) {
    int procCount, procNum;
    MPI_Status status;
    float y;
    float N = 19;
    float k = N / 2;
    float h = N / k;
    float a = N;
    float b = N * 2;
    float z = pow(N, 2);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procNum);

    float result[3];
    float currentResult[3];

    if (procNum == 0) {
        for (int i = 1; i < procCount; i++) {
            MPI_Recv(currentResult, 3, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            cout << "Process №" << currentResult[0] << " f(" << currentResult[1] << ") = " << currentResult[2] << endl;
        }
    } else {
        float x = procNum * h;
        y = f(x);
        result[0] = (float) procNum;
        result[1] = x;
        result[2] = y;
        MPI_Send(result, 3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
