//Henna Mian
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/missileAway.h"
#include "images/rocket.h"
#include "images/lifeRocket.h"
#include "images/enemy.h"
#include "images/win.h"
#include "images/galaga.h"
#include "images/stars.h"
#include "images/gameOver.h"
#include "images/missileTo.h"


#include "text.h"

typedef enum {
  START,
  CHECK,
  WORDS,
  //TRANSITIONSCREENLEVEL1,
  CREATE,
  PLAY,
  WIN,
  LOSE,
} GBAState;



// u32 row1[7] = {1,1,1,1,1,1,1};
// u32 row2[7] = {1,1,1,1,1,1,1};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;


  ROCKET rock;
  rock.row = 125;
  rock.col = 114;
  rock.cd = 0;
  rock.rd = 0;

  MISSILE miss;
  //MISSTO to;

  unsigned int highScore = 0;
  unsigned int score = 0;
  int lives = 3;
  int numEnemies = 5;
  //int shooting = 6;
  //int loosing = 0;
  //int level = 0;

  char scoreBuffer[100];
  char highScoreBuffer[100];
  //char levelBuffer[100];

  ENEMY enemies[5];


  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons

    waitForVBlank();
    

    switch (state) {
      case START:
          drawFullScreenImageDMA(galaga);
          drawCenteredString(3, 110, 6, 8, "HIGH SCORE", RED);
          sprintf(highScoreBuffer, "%d", highScore);
          drawString(13, 108, highScoreBuffer, WHITE);
          drawString(138, 70, "PRESS A TO PLAY", RED);
          state = CHECK;
        break;

      case CHECK:
          if (KEY_JUST_PRESSED(BUTTON_A, currentButtons, previousButtons)) {
            score = 0;
            rock.col = 114;
            lives = 3;
            numEnemies = 5;
            state = START;
          state = WORDS;
          }
          break;
      case WORDS:
        fillScreenDMA(BLACK);
        drawString(37, 48, "MOVE with ARROW keys", WHITE);
        drawString(67, 72, "SHOOT with B", WHITE);
        drawString(97, 8, "Press SELECT to go back to HOME screen", WHITE);
        time(100);

        state = CREATE;

      // case TRANSITIONSCREENLEVEL1:
      // supposed to show the level, but not used since there's only one level

      //     fillScreenDMA(BLACK);
      //     level++;

      //     drawString(62, 100, "LEVEL", WHITE);
      //     sprintf(levelBuffer, "%d", level);
      //     drawString(85, 113, levelBuffer, WHITE);

      //     time(95);

      //     state = CREATE;
      //     break;

      case CREATE:
        setScene(highScoreBuffer, lives, highScore, rock, scoreBuffer, score, enemies);
        state = PLAY;
        break;

      case PLAY:
          

        //doActions(rock, currentButtons, enemies, previousButtons);
        // supposed to call a helper method to do all the below code

       // trying to put all the logic for shooting in this case
       // shooting = randint(0, 5);

        // to.col = enemies[shooting].col + 6;
        // to.row = 0;
        // for (int i = enemies[shooting].row + 20; i < 160; i += 3) {
        //   to.row = i;
        //   if (to.row != enemies[shooting].row + 20) {
        //     drawRectDMA(to.row - 3, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, BLACK);
        //   }
        //   drawImageDMA(to.row, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, missileTo);


        //   if (to.row + 10 == rock.row && (to.col > rock.col + 1) && (to.col < rock.col + 15)) {
        //     drawRectDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
        //     state = LOSE;
        //   }
        // }
        // drawRectDMA(to.row, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, BLACK);
        // time(1);
      



        rock.cd = 0;
        rock.rd = 0;

        if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
          rock.cd = 10;
        }

        if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons))  {
          rock.cd = -10;
        }

        if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons))  {
          rock.rd = -10;
        }
        if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons))  {
          rock.rd = 10;
        }


        if (rock.cd != 0)
        {
          rock.col += rock.cd;
          
          if (rock.col > 225) {
            rock.col = 225;
          } else if (rock.col < 15) {
            rock.col = 15;
          }

          drawRectDMA(rock.row, rock.col - rock.cd, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
          drawImageDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, rocket);
        }

        if (rock.rd != 0)
        {
          rock.row += rock.rd;
          
          if (rock.row > 125) {
            rock.row = 125;
          } else if (rock.row < 80) {
            rock.row = 80;
          }

          drawRectDMA(rock.row - rock.rd, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
          drawImageDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, rocket);
        }

        if (KEY_JUST_PRESSED(BUTTON_B, currentButtons, previousButtons))  {
          miss.col = rock.col + 6;
          miss.row = 0;
          for (int i = rock.row - 10; i > 21; i -= 3) {
            miss.row = i;
            if (miss.row != rock.row - 10) {
              drawRectDMA(miss.row + 3, miss.col, MISSILEAWAY_WIDTH, MISSILEAWAY_HEIGHT, BLACK);
            }
            drawImageDMA(miss.row, miss.col, MISSILEAWAY_WIDTH, MISSILEAWAY_HEIGHT, missileAway);
            time(1);

            int j = 0;
            for (j = 0; j < 5; j++){
              if ((miss.row == enemies[j].row + 21 || miss.row == enemies[j].row + 20 || miss.row == enemies[j].row + 19) && (miss.col > enemies[j].col + 1) && (miss.col < enemies[j].col + 15)) {
                drawRectDMA(enemies[j].row, enemies[j].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
                enemies[j].row = 0;

                //if enemy is hit
                numEnemies--;
                score += 20;
                sprintf(scoreBuffer, "%d", score);
                drawRectDMA(13, 15, 10, 10, BLACK);
                drawString(13, 15, scoreBuffer, WHITE);

                if (numEnemies == 0) {
                  state = WIN;
                }
                
                j = 6;
              }
            }

            if (j == 7) {
              i = 20;
            }
          }
          drawRectDMA(miss.row, miss.col, MISSILEAWAY_WIDTH, MISSILEAWAY_HEIGHT, BLACK);
        }




          break;

      case WIN:

          drawFullScreenImageDMA(win);

          drawString(3, 10, "SCORE", RED);
          sprintf(scoreBuffer, "%d", score);
          drawString(13, 15, scoreBuffer, WHITE);
          if (score > highScore) {
            highScore = score;
          }
          drawCenteredString(3, 110, 6, 8, "HIGH SCORE", RED);
          sprintf(highScoreBuffer, "%d", highScore);
          drawString(13, 108, highScoreBuffer, WHITE);
          drawString(138, 7, "PRESS SELECT TO GO BACK TO HOME SCREEN", RED);
          drawString(120, 60, "PRESS A TO PLAY AGAIN", RED);
          state = CHECK;

        // state = ?
        break;
       case LOSE:
          drawFullScreenImageDMA(gameOver);

          drawString(3, 10, "SCORE", RED);
          sprintf(scoreBuffer, "%d", score);
          drawString(13, 15, scoreBuffer, WHITE);
          if (score > highScore) {
            highScore = score;
          }
          drawCenteredString(3, 110, 6, 8, "HIGH SCORE", RED);
          sprintf(highScoreBuffer, "%d", highScore);
          drawString(13, 108, highScoreBuffer, WHITE);
          drawString(138, 7, "PRESS SELECT TO GO BACK TO HOME SCREEN", RED);
          drawString(120, 70, "PRESS A TO PLAY AGAIN", RED);
          state = CHECK;

      //   // state = ?
      //   break;
    }
    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
      score = 0;
      rock.col = 114;
      lives = 3;
      state = START;
      numEnemies = 5;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

  return 0;
}














