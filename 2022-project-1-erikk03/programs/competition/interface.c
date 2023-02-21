#include <raylib.h>
#include <stdio.h>
#include "state.h"
#include "interface.h"

// Assets
Texture jet_img, jet_l_img, jet_r_img, ufo_img, sattelite_img, missile_img, dimond_img;
Texture2D background, menu_background;
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
	ufo_img = LoadTextureFromImage(LoadImage("assets/ufo.png"));
	sattelite_img = LoadTextureFromImage(LoadImage("assets/sattelite.png"));
	missile_img = LoadTextureFromImage(LoadImage("assets/missile.png"));
	dimond_img = LoadTextureFromImage(LoadImage("assets/dimond.png"));

	background = LoadTexture("assets/background.png");
	menu_background = LoadTexture("assets/menu_background.png");

	game_over_snd = LoadSound("assets/game_over.mp3");

	song_snd = LoadMusicStream("assets/song.mp3");
}

// Κλείνει το interface του παιχνιδιού
void interface_close(){

	// Unload Textures
	UnloadTexture(jet_img);
	UnloadTexture(ufo_img);
	UnloadTexture(sattelite_img);
	UnloadTexture(missile_img);
	UnloadTexture(dimond_img);

	// Unload Sounds
	UnloadSound(game_over_snd);

	// Unload Music
	UnloadMusicStream(song_snd);

    CloseAudioDevice();
	CloseWindow();
}

