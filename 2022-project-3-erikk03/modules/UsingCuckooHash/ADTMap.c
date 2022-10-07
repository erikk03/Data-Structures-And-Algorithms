/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Cuckoo hashing
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ADTMap.h"

#define MAX_LOAD_FACTOR 0.5

// Κατασταση κομβων
typedef enum {
	EMPTY,
	OCCUPIED,
	DELETED
} State;

// Μεγεθος Hash Table
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};


// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map).
struct map_node{
	Pointer key;
	Pointer value;
	State state;
};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable).
struct map {
	MapNode array1, array2;						// Οι δυο πινακες που θα χρησιμοποιησουμε
	int capacity;								// Ο δεσμευμενος χωρος	
	int size;									// Αριθμος στοιχειων
	int reallocations;							// Αριθμος στοιχειων που εγιναν ανακατανομη
	CompareFunc compare;						// Συναρτηση για συγκριση δεικτων
	HashFunc hash_function;						// Συναρτηση για να παιρνουμε το hash code καθε αντικειμενου
	DestroyFunc destroy_key, destroy_value;		// Συναρτησεις για διαγραφη κομβου
};

// Δημιουργεί και επιστρέφει ένα map.
Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Δεσμευση απαραιτητου χωρου
	Map map = malloc(sizeof(*map));
	map->capacity = prime_sizes[0];
	map->array1 = malloc(map->capacity * sizeof(struct map_node));
	map->array2 = malloc(map->capacity * sizeof(struct map_node));
	
	// Αρχικοποιηση
	for(int i=0; i < map->capacity; i++){
		map->array1[i].state = EMPTY;
		map->array2[i].state = EMPTY;
	}
	map->size = 0;
	map->reallocations = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Συνάρτηση για την επέκταση του Hash Table σε περίπτωση που ο load factor μεγαλώσει πολύ.

static void rehash(Map map) {
	// Αποθήκευση των παλιών δεδομένων
	int old_capacity = map->capacity;
	MapNode old_array1 = map->array1;
	MapNode old_array2 = map->array2;

	// Βρίσκουμε τη νέα χωρητικότητα, διασχίζοντας τη λίστα των πρώτων ώστε να βρούμε τον επόμενο. 
	int prime_no = sizeof(prime_sizes) / sizeof(int);	// το μέγεθος του πίνακα
	for (int i = 0; i < prime_no; i++) {					// LCOV_EXCL_LINE
		if (prime_sizes[i] > old_capacity) {
			map->capacity = prime_sizes[i]; 
			break;
		}
	}
	// Αν έχουμε εξαντλήσει όλους τους πρώτους, διπλασιάζουμε
	if (map->capacity == old_capacity)					// LCOV_EXCL_LINE
		map->capacity *= 2;								// LCOV_EXCL_LINE

	// Δημιουργούμε ένα μεγαλύτερο hash table
	map->array1 = malloc(map->capacity * sizeof(struct map_node));
	map->array2 = malloc(map->capacity * sizeof(struct map_node));
	for (int i = 0; i < map->capacity; i++){
		map->array1[i].state = EMPTY;
		map->array2[i].state = EMPTY;
	}
	// Τοποθετούμε ΜΟΝΟ τα entries που όντως περιέχουν ένα στοιχείο (το rehash είναι και μία ευκαιρία να ξεφορτωθούμε τα deleted nodes)
	map->size = 0;
	for (int i = 0; i < old_capacity; i++){
		if (old_array1[i].state == OCCUPIED){
			map_insert(map, old_array1[i].key, old_array1[i].value);
		}
		if (old_array2[i].state == OCCUPIED){
			map_insert(map, old_array2[i].key, old_array2[i].value);
		}
	}

	//Αποδεσμεύουμε τον παλιό πίνακα ώστε να μήν έχουμε leaks
	free(old_array1);
	free(old_array2);
}

