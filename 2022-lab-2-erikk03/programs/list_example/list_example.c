#include <stdlib.h>
#include <stdio.h>

#include "ADTList.h"

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

int main() {
	List list = list_create(free);

	// προσθήκη των αριθμών από το 0 έως 9 στη λίστα
	for (int i = 0; i < 10; i++){
		int* pointer;
		pointer = create_int(i);
		list_insert_next(list, list_last(list), pointer);
	}
	for (ListNode node=list_first(list); node != LIST_EOF; node = list_next(list, node)){
		int* value;
		value = list_node_value(list, node);
		printf("%d", *value);
	}
	printf("\n");
	list_destroy(list);
} 