//trying to undraw the background
// void undrawWithBackgroundImage(const u16* image, int r, int c, int w, int h) {
// }


void setScene(char highScoreBuffer[], int lives, unsigned int highScore, ROCKET rock, char scoreBuffer[], unsigned int score, ENEMY enemies[]){
  //drawFullScreenImageDMA(stars);
  fillScreenDMA(BLACK);


  drawString(3, 10, "SCORE", RED);
  score = 0;
  sprintf(scoreBuffer, "%d", score);
  drawString(13, 15, scoreBuffer, WHITE);

  drawCenteredString(3, 120, 6, 8, "HIGH SCORE", RED);
  sprintf(highScoreBuffer, "%d", highScore);
  drawString(13, 118, highScoreBuffer, WHITE);



  //trying to make two rows of aliens
  // int numE = 0;
  // for (int height = 30; height <= 60; height += 30){
  //   if (height == 30) {
  //     for (int width = 15; width <= 215; width += 50) {
  //       enemies[numE].row = height;
  //       enemies[numE].col = width;
  //       drawImageDMA(enemies[numE].row, enemies[numE].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //       numE++;
  //     }
  //   } else {
  //     for (int width = 40; width <= 215; width += 50) {
  //       drawImageDMA(height, width, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     }
  //   }
  // }

  int numE = 0;
  for (int width = 15; width <= 215; width += 50) {
    enemies[numE].row = 40;
    enemies[numE].col = width;
    enemies[numE].cd = 2;
    enemies[numE].rd = 2;
    enemies[numE].ogRow = enemies[numE].row;
    enemies[numE].ogCol = enemies[numE].col;
    drawImageDMA(enemies[numE].row, enemies[numE].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    numE++;
  }




  drawImageDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, rocket);

  
  lives = lives;
  //trying to draw small rockets to represent lives of the rocket
  // int width = 6;
  // for (int times = 0; times < lives; times++) {
  //   drawImageDMA(145, width, LIFEROCKET_WIDTH, LIFEROCKET_HEIGHT, lifeRocket);
  //   width += 12;
  // }
}

void doActions(ROCKET rock, u32 currentButtons, ENEMY enemies[], u32 previousButtons){ //supposed to call moveRocket and moveEn but not used
  moveRocket(rock, currentButtons, previousButtons);
  //moveEn(enemies);
  enemies[0] = enemies[0];
}


