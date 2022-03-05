#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


//===========================================================
//   /\    /\    /\    /\    /\    /\    /\    /\    /\    /
//  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /
// /    \/    \/    \/    \/    \/    \/    \/    \/    \/
//===========================================================
//=========  button object  =================================
class Button
{
  int buttonPin;
  unsigned long previousMillis;
  const unsigned long debounce = 120;
  bool oldState = HIGH;
  bool justOnce= false;

  // Constructor - creates a button 
  public:
  Button(int pin){
    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);
  }
  
  bool onPress(){ //return true for debounced presses
    bool newState = digitalRead(buttonPin);
    unsigned long now = millis();

    if(justOnce && (now > previousMillis + debounce)) { //after debounce time
      justOnce = false;
      if(!newState) return true; //only if still pressed
    }

    if((newState == LOW) && (oldState == HIGH)) {
      previousMillis = now;
      justOnce = true;
    }
    
    oldState = newState;
    return false;
  }
};
//=========  button object  =================================
//===========================================================
//   /\    /\    /\    /\    /\    /\    /\    /\    /\    /
//  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /
// /    \/    \/    \/    \/    \/    \/    \/    \/    \/
//===========================================================





//=========  setup const & objects  =========================
const int BUTTON_1_PIN = 9;
const int BUTTON_2_PIN = 12;

Button up(BUTTON_1_PIN); //create the 'up' button object
Button dn(BUTTON_2_PIN); //create the 'down' button object

const int PIXEL_PIN   = 6;
const int PIXEL_COUNT = 142;

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
//=========  setup const & objects  =========================





//===========================================================
//   /\    /\    /\    /\    /\    /\    /\    /\    /\    /
//  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /
// /    \/    \/    \/    \/    \/    \/    \/    \/    \/
//===========================================================
//========  main functions  =================================
void setup() {
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  announce(0, strip.Color( 0, 0,80)); //start with pixel '0'
  Serial.begin(9600);
}

unsigned long pix_num = 142*100; //partway up, a clean multiple of 142

void loop() {
  if(up.onPress()){
    pix_num = pix_num +1;
    announce(pix_num);
  }
  if(dn.onPress()){
    pix_num = pix_num -1;
    announce(pix_num);
  }
}
//========  main functions  =================================
//===========================================================
//   /\    /\    /\    /\    /\    /\    /\    /\    /\    /
//  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /
// /    \/    \/    \/    \/    \/    \/    \/    \/    \/
//===========================================================






//===========================================================
//   /\    /\    /\    /\    /\    /\    /\    /\    /\    /
//  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /  \  /
// /    \/    \/    \/    \/    \/    \/    \/    \/    \/
//===========================================================
//========  LED movements  ==================================

void announce(int pix){ //overloaded function
  announce(pix, strip.Color(80,80, 0));
}

void announce(int pix, uint32_t color) {
  uint32_t off = strip.Color( 0, 0, 0);
  strip.setPixelColor(mod(pix-1), off);
  strip.setPixelColor(mod(pix), color);
  strip.setPixelColor(mod(pix+1), off);
  strip.show();
  
  Serial.print("currently pixel #");
  Serial.print(mod(pix));
  Serial.print("\t\t\t");
  Serial.println(pix);
}

int mod(long pix){
  return pix % strip.numPixels();
}
