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

void *thread_process(void *args); //The function to process the threads with the arguments passed as a pointer to void.
void print_matrix(u_int **, int); //Function to print the matrices if desired by the user.
void create_matrix(int, u_int **); //Function to create and initialise the matrices with 0 and 1.



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

create_matrix(dimension, A);
create_matrix(dimension, B);
create_matrix(dimension, C);

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

/*Now we craete and intiliase the threads of our program. The thread creation function takes in 4 paramteres, the thread ids 
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
void create_matrix(int dimension, u_int **ptr){


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