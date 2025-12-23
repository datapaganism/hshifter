#include <Arduino.h>
#include <Joystick.h>

#define GEAR_1 15
#define GEAR_2 5
#define GEAR_3 14
#define GEAR_4 6
#define GEAR_5 16
#define GEAR_6 8
#define GEAR_7 10
#define GEAR_8 7

#define GRND_1 4
#define GRND_2 A1

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   10, 0,                // Button Count, Hat Switch Count
                   false, false, false,  // X and Y, but no Z Axis
                   false, false, false,  // No Rx, Ry, or Rz
                   false, false,         // No rudder or throttle
                   false, false, false); // No accelerator, brake, or steering

struct gear
{
  uint8_t joy_number;
  int8_t joy_sequential_number;
  uint8_t pin;
  bool current;
  bool last;
  unsigned long last_debounce;
} typedef gear_t;

gear_t gears[] = {
    {0, -1, GEAR_1, false, false},
    {1, -1, GEAR_2, false, false},
    {2,  8, GEAR_3, false, false},
    {3,  9, GEAR_4, false, false},
    {4, -1, GEAR_5, false, false},
    {5, -1, GEAR_6, false, false},
    {6, -1, GEAR_7, false, false},
    {7, -1, GEAR_8, false, false},
};
const int gears_count = sizeof(gears) / sizeof(gears[0]);

bool sequential_mode = false;
unsigned long last_refresh = 0;
unsigned int debounce = 50;


void setup()
{
  pinMode(GRND_1, OUTPUT);
  pinMode(GRND_2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  TXLED0;
  RXLED0;

  for (int i = 0; i < gears_count; i++)
  {
    pinMode(gears[i].pin, INPUT_PULLUP);
  }

  digitalWrite(GRND_1, LOW);
  digitalWrite(GRND_2, LOW);

  digitalWrite(LED_BUILTIN, !sequential_mode);

  Joystick.begin(false);

#if DEBUG == 1
  Serial.begin(115200);
#endif
}

void reset_all(int size)
{
  for (int i = 0; i < size; i++)
  {
    Joystick.setButton(i, 0);
  }
}

void loop()
{
  // Define what two buttons pressed at the same time toggle sequential mode
  // Change the array index (ignore the -1)
  gear_t *gear_x = (gear_t *)(&gears[(3) -1]);
  gear_t *gear_y = (gear_t *)(&gears[(8) -1]);
  static bool last_gear_x = 0;
  static bool last_gear_y = 0;

  static bool need_send = false;

  for (int i = 0; i < gears_count; i++)
  {
    gear_t *gear = (gear_t *)(&gears[i]);

    bool reading = !(bool)digitalRead(gear->pin);

    if (reading != gear->last)
    {
      gear->last_debounce = millis();
    }

    if ((millis() - gear->last_debounce) > debounce)
    {
      if (reading != gear->current)
      {
        gear->current = reading;
        if (sequential_mode)
        {
          // Set default number to 0
          Joystick.setButton(gear->joy_number, 0);
          if (gear->joy_sequential_number > 0)
          {
            Joystick.setButton(gear->joy_sequential_number, gear->current);
            need_send = true;
          }
        }
        else
        {
          Joystick.setButton(gear->joy_number, gear->current);
          need_send = true;
        }
      }
    }

    gear->last = reading;
  }

  if ((gear_x->current != last_gear_x) && (gear_y->current != last_gear_y))
  {
      if (gear_x->current && gear_y->current)
      {
        sequential_mode = !sequential_mode;
#if DEBUG == 1
        Serial.print("Sequencial mode ");
        Serial.print(sequential_mode);
        Serial.println();
#endif
        digitalWrite(LED_BUILTIN, !sequential_mode);

        reset_all(9);
      }

    last_gear_x = gear_x->current;
    last_gear_y = gear_y->current;
  }

  if (need_send)
  {
    Joystick.sendState();
    need_send = false;
  }

}
