class Button
{
  int buttonPin;
  int ledPin;
  unsigned long previousMillis;
  const unsigned long debounce = 120;
  bool oldState = HIGH;
  bool justOnce= false;

  // Constructor - creates a button 
  public:
  Button(int pin, int led){
    buttonPin = pin;
    ledPin = led;
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
  }
  
  bool onPress(){ //return true for debounced presses
    bool newState = digitalRead(buttonPin);
    digitalWrite(ledPin, !newState);
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
