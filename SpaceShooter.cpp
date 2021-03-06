/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim

Main program
----------------------------------------------------*/

#include <Arduino.h>

#include <MCUFRIEND_kbv.h>

//needed to schedule events to occurat regular intervals
#include <Metro.h>

//needed to read/write to eeprom 
//(memory whose values are kept when the board is turned off)
#include <EEPROM.h>

//needed for reading/writing to SD card
#include <SD.h>

#include "lcd_image.h"
#include "classes.h"
#include "drawings.h"
#include "consts.h"

#define SD_CS 10

#define ORIENTATION  45
#define JOY_VERT  A9 
#define JOY_HORIZ A8
#define JOY_SEL   53
#define JOY_CENTER   512
#define JOY_DEADZONE 64

//status of enemy
#define UP 0
#define DOWN 1

MCUFRIEND_kbv tft;

uint8_t ShipBulletSpeed = 10;
uint8_t EnemyBulletSpeed = 25;

// The variables to be shared across the project, they are declared here!
// The type shared_vars is a struct declared in consts.h
shared_vars shared;

//all the objects to be used
Bullet shipBullets[objectData::max_ship_bullets];
Bullet enemyBullets[objectData::max_enemy_bullets];
Bullet opponentBullets[objectData::max_opponent_bullets];

Ship myShip(objectData::ship_start_x, objectData::ship_start_y); //x, y
Enemy tieFighter(390, 120, 20); //x, y, speed


void setup() {
    // initialize Arduino
	init();

	Serial.begin(9600);
    Serial3.begin(9600);

	pinMode(JOY_SEL, INPUT_PULLUP);

    // tft display initialization
    shared.tft = &tft;
	uint16_t ID = tft.readID();    // read ID from display
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
        
    // LCD gets ready to work
    tft.begin(ID);

    // initialize SD card
    Serial.print("SD init...");
    if (!SD.begin(SD_CS)) {
        Serial.println("failed! Is it inserted properly?");
        while (true) {}
    }
    Serial.println("OK!");  

    //initialise all objects to be used
    shared.myShip = &myShip;
    shared.tieFighter = &tieFighter;
    shared.shipBullets = shipBullets;
    shared.enemyBullets = enemyBullets;
    shared.opponentBullets = opponentBullets;

    //check orientation by reading from pin 45
    shared.rightOrientation = false;
    if (digitalRead(ORIENTATION) == HIGH) {
        shared.rightOrientation = true;
        tft.setRotation(3);
    }
    else {
        tft.setRotation(1);  
    }

    //to delete after first run
    // EEPROM.write(0 , 0);
    // EEPROM.write(1, 0);

    //read the highest score from the EEPROM, assuming highestScore to be two bytes long
    //addr 1 stores the lower 8 bits of highestScore
    byte xlo = EEPROM.read(1);	
    //addr 0 stores the upper 8 bits of highestScore
    byte xhi = EEPROM.read(0);
    //compute the highestScore by merging the two bytes using bitwise operation
    shared.highestScore = (xhi << 8) | xlo;

    //initialise enemy controls
    shared.enemyDirection = DOWN;
    shared.enemyActive = false;

    //causes the sequence of values generated by random() to differ
    randomSeed(analogRead(0));
}

//schedule event to occur at regulat intervals
//s: update ship after each 50ms
//ea: enemy appearance at a random interval for each player (25000, 40000) ms
//e: update enemy after each 250ms
//b: update bullets after each 20ms
//T: update timer after each 1000ms
Metro s = Metro(50), ea = Metro(random(25000, 40000)), e = Metro(250), b = Metro(20), T = Metro(1000);

