#include <mpi.h>
#include <stdio.h>

//
float mean(int arr[] , int n);
float max(int arr[] , int n);
float min(int arr[] , int n);
float median(int x[] , int n);
int mode1(int a[],int n);
int mode2(int a[],int n);
//


int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

	//MPI_Status status;
	
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	
	//--------------------------question 1
	//if there are not 5 processes, just exit;
	//else continue;
	if(world_size != 5){
		MPI_Finalize();
		printf("there are not 5 processes exactly!");
		return 0;
	}
	
	
	//--------------------------question 2
	//else continue;
	// Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	
	int length = 10000;
	float number;
	
	
	if (world_rank == 0) {
		
		//--------------------------question 3
		//if it is root process, just read the data.txt file into an array;
		//read file
		FILE *fp = fopen("data.txt","r");
		if(fp == NULL)
		{
			printf("can not read the file.\n");
			return -1;
		}
		
		
		char a[length*6];	//array = (float *)malloc(10*sizeof(float));
		int array[length];
		
		int helper = 0;
		int flag = 0;
		
		//read integers separated by comma
		int i = 0;
		while(fscanf(fp, "%c", &a[i]) != EOF){
			int number = a[i] - 48;
			
			if(number >= 0){
				helper = helper * 10 + number;
			}else{
	//			printf("%d \n", helper);
				array[flag] = helper;
				flag ++;
				helper = 0;
			}
			
			i++;
		}
		
		
		
		//--------------------------question 4
		//broadcast array to the other processes
		//
		printf("broadcast to other processes from root \n");
		MPI_Send(array, flag, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(array, flag, MPI_INT, 2, 0, MPI_COMM_WORLD);
		MPI_Send(array, flag, MPI_INT, 3, 0, MPI_COMM_WORLD);
		MPI_Send(array, flag, MPI_INT, 4, 0, MPI_COMM_WORLD);
		
		//MPI_Bcast(array,flag,MPI_INT,0,MPI_COMM_WORLD);
		
		
		
		//--------------------------question 5
		//different processes do different jobs
		number = mean(array, flag);
		
	} else if (world_rank == 1) {
		int array[length];
		MPI_Recv(array, length, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//printf("Process 1 received number %d from process 0\n",array[0]);
		printf("process 1 get data from root process. \n");
		float result = max(array, length);
		//printf("Process 1 received number %f from process 0\n",result);
		
		//
		float buffer[1];
		buffer[0] = result;
		MPI_Send(buffer, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
		
	} else if (world_rank == 2) {
		int array[length];
		MPI_Recv(array, length, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//printf("Process 1 received number %d from process 0\n",array[0]);
		printf("process 2 get data from root process. \n");
		float result = min(array, length);
		//printf("Process 2 received number %f from process 0\n",result);
		
		//
		float buffer[1];
		buffer[0] = result;
		MPI_Send(buffer, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
	} else if (world_rank == 3) {
		int array[length];
		MPI_Recv(array, length, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//printf("Process 1 received number %d from process 0\n",array[0]);
		printf("process 3 get data from root process. \n");
		float result = median(array, length);
		//printf("Process 3 received number %f from process 0\n",result);
		
		//
		float buffer[1];
		buffer[0] = result;
		MPI_Send(buffer, 1, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
	} else if (world_rank == 4) {
		int array[length];
		MPI_Recv(array, length, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//printf("Process 1 received number %d from process 0\n",array[0]);
		printf("process 4 get data from root process. \n");
		float result1 = (float) mode1(array, length);
		//printf("Process 4 received number %f from process 0\n",result1);
		float result2 = (float) mode2(array, length);
		//printf("Process 4 received number %f from process 0\n",result2);
		
		//
		float buffer[2];
		buffer[0] = result1;
		buffer[1] = result2;
		MPI_Send(buffer, 2, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
	} 
	
	
	//--------------------------question 6
	//all processes are synchronized at a barrier
	MPI_Barrier(MPI_COMM_WORLD);
	
	
	//--------------------------question 7
	//the root process receives all results
	//the root process print all results
	if(world_rank == 0){
		//
		//
		printf("the followings are average, max, min, median, mode and the number of occurrencies of the mode \n");
		printf("%f from process 0 \n", number);
		
		float buffer0[1];
		int i = 1;
		while(i < 4){
			//MPI_Recv(a, 10, MPI_INT, 0, 1, MPI_COMM_WORLD);
			MPI_Recv(buffer0,1,MPI_FLOAT,i,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
			printf("%f from process %d \n", buffer0[0] , i);
			i++;
		}
		
		float buffer1[2];
        MPI_Recv(buffer1,2,MPI_FLOAT,4,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
        printf("%f, %f from process 4 \n", buffer1[0], buffer1[1]);
	}
	
	

    // Finalize the MPI environment.
    MPI_Finalize();
	
	return 1;
}

float mean(int arr[] , int n){
	float result = arr[0];
	int i = 1;
	while(i < n){
		result += arr[i];
		i++;
	}
	
	return result/n;
}

float max(int arr[] , int n){
	float result = arr[0];
	int i = 1;
	while(i < n){
		if(result < arr[i]){
			result = arr[i];
		}
		i++;
	}
	
	return result;
}

float min(int arr[] , int n){
	float result = arr[0];
	int i = 1;
	while(i < n){
		if(result > arr[i]){
			result = arr[i];
		}
		i++;
	}
	
	return result;
}

float median(int x[] , int n) {
    float temp;
    int i, j;
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) / 2.0);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}

//
int mode1(int a[],int n) {
   int maxValue = 0, maxCount = 0, i, j;

   for (i = 0; i < n; ++i) {
      int count = 0;
      
      for (j = 0; j < n; ++j) {
         if (a[j] == a[i])
         ++count;
      }
      
      if (count > maxCount) {
         maxCount = count;
         maxValue = a[i];
      }
   }

   return maxValue;
}

//
int mode2(int a[],int n) {
   int maxValue = 0, maxCount = 0, i, j;

   for (i = 0; i < n; ++i) {
      int count = 0;
      
      for (j = 0; j < n; ++j) {
         if (a[j] == a[i])
         ++count;
      }
      
      if (count > maxCount) {
         maxCount = count;
         maxValue = a[i];
      }
   }

   return maxCount;
}