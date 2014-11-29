#include <stdlib.h>
#include <stdio.h>


void p_tx(int* arr, int len, int (*tx)(int)){
	int i;
	for (i=0;i<len;++i)
		printf("%d ",tx(arr[i]));
}
void p_tx2(int* arr, int len, int (*tx)(int)){
	int i;
	for (i=0;i<len;++i)
		printf("%d ",tx(arr[i]));
}
int s(int x) {return x*x;}
int c(int x) {return x*s(x);}

int main(void){
	int i;
	int arr[] = {0,1,2,3,4,5};
	p_tx(arr,6,s);
	p_tx2(arr,6,c);
	scanf("%d",&i);
	return 0;
}