// Βοηθητικη αναδρομικη συναρτηση για εισαγωγη στοιχειων στην καταλληλη θεση
// Η μεταβλητη array μας δειχνει καθε φορα σε ποιον πινακα ειμαστε για να λαβουμε το σωστο position για τα αντικειμενα
void reallocate(Map map, int array, Pointer key, Pointer value){
	unsigned int pos;
	
	if(array == 1){
		pos = map->hash_function(key) % map->capacity;

		// Αν ειναι αδεια η συγκεκριμεη θεση, προσθεσε
		if(map->array1[pos].state == EMPTY){
			map->array1[pos].key = key;
			map->array1[pos].value = value;
			map->array1[pos].state = OCCUPIED;
			map->size++;

		}
		// Αλλιως αν υπαρχει στοιχειο με ιδιο key, κανε αντικατασταση
		else if(map->array1[pos].state == OCCUPIED && map->compare(map->array1[pos].key, key) == 0){
			
			map->array1[pos].key = key;
			map->array1[pos].value = value;

		}
		// Σε καθε αλλη περιπτωση, ψαξε στον αλλο πινακα για καταλληλη θεση
		else if(map->array1[pos].state == OCCUPIED){
			// Κραταμε προσωρινα
			Pointer temp_key = map->array1[pos].key;
			Pointer temp_value = map->array1[pos].value;
			
			// Κανουμε αντικατασταση
			map->array1[pos].key = key;
			map->array1[pos].value = value;

			// Παμε στον αλλο πινακα και δινουμε τις προσωρινες τιμες για καταλληλη προσθηκη
			reallocate(map, 2, temp_key, temp_value);
		}
	}
	if(array == 2){
		pos = ((map->hash_function(key) * map->hash_function(key))/map->capacity) % map->capacity;
		
		// Αν ειναι αδεια η συγκεκριμεη θεση, προσθεσε
		if(map->array2[pos].state == EMPTY){
			map->array2[pos].key = key;
			map->array2[pos].value = value;
			map->array2[pos].state = OCCUPIED;
			map->size++;

		}
		// Αλλιως αν υπαρχει στοιχειο με ιδιο key, κανε αντικατασταση
		else if(map->array2[pos].state == OCCUPIED && map->compare(map->array2[pos].key, key) == 0){
			
			map->array2[pos].key = key;
			map->array2[pos].value = value;

		}
		// Σε καθε αλλη περιπτωση, ψαξε στον αλλο πινακα για καταλληλη θεση
		else if(map->array2[pos].state == OCCUPIED){
			// Κραταμε προσωρινα
			Pointer temp_key = map->array2[pos].key;
			Pointer temp_value = map->array2[pos].value;

			// Κανουμε αντικατασταση
			map->array2[pos].key = key;
			map->array2[pos].value = value;
			
			// Παμε στον αλλο πινακα και δινουμε τις προσωρινες τιμες για καταλληλη προσθηκη
			reallocate(map, 1, temp_key, temp_value);
		}

	}
	
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.
void map_insert(Map map, Pointer key, Pointer value){
	
	// Βοηθητικη αναδρομικη συναρτηση για την εισαγωγη στοιχειων
	reallocate(map, 1, key, value);

	// Αν με την νέα εισαγωγή ξεπερνάμε το μέγιστο load factor, πρέπει να κάνουμε rehash
	float load_factor = (float)map->size / map->capacity;
	if(load_factor > MAX_LOAD_FACTOR || map->reallocations == map->size){
		rehash(map);
	}

}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {
	// Βρισκουμε τον κομβο
	MapNode node = map_find_node(map, key);
	
	if(node == MAP_EOF){
		return false;
	}
	
	// Αν υπαρχει και εχουν δωθει συναρτησεις DestroyFunc
	if(map->destroy_key != NULL){
		map->destroy_key(node->key);
	}
	if(map->destroy_value != NULL){
		map->destroy_value(node->value);
	}

	// Οριζουμε το στοιχειο deleted και μειωνουμε το μεγεθος
	node->state = DELETED;
	map->size--;

	return true;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.
Pointer map_find(Map map, Pointer key) {
	// Βρισκουμε τον κομβο
	MapNode node = map_find_node(map, key);
	
	// Επιστρεφουμε την τιμη του κομβου
	if(node != MAP_EOF){
		return node->value;
	}
	else{
		return NULL;
	}
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση key/value.
// Επιστρέφει την προηγούμενη τιμή της συνάρτησης.
DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

// Απελευθέρωση μνήμης που δεσμεύει το map.
void map_destroy(Map map) {
	for (int i = 0; i < map->capacity; i++) {
		if (map->array1[i].state == OCCUPIED) {
			if (map->destroy_key != NULL)
				map->destroy_key(map->array1[i].key);
			if (map->destroy_value != NULL)
				map->destroy_value(map->array1[i].value);
		}

		if (map->array2[i].state == OCCUPIED) {
			if (map->destroy_key != NULL)
				map->destroy_key(map->array2[i].key);
			if (map->destroy_value != NULL)
				map->destroy_value(map->array2[i].value);
		}
	}

	free(map->array1);
	free(map->array2);
	free(map);
}

////////// Διάσχιση του map μέσω κόμβων //////////

MapNode map_first(Map map) {
	// Το πρωτο στοιχειο που θα βρεθει στο 1ο array, ειναι και το 1ο του map 
	for(int i = 0; i < map->capacity; i++){
		if(map->array1[i].state == OCCUPIED){
			return &map->array1[i];
		}
	}
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	// Το node είναι pointer στο i-οστό στοιχείο του array, οπότε node - array == i  (pointer arithmetic!)
	for (int i = node - map->array1 + 1; i < map->capacity; i++){
		if (map->array1[i].state == OCCUPIED)
			return &map->array1[i];
		else if (map->array2[i].state == OCCUPIED)
			return &map->array2[i];
	}
	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}

MapNode map_find_node(Map map, Pointer key) {
	uint pos; 
	
	// Ελεγχουμε τους πινακες, αν υπαρχει το στοιχειο με το συγκεκριμενο κλειδι, επιστρεφουμε τον κομβο
	// Το στοιχειο θα υπαρχει ειτε στο ενα, ειτε στο αλλο πινακα
	// Για τον 1ο πινακα
	pos = map->hash_function(key) % map->capacity;
	if(map->array1[pos].state == OCCUPIED && map->compare(map->array1[pos].key, key) == 0){
		return &map->array1[pos];
	}

	// Για τον 2ο πινακα
	pos = map->hash_function(key) * map->hash_function(key)/map->capacity % map->capacity;
	if(map->array2[pos].state == OCCUPIED && map->compare(map->array2[pos].key, key) == 0){
		return &map->array2[pos];
	}

	// Αν δεν βρεθει
	return MAP_EOF;

}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

////////// Επιπλέον συναρτήσεις για υλοποιήσεις βασισμένες σε hashing //////////

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