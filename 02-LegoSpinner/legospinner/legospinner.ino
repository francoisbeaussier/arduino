// Motor driven by the L293D chip
#define ENGINE_ENABLE 11
#define ENGINE_DIR_A 9
#define ENGINE_DIR_B 10

#define BUTTON 7

int i;
 
void setup() {
  // Engine
  pinMode(ENGINE_ENABLE, OUTPUT);
  pinMode(ENGINE_DIR_A, OUTPUT);
  pinMode(ENGINE_DIR_B, OUTPUT);

  // button
  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
}

int button_read;
int button_old_read;

bool engine_enabled = false;

void change_engine_state() {
  Serial.println("Changing engine state");

  if (engine_enabled) {
    // turn off
    analogWrite(ENGINE_ENABLE, 0);
    Serial.println("  enable = 0");
  } else {
    // turn on
    analogWrite(ENGINE_ENABLE, 255);
    Serial.println("  enable = 255");
    digitalWrite(ENGINE_DIR_A, HIGH); //one way
    digitalWrite(ENGINE_DIR_B, LOW);

  }

  engine_enabled = !engine_enabled;
}

void loop() {
  // Serial.println("PWM full then slow");

  button_read = digitalRead(BUTTON);

  if ((button_read == HIGH) && (button_old_read == LOW)) {
    Serial.println("Button pressed");
    // engine_enabled = !engine_enabled;
    change_engine_state();
    delay(10);
  }

  button_old_read = button_read;

  // if (engine_enabled)
  // button_stop_read = digitalRead(BUTTON_STOP);
  // if (button_stop_read == HIGH) {
  //   Serial.println("Button stop pressed");
  //   engine_on = false;
  //   delay(10);
  // }

  // delay(10);

  //---PWM example, full speed then slow
  // analogWrite(ENGINE_ENABLE, 128);
  // digitalWrite(ENGINE_DIR_A, HIGH); //one way
  // digitalWrite(ENGINE_DIR_B, LOW);
  // delay(2000);

  // analogWrite(ENABLE, 128); //half speed
  // delay(2000);
  
  // digitalWrite(ENABLE,LOW); //all done
  // delay(500);
}
   
