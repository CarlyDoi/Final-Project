#include "game.h"
#include <stdlib.h>
#include "myLib.h"
#include "spritesheet.h"

//variables
PLAYER player;
ENEMY enemy[ENEMIES];
BULLET bullet[BULLCOUNT];
ANIENEMY aniEnemy[ANIENEMIES];
int totalEnemies;
int seed;



///////////////////////////////// GAME /////////////////////////////////

// Initialize the game
void initGame() {

	initPlayer();
	initBullets();
	initEnemies();
	initAniEnemies();

	DMANow(3, &shadowOAM, OAM, 128*4);

	totalEnemies = 15;

}

void updateGame() {

	seed++;
	//update player
	updatePlayer();

	//update all animated enemies
	for (int i = 0; i < ANIENEMIES; i++) {
		updateAniEnemies(&aniEnemy[i]);
	}

	//update all normal enemies
	for (int i = 0; i < ENEMIES; i++) {
		updateEnemies(&enemy[i]);
	}

	//update all bullets
	for (int i = 0; i < BULLCOUNT; i++) {
		updateBullet(&bullet[i]);
	}
	DMANow(3, &shadowOAM, OAM, 128*4);
}



///////////////////////////////// PLAYER /////////////////////////////////

void initPlayer() {

	//set all player values
    player.row = SCREENHEIGHT - 16;
	player.col = SCREENWIDTH/2 - 16/2;
    player.cdel = 1;
	player.width = 16;
    player.height = 16;
    player.bulletTimer = 0;
}

void updatePlayer() {

	//move player
	if (BUTTON_HELD(BUTTON_LEFT)
		&& player.col >= 1) {

		player.col -= player.cdel;

	} else if (BUTTON_HELD(BUTTON_RIGHT)
		&& player.col + player.width < SCREENWIDTH) {

		player.col += player.cdel;

	}

	//fire bullet
	if (BUTTON_PRESSED(BUTTON_A) && player.bulletTimer >= 13) {

		fireBullet();
		player.bulletTimer = 0;
	}

	player.bulletTimer++;

	//set up in shadowOAM
    shadowOAM[0].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | player.row;
    shadowOAM[0].attr1 = ATTR1_SMALL | player.col;
    shadowOAM[0].attr2 = ATTR2_TILEID(0, 3*2);

	//fire bullets
}


///////////////////////////////// BULLETS /////////////////////////////////

void initBullets() {

	//for loop, set all values
	for (int i = 0; i < BULLCOUNT; i++) {
    	bullet[i].row = bullet[i].row;
    	bullet[i].col = bullet[i].col;
    	bullet[i].rdel = 2;
    	bullet[i].width = 8;
    	bullet[i].height = 8;
    	bullet[i].isGood = i % 3;
    	bullet[i].isActive = 0;
	}
}

void fireBullet() {

	//find first inactive bullet

	for (int i = 0; i < BULLCOUNT; i++) {
		if (!bullet[i].isActive) {

			//position the new bullet depending on isGood
			if (bullet[i].isGood) {
				bullet[i].row = player.row;
				bullet[i].col = player.col + player.width / 2 - bullet[i].width / 2;

			    shadowOAM[46+i].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | bullet[i].row;
    			shadowOAM[46+i].attr1 = ATTR1_SMALL | bullet[i].col;
    			shadowOAM[46+i].attr2 = ATTR2_TILEID(0, 4*2);

			}
			//take the bullet out of the pool
			bullet[i].isActive = 1;

			//break out of loop
			// break;
		}
	}

}

void updateBullet(BULLET* b) {

	//if active, update. move the bullet up or down the screen
	if (b->isActive && b->isGood) {
		if (b->row + b->height > 0) {

			b->row -= b->rdel;

			for (int i = 0; i < BULLCOUNT; i++) {
				shadowOAM[46+i].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | (b->row);
    			shadowOAM[46+i].attr1 = ATTR1_SMALL | (b->col);
    			shadowOAM[46+i].attr2 = ATTR2_TILEID(0, 4*2);
    		}

		} else {
			b->isActive = 0;
		}
	}
	for (int i = 0; i < ENEMIES; i ++) {
		if (collision (b->row, b->col, b->height, b->width, enemy[i].row, enemy[i].col, enemy[i].height, enemy[i].width)) {
			enemy[i].isActive = 0;
			b->isActive = 0;

			if (enemy[i].isBlue) {
				shadowOAM[1+i].attr0 = ATTR0_HIDE;

			} else if (!enemy[i].isBlue) {
				shadowOAM[16+i].attr0 = ATTR0_HIDE;

			}
		}
	}
	for (int i = 0; i < ANIENEMIES; i++) {
		if (collision (b->row, b->col, b->height, b->width, aniEnemy[i].row, aniEnemy[i].col, aniEnemy[i].height, aniEnemy[i].width)) {
			aniEnemy[i].isActive = 0;
			b->isActive = 0;
			shadowOAM[i+31].attr0 = ATTR0_HIDE;

		}
	}


}


