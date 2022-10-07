//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "state.h"
#include "set_utils.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);
	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	TEST_ASSERT(info->missile == NULL);
}

void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το αεροσκάφος μετακινείται 3 pixels πάνω
	Rectangle old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x && new_rect.y == old_rect.y -3);

	// Με πατημένο το πανω βέλος, το αεροσκάφος μετακινείται 6 pixels μπροστά
	keys.up = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 6 );
	
	// Με πατημενο το κατω βελος, το αεροσκαφος μετακινειτε 2 pixels μπροστα
	keys.up = false;
	keys.down = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 2 );

	// Με πατημενο το αριστερο βελος, το αεροσκαφος μετακινειτε 3 pixels αριστερα
	keys.left = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x - 3 );

	// Με πατημενο το δεξι βελος, το αεροσκαφος μετακινειτε 3 pixels δεξια
	keys.left = false;
	keys.right = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x + 3 );
	
	// Αν πατηθει space, εκτοξευονται πυραυλοι
	keys.space = true;
	state_update(state, &keys);
	old_rect = state_info(state)->missile->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->missile->rect;

	TEST_ASSERT(new_rect.y == old_rect.y -10);
	
	// Αν γινει καποια συγκρουση
	List list = state_objects(state, 0, SCREEN_HEIGHT);
	for(ListNode node=list_first(list); node!=LIST_EOF; node=list_next(list, node)){
		Object obj = list_node_value(list, node);
		Rectangle rec1 = state_info(state)->missile->rect;
		Rectangle rec2 = obj->rect;
		if(CheckCollisionRecs(rec1, rec2)){
			TEST_ASSERT( rec1.x == rec2.x && rec1.y == rec2.y);
		}
	}
}

void test_state_objects(){
	State state = state_create();

	List list = state_objects(state, 0, SCREEN_HEIGHT);
	for(ListNode node=list_first(list); node!=LIST_EOF; node=list_next(list, node)){
		Object obj = list_node_value(list, node);
		TEST_ASSERT(!(obj->rect.y > 0));
		TEST_ASSERT(!(obj->rect.y < SCREEN_HEIGHT));
		TEST_ASSERT(obj->rect.y <= 0 && obj->rect.y >= SCREEN_HEIGHT);
	}
	
}

// Συναρτηση compare
int compare(Pointer a, Pointer b){
    int* ia = a;
    int* ib = b;
    return *ib - *ia;	// φθινουσα ταξινομιση
}

// Συναρτηση create_int
int* create_int(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}

void test_set_find_eq_or_greater(){
	Set set = set_create(compare, free);
	int* ret;

	// Γεμιζουμε το Set
	for(int i=-2; i<=5; i++){
		set_insert(set, create_int(i));
	}

	ret = set_find_eq_or_greater(set, create_int(1));
	TEST_ASSERT( *ret == 1);

	ret = set_find_eq_or_greater(set, create_int(6));
	TEST_ASSERT( ret == NULL);

	ret = set_find_eq_or_greater(set, create_int(0));
	TEST_ASSERT( *ret == 0);

	ret = set_find_eq_or_greater(set, create_int(-3));
	TEST_ASSERT( *ret == -2);

	set_destroy(set);
}

void test_set_find_eq_or_smaller(){
	Set set = set_create(compare, free);
	int* ret;

	// Γεμιζουμε το Set
	for(int i=-2; i<=5; i++){
		set_insert(set, create_int(i));
	}

	ret = set_find_eq_or_smaller(set, create_int(6));
	TEST_ASSERT( *ret == 5);
	
	ret = set_find_eq_or_smaller(set, create_int(1));
	TEST_ASSERT( *ret == 1);
	
	ret = set_find_eq_or_smaller(set, create_int(-3));
	TEST_ASSERT( ret == NULL);

	ret = set_find_eq_or_smaller(set, create_int(-2));
	TEST_ASSERT( *ret == -2);

	set_destroy(set);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },
	{ "test_state_objects", test_state_objects },
	{ "test_set_find_eq_or_greater", test_set_find_eq_or_greater },
	{ "test_set_find_eq_or_smaller", test_set_find_eq_or_smaller },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};