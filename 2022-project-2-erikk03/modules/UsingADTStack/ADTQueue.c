///////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Queue μέσω του ADT Stack (dependent data structure)
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>

#include "ADTQueue.h"
#include "ADTStack.h"
#include "ADTVector.h"


// Ένα Queue είναι pointer σε αυτό το struct.
struct queue {
	Stack stack;
	Pointer front;
	int steps;																// Αριθμος βηματων που πραγματοποιησε η συναρτηση queue_* που κληθηκε τελευταια 
};


Queue queue_create(DestroyFunc destroy_value) {
	Queue queue = malloc(sizeof(*queue));
	queue->stack = stack_create(destroy_value);
	queue->steps = 0;
	return queue;
}

int queue_size(Queue queue) {
	queue->steps = 1;
	return stack_size(queue->stack);										// Επιστρεφουμε το μεγεθος της στοιβας
}

int queue_steps(Queue queue){
	return queue->steps;
}

Pointer queue_front(Queue queue) {
	queue->steps = 1;
	return queue->front;													// Επιστρεφουμε το στοιχειο στη βαση της στοιβας
}

Pointer queue_back(Queue queue) {
	queue->steps = 1;
	return stack_top(queue->stack);											// Επιστρεφουμε το στοιχειο που βρισκεται στην κορυφη
}

void queue_insert_back(Queue queue, Pointer value) {
	queue->steps = 1;
	stack_insert_top(queue->stack, value);									// Προσθηκη στοιχειου στην κορυφη
	if(stack_size(queue->stack) == 1){										// Οταν γινει προσθηκη του 1ου στοιχειου
		queue->front = stack_top(queue->stack);								// Εχουμε ενα δεικτη που δειχνει παντα στο πρωτο στοιχειο της στοιβας
	}
}

void queue_remove_front(Queue queue) {
	Vector vector = vector_create(0, NULL);
	do{
		vector_insert_last(vector, stack_top(queue->stack));				// Προσθηκη στοιχειων της στοιβας στο Vector
		stack_remove_top(queue->stack);										// Αφαιρεση στοιχειων της στοιβας
	}while(stack_size(queue->stack)!=0);									// Μεχρι να αδιασει η στοιβα

	vector_remove_last(vector);												// Αφαιρεση τελευταιου στοιχειου του Vector καθως ειναι εκεινο που ηταν στην βαση της στοιβας
	
	int size = vector_size(vector);
	for(int i=0; i<size; i++){
		queue_insert_back(queue, vector_node_value(vector, vector_last(vector)));	//Προσθηκη στοιχειων πισω στη στοιβα
		vector_remove_last(vector);											// Αφαιρεση στοιχειων απο το Vector
	}
	vector_destroy(vector);													// Αποδεσμευση μνημης που καταλαμβανε το Vector
	queue->steps = stack_size(queue->stack);
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
	queue->steps = 1;
	return stack_set_destroy_value(queue->stack, destroy_value);
}

void queue_destroy(Queue queue) {
	queue->steps = 1;
	stack_destroy(queue->stack);											// Αποδεσμευση μνημης του stack
	free(queue);															// Αποδεσμευση μνημης του struct που περιεχει το stack που δημιουργησαμε εμεις
}