///////////////////////////////// ENEMIES /////////////////////////////////

void initEnemies() {
	//for loop, set all values
	int space = 0;
	for (int i = 0; i < ENEMIES; i++) {
	    enemy[i].row = 0;
    	enemy[i].col = 16*space;
    	enemy[i].cdel = 1;
    	enemy[i].rdel = 1;
    	enemy[i].width = 16;
    	enemy[i].height = 16;
    	enemy[i].isBlue = i % 2;
    	enemy[i].isActive = 1;
    	space++;
	}

	//for loop, set all sprites
	for (int i = 0; i < ENEMIES; i++) {
		if (enemy[i].isActive) {

			//if the enemy is blue, put on 0 row
			if (enemy[i].isBlue) {
				for (int i = 0; i < ENEMIES/2; i++) {
    				shadowOAM[i+1].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | enemy[i].row;
    				shadowOAM[i+1].attr1 = ATTR1_SMALL | enemy[i].col;
    				shadowOAM[i+1].attr2 = ATTR2_TILEID(0, 2*2);
    				space++;
				}

			//if enemy is not blue (yellow), put on 1 row
			} else {
				for (int i = 0; i < ENEMIES/2; i++) {
    				shadowOAM[i+16].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | (enemy[i].row + 16);
    				shadowOAM[i+16].attr1 = ATTR1_SMALL | enemy[i].col;
    				shadowOAM[i+16].attr2 = ATTR2_TILEID(0, 2*1);
    				space++;
				}
			}
		}
	}
}

void updateEnemies(ENEMY* e) {

	int end = e->col + e->width;

	if (e->isActive && !(seed % 15)) {
		//move enemies
		e->col += e->cdel;
		for (int i = 0; i < ENEMIES/2; i++) {
    		shadowOAM[i+16].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | enemy[i].row;
    		shadowOAM[i+16].attr1 = ATTR1_SMALL | enemy[i].col;
    		shadowOAM[i+16].attr2 = ATTR2_TILEID(0, 2*2);
		}
		for (int i = 0; i < ENEMIES/2; i++) {
    		shadowOAM[i+1].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | (enemy[i].row + 16);
    		shadowOAM[i+1].attr1 = ATTR1_SMALL | enemy[i].col;
    		shadowOAM[i+1].attr2 = ATTR2_TILEID(0, 2*1);
		}

		if (end > SCREENWIDTH) {
			e->row += 15;
			e->cdel = -1;

		} else if (e->col < 1) {
			e->row += 35;
			e->cdel = 1;
		}

	}

}


///////////////////////////////// ANIMATED ENEMIES /////////////////////////////////

void initAniEnemies() {
	//for loop, set all values
	int space = 0;
	for (int i = 0; i < ANIENEMIES; i++) {
		aniEnemy[i].row = 32;
		aniEnemy[i].col = 16 * space;
    	aniEnemy[i].rdel = 1;
    	aniEnemy[i].cdel = 1;
		aniEnemy[i].width = 16;
    	aniEnemy[i].height = 16;
    	aniEnemy[i].curFrame = 0;
    	aniEnemy[i].isActive = 1;
    	space++;
	}

	//set up sprites on screen

	for (int i = 0; i < ANIENEMIES; i++) {
    	shadowOAM[i+31].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | (aniEnemy[i].row);
    	shadowOAM[i+31].attr1 = ATTR1_SMALL | (aniEnemy[i].col);
    	shadowOAM[i+31].attr2 = ATTR2_TILEID(0, 0);

	}

}



void updateAniEnemies(ANIENEMY* a) {

	int end = a->col + a->width;

	if (a->isActive && !(seed % 15)) {
		//move enemies
		a->col += a->cdel;
		//change state
		a->curFrame = (a->curFrame + 1) % 2;


		for (int i = 0; i < ANIENEMIES; i++) {
    		shadowOAM[i+31].attr0 = ATTR0_REGULAR | ATTR0_4BPP | ATTR0_SQUARE | (enemy[i].row + 32);
    		shadowOAM[i+31].attr1 = ATTR1_SMALL | enemy[i].col;
    		shadowOAM[i+31].attr2 = ATTR2_TILEID(a->curFrame*2, 0);
		}

		if (end > SCREENWIDTH) {
			a->row += 10;
			a->cdel = -1;

		} else if (a->col < 1) {
			a->row += 55;
			a->cdel = 1;
		}

	}
}