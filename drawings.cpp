/*----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim

Final Project - Space Shooter
----------------------------------------------------*/

#include "drawings.h"

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 940
#define TS_MAXY 920

// touch screen pins, obtained from the documentaion
#define YP A3
#define XM A2
#define YM 9
#define XP 8

// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000
#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

//loading icon variable
#define PI 3.1415926535897932384626433832795

// a multimeter reading says there are 300 ohms of resistance across the plate,
// so initialize with this to get more accurate readings
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//importing menu image and highscore icons
lcd_image_t spaceImage = {"space.lcd", 480, 320};
lcd_image_t highIcon = {"high.lcd", 180, 180};
lcd_image_t newHighIcon = {"nhigh.lcd", 150, 120};

//set timer variables
int8_t m = 3;
int8_t s1 = 0;
int8_t s2 = 0;

bool checkTouch(uint8_t displayNum) { 
    TSPoint touch = ts.getPoint();

    // restore pinMode to output after reading the touch
    // this is necessary to talk to tft display
    pinMode(YP, OUTPUT); 
    pinMode(XM, OUTPUT); 

    //checking if a particular touch is valid
    if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
        return false;
    }

    //mapping point from the touchscreen display to the tft display
    int16_t pt_x = map(touch.y, TS_MINX, TS_MAXX, displayData::display_width, 0);
    int16_t pt_y = map(touch.x, TS_MINY, TS_MAXY, displayData::display_height, 0);

    //for left orientation
    if (shared.rightOrientation == false) {
        if (displayNum == 0){ //start
            if (pt_x >=150.5 && pt_x <= (150.5+195) && pt_y >= 95.5 && pt_y <=(95.5+40)){
                return true;
            }
            else{
                return false;
            }
        }
        else if (displayNum == 1) { //main menu
            if (pt_x >=160 && pt_x <= 330 && pt_y >= 255 && pt_y <=295){
                return true;
            }
            else{
                return false;
            }
        }
        else if (displayNum == 2){ //highscore
        	if (pt_x >=150.5 && pt_x <= (150.5+195) && pt_y >= 173 && pt_y <= (173+40)){
                return true;
            }
            else{
                return false;
            }
        }
    }
    //for right orientation
    else {
        if (displayNum == 0) { //start
            if (pt_x >=150.5 && pt_x <= (150.5+195) && pt_y >= 184.5 && pt_y <=224.5) {
                return true;
            }
            else{
                return false;
            }
        }
        else if (displayNum == 1) { //main menu
            if (pt_x >=160 && pt_x <= 330 && pt_y >= 25 && pt_y <=65){
                return true;
            }
            else{
                return false;
            }
        }
        else if (displayNum == 2){ //highscore
            if (pt_x >=150.5 && pt_x <= (150.5+195) && pt_y >= 107 && pt_y <= 147){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

void drawMenuPage() {
    lcd_image_draw(&spaceImage, shared.tft, 0, 0, 0, 0, 480, 320);

    shared.tft->drawRect(150.5, 95.5, 195, 40, TFT_GREEN);

    shared.tft->drawRect(150.5, 173, 195, 40, TFT_GREEN);
}

void displayMsg(uint16_t x, uint16_t y, const char *msg) {
  shared.tft->fillRect(70, 225, 360, 40, TFT_WHITE);

  shared.tft->setFont(&FreeSans9pt7b);
  shared.tft->setCursor(x, y);
  shared.tft->setTextColor(TFT_BLACK);
  shared.tft->setTextSize(2);
  shared.tft->print(msg);
}

void loadingIcon() {
    for (int i = 8; i > 0; i--) {
      shared.tft->fillCircle(240 + 40 * (cos(-i * PI / 4)), 120 + 40 * (sin(-i * PI / 4)), 10,  loadingData::col[0]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 1)*PI / 4)), 120 + 40 * (sin(-(i + 1)*PI / 4)), 10,  loadingData::col[1]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 2)*PI / 4)), 120 + 40 * (sin(-(i + 2)*PI / 4)), 10,  loadingData::col[2]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 3)*PI / 4)), 120 + 40 * (sin(-(i + 3)*PI / 4)), 10,  loadingData::col[3]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 4)*PI / 4)), 120 + 40 * (sin(-(i + 4)*PI / 4)), 10,  loadingData::col[4]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 5)*PI / 4)), 120 + 40 * (sin(-(i + 5)*PI / 4)), 10,  loadingData::col[5]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 6)*PI / 4)), 120 + 40 * (sin(-(i + 6)*PI / 4)), 10,  loadingData::col[6]); delay(15);
      shared.tft->fillCircle(240 + 40 * (cos(-(i + 7)*PI / 4)), 120 + 40 * (sin(-(i + 7)*PI / 4)), 10,  loadingData::col[7]); delay(15);
    }
}

