/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim 

Final Project - Space Shooter
----------------------------------------------------*/

#include "classes.h"

Ship::Ship(int x,  int y){
    this->x = x;
    this->y = y;
    score = 0;
    speed = 10;
}

void Ship::move(int8_t direction) {
    //up: direction = -1    down: direction = 1
    y += speed*direction;    
    y = constrain(y, GAMEPLAY_START_Y+1, GAMEPLAY_END_Y - SPACESHIP_HEIGHT - 1);
}

void Ship::setX(int newX) {
    x = newX;
}

void Ship::setY(int newY){
    y = newY;
}

int Ship::getX() {
    return x;
}

int Ship::getY() {
    return y;
}

void Ship::setScore(int newScore) {
    score = newScore;
}

int Ship::getScore() {
    return score;
}

uint8_t Ship::getSpeed() {
    return speed;
}

void Ship::reset() {
    x = SHIP_START_X;
    y = SHIP_START_Y;
    score = 0;
    speed = 10;
}


Enemy::Enemy(int x,  int y, uint8_t speed) {
    this->x = x;
    this->y = y;
    this->speed = speed;
}

int Enemy::move(int8_t direction) {
    //up: direction = -1    down: direction = 1
    int prev_y = y;
    y += speed*direction;
    y = constrain(y, GAMEPLAY_START_Y, GAMEPLAY_END_Y - ENEMY_SIZE);

    return prev_y;
}

int Enemy::getX() {
    return x;
}

int Enemy::getY() {
    return y;
}

uint8_t Enemy::getSpeed() {
    return speed;
}  

void Enemy::setSpeed(uint8_t newSpeed) {
    speed = newSpeed;
}

void Enemy::reset() {
    x = ENEMY_SPAWN_X;
    y = GAMEPLAY_START_Y;
}


Bullet::Bullet(int x, int y, uint8_t speed) {
    this->x = x;
    this->y = y;
    this->speed = speed;
    exist = 1;
}

Bullet::Bullet() {
    exist = 0;
}

void Bullet::move(int8_t direction) {
    //direction = -1: towards left (enemy bullet) or 1: towards right (ship bullet)
    x += (speed*direction);
}

uint8_t Bullet::getSpeed() {
    return speed;
}

int Bullet::getX() {
    return x;
}

int Bullet::getY() {
    return y;
}

char Bullet::getExist() {
    return exist;
}

void Bullet::setExist(char exist) {
    this->exist = exist;
}

uint8_t Bullet::collisionShip(Ship& ship) {
    //checks if the (x, y) position of the bullet coincides with any coordinates 
    //currently occupied by the ship
    if ((x >= SHIP_START_X) && (x <= SHIP_START_X + SPACESHIP_WIDTH)) {
        if ((y >= ship.getY()) && (y <= (ship.getY() + SPACESHIP_HEIGHT))) {
            return 1;
        }
    }
    return 0;
}

uint8_t Bullet::collisionEnemy(Enemy& enemy) {
    //checks if the (x, y) position of the bullet coincides with any coordinates 
    //currently occupied by the enemy
    if ((y >= enemy.getY()) && (y <= (enemy.getY() + ENEMY_SIZE))) {
        return 1;
    }
    return 0;
}


