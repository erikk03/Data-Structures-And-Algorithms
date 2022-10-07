#include <stdlib.h>
#include <stdio.h>

#include "ADTPriorityQueue.h"
#include "ADTSet.h"

int compare(Pointer a, Pointer b){
    int* ia = a;
    int* ib = b;
    return *ia - *ib;
}

// Δεσμεύει μνήμη για έναν ακέραιο, αντιγράφει το value εκεί και επιστρέφει pointer
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

int main() {
    PriorityQueue pqueue = pqueue_create(compare, free, NULL);
    
    //Προσθετει 20 τυχαιους αριθμους σε μια ουρα προτεραιοτητς
    for(int i=0; i<20; i++){
        int* pointer;
        pointer = create_int(rand());
        pqueue_insert(pqueue, pointer);
    }

    // Μεγεθος ουρας|Αριθμος στοιχειων
    int size = pqueue_size(pqueue);

    // Τυπωνει τον αριθμο στη κορυφη της ουρας
    // Τον αφαιρει μεχρι η ουρα να αδιασει
    printf("Οι αριθμοι της ουρας σε φθινουσα σειρα ειναι:\n");
    for(int i=0; i<size; i++){
        printf("%d\n", *(int*)pqueue_max(pqueue));
        pqueue_remove_max(pqueue);
    }
    pqueue_destroy(pqueue);

    Set set = set_create(compare, free);

    // Προσθετει 20 τυχαιους αριθμους στο συνολο
    for(int i=0; i<20; i++){
        int* pointer;
        pointer = create_int(rand());
        set_insert(set, pointer);
    }

    // Διασχιζει τα στοιχεια του συνολου
    // Τα τυπωνει σε αυξουσα σειρα
    printf("Οι αριθμοι του συνολου σε αυξουσα σειρα ειναι:\n");
    for(SetNode node = set_first(set); node != SET_EOF; node = set_next(set, node)){
        printf("%d\n", *(int*)set_node_value(set, node));
    }
    printf("\n");
    set_destroy(set);
}
