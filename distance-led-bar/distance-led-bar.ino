#include <Modulino.h>
#include <Arduino_LED_Matrix.h>

ModulinoColor OFF(0, 0, 0); 
ModulinoColor ORANGE(255,165,0);
const int brightness = 10;                      // Set brightness to 10 % as LEDs are very bright	

ModulinoPixels mod_leds;  
ModulinoDistance mod_distance;
ArduinoLEDMatrix mod_matrix;
ModulinoBuzzer mod_buzzer;

void on_happy();
void on_sad();
void on_danger();

void setup() {

  const unsigned int baud_rate = 9600;
  Serial.begin(baud_rate);
  delay(1500);
  
  Modulino.begin();
  mod_leds.begin();
  pinMode(LED_BUILTIN, OUTPUT);

  mod_distance.begin();
  mod_matrix.begin();
  mod_buzzer.begin();

}

void set_level(const int level)
{
  
  mod_leds.clear();

  const short max_level = 7; // 8 leds 0-7

  for(int i=0; i<max_level; ++i){
    mod_leds.set(i, RED, brightness);
  }
  
  for(int i=0; i<level; ++i){
    mod_leds.set(i, GREEN, brightness);
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

const unsigned int calculate_level_by_distance(const unsigned int distance_mm)
{
  if( distance_mm == 0 ){
    return 0;
  }

  if( distance_mm > 80 ){
    return 8;
  }
  
  return (unsigned int)(distance_mm / 10);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(!mod_distance.available()) {
   // Serial.println("No distance available");
    set_color(OFF);
    mod_matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
    return;
  }
  
  const unsigned int distance_mm = mod_distance.get();

  Serial.print("distance in mm:");
  Serial.println(distance_mm);
  
  const unsigned int level = calculate_level_by_distance(distance_mm);

  Serial.print("level:");
  Serial.println(level);

  set_level( level );

  if( level > 0 && level <= 3 ){
    on_danger();
  }else if( level > 3 && level <= 7){
   on_sad();
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