void HighscoreDisplay() {

	shared.tft->fillScreen(TFT_WHITE);

    //draw highscore icon
	lcd_image_draw(&highIcon, shared.tft, 0, 0, 30, 70, 180, 180);

    //write the highscore in a rounded rectangle
	shared.tft->fillRoundRect(250, 120, 190, 60, 30, shared.tft->color565(15, 232, 44));
    shared.tft->setTextSize(2);
    shared.tft->setTextColor(TFT_RED);
    shared.tft->setFont(&FreeSerif12pt7b);
    shared.tft->setCursor(312, 165);
    shared.tft->println(shared.highestScore);

    //draw main menu button
	shared.tft->fillRoundRect(160, 255, 170, 40, 10, shared.tft->color565(34, 19, 151));
    shared.tft->setTextSize(1);
    shared.tft->setTextColor(TFT_WHITE);
    shared.tft->setFont(&FreeSerif12pt7b);
    shared.tft->setCursor(176, 281);
    shared.tft->println("MAIN MENU");

    //wait on high score page until the main menu button is pressed
	while (!checkTouch(1)) {
    }

    drawMenuPage();
}

//simple finite state machine to set up communication between the two arduinos
enum currentState {
  mainMenu, playerReady, opponentReady
};


void StartCommunication() {

    bool matchStarted = false;
    bool loading = false;
    currentState current = mainMenu;
    while (matchStarted == false) {
        while (current == mainMenu){
            //opponent is already ready
            if (Serial3.available()) {
              uint8_t signal = Serial3.read();
              if (signal == 'S') {
                current = opponentReady;
              }
            }
            //player is first to be ready
            else if(checkTouch(0) == true) {
                loading = true;
                Serial3.write('S');
                current = playerReady;
                
                shared.tft->fillScreen(TFT_WHITE);
                displayMsg(70, 250, "Waiting for opponent...");
            }
            else if (checkTouch(2) == true){
            	HighscoreDisplay();
            }
           
        }


        if (current == opponentReady) {
            while (!checkTouch(0)) {
              delay(1);
            }
            Serial3.write('A');

            long long waitingTime = 5000; //wait for 5s
            unsigned long deadline = millis() + waitingTime;

            shared.tft->fillScreen(TFT_WHITE);
            displayMsg(120, 250, "Match starting...");

            while(millis()<deadline) {
              loadingIcon();
            }
            matchStarted = true;

        }

        while(loading) {
            loadingIcon();

            if (Serial3.available()) {
                uint8_t signal = Serial3.read();
                if (signal == 'A') {
                    long long waitingTime = 5000; 
                    unsigned long deadline = millis() + waitingTime;
                    
                    displayMsg(120, 250, "Match starting...");
                    while(millis()<deadline) { //wait for 5s
                      loadingIcon();
                    }
                    matchStarted = true;
                    loading = false;
                }
            }
        }
        //short delay, before the player who pressed start game sends a final start game signal and starts his match
        delay(100);
        if (current == opponentReady) {
            Serial3.write('S');
            delay(10);
        }
        else {
            //The first player to press start game starts his match immediately upon receiving the final start game signal
            while(Serial3.available() == 0) {
              delay(1);
            }
            Serial3.read();
        }
    }
}


void dataWrite(uint16_t data) {
    //writes two bytes to Serial3
    Serial3.write((char) (data >> 0));
    Serial3.write((char) (data >> 8));
}

void updateScore(int add) {

    //catering for the orientation 
    if (shared.rightOrientation == false) {
        shared.tft->setRotation(1);
    }
    else {
        shared.tft->setRotation(3);
    }

    //update player's score
	int playerScore = (shared.myShip->getScore()) + add;
	shared.myShip->setScore(playerScore);

    //display score on screen
	shared.tft->fillRect(82,displayData::gameplay_end_y,100, 30, shared.tft->color565(34, 19, 151));
	shared.tft->setCursor(82, 312);
    shared.tft->print(shared.myShip->getScore());
    shared.tft->setRotation(1);
}



