#include <stdio.h>


float mean(int arr[] , int n);
float max(int arr[] , int n);
float min(int arr[] , int n);
float median(int x[] , int n);
int mode1(int a[],int n);
int mode2(int a[],int n);

int main(int argc, char** argv) {
	
	//
	FILE *fp = fopen("sub.txt","r");
	if(fp == NULL)
	{
		printf("can not read the file.\n");
		return -1;
	}
	
	int length = 60000;
	char a[length*2];	//array = (float *)malloc(10*sizeof(float));
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
	//the last one
//	printf("%d \n", helper);
//	array[flag] = helper;
//	flag ++;
	
	//print out
	printf("how many numbers are taken into account: \t %d \n", flag); 
//	for(i = 0; i < flag ;i++){
//		printf("%d \n", array[i]);
//	}

	float v0 = mean(array , flag);
	printf("%f \n", v0);
	
	float v1 = max(array , flag);
	printf("%f \n", v1);
	
	float v2 = min(array , flag);
	printf("%f \n", v2);	
	
	float v3 = median(array , flag);
	printf("%f \n", v3);
	
	//
	float v4 = (float) mode1(array , flag);
	printf("%f \n", v4);
	float v5 = (float) mode2(array , flag);
	printf("%f \n", v5);	
	
	
	//clsoe
	fclose(fp);
	
	return 0;
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

