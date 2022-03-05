
#include <Adafruit_NeoPixel.h>
//https://medium.com/jungletronics/arduino-multi-file-sketch-583a7833162c
#include "button.h"
#include "animations.h"

const int PIN = 6; //hello!
const int NUMPIXELS = 142;
const int DELAYVAL = 50; // Time (in milliseconds) to pause between pixels
const int button_1_pin = 9;
const int button_1_led = 10;
const int button_2_pin = 12;
const int button_2_led = 11;

/*      drawn LED section KEY
 *  | <<==supply
 *  |
 *    |pressed  |unpress
 *  |
 *  | <<==midTop
 *  |------dataln---------|| <<==mosfet
 *  | <<==midBot
 *  /
 *  \
 *  / <<== resistor
 *  \
 *  |
 *  | <<==ground 
 */

const int supply[]= {87,86,85,84,83,82,81,80,79,78,77,76,75,74,73};
#define supply_length 15
const int pressd[]= {66,65,64,63,62,61};
#define pressd_length 6
const int unpres[]= {72,71,70,69,68,67};
#define unpres_length 6
const int midtop[]= {60,59,58,57,56,55,54,53,52,51,50,49,48};
#define midtop_length 13
const int midbot[]= {47,46,45,44,43};
#define midbot_length 5
const int resist[]= {42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10};
#define resist_length 33
const int ground[]= {9,8,7,6,5,4,3,2,1,0};
#define ground_length 10
const int mosfet[]= {92,93,91,88,89,90};
#define mosfet_length 6
const int dataln[]= {141,141,140,139,138,137,136,135,134,133,132,131,130,129,128,127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96,95,94};
#define dataln_length 48

volatile bool pressed  = 0;
const int pressed_LED  = button_1_led;
volatile bool floating = 0;
const int floating_LED = button_2_led;
volatile bool p_debounce = 0;
volatile bool f_debounce = 0;

Button up(button_1_pin, button_1_led);
Button dn(button_2_pin, button_2_led);

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//====== interrupt routines ===================================
void press_latch(){
  if(p_debounce){
    return;
  }
  p_debounce = 1;
  pressed = 1;
  pixels.clear(); // Set all pixel colors to 'off'
}
void floating_latch(){
  if(p_debounce){
    return;
  }
  f_debounce = 1;
  floating = 1;
  pixels.clear(); // Set all pixel colors to 'off'
}
//====== interrupt routines ===================================

void setup() {
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();            // Turn OFF all pixels ASAP
  pixels.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  pinMode(pressed_LED, OUTPUT);
  digitalWrite(pressed_LED, LOW);
  pinMode(floating_LED, OUTPUT);
  digitalWrite(floating_LED, LOW);
  //attachInterrupt(0, press_latch, RISING); //on pin 2
  //attachInterrupt(1, floating_latch, RISING); //on pin 3
}

void loop() {
  if(up.onPress()) press_latch();
  if(dn.onPress()) floating_latch();
  
  if(f_debounce || p_debounce){
    if(floating){
      if(pressed){
        //pressed button -- floating pin
        int w = 0;
        while(w<5){
          pressed_floating();
          delay(300);
          unpressed_floating();
          delay(300);
          w++;
        }
      }else{
        //unpressed button -- floating pin
        unpressed_floating();
      }  
    }else{
      if(pressed){
        //pressed button -- grounded pin
        int w=0;
        while(w<5){
          pressed_grounded();
          delay(300);
          unpressed_grounded();
          delay(300);
          w++;
        }
      }else{
        //unpressed button -- grounded pin
        //unpressed_grounded();
      }  
    }
  }
  //finished the requested animations
//  pressed  = 0;
//  p_debounce = 0;
//  f_debounce = 0;
  //start 'DEMO' animation
  //demo_animation();
  //rainbow(2);             // Flowing rainbow cycle along the whole strip

}

