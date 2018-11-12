#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "game.h"

#include "winBG.h"
#include "loseBG.h"
#include "pauseBG.h"
#include "gameBG.h"
#include "startBG.h"
#include "spritesheet.h"



//sorry this is so incomplete...I got really overwhelmed this week and I can't focus on 
//working on this anymore ><

// Prototypes
void initialize();

// State Prototypes
void goToStart();
void start();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();

// States
enum {START, GAME, PAUSE, WIN, LOSE};
int state;

// Button Variables
unsigned short buttons;
unsigned short oldButtons;


OBJ_ATTR shadowOAM[128];



int main() {

    initialize();

    while(1) {

        // Update button variables
        oldButtons = buttons;
        buttons = BUTTONS;
                
        // State Machine
        switch(state) {

            case START:
                start();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
        }
               
    }
}

// Sets up GBA
void initialize() {

    REG_DISPCTL = MODE0 | BG0_ENABLE;
    // initGame();

    // Set up the first state
    goToStart();
}

// Sets up the start state
void goToStart() {

    REG_BG0CNT = BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP;

    //DMA background
    DMANow(3, &startBGTiles, &CHARBLOCK[0], startBGTilesLen/2);
    DMANow(3, &startBGMap, &SCREENBLOCK[28], startBGMapLen/2); //4 (2x2) maps for the large background (usually256x256)
    DMANow(3, &startBGPal, PALETTE, startBGPalLen/2);

    //hide sprites
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }

    DMANow(3, &shadowOAM, OAM, 128*4);

    waitForVBlank();
    flipPage();

    state = START;

}

// Runs every frame of the start state
void start() {

    waitForVBlank();
    flipPage();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) {

        initGame();
        goToGame();

    }
}

// Sets up the game state
void goToGame() {

    REG_BG0CNT = BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP;

    DMANow(3, &gameBGTiles, &CHARBLOCK[0], gameBGTilesLen/2);
    DMANow(3, &gameBGMap, &SCREENBLOCK[28], gameBGMapLen/2); //4 (2x2) maps for the large background (usually256x256)
    DMANow(3, &gameBGPal, PALETTE, gameBGPalLen/2);

    //DMA sprites
    DMANow(3, &spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, &spritesheetPal, SPRITEPALETTE, spritesheetPalLen/2);
    
    //spritesheet palette already with the start bg palette
    hideSprites();

    //enable sprites
    REG_DISPCTL = MODE0 | BG0_ENABLE | SPRITE_ENABLE;

    waitForVBlank();

    state = GAME;
}

// Runs every frame of the game state
void game() {

    updateGame();

    waitForVBlank();
    flipPage();


    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) 
        goToPause();

    else if (totalEnemies == 0)
        goToWin();
    //for the enemies or for the player
    else if (BUTTON_PRESSED(BUTTON_B))
        goToLose();

}

// Sets up the pause state
void goToPause() {

    REG_BG0CNT = BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP;

    DMANow(3, &pauseBGTiles, &CHARBLOCK[0], pauseBGTilesLen/2);
    DMANow(3, &pauseBGMap, &SCREENBLOCK[28], pauseBGMapLen/2); //4 (2x2) maps for the large background (usually256x256)
    DMANow(3, &pauseBGPal, PALETTE, pauseBGPalLen/2);

    //hide sprites
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }

    DMANow(3, &shadowOAM, OAM, 128*4);



    waitForVBlank();
    flipPage();

    state = PAUSE;
}

// Runs every frame of the pause state
void pause() {
    
    // Lock the framerate to 60 fps
    waitForVBlank();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START))
        goToGame();
    else if (BUTTON_PRESSED(BUTTON_SELECT))
        goToStart();
}

// Sets up the win state
void goToWin() {

    REG_BG0CNT = BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP;

    DMANow(3, &winBGTiles, &CHARBLOCK[0], winBGTilesLen/2);
    DMANow(3, &winBGMap, &SCREENBLOCK[28], winBGMapLen/2); //4 (2x2) maps for the large background (usually256x256)
    DMANow(3, &winBGPal, PALETTE, winBGPalLen/2);

    //hide sprites
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }

    DMANow(3, &shadowOAM, OAM, 128*4);


    waitForVBlank();
    flipPage();

    state = WIN;
}

// Runs every frame of the win state
void win() {
    
    // Lock the framerate to 60 fps
    waitForVBlank();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START))
        goToStart();
}

// Sets up the lose state
void goToLose() {

    REG_BG0CNT = BG_SIZE_LARGE | BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP;

    DMANow(3, &loseBGTiles, &CHARBLOCK[0], loseBGTilesLen/2);
    DMANow(3, &loseBGMap, &SCREENBLOCK[28], loseBGMapLen/2); //4 (2x2) maps for the large background (usually256x256)
    DMANow(3, &loseBGPal, PALETTE, loseBGPalLen/2);

    //hide sprites
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }

    DMANow(3, &shadowOAM, OAM, 128*4);


    waitForVBlank();
    flipPage();

    state = LOSE;
}

// Runs every frame of the lose state
void lose() {
     
    // Lock the framerate to 60 fps
    waitForVBlank();

    // State transitions
    if (BUTTON_PRESSED(BUTTON_START)) 
        goToStart();
}