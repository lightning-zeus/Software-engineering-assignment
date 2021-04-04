/************************************************************ 
 * Name : SK ASIF
 * Roll : 001911001064
 * Dept : Information Technology
 * Year : 2nd   Semester : 4th
 * Subject : Software Engineering Lab
 * ----------------------------------------------------------------------------
   * Assignment : Matrix Multiplication Using a Parallel Algorithm 
 * ----------------------------------------------------------------------------
 *  The objective of this assignment is to write a C program which will do 
 *  matrix multiplication using a parallel algorithm. The two input matrices 
 *  are A and B. The resultant matrix after multiplication is C. 
 * 
 *  The program will accept three arguments; the first one is for creating 
 *  number of parallel threads, the second one is the dimension of the 
 *  square matrix and the third one is about displaying matrices. 
 * 
 *  The value of the first argument should be equal to the number of 
 *  CPUs/Logical processors in the system. 
 *  The value of the 2​nd argument should be  around 1000/1500/2000. 
 *  The value of the third argument should be either 1 or 0. 
 *  If it is 1 then your program will display all matrices at the end. 
 *  If the value of the third argument is 0, then no matrix will be displayed.
 * ----------------------------------------------------------------------------
 * Compilation : 
 *      gcc 64_SK_ASIF.c -o 64_SK_ASIF -lpthread
 * ----------------------------------------------------------------------------
 * Execution :
 *      ./64_SK_ASIF [threads] [dimension] [display]
 * 
 *          * [threads]   : - number of threads to be used 
 *                          - Should be greater than 0
 *          * [dimension] : - dimension of the square matrix
 *                          - Should be greater than 0    
 *          * [display]   : - Should be either 0 or 1 
 *                          - If it is 1 all matrices are displayed at the end
 * ----------------------------------------------------------------------------
 * Sample Execution[1] : [Validating Correctness of Matrix Multiplication]
 *  ./64_SK_ASIF 4 4 1
 * 
 * Output Generated :
 * TIME TAKEN = 0.448 seconds
 * 
 * Matrix A:
     1 0 1 1 
     1 1 0 0 
     1 1 0 1 
     0 1 1 0 

Matrix B:
     0 0 0 0 
     1 0 1 1 
     0 0 0 1 
     1 1 1 0 

Matrix C: 
     1 1 1 1 
     1 0 1 1 
     2 1 2 1 
     1 0 1 2  
* ----------------------------------------------------------------------------
* Sample Execution[2] : Validating usage of multiple Processors
 *  ./64_SK_ASIF 8 1000 0
 * 
 * Output Generated :
 * Time taken = 1.816090 seconds
 * 
 * Simultaneously mpstat was used to measure CPU usage
 * 
 * CPU usage before running the program :
     08:18:32 PM IST  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
     08:18:33 PM IST  all   14.38    0.00    3.40    0.13    0.76    0.13    0.00    0.00    0.00   81.21
     08:18:33 PM IST    0    7.22    0.00    2.06    0.00    1.03    0.00    0.00    0.00    0.00   89.69
     08:18:33 PM IST    1   18.63    0.00    2.94    0.00    0.98    0.00    0.00    0.00    0.00   77.45
     08:18:33 PM IST    2   19.19    0.00    3.03    0.00    0.00    0.00    0.00    0.00    0.00   77.78
     08:18:33 PM IST    3   21.00    0.00    3.00    0.00    0.00    0.00    0.00    0.00    0.00   76.00
     08:18:33 PM IST    4   13.40    0.00    6.19    0.00    1.03    0.00    0.00    0.00    0.00   79.38
     08:18:33 PM IST    5   10.10    0.00    3.03    0.00    1.01    0.00    0.00    0.00    0.00   85.86
     08:18:33 PM IST    6   11.00    0.00    4.00    0.00    1.00    1.00    0.00    0.00    0.00   83.00
     08:18:33 PM IST    7   14.14    0.00    3.03    1.01    1.01    0.00    0.00    0.00    0.00   80.81
*

* CPU usage while the program was running
     08:18:39 PM IST  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
     08:18:40 PM IST  all   97.25    0.00    2.13    0.00    0.38    0.00    0.00    0.00    0.00    0.25
     08:18:40 PM IST    0  100.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    1   94.95    0.00    5.05    0.00    0.00    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    2   98.02    0.00    0.99    0.00    0.99    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    3   96.97    0.00    3.03    0.00    0.00    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    4   96.04    0.00    2.97    0.00    0.99    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    5   97.03    0.00    1.98    0.00    0.99    0.00    0.00    0.00    0.00    0.00
     08:18:40 PM IST    6   97.98    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    2.02
     08:18:40 PM IST    7   96.97    0.00    3.03    0.00    0.00    0.00    0.00    0.00    0.00    0.00
*
* As we can see when we run the program the usage of all 8 CPUs rise to 
* around 100% which confirms that all the 8 CPUs are used in this process.

*******************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))




/*Declaring the matrices as global variables since it provides 
easy access to all the functions of the program*/

