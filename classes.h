/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim 

Final Project - Space Shooter
----------------------------------------------------*/

#ifndef classes_H
#define classes_H

#include "Arduino.h"

#define GAMEPLAY_START_Y 30
#define GAMEPLAY_END_Y 290
#define GAMEPLAY_WIDTH 480
#define GAMEPLAY_HEIGHT 260

//spaceship info
#define SPACESHIP_WIDTH 20
#define SPACESHIP_HEIGHT 36
#define SHIP_START_X 10
#define SHIP_START_Y 142

//enemy info
#define ENEMY_SIZE 34
#define ENEMY_SPAWN_X 390

/*
Represents the enemy, with all the information about the enemy
*/
class Enemy {

private:
    
    //location of enemy
    int x, y; 

    //the speed of the movement
    uint8_t speed; 

public:

    //constructor: creates the enemy at position (x, y) and with speed speed
	Enemy(int x,  int y, uint8_t speed);

    //moves the enemy, if direction = -1, then enemy moves upwards,
    //if direction = 1, then enemy moves downwards 
    //also, returns the previous y position of the enemy
	int move(int8_t direction);

    // returns the x position of the enemy 
	int getX();

    //returns the y position of the enemy
    int getY();

    //returns the speed of the enemy
    uint8_t getSpeed();

    //updates the speed of the enemy
    void setSpeed(uint8_t newSpeed);

    //reset the enemy to its starting position
    void reset();
};

/*
Represents the spaceship of the player, with all the information about the spaceship
*/
class Ship {

private:

    //location of spaceship
    int x, y; 

    //the speed of the movement
    uint8_t speed; 

    //the score of the ship
    int score; 

public:

    //constructor: creates the spaceship at position (x, y)
    Ship(int x,  int y);

    //moves the spaceship, if direction = -1, then spaceship moves upwards,
    //if direction = 1, then spaceship moves downwards 
    void move(int8_t direction);

    //set the new x position of the spaceship
    void setX(int newX);

    //set the new y position of the spaceship
    void setY(int newY);

    //returns the x position of the spaceship
    int getX();

    //returns the y position of the spaceship
    int getY();

    //stores the new score of the spaceship
    void setScore(int newScore);

    //returns the score of the spaceship
    int getScore();

    //returns the speed of the spaceship
    uint8_t getSpeed();

    //reset all the sapceship attributes to their initial values
    void reset();
};


/*
Represents a bullet, with all the information about that bullet
*/
class Bullet {
private:

    //location of enemy
    int x, y;

    //the speed of the movement
    uint8_t speed;

    //status of the bullet, if 1: bullet exists, if 0; bullet does not exist
    char exist;

public:

    //parametrized constructor: creates a bullet at position (x, y) and with speed speed
    Bullet(int x, int y, uint8_t speed);

    //default constructor: creates an instance of bullet with exist = 0
    Bullet();

    //moves the bullet, if direction = -1, then bullet moves towards left (enemy bullet),
    //if direction = 1, then bullet moves towards right (ship bullet) 
    void move(int8_t direction);

    //returns the speed of the bullet
    uint8_t getSpeed();

    //returns the x position of the bullet
    int getX();

    //returns the x position of the bullet
    int getY();

    //returns 1 if the bullet exists, else returns 0
    char getExist();

    //updates the value of exist
    void setExist(char exist);

    //checks if the bullet has collided with the ship, returns 1 if collided, else returns 0
    uint8_t collisionShip(Ship& ship);

    //checks if the bullet has collided with the enemy, returns 1 if collided, else returns 0
    uint8_t collisionEnemy(Enemy& enemy);
};

#endif