uint16_t dataRead() {

    uint16_t data = 0;
    //reads two bytes from Serial3
    if (Serial3.available() >= 2) {
        data = data | ((uint16_t) Serial3.read()) << 0;
        data = data | ((uint16_t) Serial3.read()) << 8;

        //score value 5
        if (data==50000) { 
            updateScore(5);
            return 0;
        }
        //score value 20
        else if (data==60000) { 
            updateScore(20);
            return 0;
        }
        //y-coordinate of bullet
        else if (data < 320) {
        	data = 320 - data;   
            return data;
        }
        else{
            return 0;
        }
    }
    else{
        return data;
    }
    
}


void drawShip(uint16_t ShipX, uint16_t ShipY, int8_t direction) {
    shared.tft->setRotation(1);

    //draw black rectangle either at the top or bottom of the ship
    if (direction == -1){
        shared.tft->fillRect(ShipX, ShipY + (objectData::spaceship_height), (objectData::spaceship_width), shared.myShip->getSpeed(), TFT_BLACK);
    }
    else if (direction == 1){
        shared.tft->fillRect(ShipX, ShipY - shared.myShip->getSpeed(), objectData::spaceship_width, shared.myShip->getSpeed(), TFT_BLACK);
    }
    
    //draw ship
    shared.tft->fillRect(ShipX, ShipY, objectData::spaceship_width, objectData::spaceship_height, TFT_WHITE);
    shared.tft->fillRect(ShipX+10, ShipY+8, 10, 9, TFT_BLACK);
    shared.tft->fillRect(ShipX+10, ShipY+19, 10,9, TFT_BLACK);
    
}

void drawEnemy(int previousY) {
    shared.tft->fillRect(objectData::enemy_spawn_x, previousY, objectData::enemy_width, objectData::enemy_height, TFT_BLACK);
    delay(10);
    shared.tft->drawRGBBitmap(objectData::enemy_spawn_x, shared.tieFighter->getY(), Tf, objectData::enemy_width, objectData::enemy_height);
}

void redrawEnemyBullet(Bullet& enemyBullet) {
    shared.tft->drawCircle(enemyBullet.getX() + enemyBullet.getSpeed(), enemyBullet.getY(), 2, TFT_BLACK);
    shared.tft->drawCircle(enemyBullet.getX(), enemyBullet.getY(), 2, TFT_RED);
}


void drawPlayerBullet(Bullet& playerBullet) {
	shared.tft->drawCircle(playerBullet.getX(), playerBullet.getY() +1, 2, TFT_GREEN);
    shared.tft->drawCircle(playerBullet.getX() - playerBullet.getSpeed(), playerBullet.getY() +1, 2, TFT_BLACK);
}


void drawBackground() {
    shared.tft->drawPixel(35, 45, TFT_WHITE);
    shared.tft->drawPixel(60, 75, TFT_WHITE);
    shared.tft->drawPixel(90, 50, TFT_WHITE);
    shared.tft->drawPixel(100, 45, TFT_WHITE);
    shared.tft->drawPixel(200, 150, TFT_WHITE);
    shared.tft->drawPixel(250, 90, TFT_WHITE);
    shared.tft->drawPixel(165, 95, TFT_WHITE);
    shared.tft->drawPixel(180, 58, TFT_WHITE);
    shared.tft->drawPixel(470, 69, TFT_WHITE);
    shared.tft->drawPixel(450, 200, TFT_WHITE);
    shared.tft->drawPixel(220, 165, TFT_WHITE);
    shared.tft->drawPixel(290, 250, TFT_WHITE);
    shared.tft->drawPixel(210, 50, TFT_WHITE);
    shared.tft->drawPixel(20, 260, TFT_WHITE);
    shared.tft->drawPixel(25, 40, TFT_WHITE);
    shared.tft->drawPixel(150, 60, TFT_WHITE);
    shared.tft->drawPixel(100, 250, TFT_WHITE);
    shared.tft->drawPixel(190, 280, TFT_WHITE);
    shared.tft->drawPixel(120, 135, TFT_WHITE);
    shared.tft->drawPixel(220, 220, TFT_WHITE);
    shared.tft->drawPixel(70, 135, TFT_WHITE);
    shared.tft->drawPixel(60, 245, TFT_WHITE);
}


