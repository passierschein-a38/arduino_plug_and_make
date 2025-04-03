#include <Modulino.h>

// Note frequencies in Hz (using uint16_t for >255 Hz)
constexpr uint16_t NOTE_D4  = 294;
constexpr uint16_t NOTE_F4  = 349;
constexpr uint16_t NOTE_G4  = 392;
constexpr uint16_t NOTE_A4  = 440;
constexpr uint16_t NOTE_Bb4 = 466;
constexpr uint16_t NOTE_C5  = 523;
constexpr uint16_t NOTE_D5  = 587;
constexpr uint16_t NOTE_C4 = 262;

// Frequencies in Hz
constexpr uint16_t NOTE_E4 = 330;
constexpr uint16_t NOTE_B4 = 494;
constexpr uint16_t NOTE_E5 = 659;

// Note structure
struct Note {
  uint16_t freq;   // Frequency in Hz
  uint16_t dur;    // Duration in ms
};

// Ode to Joy – Full melody
constexpr Note melody[] = {
  {NOTE_E4, 300}, {NOTE_E4, 300}, {NOTE_F4, 300}, {NOTE_G4, 300},
  {NOTE_G4, 300}, {NOTE_F4, 300}, {NOTE_E4, 300}, {NOTE_D4, 300},
  {NOTE_C4, 300}, {NOTE_C4, 300}, {NOTE_D4, 300}, {NOTE_E4, 300},
  {NOTE_E4, 450}, {NOTE_D4, 150}, {NOTE_D4, 600},

  {NOTE_E4, 300}, {NOTE_E4, 300}, {NOTE_F4, 300}, {NOTE_G4, 300},
  {NOTE_G4, 300}, {NOTE_F4, 300}, {NOTE_E4, 300}, {NOTE_D4, 300},
  {NOTE_C4, 300}, {NOTE_C4, 300}, {NOTE_D4, 300}, {NOTE_E4, 300},
  {NOTE_D4, 450}, {NOTE_C4, 150}, {NOTE_C4, 600},

  {NOTE_D4, 300}, {NOTE_D4, 300}, {NOTE_E4, 300}, {NOTE_C4, 300},
  {NOTE_D4, 300}, {NOTE_E4, 150}, {NOTE_F4, 150}, {NOTE_E4, 300},
  {NOTE_C4, 300}, {NOTE_D4, 300}, {NOTE_E4, 150}, {NOTE_F4, 150},
  {NOTE_E4, 300}, {NOTE_D4, 300}, {NOTE_C4, 300}, {NOTE_D4, 600},

  {NOTE_E4, 300}, {NOTE_E4, 300}, {NOTE_F4, 300}, {NOTE_G4, 300},
  {NOTE_G4, 300}, {NOTE_F4, 300}, {NOTE_E4, 300}, {NOTE_D4, 300},
  {NOTE_C4, 300}, {NOTE_C4, 300}, {NOTE_D4, 300}, {NOTE_E4, 300},
  {NOTE_D4, 450}, {NOTE_C4, 150}, {NOTE_C4, 600},
};

constexpr size_t melodyLength = sizeof(melody) / sizeof(melody[0]);

ModulinoBuzzer mod_buzzer;

void playMelody() {
  for (size_t i = 0; i < melodyLength; ++i) {
    mod_buzzer.tone(melody[i].freq, melody[i].dur);
    delay(melody[i].dur * 13 / 10); // ~30% gap
  }
}

void setup() {
  Modulino.begin();
  mod_buzzer.begin();


  playMelody();

}

void loop() {
  // put your main code here, to run repeatedly:

}
