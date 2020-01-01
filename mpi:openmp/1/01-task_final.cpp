#include <mpi.h>
#include <iostream>

const int N = 1000;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int process_num;
    MPI_Comm_size(MPI_COMM_WORLD, &process_num);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Status status;

    if(rank == 0){
        int elements = N / (process_num - 1);

        if (process_num != 1){
            int sum = 0, tmp_sum;

            int *a = new int[N];
            for (int i = 0; i < N; ++i){
                a[i] = i + 1;
            }

            for (int i = 0; i < N; ++i){
                MPI_Send(&elements, 1, MPI_INT, i , 1, MPI_COMM_WORLD);
                MPI_Send(a + elements * (i - 1), elements, MPI_INT, i , 1, MPI_COMM_WORLD);
            }

            int last_part_ind = N - elements * (process_num - 2);
            MPI_Send(&last_part_ind, 1, MPI_INT, process_num - 1, 1, MPI_COMM_WORLD);
            MPI_Send(a + elements * (process_num - 2), last_part_ind, MPI_INT, process_num - 1, 1, MPI_COMM_WORLD);

            for(int i = 1; i < process_num; ++i){
                MPI_Recv(&tmp_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
                std::cout<<"Process num"<<i<<":"<<tmp_sum <<'\n';
                sum += tmp_sum;
            }

            std::cout<<"Res :"<<sum<<'\n';
            delete[] a;
        }

        int s = (1 + N) * N / 2;
        std::cout<<"Real sum " << s<<'\n';
    } else {
        int cur_p_sum = 0;
        int size;
        MPI_Recv(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        int *a = new int[size];
        MPI_Recv(a, size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        for (int i = 0; i < size; ++i){
            std::cout<<a[i]<< ' ';
        }

        for (int i = 0; i < size; ++i){
            std::cout<<a[i]<< ' ';
        }

        for(int i = 0; i < size; ++i){
            cur_p_sum += a[i];
        }

        MPI_Send(&cur_p_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        delete[] a;
    }

    MPI_Finalize();
    return 0;
}
