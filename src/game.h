//Henna Mian
#ifndef GAME_H
#define GAME_H

#include "gba.h"

typedef struct rocket
{
    int row;
    int col;
    int width;
    int height;
    int cd;
    int rd;

} ROCKET;

typedef struct missile
{
    int row;
    int col;
    int width;
    int height;
    int cd;
    int rd;
 
} MISSILE;

typedef struct missileto
{
    int row;
    int col;
    int width;
    int height;
    int cd;
    int rd;
 
} MISSTO;

typedef struct enemy
{
    int row;
    int col;
    int cd;
    int rd;
    int ogRow;
    int ogCol;

} ENEMY;

void undrawWithBackgroundImage(const u16* image, int r, int c, int w, int h);
void setScene(char highScoreBuffer[], int lives, unsigned int highScore, ROCKET rock, char scoreBuffer[], unsigned int score, ENEMY enemies[]);
void doActions(ROCKET rock, u32 currentButtons, ENEMY enemies[], u32 previousButtons);
void moveRocket(ROCKET rock, u32 currentButtons, u32 previousButtons);
void moveEn(ENEMY enemies[]);
// void isA(u32 currentButtons, u32 previousButtons, GBAState state);
void time(int sec);
void enemyShoot(ROCKET rock, ENEMY enemies[], MISSTO to, int loosing);


#endif
