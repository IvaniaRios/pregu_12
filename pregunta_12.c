#include <mpi.h>
#include <stdio.h>

long long int fibonacci(int n) {
    if (n <= 1)
        return n;

    long long int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main(int argc, char *argv[]) {
    int rank, size, n;
    long long int fib_result;

    // Inicializar MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) { // Proceso MASTER
        printf("Ingrese el término de Fibonacci a calcular: ");
        scanf("%d", &n);

        // Enviar el término n a los procesos SLAVE
        for (int i = 1; i < size; i++) {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Calcular el término de Fibonacci en el proceso MASTER
        fib_result = fibonacci(n);
        printf("Proceso %d calculó Fibonacci(%d) = %lld\n", rank, n, fib_result);

        // Recibir los resultados de los procesos SLAVE
        for (int i = 1; i < size; i++) {
            MPI_Recv(&fib_result, 1, MPI_LONG_LONG_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d calculó Fibonacci(%d) = %lld\n", i, n, fib_result);
        }

    } else { // Procesos SLAVE
        // Recibir el término n del proceso MASTER
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calcular el término de Fibonacci
        fib_result = fibonacci(n);

        // Enviar el resultado al proceso MASTER
        MPI_Send(&fib_result, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Finalizar MPI
    MPI_Finalize();
    return 0;
        /*
    compilar 
        mpicc -o sol pregunta_12.c

    ejecutar
        mpiexec -n 1 ./sol1
    
    */
}
