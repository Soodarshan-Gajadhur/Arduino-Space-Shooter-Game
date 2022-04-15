----------------------------------------------------
Names: Soodarshan Gajadhur & Ren Wen Lim 

Group Project: Multi-Player Space Shooter on two Arduinos connected together
----------------------------------------------------------------------

# Description:
Using 2 Arduinos with TFT-displays connected head-to-head, players will each control a spaceship, located at opposite ends of the displays, and have the goal of hitting the other player and enemy spaceship(s) in the centre. Players will be able to move up and down and fire their cannon.

# Included Files:
### In folder SpaceShooter:
    SpaceShooter.cpp
    classes.cpp
    classes.h
    consts.h
    drawings.cpp
    drawings.h
    lcd_image.cpp
    lcd_image.h
    Tf.c
    ex.c
    exsmall.c
    README
    Makefile

### In folder imageToSD:
Store all the .lcd files (high.lcd, nhigh.lcd, space.lcd) found in this folder in the SD card for both arduino.

Please note that the space.lcd image does not display correctly on the display since there was a bug in the provided tool
bmp2lcd.

# Additional libraries (in folder libraries):
	*Metro library
	*EEPROM library

# Accessories:
    2xArduino Mega Board (AMG)
    2xTFT Display
    2xJoystick
    Wires
    2xSD card

# Wiring instructions:
    Arduino Pin A8  <---> Joystick VRx Pin
    Arduino Pin A9 <--> Joystick VRy Pin
    Arduino Pin 53 <--> Joystick sw Pin
    Arduino 5V <--> Joystick 5V Pin
    Arduino GND <---> Joystick GND Pin
    Arduino TX3 Pin <--> Other Arduino RX3 Pin
    Arduino RX3 Pin <--> Other Arduino TX3 Pin
    For left-side arduino: 
    Arduino Pin 45  <---> Arduino GND
    For right-side arduino: 
    Arduino Pin 45  <---> Arduino 5V

# Running instructions:
With the arduinos wired according to the instructions above and with the both of them oriented such that their usb cables are facing away from each other (see video), connect both arduinos to the computer. From the directory containing the included files (SpaceShooter directory), use the command "make upload-0" & "make upload-1" to upload the code to both arduinos

# Notes:
### Communication() function:
Our handshake protocol does not designate an arduino as server/client, either arduino can send a start-game request to the other arduino and then wait for a reply. Once the second arduino's user presses the start button, an acknowledgement is first sent to have the first arduino exit its initial while loop and to inform the user that the match is starting. After a short delay, the second arduino sends another signal before starting the match while the first arduino also starts the match the moment it receives said signal. As such, both player should not press start game at the same time.

# Screenshots:
![gameplay](https://user-images.githubusercontent.com/84073248/126904682-53523a1b-8a12-4c9e-b20e-d47feb1fc1f8.png)
![game_over](https://user-images.githubusercontent.com/84073248/126904685-8b986a0f-a2f3-4ea2-a671-b52f3faabbc5.png)
![setup](https://user-images.githubusercontent.com/84073248/126904688-cb1267c6-d096-4844-9df6-ffbd5df00843.png)

	
	







