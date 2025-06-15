#include <Arduino.h>
#include <Joystick.h>


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  10, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

bool sequential_mode = false;

void setup() {
  pinMode(4, OUTPUT);
  pinMode(A1, OUTPUT);  
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(LED_BUILTIN_TX, OUTPUT);
  // pinMode(LED_BUILTIN_RX, OUTPUT);
  TXLED0;
  RXLED0; 

  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  digitalWrite(4,0);
  digitalWrite(A1,0);

  digitalWrite(LED_BUILTIN, !sequential_mode);
  // digitalWrite(LED_BUILTIN_RX, !sequential_mode);
  // digitalWrite(LED_BUILTIN_TX, !sequential_mode);

  Joystick.begin(false);

#if 0
  Serial.begin(115200);
#endif

}

void reset_all(int size)
{
  for (int i = 0; i < size; i++)
  {
    Joystick.setButton(i,0);
  }
}

void loop() {

  static bool last_g3 = 0;
  static bool last_g8 = 0;

  bool g1 = !(bool)digitalRead(15);
  bool g2 = !(bool)digitalRead(5);
  bool g3 = !(bool)digitalRead(14);
  bool g4 = !(bool)digitalRead(6);
  bool g5 = !(bool)digitalRead(16);
  bool g6 = !(bool)digitalRead(8);
  bool g7 = !(bool)digitalRead(10);
  bool g8 = !(bool)digitalRead(7);

  if (g3 != last_g3 && g8 != last_g8)
  {

    if (g3 && g8)
    {
      sequential_mode = !sequential_mode;
      digitalWrite(LED_BUILTIN, !sequential_mode);
      // digitalWrite(LED_BUILTIN_RX, !sequential_mode);
      // digitalWrite(LED_BUILTIN_TX, !sequential_mode);

      reset_all(9);
    }

    last_g3 = g3;
    last_g8 = g8;
  }
  else if (sequential_mode)
  {
    Joystick.setButton(8, g3);
    Joystick.setButton(9, g4);
    Joystick.setButton(2, 0);
    Joystick.setButton(3, 0);
  }
  else
  {
    Joystick.setButton(0,g1);
    Joystick.setButton(1,g2);
    Joystick.setButton(2,g3);
    Joystick.setButton(3,g4);
    Joystick.setButton(4,g5);
    Joystick.setButton(5,g6);
    Joystick.setButton(6,g7);
    Joystick.setButton(7,g8);
    Joystick.setButton(8, 0);
    Joystick.setButton(9, 0);
  }

#if 0
  Serial.print(g1);
  Serial.print(" "); 
  Serial.print(g2);
  Serial.print(" ");
  Serial.print(g3);
  Serial.print(" ");
  Serial.print(g4);
  Serial.print(" ");
  Serial.print(g5);
  Serial.print(" ");
  Serial.print(g6);
  Serial.print(" ");
  Serial.print(g7);
  Serial.print(" ");
  Serial.print(gR);
  Serial.println();
#endif

  Joystick.sendState();
  delay(10);
}