//===== mid-level logic ==============================================
void simple_segment( int dp[],int len, int color, int perPixelDelay){
  for(int i=0; i<len; i++) { // For each pixel...
    pixels.setPixelColor(dp[i], color);
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(perPixelDelay); // Pause before next pass through loop
  }
}
void rainbow_segment(int dp[],int len, int minVolt, int maxVolt, bool highToLow){
  int voltDelta = (maxVolt - minVolt)/len;
  int voltsCounter;
  if (highToLow){
    voltsCounter = maxVolt;
  }else{
    voltsCounter = minVolt;
  }
  for(int i=0; i<len; i++) { // For each pixel...
    pixels.setPixelColor(dp[i], voltsColor(voltsCounter, 255));
    if (highToLow){
      voltsCounter - voltDelta;
    }else{
      voltsCounter + voltDelta;
    }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}
void flatColorAll(int color, int perPixelDelay){
  pixels.clear(); // Set all pixel colors to 'off'
  simple_segment(supply, supply_length, color, perPixelDelay);
  simple_segment(pressd, pressd_length, color, perPixelDelay);
  simple_segment(unpres, unpres_length, color, perPixelDelay);
  simple_segment(midtop, midtop_length, color, perPixelDelay);
  simple_segment(dataln, dataln_length, color, perPixelDelay);
  simple_segment(mosfet, mosfet_length, color, perPixelDelay);
  simple_segment(midbot, midbot_length, color, perPixelDelay);
  simple_segment(resist, resist_length, color, perPixelDelay);
  simple_segment(ground, ground_length, color, perPixelDelay);
}
unsigned long previousFrame = 0;
void rainbow_wo_delay(int wait) { // Rainbow cycle along whole pixels. non-delay wait times.
  if(millis()>= previousFrame + wait){
    for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
      for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in pixels...
        int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
        pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
      }
      pixels.show(); // Update pixels with new contents
    }
    previousFrame = millis();
  }
}
int voltsColor(int mV, int brightness){
  //millivolts can be 5000 to 0
  //brightness can be 255-0
  unsigned int pixelHue = map(mV, 0, 5000, 0, 43691); //max hue value is 65536 --> looks good if it stops at 70% of that...
  int colorCode = pixels.gamma32(pixels.ColorHSV(pixelHue,255,brightness));
  return colorCode;  
}
void concatArray(int bigArray[], int bigArray_len, int subArray[], int subArray_len){
  int i = 0;
  //find first 'zero'
  for(int j=0; j<bigArray_len; j++){
    if(bigArray[j] == 0){
      if(j = 0){  //first array concat
        i = j;  
      }else if(bigArray[j-1] == 1){ //the one array that ends in zero
        i = j+1;
      }else{    //all other concats
        i = j;
      }
      break;
    }
  }
  for(int k=0; k<subArray_len;k++){
    bigArray[i+k] = subArray[k];
  }
}
//===== mid-level logic ==============================================

//===== animations logic =============================================
/*    \/baseline code for static segments
  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0);
  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
  simple_segment(midbot, midbot_length, voltsColor(5000, 255),0);
  simple_segment(resist, resist_length, voltsColor(5000, 255),0);
  simple_segment(ground, ground_length, voltsColor(5000, 255),0);
 */
void unpressed_grounded(){
  digitalWrite(pressed_LED, LOW);
  digitalWrite(floating_LED, LOW);
  pixels.clear();
  //dark inline button
  //above button
  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
  //floating disconnected button
  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0); //<<---------These need changed, for sure!
  //below button
  simple_segment(midtop, midtop_length, voltsColor(0, 255),0);
  simple_segment(dataln, dataln_length, voltsColor(0, 255),0);
  simple_segment(mosfet, mosfet_length, voltsColor(0, 255),0);
  simple_segment(midbot, midbot_length, voltsColor(0, 255),0);
  simple_segment(resist, resist_length, voltsColor(0, 255),0);
  simple_segment(ground, ground_length, voltsColor(0, 255),0);
}
void pressed_grounded(){
  digitalWrite(pressed_LED, HIGH);
  digitalWrite(floating_LED, LOW);
  pixels.clear();
  //dark disconnected button
  //above resistor
  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0);
  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
  simple_segment(midbot, midbot_length, voltsColor(5000, 255),0);
  //rainbow resistor
  rainbow_segment(resist, resist_length, 0, 5000, true);
  //below resistor
  simple_segment(ground, ground_length, voltsColor(0, 255),0);
}
void unpressed_floating(){
  digitalWrite(pressed_LED, LOW);
  digitalWrite(floating_LED, HIGH);
  pixels.clear();
  //dark inline button
  //above button
  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
  //floating disconnected button
  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0); //<<---------These need changed, for sure!
  //floating input pin
  simple_segment(midtop, midtop_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
  simple_segment(dataln, dataln_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
  simple_segment(mosfet, mosfet_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
  //dark gounding leg
}
void pressed_floating(){
  digitalWrite(pressed_LED, HIGH);
  digitalWrite(floating_LED, HIGH);
  pixels.clear();
  //supply to input
  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
  //dark disconnected button
  //dark grounding leg
}

void rainbow_horizontal(){
  //full HSV color wipe
}
void rainbow_vertical(){
  //full HSV color wipe
  
}
void demo_animation(){
  for(int i=0;i<10;i++){
    rainbow_horizontal();
  }
  for(int i=0;i<10;i++){
    rainbow_vertical();
  }
  //some other stuff
  rainbow_wo_delay(50);
}
//===== animations logic =============================================

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