void TimerDisplay() {

    //catering for the orientation
    if (shared.rightOrientation == false) {
        shared.tft->setRotation(1);
    }
    else {
        shared.tft->setRotation(3);
    }

    shared.tft->setTextColor(TFT_WHITE, shared.tft->color565(34, 19, 151));

    //second digit of second
    s2--;
    if (s2 == -1){
        s2=9;
        s1--;

        //first digit of second
        if (s1==-1){
            m--;
            //digit representing minute
            if (m == -1){
                //end game
                shared.GameOver = true;
                return;
            }

            s1=5;
            s2=9;

            //redraw m, s1 and s2
            shared.tft->fillRect(370, 293, 15, 24, shared.tft->color565(34, 19, 151));
            shared.tft->fillRect(390, 293, 12, 24, shared.tft->color565(34, 19, 151)); //s1
            shared.tft->fillRect(403, 293, 13, 24, shared.tft->color565(34, 19, 151)); //s2
            
            shared.tft->setCursor(372, 312);
            shared.tft->println(m);
            shared.tft->setCursor(391, 312);
            shared.tft->println(s1);
            shared.tft->setCursor(404, 312);
            shared.tft->println(s2);
        }

        //redraw s1 and s2
        shared.tft->fillRect(390, 293, 12, 24, shared.tft->color565(34, 19, 151)); //s1
        shared.tft->fillRect(403, 293, 13, 24, shared.tft->color565(34, 19, 151)); //s2
        shared.tft->setCursor(391, 312);
        shared.tft->println(s1);
        shared.tft->setCursor(404, 312);
        shared.tft->println(s2);
    }
    else{
        //redraw s2 only
        shared.tft->setCursor(404, 312);
        shared.tft->fillRect(403, 293, 13, 24, shared.tft->color565(34, 19, 151)); //s2
        shared.tft->println(s2);
    }
    shared.tft->setRotation(1);
}

void drawFrame() {
    shared.tft->fillScreen(TFT_BLACK);

    //catering for the orientation
    if (shared.rightOrientation == false) {
        shared.tft->setRotation(1);
    }
    else {
        shared.tft->setRotation(3);
    }


    shared.tft->fillRect(0,0,480, 30, shared.tft->color565(34, 19, 151));
    shared.tft->fillRect(0,displayData::gameplay_end_y,480, 30, shared.tft->color565(34, 19, 151));

    shared.tft->setTextSize(1);
    shared.tft->setFont(&FreeSerifBold12pt7b);
    shared.tft->setTextColor(TFT_WHITE);

    shared.tft->setCursor(180,25);

    //printing header on screen accordig to orientation
    if (shared.rightOrientation == false) {
        shared.tft->print("PLAYER 1");
    }
    else {
        shared.tft->print("PLAYER 2");
    }

    shared.tft->setFont(&FreeSerif12pt7b);

    //printing score on screen
    shared.tft->setCursor(10, 312);
    shared.tft->print("Score: ");
    shared.tft->setCursor(82, 312);
    shared.tft->print(0);

    //printing timer on screen
    shared.tft->setCursor(300, 312);
    shared.tft->print("Timer:");

    shared.tft->setCursor(372, 312);
    shared.tft->print(m);

    shared.tft->setCursor(385, 312);
    shared.tft->print(":");

    shared.tft->setCursor(391, 312);
    shared.tft->print(s1);

    shared.tft->setCursor(404, 312);
    shared.tft->print(s2);
}


void reset() {

    //reset ship and tieFighter
    shared.myShip->reset();
    shared.tieFighter->reset();

    //reset all bullets
    for (uint8_t i = 0; i < objectData::max_ship_bullets; i++) {
        shared.shipBullets[i].setExist(0);
        shared.opponentBullets[i].setExist(0);

        if (i < objectData::max_enemy_bullets){
            shared.enemyBullets[i].setExist(0);
        }
    }

    //reset enemy initial state
    shared.enemyDirection = 1;
    shared.enemyCycles = 0;
    shared.tickCounter = 0; 
    shared.enemyActive = false;

    //reset timer
    m = 3;
    s1 = 0;
    s2 = 0;
}


void scoreWrite(int16_t data) {

    //converts negative data to a large positive value  before sending as 
    //performing bitwise operations on negative values can yield undefined results
    if (data < 0){
        data *= -1000;
    }

    //writes two bytes to Serial3
    Serial3.write((char) (data >> 0));
    Serial3.write((char) (data >> 8));
}



