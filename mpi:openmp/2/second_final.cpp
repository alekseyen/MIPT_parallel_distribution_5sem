#include <mpi.h>
#include <cmath>
#include <iostream>
#include <cstdlib>

const double L = 1;
const double timeStep = 0.0002;
const double h = 0.02; 
const int N = 10;
const double T = 0.1;

double nextTemp(double y1, double y2, double y3) {
    return y2 + timeStep / h / h * (y1 - 2 * y2 + y3);
}

double strangeThing(double t, double x, int m) {
    return std::exp(-M_PI * M_PI * (2 * m + 1) * (2 * m + 1) * t) / (2 * m + 1) * std::sin(M_PI * (2 * m + 1) * x);
}

double realValue(double x, double t) {
    const double accuracy = 0.001;
    double uTemp = strangeThing(t, x, 0);
    double u = uTemp + strangeThing(t, x, 1);
    int m = 2;
    while (std::abs(u - uTemp) > accuracy) {
        uTemp = u;
        u += strangeThing(t, x, m++);
    }
    return 4 * u / M_PI;
}

void solution(){
    std::cout<<"\n Analitics solution \n";
    double k = 1.0, T = 0.1, l = 1.0;

    double step = 0.1;
    double current_len = 0;
    int point_num = 0;
    while(current_len <= 1.0){
        double sum = 0;
        for (int m = 0; m < int(1e5); ++m){
            sum += exp(-k * M_PI * M_PI * (2*m + 1) * (2*m + 1) * T /l /l) * sin(M_PI * (2*m + 1) * current_len / l)/(2*m + 1);
        }
        double u = 4 / M_PI * sum;

        std::cout<<current_len<< ", temp = " <<u <<'\n';
        point_num += 1;
        current_len += step;
    }
}

int main(int argc, char *argv[]) {

    MPI_Init(&argc, &argv);
    double startTime = MPI_Wtime(), endTime = 0;
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double *u = NULL;
    if (rank == 0) {
        u = new double[N];

        for (int j = 1; j < N - 1; j++) {
            u[j] = 1;
        }

        u[0] = 0;
        u[N - 1] = 0;
    } 

    int *begin = new int[p];
    int *end = new int[p];
    int *num = new int [p];
    
    const int div = N / p;
    const int mod = N % p;

    for (int r = 0; r < p; r++) {
        int currentBegin = r < mod ? r * div : (div + 1) * mod + (r - mod) * div;
        int currentEnd = currentBegin + div - 1;
        int currentNum = div;

        if (r > 0) {
            currentBegin -= 1;
            currentNum += 1;
        }

        if (r < p - 1) {
            currentEnd += 1;
            currentNum += 1;
        }

        begin[r] = currentBegin;
        end[r] = currentEnd;
        num[r] = currentNum;
    }

    double *buf1 = new double[num[rank]];
    double *buf2 = new double[num[rank]];
    double *currentU = buf1;
    double *lastU = buf2;
    
    if (rank == 0) {
        for (int r = 1; r < p; r++) {
            MPI_Send(u + begin[r], num[r], MPI_DOUBLE, r, 1, MPI_COMM_WORLD);  
        }
        
        for (int i = begin[rank]; i < end[rank]; i++){
            currentU[i] = u[i];
        }

    } else {
        MPI_Recv(currentU, num[rank], MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, NULL);
    }

    for (int i = 0; i < T / timeStep; i++) {
        for (int j = 1; j < num[rank] - 1; j++) {
            lastU[j] = nextTemp(currentU[j - 1], currentU[j], currentU[j + 1]);
        }

        // аля философы
        // проверка, что ты не последний процесс и можем отправить/принять от процесса справа
        // либо аналогично что ты не первый процесс
        if (rank % 2) { 

            if (rank < p - 1) {
                MPI_Send(&lastU[num[rank] - 2], 1, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD);
            } 
            if (rank > 0) {
                MPI_Send(&lastU[1], 1, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD);
            }
            if (rank < p - 1) {
                MPI_Recv(&lastU[num[rank] - 1], 1, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD, NULL);
            } 
            if (rank > 0) {
                MPI_Recv(&lastU[0], 1, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, NULL);
            }
        } else {
            if (rank > 0) {
                MPI_Recv(&lastU[0], 1, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD, NULL);
            }
            if (rank < p - 1) {
                MPI_Recv(&lastU[num[rank] - 1], 1, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD, NULL);
            }
            if (rank > 0) {
                MPI_Send(&lastU[1], 1, MPI_DOUBLE, rank - 1, 2, MPI_COMM_WORLD);
            }
            if (rank < p - 1) {
                MPI_Send(&lastU[num[rank] - 2], 1, MPI_DOUBLE, rank + 1, 2, MPI_COMM_WORLD);
            }            
        }

        double *t = currentU;
        currentU = lastU;
        lastU = t;
    }

    if (rank == 0) {
        for (int i = 0; i < end[rank]; i++) {
            u[i] = currentU[i];
        }

        for (int r = 1; r < p - 1; r++) {
            MPI_Recv(u + begin[r] + 1, num[r] - 2, MPI_DOUBLE, r, 3, MPI_COMM_WORLD, NULL);
        }

        MPI_Recv(u + begin[p - 1] + 1, num[p - 1] - 1, MPI_DOUBLE, p - 1, 3, MPI_COMM_WORLD, NULL);
    } else {
        int count = num[rank] - 2;
        if (rank == p - 1)
            ++count;
        MPI_Send(&currentU[1], count, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        for (int j = 0; j < N; j++) {
            std::cout << j * L / N << ' ' << u[j] << ' ' << realValue(j * L / N, T) << '\n';
        }
        delete[] u;
    }

    delete[] num;
    delete[] begin;
    delete[] end;
    delete[] buf1;
    delete[] buf2;

    endTime = MPI_Wtime();
    if (rank == 0) {
        std::cout << "process num:"<< p;

        std::cout << "Parralles Time: " << endTime - startTime << '\n';

        double startTime = MPI_Wtime(), endTime = 0;
        solution();
        endTime = MPI_Wtime();
        std::cout << "Analytics Time: " << endTime - startTime << '\n';
    }

    MPI_Finalize();

    return 0;
}