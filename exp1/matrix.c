#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

/**
 * C = aAB + bC
 * 
 * processing speed(GFLOPS/sec) = flops / 10^9 / time(sec)
*/

#define MAXN 803
const int ITERATIONS = 10;
const float diff = 1e-5;

char BOOL[2][10] = {"false", "true"};

float A[MAXN][MAXN];
float B[MAXN][MAXN];
float C[MAXN][MAXN];
float ans[MAXN][MAXN];

double timestamp() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

void init(int n) {
    int i, j;
    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j)
            C[i][j] = 0;
}

int check(int n) {
    int i, j;
    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j)
            if(abs(ans[i][j] - C[i][j]) > diff)
                return 0;
    return 1;
}

void simple_matrix(int x_start, int x_end, int y_start, int y_end, int n) {
    // int p = omp_in_parallel();
    // int t = omp_get_thread_num();
    // printf("p = %d, thread id = %d, (x1, y1, x2, y2) = (%d, %d, %d, %d)\n", p, t, x_start, y_start, x_end, y_end);
    int i, j, k;
    for(i = x_start; i < x_end; ++i)
        for(k = 0; k <n; ++k)
            for(j = y_start; j < y_end; ++j)
                C[i][j] = A[i][k] * B[k][j] + C[i][j];
}

void partition_matrix(int n) {
    int s = 4;
    int block_num = s * s;
    int interval = n / s;
    int i, j, k;

    #pragma omp parallel shared(C, A, B)
        #pragma omp single
        for(i = 0; i < n; i += interval)
            for( j = 0; j < n; j += interval)
                #pragma omp task
                simple_matrix(i, i + interval, j, j + interval, n);
}

void vector_parition(int n) {

}

int main(void) {
    double start_time;
    double end_time;
    double time_simple;
    double time_partition;
    int check_partition;
    int i, j, k, n;
    n = 800;

    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j) {
            A[i][j] = (float)rand()/(float)(RAND_MAX);
            B[i][j] = (float)rand()/(float)(RAND_MAX);
            C[i][j] = 0;
            ans[i][j] = 0;
        }
    
    start_time = timestamp();
    for(i = 0; i < n; ++i)
        for(j = 0; j < n; ++j)
            for(k = 0; k < n; ++k)
                ans[i][j] = ans[i][j] + A[i][k] * B[k][j];

    init(n);

    start_time = timestamp();
    simple_matrix(0, n, 0, n, n);
    end_time = timestamp();
    time_simple = end_time - start_time;

    init(n);
    
    start_time = timestamp();
    partition_matrix(n);
    end_time = timestamp();
    time_partition = end_time - start_time;
    check_partition = check(n);

    init(n);

    

    // for(i = 0; i < n; ++i) {
    //     for(j = 0; j < n; ++j)
    //         printf("%lf ", C[i][j]);
    //     puts("");
    // }

    // puts("");
    // for(i = 0; i < n; ++i) {
    //     for(j = 0; j < n; ++j)
    //         printf("%lf ", ans[i][j]);
    //     puts("");
    // }
    // puts("");

    printf("time for simple matrix is %lf\n", time_simple);
    printf("time for partiton matrix is %lf\n", time_partition);
    printf("result check for partition matrix is %s\n", BOOL[check_partition]);
    
    return 0;
}