int16_t scoreRead() {
    int16_t data = 0;
    // while (Serial3.available() < 2) {
    //     delay(1);
    // }

    data = data | ((uint16_t) Serial3.read()) << 0;
    data = data | ((uint16_t) Serial3.read()) << 8;

    //converts this large vaue to its corresponding -ve value
    if (data >=5000){
        data = data/(-1000);
    }
    return data;
}

bool  wait_on_serial3( uint8_t  nbytes , long  timeout ) {
    unsigned  long  deadline = millis () + timeout;// wraparound  not a problem
    while (Serial3.available ()<nbytes  && (timeout <0 ||  millis ()<deadline)){
        delay (1);
    }
    return  Serial3.available () >=nbytes;
}

void GameOverDisplay() {

    //clear the buffer
    while (Serial3.available()) {
        Serial3.read();
    }
    delay(300);

    enum HandshakeState {SendingScore, WaitingAck, WaitingScore, SendingAck, DataExchange};

    int16_t myScore = shared.myShip->getScore();

    int16_t opponentrightScore, opponentleftScore;

    //sending scores to each other for comparison by setting up a handshaking protocol
    if (shared.rightOrientation == false) {
        shared.tft->setRotation(1);

        HandshakeState current = SendingScore;

        while (current!=DataExchange){
            if (current == SendingScore){
                scoreWrite(myScore);

                current = WaitingAck;
            }
            else if (current == WaitingAck){
                if (wait_on_serial3(1, 1000)){
                    char Ack = Serial3.read();

                    if (Ack != 'A'){
                        current = SendingScore;
                    }
                    else{
                        current = WaitingScore;
                    }
                }
                else{
                    //timeout
                    current = SendingScore;
                }
            }
            else if (current == WaitingScore){
                if (wait_on_serial3(2, 1000)){
                    opponentrightScore = scoreRead();
                    current = SendingAck;
                }
                else{
                    //timeout
                    current = SendingScore;
                }
            }
            else if (current == SendingAck){
                Serial3.write('A');
                current = DataExchange;
            }
        }

        Serial.print("Opponent right score: ");
        Serial.println(opponentrightScore);

    }
    else { //server
        shared.tft->setRotation(3);

        HandshakeState current = WaitingScore;

        while (current!=DataExchange){
            if (current == WaitingScore){
                if (wait_on_serial3(2, 1000)){
                    opponentleftScore = scoreRead();
                    current = SendingAck;
                }
                else{
                    current == WaitingScore;
                }
            }
            else if (current == SendingAck){
                Serial3.write('A');
                current = SendingScore;
            }
            else if (current == SendingScore){
                scoreWrite(myScore);
                current = WaitingAck;
            }
            else if (current == WaitingAck){
                if (wait_on_serial3(1,1000)){
                    char Ack = Serial3.read();

                    if (Ack != 'A'){
                        current = WaitingScore;
                    }
                    else{
                        current = DataExchange;
                    } 
                }
                else{
                    //timeout
                    current = WaitingScore;
                }
            }
        }

        Serial.print("Opponent left score: ");
        Serial.println(opponentleftScore);
    }

    shared.tft->setCursor(150, 160);
    shared.tft->println("GAME OVER");

    Serial.print("My score: ");
    Serial.println(myScore);

    delay(20);

    //render display
    shared.tft->fillScreen(TFT_WHITE);
    shared.tft->setTextSize(2);
    shared.tft->setFont(&FreeSerif12pt7b);
    shared.tft->setTextColor(TFT_RED);
    shared.tft->setCursor(100, 75);
    shared.tft->println("GAME OVER!");

    shared.tft->setTextSize(2);
    shared.tft->setFont(&FreeSans9pt7b);
    shared.tft->setCursor(175, 160);
    shared.tft->setTextColor(TFT_BLACK);

    //deciding the winner
    if (shared.rightOrientation == false) {
        bool win = false;
        if (myScore > opponentrightScore){
            shared.tft->println("You Won");
            win = true;
        }
        else if (myScore < opponentrightScore){
            shared.tft->println("You Lost :(");
        }
        else{
            shared.tft->println("Tie!");
            win = true; //in case tie and both got highest score
        }

        //if the highest score has been exceeded, set new highest score
        if ((myScore > shared.highestScore) || (opponentrightScore > shared.highestScore)){

            if (win){
                shared.highestScore = shared.myShip->getScore();
                lcd_image_draw(&newHighIcon, shared.tft, 0, 0, 20, 120, 150, 120);
            }
            else{
                shared.highestScore = opponentrightScore;
            }

            //writing a two-byte value to eeprom;
            //since it is a byte addressable memory, split the two-byte value
            //into two separate byte (lower byte and upper byte) and write each byte at
            //two different memory addresses
            byte xlo = shared.highestScore;
            byte xhi = (shared.highestScore >> 8);
            EEPROM.update(1, xlo);
            EEPROM.update(0, xhi);
        }
    }
    else{
        bool win = false;
        if (myScore > opponentleftScore){
            shared.tft->println("You Won");
            win = true;
        }
        else if (myScore < opponentleftScore){
            shared.tft->println("You Lost :(");
        }
        else{
            shared.tft->println("Tie!");
            win = true; //in case tie and both got highest score
        }

        //if the highest score has been exceeded, set new highest score
        if ((myScore > shared.highestScore) || (opponentleftScore > shared.highestScore)){

            if (win){
                shared.highestScore = shared.myShip->getScore();
                lcd_image_draw(&newHighIcon, shared.tft, 0, 0, 20, 120, 150, 120);
            }
            else{
                shared.highestScore = opponentleftScore;
            }

            //writing a two-byte value to eeprom;
            //since it is a byte addressable memory, split the two-byte value
            //into two separate byte (lower byte and upper byte) and write each byte at
            //two different memory addresses
            byte xlo = shared.highestScore;
            byte xhi = (shared.highestScore >> 8);
            EEPROM.update(1, xlo);
            EEPROM.update(0, xhi);
        }
    }

    shared.tft->setTextSize(1);
    shared.tft->setCursor(200, 220);
    shared.tft->println("Score: ");

    shared.tft->setCursor(267, 220);
    shared.tft->print(myScore);

    Serial.print("Highest Score: ");
    Serial.println(shared.highestScore);

    //draw main menu button
    shared.tft->fillRoundRect(160, 255, 170, 40, 10, shared.tft->color565(34, 19, 151));
    shared.tft->setTextSize(1);
    shared.tft->setTextColor(TFT_WHITE);
    shared.tft->setFont(&FreeSerif12pt7b);
    shared.tft->setCursor(176, 281);

    shared.tft->println("MAIN MENU");

    //reset all stuffs
    reset();

    //wait until main menu is pressed
    while (!checkTouch(1)) {
    }
}


