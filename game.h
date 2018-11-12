


//player struct

typedef struct {
	int row;
	int col;
	int rdel;
	int width;
	int height;
	int aniCounter;
	int aniState;
	int prevAniState;
	int curFrame;
	int numFrames;

} PLAYER;

//enemy
typedef struct {
	int row;
	int col;
	int rdel;
	int cdel;	//is this needed?
	int width;
	int height;
	int aniCounter;
	int aniState;
	int prevAniState;
	int curFrame;
	int numFrames;
	int isActive;

} ENEMY;

//coin
typedef struct {
	int row;
	int col;
	int width;
	int height;
	int aniCounter;
	int aniState;
	int prevAniState;
	int curFrame;
	int numFrames;
	int isActive;

} COIN;

//lives
typedef struct {
	int row;
	int col;
	int width;
	int height;
	int isActive;

} LIFE;

//constants
#define ENEMIES 10
#define LIVES 3
#define COINS 5

//variable
extern PLAYER player;
extern ENEMY enemies[ENEMIES];
extern COIN coins[COINS];
extern LIFE lives[LIVES];
int livesLeft;

//enum for the states of the player, enemies, coins
enum{PLAYERUP, PLAYERDOWN};
enum{ENEMYUP, ENEMYDOWN};
enum{FACE, SIDE};

//prototypes
void initGame();
void updateGame();
void drawGame();

void initPlayer();
void updatePlayer();

void initCoins();
void updateCoins();

void initLives();
void updateLives();
