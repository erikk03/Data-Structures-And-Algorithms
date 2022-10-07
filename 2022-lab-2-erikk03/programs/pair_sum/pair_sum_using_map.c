#include <stdlib.h>

#include "pair_sum.h"
#include "ADTMap.h"

int compare(Pointer a, Pointer b){
    int* ia = a;
    int* ib = b;
    return *ia - *ib;
}

Pair pair_sum_using_map(int target, Vector numbers){
    Map map = map_create(compare, NULL, NULL);
    
    // Προσθηκη στοιχειων του Vector στο Map
    map_insert(map, numbers, numbers);

    int size = vector_size(numbers);
	Pair num;
    for(int i=0; i<size; i++){
		int* a = vector_get_at(numbers, i);
        int* b = target - *a;
        if(map_find(map, b) != NULL){
            num->first = *a;
			num->second = map_find(map, b);
			return num;
        }
	}
    return NULL;
};