void EndCommunication(){
    if (shared.rightOrientation == false) {
            Serial3.write('E');
            delay(10);
        }
    else {
        while(Serial3.available() == 0) {
              delay(1);
        }
        Serial3.read();
    }  
}


void explosion(Enemy& enemy) {
    shared.tft->fillRect(enemy.getX(), enemy.getY(), 39, 34, TFT_BLACK);
    shared.tft->drawRGBBitmap(enemy.getX(), enemy.getY(), exsmall, 24, 21);
    delay(60);
    shared.tft->drawRGBBitmap(enemy.getX(), enemy.getY(), ex, 35, 30);
    delay(50);
    shared.tft->fillRect(enemy.getX(), enemy.getY(), 35, 30, TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()-5, enemy.getY()-5, TFT_WHITE);
    shared.tft->drawPixel(enemy.getX()+38, enemy.getY()+45, TFT_WHITE);
    shared.tft->drawPixel(enemy.getX()-10, enemy.getY(), TFT_WHITE);
    shared.tft->drawPixel(enemy.getX()+40, enemy.getY()+5, TFT_WHITE);
    shared.tft->drawPixel(enemy.getX()+10, enemy.getY()-20, TFT_WHITE);
    shared.tft->drawPixel(enemy.getX()+35, enemy.getY()+35, TFT_WHITE);
    delay(50);
    shared.tft->drawPixel(enemy.getX()-5, enemy.getY()-5, TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()+38, enemy.getY()+45, TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()-10, enemy.getY(), TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()+40, enemy.getY()+5, TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()+10, enemy.getY()-20, TFT_BLACK);
    shared.tft->drawPixel(enemy.getX()+35, enemy.getY()+35, TFT_BLACK);
}

