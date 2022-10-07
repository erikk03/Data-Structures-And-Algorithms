///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω ταξινομημένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"
#include "ADTList.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί List


// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	List list;		// η λίστα στην οποία αποθηκεύουμε τα στοιχεία
	CompareFunc compare;
	DestroyFunc destroy_value;
};



// Βοηθητικές συναρτήσεις //////////////////////////////////////////////////////////////////

// Αρχικοποιεί την ταξινομημένη λίστα από τα στοιχεία του vector values.

static void naive_initialize_list(PriorityQueue pqueue, Vector values) {
	// Απλά κάνουμε insert τα στοιχεία ένα ένα (όπως η naive_heapify).
	// Προαιρετικά: sort τα στοιχεία και εισαγωγή όλα μαζί (πιο αποδοτικό).
	int size = vector_size(values);
	for(int i=0; i<size; i++){
		pqueue_insert(pqueue, vector_get_at(values, i));
	}
}

// Συναρτήσεις του ADTPriorityQueue ////////////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	// Για την κλήση της συνάρτησης destroy_value, την περνάμε απλά στη list_create
	// οπότε θα καλείται αυτόματα όταν το στοιχείο αφαιρεθεί από τη λίστα.
	pqueue->list = list_create(destroy_value);

	// Αν values != NULL, αρχικοποιούμε την ταξινομημένη λίστα.
	if (values != NULL)
		naive_initialize_list(pqueue, values);
	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return list_size(pqueue->list);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	return list_node_value(pqueue->list, list_first(pqueue->list));					// Το πρωτο στοιχειο της λιστας ειναι το μεγαλυτερο
}
void pqueue_insert(PriorityQueue pqueue, Pointer value) {
	if(list_size(pqueue->list)==0){													// Αν η λιστα ειναι κενη
		list_insert_next(pqueue->list, LIST_BOF, value);							// Προσθεσε το αρχειο
	}
	else{
		ListNode node = list_first(pqueue->list);									// Ξεκιναμε την επαναληψη απο το 1ο κομβο
		ListNode prev = LIST_BOF;													// prev θα δειχνει παντα στο προηγουμενο κομβο
		int prev_size = list_size(pqueue->list);
		do{
			if(value > list_node_value(pqueue->list, node)){						// Αν το value ειναι μεγαλυτερο απο το στοιχειο του τρεχον κομβου
			list_insert_next(pqueue->list, prev, value);							// Προσθεσε το στον προηγουμενο κομβο
			break;																	// Αν γινει προσθηκη στοιχειου, σταματα την επαναληψη
			}
			prev=node;
			node=list_next(pqueue->list, node);										// Συνεχιζουμε στον επομενο κομβο
		}while(node!=LIST_EOF);														// Επαναληψη μεχρι να φτασουμε στο τελευταιο κομβο της λιστας
		if(list_size(pqueue->list)==prev_size){										// Αν δεν εχει γινει προσθηκη στοιχειου
			list_insert_next(pqueue->list, list_last(pqueue->list), value);			// Βαλε το στοιχειο στο τελος, καθως θα ειναι το μικροτερο ολων
		}
	}
}

void pqueue_remove_max(PriorityQueue pqueue) {
	list_remove_next(pqueue->list, LIST_BOF);										// Αφαιρουμε το πρωτο στοιχειο της λιστας καθως ειναι το μεγαλυτερο
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	// Απλά καλούμε την αντίστοιχη συνάρτηση της λίστας
	return list_set_destroy_value(pqueue->list, destroy_value);
}

void pqueue_destroy(PriorityQueue pqueue) {
	list_set_destroy_value(pqueue->list, pqueue->destroy_value);
	list_destroy(pqueue->list);
	free(pqueue);

}
