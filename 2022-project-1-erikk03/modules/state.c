
#include <stdlib.h>

#include "ADTList.h"
#include "state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	List objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι το αριστερό μέρος της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά. Η πίστα έχει σταθερό πλάτος, οπότε όλες οι
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_WIDTH.
//
// - Στον άξονα y το 0 είναι η αρχή της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα αντιθέτως επεκτείνεται προς τα πάνω, οπότε όλες οι
//   συντεταγμένες των αντικειμένων είναι _αρνητικές_.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_y) {
	// Προσθέτουμε BRIDGE_NUM γέφυρες.
	// Στο διάστημα ανάμεσα σε δύο διαδοχικές γέφυρες προσθέτουμε:
	// - Εδαφος, αριστερά και δεξιά της οθόνης (με μεταβαλλόμενο πλάτος).
	// - 3 εχθρούς (ελικόπτερα και πλοία)
	// Τα αντικείμενα έχουν SPACING pixels απόσταση μεταξύ τους.

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x στο αριστερό άκρο της οθόνης
			start_y - 4 * (i+1) * SPACING,	// Η γέφυρα i έχει y = 4 * (i+1) * SPACING
			SCREEN_WIDTH,					// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terain_left = create_object(
			TERAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_WIDTH/3),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_WIDTH/2)*0.5;
		Object terain_right = create_object(
			TERAIN,
			SCREEN_WIDTH - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		list_insert_next(state->objects, list_last(state->objects), terain_left);
		list_insert_next(state->objects, list_last(state->objects), terain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 75)/2, y, 75, 45)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 55)/2, y, 55, 50);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			list_insert_next(state->objects, list_last(state->objects), enemy);
		}
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 30)/2,  0, 30, 67);

	// Δημιουργούμε τη λίστα των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = list_create(NULL);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to) {
	List list = list_create(NULL);
	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)) {
		Object obj = list_node_value(state->objects, node);
		if(obj->rect.y >= y_from && obj->rect.y <= y_to) {
			list_insert_next(list, LIST_BOF, obj);
		}
		
	}
	return list;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys){

	// Εκκινηση και διακοπη
	if(!state->info.playing){
		if(keys->enter){
			state->info.playing = true;
		}
		else{
			return;
		}
	}
	if(!state->info.paused){
		if(keys->p){
			state->info.paused = true;
		}
	}
	else{
		if(keys->p){
			state->info.paused = false;
		}
	}
	if(state->info.paused){
		if(!keys->n){
			return;
		}
	}

	// Κινηση αεροσκαφους
	if(keys->up){
		state->info.jet->rect.y -= 6 * (state->speed_factor);
	}
	else if(keys->down){
		state->info.jet->rect.y -= 2 * (state->speed_factor);
	}
	else{
		state->info.jet->rect.y -= 3 * (state->speed_factor);
	}
	
	if(keys->left){
		state->info.jet->rect.x -= 3 * (state->speed_factor);
	}
	if(keys->right){
		state->info.jet->rect.x += 3 * (state->speed_factor);
	}

	// Αρχικοποιηση μεταβλητης που χρησιμοποιειται για το πληθος γεφυρων της λιστας
	// Αρχικοποηση μεταβλητης που χρησιμοιειται για αποθηκευση των συντεταγμενων της τελευταιας γεφυρας 
	int bridge_num = 0;
	Rectangle br_rec;

	// Ευρεση πληθους γεφυρων που υπαρχουν στην λιστα και τις συντεταγμενες της τελευταιας
	for(ListNode node = list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj = list_node_value(state->objects, node);
		if(obj->type == BRIDGE){
			bridge_num += 1;
			br_rec = obj->rect;
		}
	}

	// Διασχιση της λιστας
	for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
		Object obj = list_node_value(state->objects, node);
		
		// Κινηση πλοιων | ελικοπτερων
		if(obj->type == HELICOPTER){
			obj->rect.x += 4 * (obj->forward ? 1 : -1) * (state->speed_factor);
		}
		else if(obj->type == WARSHIP){
			obj->rect.x += 4 * (obj->forward ? 1 :-1) * (state->speed_factor);
		}

		// Ελεγχος για συγκρουσεις
		// Αν το αεροσκαφος συγκρουστει με εδαφος, εχθρο η γεφυρα, τερματιζει το παιχνιδι
		if(obj->type == TERAIN || obj->type == HELICOPTER || obj->type == WARSHIP || obj->type == BRIDGE){
			if(CheckCollisionRecs(state->info.jet->rect, obj->rect)){
				state->info.playing = false;
				return;
			}
		}

		// Αν ενας εχθρος συγκρουστει με εδαφος, αλλαζει κατευθυνση
		if(obj->type == WARSHIP || obj->type == HELICOPTER){
			for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
				Object terain = list_node_value(state->objects, node);
				if(terain->type == TERAIN){
					if(CheckCollisionRecs(obj->rect, terain->rect)){
						if(obj->forward == true){
							obj->forward = false;
						}
						else{
							obj->forward = true;
						}
					}
				}
			}
		}

	}
	// Δημιουργια πυραυλου οταν ειναι πατημενο το space και δεν υπαρχει αλλος πυραυλος
	if((keys->space == true) && (state->info.missile == NULL)){
		state->info.missile = create_object(MISSLE, state->info.jet->rect.x + (state->info.jet->rect.width/2), state->info.jet->rect.y, 8, 20 );
	}

	// Αν υπαρχει πυραυλος
	if(state->info.missile != NULL){
		state->info.missile->rect.y -= 10 * (state->speed_factor);

		// Δηλωση 1ου node της λιστας, οπου το pre_node θα δειχνει το προηγουμενο node σε καθε επαναληψη
		ListNode prev_node = list_first(state->objects);
		for(ListNode node=list_first(state->objects); node!=LIST_EOF; node=list_next(state->objects, node)){
			Object obj = list_node_value(state->objects, node);
			
			// Συγκρουση πυραυλου με εδαφος
			if(obj->type == TERAIN){
				if(CheckCollisionRecs(state->info.missile->rect, obj->rect)){
		 			state->info.missile = NULL;
					return;
		 		}
			}

			// Αν ο πυραυλος φτασει σε μεγαλυτερη αποσταση (μεγαλυτερη της μιας οθονης) απο το αεροσκαφος καταστρεφεται
			if(state->info.missile->rect.y <= -SCREEN_HEIGHT + state->info.jet->rect.y + 100){
				state->info.missile = NULL;
				return;
			}

			// Συγκρουση πυραυλου με εχθρο η γεφυρα
			if(obj->type == HELICOPTER || obj->type == WARSHIP || obj->type == BRIDGE){
				if(CheckCollisionRecs(state->info.missile->rect, obj->rect)){

					// Μειωση πληθους γεφυρων που υπαρχουν στην λιστα
					if(obj->type == BRIDGE){
						bridge_num -=1;
					}

					// Οταν απομεινει μονο μια (η τελευταια) γεφυρα στη λιστα
					// Αυξησε ταχυτητα κατα 30% και προσθεσε νεα αντικειμενα
					if(bridge_num == 1 ){
						add_objects(state, br_rec.y);
						state->speed_factor += 0.3 * (state->speed_factor);
					}
					// Αφαιρεσε απο την λιστα το αντικειμενο που κατεστρεψε ο πυραυλος
					list_remove_next(state->objects, prev_node);
		 			state->info.missile = NULL;
		 			state->info.score += 10;
		 			return;
		 		}
				
			}
			prev_node = node;
		}
		
	}
}

// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

void state_destroy(State state) {
	// Προς υλοποίηση
}