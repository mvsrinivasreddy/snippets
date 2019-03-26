#include <stdio.h>



int merge(int *Arr, int start, int mid, int end){
	
	int temp[(end-start)+1];
	
	int s=start, m=mid+1, i=0;
	
	while((s<=mid) && (m<=end)){
		
		if(Arr[s] <= Arr[m]){
			temp[i] = Arr[s];
			s++;
		}	
		else{
			temp[i] = Arr[m];
			m++;
		}
		i++;
	}
		
	while(s<=mid){
		temp[i++] = Arr[s++];
	}
	
	while(m<=end){
		temp[i++] = Arr[m++];
	}	


	printf("Merging....till end:%d\n", end);

	i=start;
	while(i<=end){
		Arr[i] = temp[i-start];
		i++;
	}
	
	return 0;
}


/*void merge(int *Arr, int start, int mid, int end) {
	// create a temp array
	int temp[end - start + 1];

	// crawlers for both intervals and for temp
	int i = start, j = mid+1, k = 0;

	// traverse both arrays and in each iteration add smaller of both elements in temp 
	while(i <= mid && j <= end) {
		if(Arr[i] <= Arr[j]) {
			temp[k] = Arr[i];
			k += 1; i += 1;
		}
		else {
			temp[k] = Arr[j];
			k += 1; j += 1;
		}
	}

	// add elements left in the first interval 
	while(i <= mid) {
		temp[k] = Arr[i];
		k += 1; i += 1;
	}

	// add elements left in the second interval 
	while(j <= end) {
		temp[k] = Arr[j];
		k += 1; j += 1;
	}

	// copy temp to original interval
	for(i = start; i <= end; i += 1) {
		Arr[i] = temp[i - start];
	}
}*/



int mergesort(int *Arr, int start, int end){
	
	printf("Start:%d  End:%d\n", start, end);	

	if(start < end){
		mergesort(Arr, start, ((start+end)/2));
		mergesort(Arr, ((start+end)/2)+1, end);
		merge(Arr, start, ((start+end)/2), end);
	}

	printf("returning......\n");
	return 0;
}



int main(){
	int arr[10] = {10,7,4,5,8,6,2,1,9,3};
			
	mergesort(arr, 0, 9);
	
	printf("Sorted:");
	for(int i=0; i<10; i++){
		printf("%d ", arr[i]);
	}	
	printf("\n");
	return 0;
}
