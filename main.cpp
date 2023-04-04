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
    float x;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procNum);

    int stepsPerProcess = (int) round(((float) ((N + 1) / h) / (float) procCount));
    Res result[stepsPerProcess];
    Res currentResult[stepsPerProcess];

    if (procNum == 0) {
        for (int i = 1; i < procCount; i++) {
//            MPI_Recv(currentResult, stepsPerProcess, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

           for (int j = 0; j < stepsPerProcess; j++) {
               MPI_Recv({x, y}, 2, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
               cout << "Process №" << i << " f(" << x << ") = " << y << endl;
           }
        }
    } else {
        x = (1 - h) + (float) ((procNum - 1) * stepsPerProcess);
        for (int i = 0; i < stepsPerProcess; i++) {
            x += h;
            y = f(x);
            cout << x << "  " << y << endl;
            Res r{};
            r.num = (float) procNum;
            r.x = x;
            r.y = y;
            result[i] = r;

        }
        for (int j = 0; j < stepsPerProcess; j++) {
            cout << "Process №" << result[j].num << " f(" << result[j].x << ") = " << result[j].y << endl;
            MPI_Send({result[j].x, result[j].y}, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
        }

    }

    MPI_Finalize();
    return 0;
}