/*
Description: the main body of the game. Controls the whole game

Argument:
       none
        
Return:
        none
*/
void initGame(){
    int xVal = analogRead(JOY_HORIZ);
    int yVal = analogRead(JOY_VERT);
    delay(20);
    int buttonVal = digitalRead(JOY_SEL);

    //update ship and background after each 50ms
    if (s.check() == 1) {

        drawBackground();

        //move ship
        if (yVal < JOY_CENTER - JOY_DEADZONE) { //up
            myShip.move(-1);
            drawShip(myShip.getX(), myShip.getY(), -1);
        }
        else if (yVal > JOY_CENTER + JOY_DEADZONE) {  //down 
            myShip.move(1);
            drawShip(myShip.getX(), myShip.getY(), 1);
        }
    }

    //enemy appearance (at a random interval)
    if (ea.check() == 1) {
        tieFighter.reset();
        shared.enemyActive = true;
        shared.enemyCycles = 0;
        shared.tickCounter = 0;
        shared.enemyDirection = DOWN;
        tft.drawRGBBitmap(objectData::enemy_spawn_x, displayData::gameplay_start_y, Tf, objectData::enemy_width, objectData::enemy_height);
    }

    if (shared.enemyActive && (e.check() == 1)) {
        shared.tickCounter++;
        if (shared.enemyDirection == UP) {
            int prev_y = tieFighter.move(-1);
            drawEnemy(prev_y);
            if (tieFighter.getY() == displayData::gameplay_start_y) {
                shared.enemyDirection = DOWN;
                shared.enemyCycles++;
            }
        }
        else if (shared.enemyDirection == DOWN) {
            int prev_y = tieFighter.move(1);
            drawEnemy(prev_y);
            if (tieFighter.getY() == displayData::gameplay_end_y - objectData::enemy_size) {
                shared.enemyDirection = UP;
                shared.enemyCycles++;
            }
        }
        if (shared.tickCounter == 16) {
            Bullet newEnemyBullet(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, EnemyBulletSpeed);
            enemyBullets[0] = newEnemyBullet;
            tft.drawCircle(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, 2, TFT_RED);
        }
        if (shared.tickCounter == 17) {
            Bullet newEnemyBullet(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, EnemyBulletSpeed);
            enemyBullets[1] = newEnemyBullet;
            tft.drawCircle(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, 2, TFT_RED);
        }
        if (shared.tickCounter == 18) {
            shared.tickCounter = 0;
            Bullet newEnemyBullet(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, EnemyBulletSpeed);
            enemyBullets[2] = newEnemyBullet;
            tft.drawCircle(objectData::enemy_spawn_x - 2, tieFighter.getY() + 17, 2, TFT_RED);
        }
        if (shared.enemyCycles == 5) { //Disappears after moving up & down 3 times
            shared.enemyActive = false;
            tft.fillRect(objectData::enemy_spawn_x, tieFighter.getY(), objectData::enemy_width, objectData::enemy_height, TFT_BLACK);
        }
    }

    //update bullets after each 20ms
    if (b.check() == 1){

        //shoot (create) bullet if joystick is pressed
        if (buttonVal == LOW) {

            //ensuring a maximum of 5 bullets at a time
            bool freeSlot = false;
            uint8_t bulletID = 0;
            while(freeSlot == false && bulletID < objectData::max_ship_bullets) {
                if (shipBullets[bulletID].getExist() == 0) {
                    freeSlot = true;

                    Bullet NewShipBullet(myShip.getX() + (objectData::spaceship_width), myShip.getY() + (((objectData::spaceship_height)/2) - 1), ShipBulletSpeed);
                    shipBullets[bulletID] = NewShipBullet;
                }
                else {
                    bulletID++;
                }

            }
        }

        //read any incoming bullets from opponent
        uint16_t opponentBulletY = dataRead();
        if (opponentBulletY != 0){
            Serial.print("Received: ");
            Serial.println(opponentBulletY);

            //ensuring a maximum of 10 bullets at a time from opponent
            bool freeSlot = false;
            uint8_t bulletID = 0;
            while(freeSlot == false && bulletID < (objectData::max_opponent_bullets)) {
                if (opponentBullets[bulletID].getExist() == 0) {
                    freeSlot = true;

                    Bullet NewOpponentBullet(480, opponentBulletY, ShipBulletSpeed);
                    opponentBullets[bulletID] = NewOpponentBullet;
                }
                else {
                    bulletID++;
                }
            }
        }

        //moving all the player and opponent bullets
        for (uint8_t i =0;i<(objectData::max_opponent_bullets);i++) {

            if (i < objectData::max_ship_bullets){
                //check and move player bullets
                if (shipBullets[i].getExist() == 1) {
                
                    shipBullets[i].move(1);
                    drawPlayerBullet(shipBullets[i]);

                    if (shared.enemyActive && shipBullets[i].getX() >= 370 && shipBullets[i].getX() <= 410) { //enemy y position - bullet speed = 370
                        //check bullet collision with enemy
                        if (shipBullets[i].collisionEnemy(tieFighter) == 1) {
                            //delete bullet
                            tft.drawCircle(shipBullets[i].getX(), shipBullets[i].getY() +1, 2, TFT_BLACK); 
                            shipBullets[i].setExist(0);

                            //tieFighter killed!
                            explosion(tieFighter);
                            shared.enemyActive = false;                 

                            //player gets 20 pts for killing tieFighter
                            updateScore(20);       
                        }
                    }
                    //if bullet has reached end of screen, send bullet to the other arduino (screen)
                    else if (shipBullets[i].getX() > 480) {
                        dataWrite(shipBullets[i].getY());
                        Serial.print("Sending: ");
                        Serial.println(shipBullets[i].getY());
                        shipBullets[i].setExist(0);
                    }
                }
            }

            //check and move opponent bullets
            if (opponentBullets[i].getExist() == 1) {
            
                opponentBullets[i].move(-1);
                //draw opponent bullet
                tft.drawCircle(opponentBullets[i].getX(), opponentBullets[i].getY(), 2, TFT_RED);
                tft.drawCircle(opponentBullets[i].getX() + ShipBulletSpeed, opponentBullets[i].getY(), 2, TFT_BLACK);

                if (shared.enemyActive && opponentBullets[i].getX() >= 409 && opponentBullets[i].getX() <= 449){
                    //check bullet collision with enemy
                    if (opponentBullets[i].collisionEnemy(tieFighter) == 1) {
                        tft.drawCircle(opponentBullets[i].getX(), opponentBullets[i].getY(), 2, TFT_BLACK); //delete bullet
                        opponentBullets[i].setExist(0);
                        
                        //tieFighter killed!
                        explosion(tieFighter);
                        shared.enemyActive = false;

                        //send score to other arduino to update the opponent's score (60000--->20 pts)
                        dataWrite(60000);
                    }
                }
                //check bullet collision with ship
                else if (opponentBullets[i].collisionShip(myShip) == 1) {
                    //delete bullet
                    tft.drawCircle(opponentBullets[i].getX(), opponentBullets[i].getY(), 2, TFT_BLACK); 
                    opponentBullets[i].setExist(0);

                    //send score to other arduino to update the opponent's score (50000--->5 pts)
                    dataWrite(50000);
                }
                //if bullet is off screen
                else if (opponentBullets[i].getX() < 0) {
                    //delete bullet
                    opponentBullets[i].setExist(0);
                }
            }
        }

        //check and move tieFighter's bullets
        for (uint8_t i=0;i<(objectData::max_enemy_bullets);i++) {
            if (enemyBullets[i].getExist() == 1) {
                enemyBullets[i].move(-1);
                redrawEnemyBullet(enemyBullets[i]);
                
                //check enemy's bullet collision with ship
                if (enemyBullets[i].collisionShip(myShip) == 1) {
                    //delete bullet
                    tft.drawCircle(enemyBullets[i].getX(), enemyBullets[i].getY(), 2, TFT_BLACK);
                    enemyBullets[i].setExist(0);

                    //player loses 5 pts if hit by a tieFighter's bullet
                    updateScore(-5);
                }  
                //if bullet is off screen
                if (enemyBullets[i].getX() < 0) {
                    //delete bullet
                    enemyBullets[i].setExist(0);
                }
            }
        }
    }

    //update timer after each 1000ms
    if (T.check() == 1) {
        TimerDisplay();
    }
}

int main() {

    setup();

    while(true) {

        drawMenuPage();

        StartCommunication();

        //setup gameplay
        drawFrame(); 
        drawBackground();
        drawShip(myShip.getX(), myShip.getY(), 0);

        s.reset(); ea.reset(); e.reset(); b.reset(); T.reset();
        
        shared.GameOver = false;
    	while (!(shared.GameOver)) {
            initGame();
    	}
        
    	//EndCommunication();

        GameOverDisplay();
    }

	return 0;
}
