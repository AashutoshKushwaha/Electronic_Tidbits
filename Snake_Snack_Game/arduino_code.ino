#include <LedControl.h>

#define DIN 11
#define CS 10
#define CLK 13

#define JOY_X A0
#define JOY_Y A1

LedControl lc = LedControl(DIN, CLK, CS, 1);

int snakeX[64];
int snakeY[64];
int snakeLength = 3;

int foodX, foodY;

int dirX = 1;
int dirY = 0;

unsigned long lastMove = 0;
int speedDelay = 300;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  randomSeed(analogRead(2));

  snakeX[0] = 3; snakeY[0] = 4;
  snakeX[1] = 2; snakeY[1] = 4;
  snakeX[2] = 1; snakeY[2] = 4;

  spawnFood();
}

void loop() {
  readJoystick();

  if (millis() - lastMove > speedDelay) {
    moveSnake();
    lastMove = millis();
  }
}

void readJoystick() {
  int xVal = analogRead(JOY_X);
  int yVal = analogRead(JOY_Y);

  if (xVal < 300 && dirX != 1) { 
    dirX = -1; 
    dirY = 0; 
  }
  else if (xVal > 700 && dirX != -1) { 
    dirX = 1; 
    dirY = 0; 
  }
  // Swapped UP and DOWN here  (Change according to your joystick orientation)
  else if (yVal < 300 && dirY != 1) { 
    dirX = 0; 
    dirY = -1; 
  }
  else if (yVal > 700 && dirY != -1) { 
    dirX = 0; 
    dirY = 1; 
  }
}

void moveSnake() {
  int newX = snakeX[0] + dirX;
  int newY = snakeY[0] + dirY;

  // Wrap around horizontally
  if (newX < 0) newX = 7;
  if (newX > 7) newX = 0;

  // Wrap around vertically
  if (newY < 0) newY = 7;
  if (newY > 7) newY = 0;

  // Self collision (still active)
  for (int i = 0; i < snakeLength; i++) {
    if (snakeX[i] == newX && snakeY[i] == newY) {
      gameOver();
      return;
    }
  }

  // Move body
  for (int i = snakeLength; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = newX;
  snakeY[0] = newY;

  // Food eaten
  if (newX == foodX && newY == foodY) {
    snakeLength++;
    spawnFood();
  }

  drawGame();
}

void spawnFood() {
  foodX = random(0, 8);
  foodY = random(0, 8);
}

void drawGame() {
  lc.clearDisplay(0);

  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }

  // Draw food
  lc.setLed(0, foodY, foodX, true);
}

void gameOver() {
  lc.clearDisplay(0);
  delay(1000);
  snakeLength = 3;
  snakeX[0] = 3; snakeY[0] = 4;
}
