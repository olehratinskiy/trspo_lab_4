#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>
#include <fstream>

using namespace std;

float f(float x) {
    return x - 10 * sin(x) + abs(pow(x, 2) - x * exp(5));
}

struct Res {
    float x;
    float y;
};

int main(int argc, char* argv[]) {
    int procCount, procNum;
    MPI_Status status;
    float y;
    float N = 19 * 100;
    float k = N / 2;
    float h = N / k;
    float a = N;
    float b = N * 2;
    float z = pow(N, 2);
    float x;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procNum);

    int stepsPerProcess = (int) round(((float) ((N + 1) / h) / ((float) procCount - 1)));
    Res result[stepsPerProcess];
    Res currentResult[stepsPerProcess];

    if (procNum == 0) {
        double t1, t2;
        t1 = omp_get_wtime();
        for (int i = 1; i < procCount; i++) {
            float data[2];
            for (int j = 0; j < stepsPerProcess; j++) {
                MPI_Recv(data, 2, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                cout << "Process №" << i << " f(" << data[0] << ") = " << data[1] << endl;
            }
        }
        t2 = omp_get_wtime();
        cout << endl << "Time (seconds): " << setprecision(50) << t2 - t1 << endl;

    } else {
        x = (1 - h) + (float) ((procNum - 1) * stepsPerProcess) * h;
        for (int i = 0; i < stepsPerProcess; i++) {
            x += h;
            y = f(x);
            Res r{};
            r.x = x;
            r.y = y;
            result[i] = r;
        }

        for (int j = 0; j < stepsPerProcess; j++) {
            float data[2];
            data[0] = result[j].x;
            data[1] = result[j].y;
            MPI_Send(data, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;
}
