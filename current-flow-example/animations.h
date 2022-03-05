////===== animations logic =============================================
///*    \/baseline code for static segments
//  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
//  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
//  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0);
//  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
//  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
//  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
//  simple_segment(midbot, midbot_length, voltsColor(5000, 255),0);
//  simple_segment(resist, resist_length, voltsColor(5000, 255),0);
//  simple_segment(ground, ground_length, voltsColor(5000, 255),0);
// */
//void unpressed_grounded(){
//  digitalWrite(pressed_LED, LOW);
//  digitalWrite(floating_LED, LOW);
//  pixels.clear();
//  //dark inline button
//  //above button
//  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
//  //floating disconnected button
//  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0); //<<---------These need changed, for sure!
//  //below button
//  simple_segment(midtop, midtop_length, voltsColor(0, 255),0);
//  simple_segment(dataln, dataln_length, voltsColor(0, 255),0);
//  simple_segment(mosfet, mosfet_length, voltsColor(0, 255),0);
//  simple_segment(midbot, midbot_length, voltsColor(0, 255),0);
//  simple_segment(resist, resist_length, voltsColor(0, 255),0);
//  simple_segment(ground, ground_length, voltsColor(0, 255),0);
//}
//void pressed_grounded(){
//  digitalWrite(pressed_LED, HIGH);
//  digitalWrite(floating_LED, LOW);
//  pixels.clear();
//  //dark disconnected button
//  //above resistor
//  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
//  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
//  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0);
//  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
//  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
//  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
//  simple_segment(midbot, midbot_length, voltsColor(5000, 255),0);
//  //rainbow resistor
//  rainbow_segment(resist, resist_length, 0, 5000, true);
//  //below resistor
//  simple_segment(ground, ground_length, voltsColor(0, 255),0);
//}
//void unpressed_floating(){
//  digitalWrite(pressed_LED, LOW);
//  digitalWrite(floating_LED, HIGH);
//  pixels.clear();
//  //dark inline button
//  //above button
//  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
//  //floating disconnected button
//  simple_segment(unpres, unpres_length, voltsColor(5000, 255),0); //<<---------These need changed, for sure!
//  //floating input pin
//  simple_segment(midtop, midtop_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
//  simple_segment(dataln, dataln_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
//  simple_segment(mosfet, mosfet_length, voltsColor(2500, 255),0); //<<---------These need changed, for sure!
//  //dark gounding leg
//}
//void pressed_floating(){
//  digitalWrite(pressed_LED, HIGH);
//  digitalWrite(floating_LED, HIGH);
//  pixels.clear();
//  //supply to input
//  simple_segment(supply, supply_length, voltsColor(5000, 255),0);
//  simple_segment(pressd, pressd_length, voltsColor(5000, 255),0);
//  simple_segment(midtop, midtop_length, voltsColor(5000, 255),0);
//  simple_segment(dataln, dataln_length, voltsColor(5000, 255),0);
//  simple_segment(mosfet, mosfet_length, voltsColor(5000, 255),0);
//  //dark disconnected button
//  //dark grounding leg
//}
//
//void rainbow_horizontal(){
//  //full HSV color wipe
//}
//void rainbow_vertical(){
//  //full HSV color wipe
//  
//}
//void demo_animation(){
//  for(int i=0;i<10;i++){
//    rainbow_horizontal();
//  }
//  for(int i=0;i<10;i++){
//    rainbow_vertical();
//  }
//  //some other stuff
//  rainbow_wo_delay(50);
//}
////===== animations logic =============================================
//
//// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
//void rainbow(int wait) {
//  // Hue of first pixel runs 5 complete loops through the color wheel.
//  // Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
//  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
//    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
//      // Offset pixel hue by an amount to make one full revolution of the
//      // color wheel (range of 65536) along the length of the strip
//      // (strip.numPixels() steps):
//      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
//      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//      // optionally add saturation and value (brightness) (each 0 to 255).
//      // Here we're using just the single-argument hue variant. The result
//      // is passed through strip.gamma32() to provide 'truer' colors
//      // before assigning to each pixel:
//      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
//    }
//    pixels.show(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}
