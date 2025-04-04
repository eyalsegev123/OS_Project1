#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define ARRAY_SIZE (1 << 16)  // 2^16 = 65536
#define NUM_CHILDREN 4

int main() {
  int *array = malloc(ARRAY_SIZE * sizeof(int));    
  int forkn_result;
  int pids[NUM_CHILDREN];
  int total_sum = 0;
  int statuses[NPROC];
  int finished_children = 0;
  int partial_size = ARRAY_SIZE / NUM_CHILDREN;

  // Initialize the array with consecutive integers
  for (int i = 0; i < ARRAY_SIZE; i++) {
    array[i] = i;
  }

  if(NUM_CHILDREN > NPROC) {
    exit(1, "Illegal number of children");
  }

  forkn_result = forkn(NUM_CHILDREN , pids);

    // forkn failed with return val -1
  if(forkn_result < 0){
        printf("forkn failed\n");
        exit(1, "");
  }
    // forkn succeeded (returned 0 to parent and 1,2,3,4 to children)
  else {
        //The parent process will wait for all children to finish
    if(forkn_result == 0){

			if(waitall(&finished_children, statuses) < 0) {
					printf("waitall failed\n");
					exit(1, "");
			}

			printf("Parent: Created %d children with PIDs: ", NUM_CHILDREN);
			for(int i = 0; i < NUM_CHILDREN; i++) {
							printf("%d ", pids[i]);
			}
			printf("\n");

      // Verify all children completed
      if(finished_children != NUM_CHILDREN) {
        printf("Error: Only %d of %d children completed\n", 
               finished_children, NUM_CHILDREN);
        exit(1, "");
      }

			for(int i = 0; i < NUM_CHILDREN; i++) {
					total_sum += statuses[i];
			}
			printf("Total sum: %d\n", total_sum);
      free(array);
			exit(0, "");
		}

		// The child processes will calculate the partial sum
	  else {
      int partial_sum = 0;
      int start = partial_size * (forkn_result-1);
      int end = partial_size * forkn_result;
      for(int i=start; i < end; i++){
          partial_sum += array[i];
      }
      printf("Partial sum of child number %d is:  %d\n", forkn_result, partial_sum);
      exit(partial_sum, "");      
    }
  }
  exit(0, "");
}