void moveEn(ENEMY enemies[]){ // supposed to move the enemies in a square but not used
  for (int i = 0; i < 5; i++) {

    while (enemies[i].row < enemies[i].ogRow + 20) {
      drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
      enemies[i].row += enemies[i].rd;
      drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    }
    enemies[i].rd = -enemies[i].rd;

    while (enemies[i].col < enemies[i].ogCol + 20) {
      drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
      enemies[i].col += enemies[i].cd;
      drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    }
    enemies[i].cd = -enemies[i].cd;

    while (enemies[i].row > enemies[i].ogRow) {
      drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
      enemies[i].row += enemies[i].rd;
      drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    }
    enemies[i].rd = -enemies[i].rd;

    while (enemies[i].col > enemies[i].ogCol) {
      drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
      enemies[i].col += enemies[i].cd;
      drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    }
    enemies[i].cd = -enemies[i].cd;
  }

}

void moveRocket(ROCKET rock, u32 currentButtons, u32 previousButtons){ //suppoed to move the rocket, but not used
  rock.cd = 0;

  if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
    rock.cd = 10;
  }

  if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons))  {
    rock.cd = -10;
  }


  if (rock.cd != 0)
  {
    rock.col += rock.cd;
    
    if (rock.col > 225) {
      rock.col = 225;
    } else if (rock.col < 15) {
      rock.col = 15;
    }

    drawRectDMA(rock.row, rock.col - rock.cd, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
    drawImageDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, rocket);
  }
}


void time(int sec) { //adds some time between steps
  volatile int t = 0;
  for (int i = 0; i < sec * 10000; i++) {
    t++;
  }
}




//trying to make aliens shoot at rocket
// void enemyShoot(ROCKET rock, ENEMY enemies[], MISSTO to, int loosing) {
//   loosing = loosing;
//   int shooting = randint(0, 5);
//   to.col = enemies[shooting].col + 6;
//   to.row = 0;
//   for (int i = enemies[shooting].row + 20; i < 160; i += 3) {
//     to.row = i;
//     if (to.row != enemies[shooting].row + 20) {
//       drawRectDMA(to.row - 3, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, BLACK);
//     }
//     drawImageDMA(to.row, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, missileTo);
//     time(1);

//     if (to.row + 10 == rock.row && (to.col > rock.col + 1) && (to.col < rock.col + 15)) {
//       drawRectDMA(rock.row, rock.col, ROCKET_WIDTH, ROCKET_HEIGHT, BLACK);
//       loosing = 1;
//     }
//   }
//   drawRectDMA(to.row, to.col, MISSILETO_WIDTH, MISSILETO_HEIGHT, BLACK);
// }



  //   if(enemies[i].row + enemies[i].rd < enemies[i].row - 10) {
    //     drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
    //     enemies[i].row = enemies[i].row - 10;
    //     enemies[i].rd = -enemies[i].rd;
    //     drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    //   }
    //   if(enemies[i].row + enemies[i].rd > enemies[i].row + 10) {
    //     drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
    //     enemies[i].row = enemies[i].row + 10;
    //     enemies[i].rd = -enemies[i].rd;
    //     drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    //   }
    //   if(enemies[i].col + enemies[i].cd < enemies[i].col - 10) {
    //     drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
    //     enemies[i].col = enemies[i].col - 10;
    //     enemies[i].cd = -enemies[i].cd;
    //     drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    //   }
    //   if(enemies[i].col + enemies[i].cd > enemies[i].col + 10) {
    //     drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
    //     enemies[i].col = enemies[i].col + 10;
    //     enemies[i].cd = -enemies[i].cd;
    //     drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    //   } else {
    //     drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
    //     enemies[i].row += enemies[i].rd;
    //     enemies[i].col += enemies[i].cd;
    //     drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
    //   }
    // }




  //trying to make the aliens move across the page
  // for (int i = 0; i < 5; i++) {
  //     if(enemies[i].row + enemies[i].rd < enemies[i].row - 10) {
  //       drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
  //       enemies[i].row = enemies[i].row - 10;
  //       enemies[i].rd = -enemies[i].rd;
  //       drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     }
  //     if(enemies[i].row + enemies[i].rd > enemies[i].row + 10) {
  //       drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
  //       enemies[i].row = enemies[i].row + 10;
  //       enemies[i].rd = -enemies[i].rd;
  //       drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     }
  //     if(enemies[i].col + enemies[i].cd < enemies[i].col - 10) {
  //       drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
  //       enemies[i].col = enemies[i].col - 10;
  //       enemies[i].cd = -enemies[i].cd;
  //       drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     }
  //     if(enemies[i].col + enemies[i].cd > enemies[i].col + 10) {
  //       drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
  //       enemies[i].col = enemies[i].col + 10;
  //       enemies[i].cd = -enemies[i].cd;
  //       drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     } else {
  //       drawRectDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, BLACK);
  //       enemies[i].row += enemies[i].rd;
  //       enemies[i].col += enemies[i].cd;
  //       drawImageDMA(enemies[i].row, enemies[i].col, ENEMY_WIDTH, ENEMY_HEIGHT, enemy);
  //     }
  //   }
