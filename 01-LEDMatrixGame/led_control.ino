#include "LedControl.h"

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

// DataIn, LOAD(CS), CLOCK, number of 8x8 matrices
LedControl lc=LedControl(6, 10, 11, 1);

void setup() {

  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);

  /*
   The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
   */
  lc.shutdown(0, false);

  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  
  /* and clear the display */
  lc.clearDisplay(0);
}

void show_player(int px, int py, bool visible) {
  lc.setLed(0, px, py, visible);
}

void log_serial(int x_read, int y_read) {
  Serial.print("X-axis: ");
  Serial.print(x_read);
  Serial.print("\t");
  Serial.print("Y-axis: ");
  Serial.println(y_read);
  Serial.print("\n");
}

void log_target(int x, int y) {
  Serial.print("target: (");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(")\n");
}

// Player coordinates
int px = 3;
int py = 3;

// Player direction
int dx = 1;
int dy = 1;

// Target coordinates
int tx = 6;
int ty = 6;

void win_animation(int x, int y) {
  for (int i = 1; i < 8; i++) {
    for (int dx = -i; dx <= i; dx += i) {
      for (int dy = -i; dy <= i; dy += i) {
        if (dx != 0 || dy != 0) {
          int px = x + dx;
          int py = y + dy;
          lc.setLed(0, px, py, true);
        }
      }
    }
    delay(150);
  }
  
  delay(500);
  lc.clearDisplay(0);
}

void move_player(int x_read, int y_read) {

  // Handle joystick sensor values, range is 0-1024

  if (x_read > 600) {
    dx = 1;
  }
  else if (x_read < 400) {
    dx = -1;
  }
  else {
    dx = 0;
  }

  if (y_read > 600) {
    dy = 1;
  }
  else if (y_read < 400) {
    dy = -1;
  }
  else {
    dy = 0;
  }

  show_player(px, py, false);

  px = px + dx;
  if (px < 0) px = 0;
  if (px > 7) px = 7;

  py = py - dy;
  if (py < 0) py = 0;
  if (py > 7) py = 7;

  show_player(px, py, true);
}

void loop() {

  static unsigned long player_timestamp = millis();
  static unsigned long target_timestamp = millis();

  unsigned long now = millis();

  unsigned long player_delta = now - player_timestamp;
  if (player_delta > 200ul) {
    
    log_target(tx, ty);

    int x_read = analogRead(X_pin);
    int y_read = analogRead(Y_pin);

    log_serial(x_read, y_read);

    move_player(x_read, y_read);

    if (px == tx && py == ty) {
      // win

      win_animation(px, py);

      tx = abs((tx * 20219 + ty * 32303) % 8);
      ty = abs((tx * 44543 + ty * 54881) % 8);
    }

    player_timestamp = now;
  }

  unsigned long target_delta = now - target_timestamp;

  // Serial.println(target_delta);

  if (target_delta < 300ul) {
    lc.setLed(0, tx, ty, false);
  }
  else if (target_delta < 600ul) {
    lc.setLed(0, tx, ty, true);
  }
  else {
    target_timestamp = now;
  }
}
