#include <raylib.h>
#include <stdio.h>
#include "state.h"
#include "interface.h"

// Assets
Texture jet_img, jet_l_img, jet_r_img, ship_img, ship_2_img, helicopter_img, helicopter_2_img, missile_img;
Sound game_over_snd;
Music song_snd;

// Αρχικοποιεί το interface του παιχνιδιού
void interface_init(){

    // Αρχικοποίηση του παραθύρου
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
	SetTargetFPS(60);
    InitAudioDevice();

	// Φόρτωση εικόνων και ήχων
	jet_img = LoadTextureFromImage(LoadImage("assets/jet.png"));
	jet_l_img = LoadTextureFromImage(LoadImage("assets/jet_l.png"));
	jet_r_img = LoadTextureFromImage(LoadImage("assets/jet_r.png"));
	ship_img = LoadTextureFromImage(LoadImage("assets/ship.png"));
	ship_2_img = LoadTextureFromImage(LoadImage("assets/ship_2.png"));
	helicopter_img = LoadTextureFromImage(LoadImage("assets/helicopter.png"));
	helicopter_2_img = LoadTextureFromImage(LoadImage("assets/helicopter_2.png"));
	missile_img = LoadTextureFromImage(LoadImage("assets/missile.png"));

	game_over_snd = LoadSound("assets/game_over.mp3");

	song_snd = LoadMusicStream("assets/song.mp3");
}

// Κλείνει το interface του παιχνιδιού
void interface_close(){

	// Unload Textures
	UnloadTexture(jet_img);
	UnloadTexture(jet_l_img);
	UnloadTexture(jet_r_img);
	UnloadTexture(ship_img);
	UnloadTexture(ship_2_img);
	UnloadTexture(helicopter_img);
	UnloadTexture(helicopter_2_img);
	UnloadTexture(missile_img);

	// Unload Sounds
	UnloadSound(game_over_snd);

	// Unload Music
	UnloadMusicStream(song_snd);

    CloseAudioDevice();
	CloseWindow();
}

// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state){
	StateInfo info = state_info(state);

	// Offsets
	int y_offset = info->jet->rect.y -(SCREEN_HEIGHT);

	// Camera
	Camera2D camera = { 0 };
    camera.target = (Vector2){ (SCREEN_WIDTH + 35)/2, info->jet->rect.y };
    camera.offset = (Vector2){ (SCREEN_WIDTH + 35)/2, y_offset + 600};
	camera.zoom = 1.0f;

	// Αν το παιχνιδι ειναι ενεργο, παιζει μουσικη	
	bool play_music = true;
	if(play_music){
		PlayMusicStream(song_snd);
	}
	UpdateMusicStream(song_snd);
    
	BeginDrawing();

	// Καθαρισμός, θα τα σχεδιάσουμε όλα από την αρχή
	ClearBackground(SKYBLUE);
	BeginMode2D(camera);
	
	// Σχεδιάζουμε το αεροσκαφος, τα ελικοπτερα, τα πλοια, τις γεφυρες και το εδαφος
	if(IsKeyDown(KEY_RIGHT)){
		DrawTexture(jet_r_img, info->jet->rect.x, info->jet->rect.y - y_offset, WHITE);
	}
	else if(IsKeyDown(KEY_LEFT)){
		DrawTexture(jet_l_img, info->jet->rect.x, info->jet->rect.y - y_offset, WHITE);
	}
	else{
		DrawTexture(jet_img, info->jet->rect.x, info->jet->rect.y - y_offset, WHITE);
	}
	List objects = state_objects(state, info->jet->rect.y + y_offset - 400, SCREEN_HEIGHT + y_offset + 600);
	for(ListNode node = list_first(objects); node!=LIST_EOF; node=list_next(objects, node)){
		Object obj = list_node_value(objects, node);
		
		if(obj->type == HELICOPTER && obj->forward){
			DrawTexture(helicopter_2_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
		}
		else if(obj->type == HELICOPTER && obj->forward == false){
			DrawTexture(helicopter_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
		}
		else if(obj->type == WARSHIP && obj->forward){
			DrawTexture(ship_2_img, obj->rect.x, obj->rect.y - y_offset,WHITE);
		}
		else if(obj->type == WARSHIP && obj->forward == false){
			DrawTexture(ship_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
		}
		else if(obj->type == TERAIN){
			DrawRectangle(obj->rect.x, obj->rect.y - y_offset, obj->rect.width, obj->rect.height, DARKGREEN);
		}
		else if(obj->type == BRIDGE){
			DrawRectangle(obj->rect.x, obj->rect.y - y_offset, obj->rect.width, obj->rect.height ,BLACK);
		}

	}

	if(info->missile != NULL){
		DrawTexture(missile_img,info->missile->rect.x, info->missile->rect.y - y_offset, GOLD);
	}

	// Σχεδιάζουμε το σκορ και το FPS counter
	DrawText(TextFormat("%04i", info->score), 20, 200, 40, BLACK);
	DrawFPS(SCREEN_WIDTH - 100, 200);
	

	//  Αν το παιχνίδι έχει τελειώσει, σχεδιάζομαι το μήνυμα για να ξαναρχίσει, οριζουμε play_music = false για να σταματησουμε την μουσικη.
	if (!info->playing) {
		play_music = false;
		DrawText(
			"PRESS [ENTER] TO PLAY AGAIN",
			 GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
			 GetScreenHeight() / 2 + 150, 20, DARKGRAY
		);
	}
	// Ηχος, αν είμαστε στο frame που συνέβη το game_over
	if(info->playing){
		PlaySound(game_over_snd);
	}
	// Αν σταματησει το παιχνιδι, σταματαει η μουσικη
	if(!play_music){
		StopMusicStream(song_snd);
	}

	// Ελευθερώνουμε όλη τη μνήμη που δεσμεύει η λίστα list
	list_destroy(objects);
	
	EndDrawing();
	
}
