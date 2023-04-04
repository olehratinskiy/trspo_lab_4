#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>

using namespace std;

float f(float x) {
    return x - 10 * sin(x) + abs(pow(x, 2) - x * exp(5));
}

struct Res {
    float num;
    float x;
    float y;
};

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
    float x = 1 - h;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procNum);

    int stepsPerProcess = (int) round(((float) ((N + 1) / h) / (float) procCount));
    cout << stepsPerProcess << endl;
    Res result[stepsPerProcess];
    Res currentResult[stepsPerProcess];

    if (procNum == 0) {
        for (int i = 1; i < procCount; i++) {
            MPI_Recv(currentResult, stepsPerProcess, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            for (int j = 0; j < stepsPerProcess; j++)
                cout << "Process №" << currentResult[j].num << " f(" << currentResult[j].x << ") = " << currentResult[j].y << endl;
        }
    } else {
        for (int i = 0; i < stepsPerProcess; i++) {
            x += h;
            y = f(x);
            Res r{};
            r.num = (float) procNum;
            r.x = x;
            r.y = y;
            result[i] = r;
        }
        MPI_Send(result, stepsPerProcess, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
