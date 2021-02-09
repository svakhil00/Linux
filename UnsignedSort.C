//sorts unsigned integers by using radix sort on all the bits

#include <stdio.h>
#include <stdlib.h>
void radixSort(int *, const int);
int getMax(int *, const int);
void sort(int *, const int, const unsigned);

int main() {
int count;
printf("Enter the count of the numbers: ");
scanf("%d", &count);

int *nums = (int *) calloc(count, 4);

printf("Enter the numbers:n");
for(int i = 0; i < count; i++){
scanf("%d", &(nums[i]));
}
printf("n");
radixSort(nums, count);
for(int i = 0; i < count; i++){
printf("%dn", nums[i]);
}
return 0;
}

void radixSort(int *array, const int size){
int max = getMax(array, size);
for(unsigned i = 1; i < max; i *= 2){
sort(array, size, i);
}
}

void sort(int *array, const int size, const unsigned digit){
int *output = (int *) calloc(size, 4);

int index = 0;

for(int i = 0; i < size; i++){
if(!(array[i] & digit)){
output[index] = array[i];
index++;
}
}

for(int i = 0; i < size; i++){
if(array[i] & digit){
output[index] = array[i];
index++;
}
}

for(int i = 0; i < size; i++){
array[i] = output[i];
}
}

int getMax(int *array, const int size){
int max = 0;
for(int i = 0; i < size; i++){
if(array[i] > max){
max = array[i];
}
}
return max;
}
