// Υλοποίηση του ADT Queue μέσω του ADT Stack (dependent data structure)
#include <stdlib.h>
#include <assert.h>

#include "ADTQueue.h"
#include "ADTStack.h"


// Ένα Queue είναι pointer σε αυτό το struct.
struct queue {
	Stack stack1;
	Stack stack2;
	Pointer front;															// Δεικτης στο πρωτο στοιχειο της ουρας
	Pointer back;															// Δεικτης στο τελευταιο στοιχειο της ουρας
	int steps;																// Αριθμος βηματων που πραγματοποιησε η συναρτηση queue_* που κληθηκε τελευταια
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack1 = stack_create(destroy_value);
	queue->stack2 = stack_create(destroy_value);
	queue->steps = 0;
	return queue;
}

int queue_size(Queue queue) {
	queue->steps = 1;
	return stack_size(queue->stack1) + stack_size(queue->stack2);			// Το συνολικο μεγεθος ειναι το αθροισμα των μεγεθων των 2 στοιβων
}

int queue_steps(Queue queue){
	return queue->steps;
}

Pointer queue_front(Queue queue) {
	queue->steps = 1;
	return queue->front;													// Επιστρεφουμε το στοιχειο στη βαση η στη κορυφη της αντιστοιχης στοιβας
}

Pointer queue_back(Queue queue) {
	queue->steps = 1;
	return queue->back;
}

void queue_insert_back(Queue queue, Pointer value) {
	queue->steps = 1;
	stack_insert_top(queue->stack1, value);									// Προσθηκη στοιχειου στην κορυφη της 1ης στοιβας
	if(stack_size(queue->stack1) == 1){										// Οταν γινει προσθηκη του 1ου στοιχειου
		queue->front = stack_top(queue->stack1);							// Εχουμε ενα δεικτη στο πρωτο στοιχειο της 1ης στοιβας
	}
	queue->back = stack_top(queue->stack1);									// Οταν γινει insert, το στοιχειο στην κορυφη της 1ης στοιβας ειναι το τελευταιο της ουρας
}
void queue_remove_front(Queue queue) {
	if(stack_size(queue->stack2)==0){
		queue->steps = stack_size(queue->stack1); 
	}
	else{
		queue->steps = 1;
	}
	if(stack_size(queue->stack2)==0){										// Εαν η 2η στοιβα ειναι κενη
		do{
			stack_insert_top(queue->stack2, stack_top(queue->stack1));		// Αντιγραφη στοιχειων απο την 1η στη 2η στοιβα
			stack_remove_top(queue->stack1);								// Αφαιρουμε τα στοιχεια απο την 1η στοιβα
			if(stack_size(queue->stack2)==1){								// Οταν γινει remove, τo πρωτο στοιχειο της 2ης στοιβας
				queue->back = stack_top(queue->stack2);						// Ειναι το τελευταιο της ουρας
			}
		}while(stack_size(queue->stack1)!=0);								// Μεχρι η 1η στοιβα να αδιασει
	}
	if(stack_size(queue->stack2)!=0){										// Αν η 2η στοιβα δεν ειναι κενη
		stack_remove_top(queue->stack2);									// Αφαιρεση του στοιχειου στην κορυφη
	}
	if(stack_size(queue->stack2)!=0){										// Αν εξακολουθει να μην ειναι κενη 
		queue->front = stack_top(queue->stack2);							// Το πρωτο στοιχειο της ουρας ειναι το κορυφαιο της 2ης στοιβας
	}
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack1, destroy_value);
}

void queue_destroy(Queue queue) {
	queue->steps = 1;
	stack_destroy(queue->stack1);											// Αποδεσμευση μνημης του stack1
	stack_destroy(queue->stack2);											// Αποδεσμευση μνημης του stack2
	free(queue);															// Αποδεσμευση μνημης του struct που περιεχει τα stack που δημιουργησαμε εμεις
}
