#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>

using namespace std;

float f(float x) {
    return x - 10 * sin(x) + abs(pow(x, 2) - x * exp(5));
}

int main(int argc, char* argv[]) {
    int procCount, procNum;
    MPI_Status status;
    float y, x;
    float N = 19;
    float k = N / 2;
    float h = N / k;
    float a = N;
    float b = N * 2;
    float z = pow(N, 2);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &procNum);

    vector <vector<float>> result;
    vector <vector<float>> currentResult;

    if (procNum == 0) {
        for (int i = 1; i < procCount; i++) {
            MPI_Recv(currentResult, 3, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            for (int j = 0; j < currentResult.size(); j++)
                cout << "Process №" << currentResult[j][0] << " f(" << currentResult[j][1] << ") = " << currentResult[j][2] << endl;
        }
    } else {
        vector<float> tmpResult;

        if (procNum == 1) x = 1 - h;

        for (int i = 0; i < (int) round(((N + 1) / h) / (float) procCount); i++) {
            x += h;
            y = f(x);
            tmpResult.push_back((float) procNum);
            tmpResult.push_back(x);
            tmpResult.push_back(y);
            result.push_back(tmpResult);
            tmpResult.clear();
        }

        if (procNum == procCount - 1) {
            while (x + h <= N + 1) {
                x += h;
                y = f(x);
                tmpResult.push_back((float) procNum);
                tmpResult.push_back(x);
                tmpResult.push_back(y);
                result.push_back(tmpResult);
                tmpResult.clear();
            }
        }

        MPI_Send(result, 3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
    return 0;
}
