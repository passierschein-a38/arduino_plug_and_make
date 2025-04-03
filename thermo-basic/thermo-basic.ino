#include <Modulino.h>
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

// Create object instance
ModulinoThermo mod_thermo;
ArduinoLEDMatrix mod_matrix;
ModulinoBuzzer mod_buzzer;
ModulinoButtons mod_buttons;

void on_update( const float celsius, const float fahrenheit, const float humidity);
void on_humidity(const float h);
void on_display(const float celsius, const float fahrenheit, const float humidity);
void on_buttons();

constexpr uint8_t c_button_a = 1;
constexpr uint8_t c_button_b = 2;
constexpr uint8_t c_button_c = 3;

uint8_t   last_button = 0;
float       g_celsius = 0;
float    g_fahrenheit = 0;
float      g_humidity = 0;

void setup(){
  Serial.begin(9600);

  // Call all necessary .begin() function
  Modulino.begin();
  mod_thermo.begin();  
  mod_buzzer.begin();
  mod_matrix.begin();
  mod_buttons.begin();

  mod_matrix.stroke(0xFFFFFFFF);
  mod_matrix.textFont(Font_4x6);
}

void loop(){

  g_celsius = mod_thermo.getTemperature();
  g_fahrenheit = (g_celsius * 9 / 5) + 32;
  g_humidity = mod_thermo.getHumidity();

  on_update( g_celsius, g_fahrenheit, g_humidity );
}


void on_update( const float celsius, const float fahrenheit, const float humidity)
{
  Serial.print("Temperature (C) is: ");
  Serial.println(celsius);

  Serial.print("Temperature (F) is: ");
  Serial.println(fahrenheit);

  Serial.print("Humidity (rH) is: ");
  Serial.println(humidity);

  on_buttons();
  on_display(celsius, fahrenheit, humidity);
  on_humidity(humidity);
}

void on_display(const float celsius, const float fahrenheit, const float humidity)
{
  char buffer[128];

  switch( last_button )
  {
    case c_button_a:
      sprintf(buffer, "%d%%", (int)humidity);
      break;
    case c_button_b:
      sprintf(buffer, "%dC", (int)celsius);
      break;
    case c_button_c:
        sprintf(buffer, "%dF", (int)fahrenheit);
      break;
    default:
      return;
  }

  // Make it scroll!
  
  mod_matrix.beginDraw();
  mod_matrix.clear();
  mod_matrix.beginText(0, 1, 0xFFFFFF);
  mod_matrix.print(buffer);
  mod_matrix.endText(NO_SCROLL);
  mod_matrix.endDraw();
  
  
}

void on_humidity(const float h)
{
 if( h >= 60 ){
    mod_buzzer.tone(330, 50); 
    delay(50);
  }
}

void on_buttons()
{
  if (mod_buttons.update()) {
    //Check if the buttons has been pressed
    if (mod_buttons.isPressed(0)) {
      last_button = c_button_a;
      mod_buttons.setLeds(true, false, false);
      Serial.println("Button A pressed!");
    } else if (mod_buttons.isPressed(1)) {
      last_button = c_button_b;
      mod_buttons.setLeds(false, true, false);
      Serial.println("Button B pressed!");
    } else if (mod_buttons.isPressed(2)) {
      last_button = c_button_c;
      mod_buttons.setLeds(false, false, true);
      Serial.println("Button C pressed!");
    }
  }
}