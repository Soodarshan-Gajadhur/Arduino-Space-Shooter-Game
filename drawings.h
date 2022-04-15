/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim

Final Project - Space Shooter
----------------------------------------------------*/

#ifndef DRAWINGS_H
#define DRAWINGS_H

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <EEPROM.h>
#include "lcd_image.h"
#include "consts.h"

#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeSerifBold12pt7b.h"

extern const uint16_t PROGMEM Tf[];
extern const uint16_t PROGMEM ex[];
extern const uint16_t PROGMEM exsmall[];

// all shared variables in the project!
// declared in SpaceShooter.cpp
extern shared_vars shared;


/*
Description: Checks if the user has touched the MAIN MENU, START or HIGHSCORE buttons on the screen.
        Caters for both left and right orientaion.

Argument:
        displayNum (uint8_t): the button number to check for touch
                                displayNum = 0 --> main main      
                                displayNum = 1 --> game over
                                displayNum = 2 --> Highscore

Return:
        true if the corresponding button is touched, otherwise returns false
*/
bool checkTouch(uint8_t displayNum);

/*
Description: Draws and displays the main menu page

Argument:
        none

Return:
        none
*/
void drawMenuPage();

/*
Description: Displays message msg, starting at position (x, y)

Argument:
        x (uint16_t): starting x position of msg
        y (uint16_t): starting y position of msg
        msg (const char*): the message to be displayed
        
Return:
        none
*/
void displayMsg(uint16_t x, uint16_t y, const char *msg);

/*
Description: Displays the loading logo (from a predefined template)

Argument:
        none
        
Return:
        none
*/
void loadingIcon();

/*
Description: Displays the highscore page, showing the highest score ever in the game

Argument:
        none
        
Return:
        none
*/
void HighscoreDisplay();

void StartCommunication();

/*
Description: Sets up a communication at the end of each game to ensure that the
        two arduinos are still in sync
        
Argument:
        none
        
Return:
        none
*/
void EndCommunication();

/*
Description: Writes an uint16_t to Serial3, starting from the least-significant
        and finishing with the most significant byte.

Argument:
        data (uint16_t): the value to be written to Serial3
        
Return:
        none
*/
void dataWrite(uint16_t data);

/*
Description: Updates the score of each ship during the game and updates the
        new score on the screen
        
Argument:
        add (int): the increment/decrement to the score
        
Return:
        none
*/
void updateScore(int add);

/*
Description: Reads an uint16_t from Serial3, starting from the least-significant
        and finishing with the most significant byte. Checks if the data is a score 
        value or a y-coordinate of a bullet from the opponent according to the check
        defined. If score value, update score accordingly and return 0.
        If a y-coordinate of a bullet, compute the corresponding y-coordinate of that 
        bullet to be displayed on the other screen. 

Argument:
        none
        
Return:
        data (uint16_t): the computed data. if no data is read or if a score value is read,
                    returns 0.
*/
uint16_t dataRead();

/*
Description: Writes score (which is two bytes long) to Serial3, starting from the 
        least-significant and finishing with the most significant byte. If score is 
        negative, converts it to a larger positive value before writing it to Serial3. 

Argument:
        data (int16_t): the score to be written to Serial3 

Return:
        none
*/
void scoreWrite(int16_t data);

/*
Description: Reads an uint16_t from Serial3, starting from the least-significant
        and finishing with the most significant byte.  If the value read>=5000,
        this indicates that this value is supposed to be a negative value, so 
        converts to the corresponding negative value accordingly.

Argument:
        none
        
Return:
        data (int16_t): the computed data/score
*/
int16_t scoreRead();

/*
Description: Draws ship at position (x, y) which corresponds to the top left corner of the ship
        Also, draws a black rectangle at the top or bottom of the ship, depending on the value of 
        direction, to erase unnecessary part during ship movement

Argument:
        ShipX (uint16_t): the x position of the ship
        ShipY (uint16_t): the y position of the ship
        direction (int8_t): the direction to which the ship is moving
                            direction = -1 (moving up) or 1 (moving down)

Return:
        none
*/

void drawShip(uint16_t ShipX, uint16_t ShipY, int8_t direction);


/*
Description: Waits  for a certain  number  of  bytes on  Serial3  or  timeout

Arguments:
        nbytes: the  number  of  bytes we want
        timeout: timeout  period (ms);
               
Return:
        True if the  required  number  of bytes  have  arrived.
*/
bool  wait_on_serial3(uint8_t  nbytes , long  timeout);

/*
Description: Draws enemy at position its new position and draw a black rectagle at the previous enemy position 
        using previousY
        

Argument:
        previousY (int): previous y position of the enemy

Return:
        none
*/
void drawEnemy(int previousY);

/*
Description: Draws the enemy's bullet and draws a black circle at previous bullet position 

Argument:
        enemyBullet (Bullet&): the bullet to be drawn 

Return:
        none
*/
void redrawEnemyBullet(Bullet& enemyBullet);

/*
Description: Draws the player's bullet and draws a black circle at previous bullet position 

Argument:
        playerBullet (Bullet&): the bullet to be drawn 

Return:
        none
*/
void drawPlayerBullet(Bullet& playerBullet);

/*
Description: Draws the background of the gameplay, whereby each white pixels represent a star 

Argument:
        none 

Return:
        none
*/
void drawBackground();

/*
Description: Updates the timer on the sceen 

Argument:
        none 

Return:
        none
*/
void TimerDisplay();

/*
Description: Draws the frame around the gameplay display 

Argument:
        none 

Return:
        none
*/
void drawFrame();

/*
Description: Resets all the necessary variables/objects to their initial values

Argument:
        none 

Return:
        none
*/
void reset();

/*
Description: Displays who won or lost with their corresponding scores, where each arduino
        sends its score to each other for comparison. Also updates the highestScore accordingly
        

Argument:
        none
        
Return:
        none
*/
void GameOverDisplay();

/*
Description: Display an explosion when the enemy is hit by a bullet
        
Argument:
        enemy (Enemy&): a pointer to an instance of the enemy
        
Return:
        none
*/
void explosion(Enemy& enemy);

#endif
