#include <Modulino.h>
#include <Arduino_LED_Matrix.h>

ModulinoColor OFF(0, 0, 0); 
ModulinoColor ORANGE(255,165,0);
const int brightness = 10;                      // Set brightness to 10 % as LEDs are very bright	

ModulinoPixels mod_leds;  
ModulinoDistance mod_distance;
ArduinoLEDMatrix mod_matrix;
ModulinoBuzzer mod_buzzer;
ModulinoKnob mod_knob;


void on_happy();
void on_sad();
void on_danger();

void setup() {

  const unsigned int baud_rate = 115200;
  Serial.begin(baud_rate);
  delay(1500);
  
  Modulino.begin();
  mod_leds.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  mod_distance.begin();
  mod_matrix.begin();
  mod_buzzer.begin();
  mod_knob.begin();
}

void set_level(const int level)
{
  mod_leds.clear();

  const short max_level = 7; // 8 leds 0-7

  for(int i=0; i<max_level; ++i){
    mod_leds.set(i, GREEN, brightness);
  }
  
  for(int i=0; i<level; ++i){
    mod_leds.set(i, RED, brightness);
  }
  
  mod_leds.show();		
}

void set_color(ModulinoColor color){
  mod_leds.clear();
  mod_leds.set(0, color, brightness);      
  mod_leds.set(1, color, brightness);
  mod_leds.set(2, color, brightness);
  mod_leds.set(3, color, brightness);
  mod_leds.set(4, color, brightness);
  mod_leds.set(5, color, brightness);     
  mod_leds.set(6, color, brightness);   
  mod_leds.set(7, color, brightness);   
  mod_leds.show();		
}

void printlnf(const char *format, ...) {
    char buffer[256]; // Adjust size if needed

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Serial.println(buffer);
}

// Clamp a value between min and max
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Maps a distance (0–1000 mm) and knob level (1–10) to a value from 0 to 8
uint8_t map_distance_to_value(int distance_mm, int knob_level) {
    // Constants
    const float sensitivity_factor = 0.5f;
    const int max_distance = 1000;
    const int max_output_value = 8;

    // Clamp inputs
    distance_mm = clamp(distance_mm, 0, max_distance);
    knob_level = clamp(knob_level, 1, 10);

    // Calculate sensitivity factor
    float sensitivity = 1.0f + (knob_level - 1) * sensitivity_factor;

    printlnf("sensitivity: %f", sensitivity);

    // Normalize distance (0.0 = far, 1.0 = close)
    float normalized = 1.0f - ((float)distance_mm / (float)max_distance);

    printlnf("normalized: %f", normalized);

    // Apply sensitivity curve
    float scaled = powf(normalized, sensitivity);

    // Map to output range 0–8
    int output = (int)(scaled * max_output_value);

    // Clamp and return result
    return (uint8_t)clamp(output, 0, max_output_value);
}

const unsigned int calculate_level_by_distance(const unsigned int distance_mm, const unsigned int sensitivity)
{

  if( distance_mm == 0 ){
    return 0;
  }

  const unsigned int treshold = 100; //100mm steps

  if( distance_mm > (sensitivity * treshold) ){
    return 8;
  }
  
  return (unsigned int)(distance_mm / (10*sensitivity));
}

void loop() {

  int knob_sensitivity = mod_knob.get();

  if( mod_knob.isPressed() ){
    mod_knob.set(0);
    Serial.println("knob reset to 0");
  }

  if( knob_sensitivity <= 0 ){
     mod_knob.set(1);
  }

  if( knob_sensitivity >= 10 ){
     mod_knob.set(10);
  }

  knob_sensitivity = mod_knob.get();

  if(!mod_distance.available()) {
   // Serial.println("No distance available");
    set_color(OFF);
    mod_matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
    return;
  }

  printlnf("knob level: %d", knob_sensitivity);
  
  const unsigned int distance_mm = mod_distance.get();

  printlnf("distance in mm: %d", distance_mm);

  const unsigned int level = map_distance_to_value(distance_mm, knob_sensitivity);


  printlnf("led level: %d", level);

  set_level( level );

  if( level > 0 && level <= 3 ){
    on_sad();
  }else if( level > 3 && level <= 7){
   
   on_danger();
  }else{
    on_happy();
  }

}

void on_happy()
{
  mod_matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
}

void on_sad()
{
  mod_matrix.loadFrame(LEDMATRIX_EMOJI_SAD);
  mod_buzzer.tone(330, 250); 
  delay(250);
}

void on_danger()
{
  mod_matrix.loadFrame(LEDMATRIX_DANGER);
  mod_buzzer.tone(330, 50); 
  delay(50);
}

