#include <stdlib.h>

#include "pair_sum.h"


Pair pair_sum(int target, Vector numbers) {
	int size = vector_size(numbers);
	Pair num;
	for(int i=0; i<size; i++){
		for(int j=size-1; j>i; j--){
			int* a = vector_get_at(numbers, j);
			int* b = vector_get_at(numbers, i);
			if(target == *a + *b){
				
				num->first = *a;
				num->second = *b;
				return num;
			}
		}
	}
	return NULL;
}