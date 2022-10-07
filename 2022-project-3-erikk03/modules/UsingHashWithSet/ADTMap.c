/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Hash Table (με Set αντί για λίστες)
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTSet.h"
#include "ADTMap.h"

#define MAX_LOAD_FACTOR 0.5
#define FIXED_SIZE 3

typedef enum{
	EMPTY,
	OCCUPIED,
	DELETED
}State;

int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)
struct map_node{
	Pointer key;
	Pointer value;
	State state;
};

// Δομη του καθε καδου. Τα βασικα στοιχεια περιεχουν τις απαραιτητες πληροφοριες.
// Τα στοιχεια με ιδιο position θα αποθηκευονται σε ενα array με FIXED_SIZE η σε ενα Set, εξαρταται.
// Καθε στοιχειο του array θα ειναι map_node και θα περιεχει τις απαραιτητες πληροφοριες. 
struct bucket {
	Set set;
	MapNode array1;
	MapNode array2;
	int bucket_size;
	
};

typedef struct bucket *Bucket;

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	Bucket array;
	int capacity;
	int size;
	CompareFunc compare;
	HashFunc hash_function;
	DestroyFunc destroy_key, destroy_value;
};


#include <stdio.h>
Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Δεσμεύουμε κατάλληλα τον χώρο που χρειαζόμαστε για το hash table
	Map map = malloc(sizeof(*map));
	map->capacity = prime_sizes[0];
	map->array = malloc(map->capacity * sizeof(struct bucket));
	map->array->array1 = malloc(sizeof(struct map_node));
	map->array->array2 = malloc(FIXED_SIZE * sizeof(struct map_node));
	
	// Αρχικοποιούμε τους κόμβους που έχουμε σαν διαθέσιμους.
	for(int i=0; i<map->capacity; i++){
		map->array->array1[i].state = EMPTY;
	}
	for(int i=0; i<map->capacity; i++){
		for(int j=0; j<FIXED_SIZE; j++){
			map->array->array2[j].state = EMPTY;
		}
	}

	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

void map_insert(Map map, Pointer key, Pointer value) {
	uint pos = map->hash_function(key) % map->capacity;
	if(map->array->array1[pos].state == EMPTY){
		map->array->array1[pos].key = key;
		map->array->array1[pos].value = value;
		map->size ++;
		map->array->array1[pos].state = OCCUPIED;
	}	
	else if (map->compare(map->array->array1[pos].key, key) == 0){
		map->array->array1[pos].key = key;
		map->array->array1[pos].value = value;
	}
	else if (map->array->array1[pos].state == OCCUPIED){
		for(int i=0; i<FIXED_SIZE; i++){
			if(map->array->array2[i].state == EMPTY){
				map->array->array2[i].key = key;
				map->array->array2[i].value = value;
				map->array->array2[i].state = OCCUPIED;
				map->size++;
				map->array->bucket_size ++;
				break;
			}
		}
		if(map->array->array2[FIXED_SIZE-1].state == OCCUPIED){
			map->array->set = set_create(map->compare, map->destroy_value);
			set_insert(map->array->set, value);
			map->size++;
			map->array->bucket_size++;
			
		}
	}			


}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {
	MapNode node = map_find_node(map, key);
	if (node == MAP_EOF)
		return false;

	// destroy
	if (map->destroy_key != NULL)
		map->destroy_key(node->key);
	if (map->destroy_value != NULL)
		map->destroy_value(node->value);

	// θέτουμε ως "deleted", ώστε να μην διακόπτεται η αναζήτηση, αλλά ταυτόχρονα να γίνεται ομαλά η εισαγωγή
	node->state = DELETED;
	map->size--;

	return true;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

Pointer map_find(Map map, Pointer key) {
	MapNode node = map_find_node(map, key);
	if (node != MAP_EOF)
		return node->value;
	else
		return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {

}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}

MapNode map_find_node(Map map, Pointer key) {
	// Διασχίζουμε τον πίνακα, ξεκινώντας από τη θέση που κάνει hash το key, και για όσο δε βρίσκουμε EMPTY
	int count = 0;
	for (uint pos = map->hash_function(key) % map->capacity;		// ξεκινώντας από τη θέση που κάνει hash το key
		map->array->array1[pos].state != EMPTY;							// αν φτάσουμε σε EMPTY σταματάμε
		pos = (pos + 1) % map->capacity) {						// linear probing, γυρνώντας στην αρχή όταν φτάσουμε στη τέλος του πίνακα

		// Μόνο σε OCCUPIED θέσεις (όχι DELETED), ελέγχουμε αν το key είναι εδώ
		if (map->array->array1[pos].state == OCCUPIED && map->compare(map->array->array1[pos].key, key) == 0)
			return &map->array->array1[pos];

		// Αν διασχίσουμε ολόκληρο τον πίνακα σταματάμε. Εφόσον ο πίνακας δεν μπορεί να είναι όλος OCCUPIED,
		// αυτό μπορεί να συμβεί μόνο στην ακραία περίπτωση που ο πίνακας έχει γεμίσει DELETED τιμές!
		count++;
		if (count == map->capacity)
			break;
	}
	return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}