/*Declaring the matrices as double pointers to unsigned int because:
1. The matrices are vector of vectors that require the elements to be stored in a grid fashion.
2. The permitted values are only 0 and 1 so the data type unsigned int works perfectly. */
 
u_int **A;
u_int **B;
u_int **C;

// void *thread_process(void *args); //The function to process the threads with the arguments passed as a pointer to void.
void print_matrix(u_int **, int); //Function to print the matrices if desired by the user.
u_int **create_matrix(int, u_int **); //Function to create and initialise the matrices with 0 and 1.
void *process_thread(void *args){
    int *arguments = (int *) args;
    int dimension = arguments[0];
    int block_length = arguments[1];
    int block_number = arguments[2];
    
    
    /* Here we do the normal matrix multiplication using three for loops.
    We initiliase our counter with the product of the ith block and the size of the block. */
    for (int i = block_number * block_length; i < min((block_number + 1) * block_length, dimension); i++) {
        for (int j = 0; j < dimension; j++) {
            C[i][j] = 0;
            for (int k = 0; k < dimension; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    //The pthread exit function is used to terminate the thread and return back to the function call.
    pthread_exit(NULL);
    }



int main(int argc, char *argv[]){

//The first step is to validate the number of arguments passed from the command line to the program and it should be 4.

if(argc!=4){
    printf("Incorrect number of arguments. Please try again.");
    return 0;
}

//Once we recieve the arguments we need to typecast the arguments to proper data types and variables.

int no_threads = atoi(argv[1]);
int dimension = atoi(argv[2]);
int display = atoi(argv[3]);

//We take the minimum vlaue of both the units incase threads>dimension.


no_threads = min(dimension,no_threads); 

//We need to call the function to create and initilaise the array with the dimension argument we recieved from the user.

A = create_matrix(dimension, A);
B = create_matrix(dimension, B);
C = create_matrix(dimension, C);

//function to display the matrix at the end if the user desires so.
if (display == 1) {
        printf("Matrix A:\n");
       print_matrix(A, dimension);
        printf("Matrix B:\n");
       print_matrix(B, dimension);
      
    }

/*Now we get the time before the program starts the multiplying process and since the matrices have already been declared
and initliased, the time taken to do that isnt included in the matrix multiplication process.*/

struct timeval t1;
gettimeofday(&t1, NULL);

/* Now we create the threads and the number of threads as passed by the user from the command line*/

/*The pthread_t function declares the thread and since its an array,it will declare multiple threads and will return the specific 
id of the particular thread that will then be stored in the array t_id. */


pthread_t t_id[no_threads];

/*Now we create and intiliase the threads of our program. The thread creation function takes in 4 paramteres, the thread ids 
of the threads that were declared before, state of the thread and scheduling policy of the thread and since we need default values
we set it to null, a function that will use the specific thread for performing a specific task and the last partameter is the 
arguments required by the thread function that is declared earlier, its better if we pass the arguments in the form of a struct
or an array. */


for(int i=0;i<no_threads;i++){
    int *thread_attribute = (int *)malloc(sizeof(int) * 3);
    thread_attribute[0] = dimension;
    thread_attribute[1] = dimension/no_threads;
    thread_attribute[2] = i;

    int ver = pthread_create(&t_id[i], NULL, *process_thread, (void *)thread_attribute);
    if(ver != 0)
    printf("The thread creation failed. \n");
}
/*Now we need to wait for the process to complete and the thread to terminate. The thread join function is used for this,
it takes the thread id of the thread we are waiting to terminate and the exit status where the status of the thread is stored,
IN this case its null.*/

 for (int i = 0; i < no_threads; i++) {
        pthread_join(t_id[i], NULL);  // joining of threads
    }

/*As the matrix multiplication process has completed and all the threads have been terminated. We now measure the time again and then
subtract the times taken before and after the process and then convert it to mili seconds to measure how much time it takes
to multiply matrices parallely using threads. */

if(display==1){
  printf("Matrix C: \n");
        print_matrix(C, dimension);
}

struct timeval t2;
gettimeofday(&t2, NULL);

 long mili_sec =
        (t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec);
    long s = mili_sec / 1000000;
    mili_sec %= 1000000;
    printf("The matrix multiplication process took %ld.%ld secs\n", s, mili_sec);

    return 0;
}


/*A function to dynamically allocate space for a matrix of our desired dimension
that will initialise the pointer we pass to the function.*/
u_int **create_matrix(int dimension, u_int **ptr){


    //Allocating space for the number of rows in the matrix that is the measure of the dimension
     ptr = (u_int **)malloc(dimension * sizeof(int *));


    //Allocating space for the number of columns per row in the matrix
     for(int i=0;i<dimension;i++){
         ptr[i] = (u_int *)malloc(dimension * sizeof(u_int));
     }

     /*Initialising all the cells of the matrix with either 0 or 1 randomly generated with the rand function ffrom
     the standard c library and taking its modulus with 2 that will give 1 for an odd number or 0 for an even number.*/

     
     for(int i=0;i<dimension;i++){
         for(int j=0;j<dimension;j++){
             ptr[i][j] = rand() % 2;
         }
     }
      return ptr;
}


void print_matrix(u_int **matrix, int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