int isInMenu = 2;
// Σχεδιάζει ένα frame με την τωρινή κατάσταση του παιχνδιού
void interface_draw_frame(State state){
	
	// Update here
    if(isInMenu==2 || isInMenu == 4){
        if(IsKeyPressed(KEY_ENTER)) isInMenu = 3;
    }
	if(isInMenu==3 || isInMenu == 4 || isInMenu == 5){
	    if(IsKeyPressed(KEY_BACKSPACE)) isInMenu = 2;
    }
	if(isInMenu==2){
		if(IsKeyPressed(KEY_H)) isInMenu = 4;
		if(IsKeyPressed(KEY_M)) isInMenu = 5;
	}

	if(isInMenu == 2){
		ClearBackground(BLANK);

		Camera2D camera = { 0 };
		camera.target = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.offset = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.zoom = 1.0f;
		BeginMode2D(camera);

		DrawTextureEx(menu_background, (Vector2){0,100}, 0.0f, 1.0f, WHITE);
		DrawText("WELCOME!\nARE YOU READY TO PLAY?",
				GetScreenWidth() / 2 - MeasureText("WELCOME!ARE YOU READY TO PLAY?", 20) / 2,
				150, 29, WHITE
		);
        DrawText("\nPRESS [ENTER] TO START THE GAME",
				GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO START THE GAME", 20) / 2,
				250, 20, PURPLE
		);
		DrawText("\n\nPRESS [M] FOR MORE LEVELS",
				GetScreenWidth() / 2 - MeasureText("PRESS [M] FOR MORE LEVELS", 20) / 2,
				250, 20, PURPLE
		);
		DrawText("\n\n\nPRESS [H] FOR HELP",
				GetScreenWidth() / 2 - MeasureText("PRESS [H] FOR HELP", 20) / 2,
				250, 20, PURPLE
		);
	}else if(isInMenu == 5){

		ClearBackground(BLANK);
		Camera2D camera = { 0 };
		camera.target = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.offset = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.zoom = 1.0f;
		BeginMode2D(camera);

		DrawTextureEx(menu_background, (Vector2){0,100}, 0.0f, 1.0f, WHITE);
		DrawText("NEW LEVELS COMING SOON...",
				GetScreenWidth() / 2 - MeasureText("NEW LEVELS COMING SOON", 12) / 2,
				250, 12, RED
		);
		DrawText("\n\nPRESS [BACKSPACE] TO GO TO MENU",
				GetScreenWidth() / 2 - MeasureText("PRESS [BACKSPACE] TO GO TO MENU", 20) / 2,
				250, 20, PURPLE
		);
	}
	else if(isInMenu == 4){
		ClearBackground(BLANK);
		
		Camera2D camera = { 0 };
		camera.target = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.offset = (Vector2){ (SCREEN_WIDTH + 35)/2, 0};
		camera.zoom = 1.0f;
		BeginMode2D(camera);
		
		DrawTextureEx(menu_background, (Vector2){0,100}, 0.0f, 1.0f, WHITE);
		DrawText("INSTRUCTIONS!",
				GetScreenWidth() / 2 - MeasureText("INSTRUCTIONS!", 29) / 2,
				150 , 29, WHITE
		);
		DrawText("PRESS [ENTER] TO START THE GAME",
				GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO START THE GAME", 18) / 2,
				250, 18, PURPLE
		);
		DrawText("\nPRESS [BACKSPACE] TO GO TO MENU",
				GetScreenWidth() / 2 - MeasureText("PRESS [BACKSPACE] TO GO TO MENU", 18) / 2,
				250, 18, PURPLE
		);
		DrawText("\n\nPRESS [ARROWS] TO MOVE THE SPACESHIP",
				GetScreenWidth() / 2 - MeasureText("PRESS [ARROWS] TO MOVE THE SPACESHIP", 18) / 2,
				250, 18, PURPLE
		);
		DrawText("\n\n\nPRESS [SPACE] TO LAUNCH A MISSILE",
				GetScreenWidth() / 2 - MeasureText("PRESS [SPACE] TO LAUNCH A MISSILE", 18) / 2,
				250, 18, PURPLE
		);
		DrawText("\n\n\n\nPRESS [P] TO PAUSE THE GAME",
				GetScreenWidth() / 2 - MeasureText("PRESS [P] TO PAUSE THE GAME", 18) / 2,
				250, 18, PURPLE
		);
		DrawText("\n\n\n\n\nPRESS [N] TO UPDATE ONLY ONE FRAME",
				GetScreenWidth() / 2 - MeasureText("PRESS [N] TO UPDATE ONLY ONE FRAME", 18) / 2,
				250, 18, PURPLE
		);
	}
	else if(isInMenu == 3){
		// Αν δεν ειμαστε στο μενου αρχιζει το παιχνιδι
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
		ClearBackground(BLANK);
		DrawTextureEx(background, (Vector2){0,0}, 0.0f, 1.0f, WHITE);
		BeginMode2D(camera);
		
		// Σχεδιάζουμε το αεροσκαφος, τα ελικοπτερα, τα πλοια, τις γεφυρες, το εδαφος και τα νομισματα

		DrawTexture(jet_img, info->jet->rect.x, info->jet->rect.y - y_offset, WHITE);
		List objects = state_objects(state, info->jet->rect.y + y_offset - 400, SCREEN_HEIGHT + y_offset + 600);
		for(ListNode node = list_first(objects); node!=LIST_EOF; node=list_next(objects, node)){
			Object obj = list_node_value(objects, node);
			
			if(obj->type == HELICOPTER){
				DrawTexture(sattelite_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
			}
			else if(obj->type == WARSHIP){
				DrawTexture(ufo_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
			}
			else if(obj->type == TERAIN){
				DrawRectangle(obj->rect.x, obj->rect.y - y_offset, obj->rect.width, obj->rect.height, GetColor(0x052c46ff));
			}
			else if(obj->type == BRIDGE){
				DrawRectangle(obj->rect.x, obj->rect.y - y_offset, obj->rect.width, obj->rect.height ,BLACK);
			}
			else if(obj->type == COIN){
				DrawTexture(dimond_img, obj->rect.x, obj->rect.y - y_offset, WHITE);
			}

		}

		if(info->missile != NULL){
			DrawTexture(missile_img,info->missile->rect.x, info->missile->rect.y - y_offset, GOLD);
		}

		// Σχεδιάζουμε το σκορ, τα νομισματα και το FPS counter
		DrawText(TextFormat("%04i", info->score), 20, 200, 40, BLACK);
		DrawText(TextFormat("%04i", info->coins), 20, 250, 40, BLACK);
		DrawFPS(SCREEN_WIDTH - 100, 200);
		

		//  Αν το παιχνίδι έχει τελειώσει, σχεδιάζομαι το μήνυμα για να ξαναρχίσει, οριζουμε play_music = false για να σταματησουμε την μουσικη.
		if (!info->playing) {
			play_music = false;
			DrawText(
				"YOU LOST",
				GetScreenWidth() / 2 - MeasureText("YOU LOST", 50) / 2,
				GetScreenHeight() / 2 + 50, 50, BLACK
			);
			DrawText(
				"PRESS [ENTER] TO PLAY AGAIN",
				GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
				GetScreenHeight() / 2 + 150, 20, PURPLE
			);
			DrawText("PRESS [BACKSPACE] TO GO TO MENU",
				GetScreenWidth() / 2 - MeasureText("PRESS [BACKSPACE] TO GO TO MENU", 20) / 2,
				GetScreenHeight() / 2 + 200, 20, PURPLE
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
	}
	
	EndDrawing();
	
}
