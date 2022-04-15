/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim 

Final Project - Space Shooter
----------------------------------------------------*/

#ifndef __CONSTS_H
#define __CONSTS_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "classes.h"

struct shared_vars{

	// the tft display
  	MCUFRIEND_kbv* tft;

  	//all the game objects
	Ship *myShip;
	Enemy *tieFighter;
	Bullet *shipBullets;
	Bullet *enemyBullets;
	Bullet *opponentBullets;

	//to check if game is over 
	bool GameOver;

	//to check orientation 
	bool rightOrientation;

	int highestScore;

	int opponentScore;
	int myScore;

	//variables to control enemy movement and bullets of enemy
	unsigned char enemyDirection;
    int enemyCycles; //number of times the enemy has moved up&down
    int tickCounter; 
    bool enemyActive;

};

namespace objectData {

	//bullets info
	const uint8_t max_ship_bullets = 5;
	const uint8_t max_opponent_bullets = 10;
	const uint8_t max_enemy_bullets = 3;

	//spaceship info
	const uint8_t spaceship_width = 20;
	const uint8_t spaceship_height = 36;
	const uint8_t ship_start_x = 10;
	const uint8_t ship_start_y = 142;

	//enemy info
	const uint8_t enemy_width = 39;
	const uint8_t enemy_height = 34;
	const uint16_t enemy_spawn_x = 390;
	const uint8_t enemy_size = 34;


}

namespace displayData {

	const uint16_t display_width = 480;
	const uint16_t display_height = 320;

	//gameplay display info
	const uint16_t gameplay_start_y = 30;
	const uint16_t gameplay_end_y = 290;
	const uint16_t gameplay_width = 480;
	const uint16_t gameplay_height = 260;
}


extern shared_vars shared;

namespace loadingData{

	const int col[8] = {
		shared.tft->color565(155, 0, 50), 
		shared.tft->color565(170, 30, 80), 
		shared.tft->color565(195, 60, 110), 
		shared.tft->color565(215, 90, 140), 
		shared.tft->color565(230, 120, 170), 
		shared.tft->color565(250, 150, 200),
		shared.tft->color565(255, 180, 220),
		shared.tft->color565(255, 210, 240)
		};
}


#endif
