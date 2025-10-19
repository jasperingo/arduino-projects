
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define KEY_ROWS 4  // four rows
#define KEY_COLS 4  // four columns
#define BUZZER_TONE 1500
#define BUZZER_DURATION 200
#define GAME_LEVEL_STEP 1
#define GAME_LEVEL_MAX 100
#define GAME_CHARACTERS_LEN 9

char keys[KEY_ROWS][KEY_COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t key_row_pins[KEY_ROWS] = { 9, 8, 7, 6 };  //connect to the row pinouts of the keypad
uint8_t key_col_pins[KEY_COLS] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad

const uint8_t buzzer_pin = 10;

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Create an object of keypad
Keypad keypad(makeKeymap(keys), key_row_pins, key_col_pins, KEY_ROWS, KEY_COLS);

enum Mode { ModeStart, ModeMemorize, ModePlay, ModeFailed };

uint8_t game_level = 0;
uint8_t game_mode = ModeStart;
uint8_t game_memory_index = 0;
char game_memory[GAME_LEVEL_MAX];
unsigned int game_memory_tones[GAME_LEVEL_MAX];
const char game_characters[GAME_CHARACTERS_LEN] = { '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const unsigned int game_tones[GAME_CHARACTERS_LEN] = { 1485, 1465, 1390, 1490, 1440, 1420, 1395, 1430, 1400 };

uint32_t previous_time = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buzzer_pin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Memory Game");
  lcd.setCursor(4, 1);
  lcd.print("Press A");

  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(A0));
}

void loop() {
  char key = keypad.getKey();  // Read the key

  if (key) {
    Serial.print("Key Pressed : ");
    Serial.println(key);

    if (game_mode == ModeStart || game_mode == ModeFailed) {
      tone(buzzer_pin, BUZZER_TONE, BUZZER_DURATION);
    }
  }

  long random_memory = 0;

  uint32_t current_time = millis();

  switch (game_mode) {
    case ModeStart:

      if (key == 'A') {
        switchToMemorize(GAME_LEVEL_STEP);
      }

    break;

    case ModeMemorize:

      if ((current_time - previous_time) >= 1000) {
        previous_time = current_time;

        if (game_memory_index < game_level) {
          random_memory = random(GAME_CHARACTERS_LEN);
          game_memory[game_memory_index] = game_characters[random_memory];
          game_memory_tones[game_memory_index] = game_tones[random_memory];
          lcd.setCursor(7, 1);
          lcd.print(game_memory[game_memory_index]);
          tone(buzzer_pin, game_memory_tones[game_memory_index], BUZZER_DURATION);
          game_memory_index++;
        } else {
          game_mode = ModePlay;
          game_memory_index = 0;
          lcd.clear();
          lcd.setCursor(6, 0);
          lcd.print("Play");
        }

      }

    break;
    
    case ModePlay:

      if (key) {
        lcd.setCursor(7, 1);
        lcd.print(key);

        if (key == game_memory[game_memory_index]) {
          tone(buzzer_pin, game_memory_tones[game_memory_index], BUZZER_DURATION);

          game_memory_index++;

          if (game_memory_index >= game_level) {
            switchToMemorize(min(game_level + GAME_LEVEL_STEP, GAME_LEVEL_MAX));
          }
        } else {
          game_mode = ModeFailed;
          tone(buzzer_pin, BUZZER_TONE, BUZZER_DURATION);
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("Game Over!");
          lcd.setCursor(4, 1);
          lcd.print("Press A");
        }
      }

    break;

    case ModeFailed:

      if (key == 'A') {
        switchToMemorize(GAME_LEVEL_STEP);
      }

    break;
  }

}

void switchToMemorize(uint8_t level) {
  game_mode = ModeMemorize;
  game_level = level;
  game_memory_index = 0;
  previous_time = millis();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Memorize");
}
