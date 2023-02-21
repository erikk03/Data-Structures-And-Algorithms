#include "set_utils.h"
#include <stdio.h>

Pointer set_find_eq_or_greater(Set set, Pointer value){
    if(set_find(set, value) != NULL){                   //Αν υπαρχει το στοιχειο επεστρεψε το
        return set_find(set, value);
    }
    else{
        set_insert(set, value);                         //Προσθετουμε το στοιχειο στο Set
        SetNode node = set_find_node(set, value);       //Το σετ ειναι ταξινομημενο με φθινουσα σειρα
        SetNode node2 = set_previous(set, node);        //Το προηγουμενο στοιχειο του Set ειναι το αμεσως μεγαλυτερο
        set_remove(set, value);                         //Αφαιρεσουμε το στοιχειο που προσθεσαμε
        if(node2 != SET_BOF){                           //Αν υπαρχει μεγαλυτερο στοιχειο επεστρεψε το
            return set_node_value(set, node2);
        }
        else{
            return NULL;                                //Αλλιως επεστρεψε NULL
        }
    }
}

Pointer set_find_eq_or_smaller(Set set, Pointer value){
    if(set_find(set, value) != NULL){                   //Αν υπαρχει το στοιχειο επεστρεψε το
        return set_find(set, value);
    }
    else{
        set_insert(set, value);                         //Προσθεσε το
        SetNode node = set_find_node(set, value);       //Tο σετ ειναι ταξινομημενο με φθινουσα σειρα
        SetNode node2 = set_next(set, node);            //Το επομενο στοιχειο ειναι το αμεσως μικροτερο
        set_remove(set, value);                         //Αφαιρουμε το στοιχειο που προσθεσαμε
        if(node2 != SET_EOF){                           //Αν υπαρχει μικροτερο στοιχειο
            return set_node_value(set, node2);          //Επεστρεψε το
        }
        else{
            return NULL;                                //Αλλιως επεστρεψε NULL
        }
    }
}
