  /* stuff to smooth out the pot readings  
    
    Define the number of samples to keep track of. The higher the number,  
    the more the readings will be smoothed, but the slower the output will  
    respond to the input. Using a constant rather than a normal variable lets  
    use this value to determine the size of the readings array.  
  */  
  // pot readings
  const int numReadings = 10;
    
  int readings[numReadings];   // the readings from the analog input  
  int index = 0;         // the index of the current reading  
  int total = 0;         // the running total  
  int average = 0;        // the average
    
  // set the ledPins  
  int ledRed = 9;  
  int ledGreen = 10;  
  int ledBlue = 11;  
    
  // color selector pin  
  int potPin = 0;  
     
  // lightMode selector switch pin  
  int switchPin = 2;  
   
  // light mode variable  
  // initial value 0 = music  
  int lightMode = 1;  
    
  // LED Power variables  
  byte redPwr = 0;  
  byte greenPwr = 0;  
  byte bluePwr = 0;  
  byte ledPwr = 0;
  
  // variables for keeping pulse color
  byte redPulse;
  byte greenPulse;
  byte bluePulse;
  int pulseSpeed;
    
  // Set pulse to down initially  
  byte pulse = 0;  
    
  // floating variables needed to be able to pulse a fixed color   
  float redFloat;  
  float greenFloat;  
  float blueFloat;  
    
  // the amount R,G & B should step up/down to display an fixed color  
  float redKoff;  
  float greenKoff;  
  float blueKoff;  
    
  // Variables for cycleColor  
  int truColor = 0;  
    
  // misc interface variables  
  // potVal store the value of the potentiometer for various needs   
  int potVal;  
  // value from the button (debounce)  
  int switchVal;  
  int switchVal2;  
  // buttonState registers if the button has changed  
  int buttonState;  
    
  // MSGEQ7 stuff for music  
    
  int analogPin = 3; // MSGEQ7 OUT  
  int strobePin = 4; // MSGEQ7 STROBE  
  int resetPin = 7; // MSGEQ7 RESET
     
  // MSGEQ7 OUT pin produces values around 50-80  
  // when there is no input, so use this value to  
  // filter out a lot of the chaff.  
  int filterValue = 80;  
    
  // variable to control the max intensity of the LEDs when in music mode  
  int filterMax = 255;  
  
  const int numSpectrums = 7;
  const int numSpectrumReadings = 7;
  int spectrumValue[numSpectrums][numSpectrumReadings]; // store the 7 eq band values
  int spectrumAvg[numSpectrums];
  int spectrumIndex = 0;         // the index of the current reading  
  int intensity = 0;
  int oldIntensity = 0;
  
  float spectrumPow[numSpectrums] = {1.5, 0.01, 0.5, 0.01, 0.01, 0.15, 0.8};
  float colorMult[3] = {1,0,0};
  float intensityMult = 0.25;
  int baseIntensity = 20;
  int maxIntensity = 250;
  int threshold = 50;
  int intensityDiv = 5;
  int intensityStepDown = 30;
    
  void setup()  
  {  
   pinMode(ledRed, OUTPUT);  
   pinMode(ledGreen, OUTPUT);  
   pinMode(ledBlue, OUTPUT);  
     
   pinMode(potPin, INPUT);  
     
   pinMode(switchPin, INPUT);  
   buttonState = digitalRead(switchPin);   
     
     
     
   // serial for debugging purposes only  
   // Serial.begin(9600);  
     
   // pot smoothing  
   for (int thisReading = 0; thisReading < numReadings; thisReading++)  
     readings[thisReading] = 0;
     
   // eq smoothing
   for (int i=0; i<numSpectrums; i++) {
      for (int j=0; j<numSpectrumReadings; j++) {
        spectrumValue[i][j] = 0;
      }
      spectrumAvg[i] = 0;
   }     
   
   // Read from MSGEQ7 OUT  
   pinMode(analogPin, INPUT);  
   // Write to MSGEQ7 STROBE and RESET  
   pinMode(strobePin, OUTPUT);  
   pinMode(resetPin, OUTPUT);  
     
   // Set analogPin's reference voltage  
   analogReference(DEFAULT); // 5V  
     
   // Set startup values for pins  
   digitalWrite(resetPin, LOW);  
   digitalWrite(strobePin, HIGH);  
    
   // set the Arduino timers to crazyfast values for pins 3, 9, 10, and 11 so that PWM doesn't cause audible hum or feedback in the audio circuit.   
   //If changing code to use a second LED strip on the other 3 PWM pins, make sure to set TCCR0B as well, but be mindful that it will effect system timer values  
   // such as delay(), millis(), micros(), etc  
   // See: http://playground.arduino.cc/Main/TimerPWMCheatsheet for more information  
   TCCR1B = TCCR1B & 0b11111000 | 0x01;  
   TCCR2B = TCCR2B & 0b11111000 | 0x01;  
  }  
    
  void loop()  
  {  
   /*switchVal = digitalRead(switchPin);   // read input value and store it in val  
   delay(10);             // 10 milliseconds is a good amount of time  
      
   switchVal2 = digitalRead(switchPin);   // read the input again to check for bounces  
   if (switchVal == switchVal2) {         // make sure we got 2 consistant readings!  
    if (switchVal != buttonState) {     // the button state has changed!  
     if (switchVal == LOW) {        // check if the button is pressed  
      switch (lightMode) {     // light is initially in music react mode  
       case 0:  
          lightMode = 1;      // light is on and responds to pot to choose color  
       break;   
       case 1:  
         lightMode = 2;      // light cycles thru colors 
       break;   
       case 2:     
         lightMode = 3;      // lights off 
       break;  
       case 3:  
          lightMode = 0;      // music react  
       break;                       
      } // END switch (lightMode)    
     } // END if (switchVal == LOW)  
    } // END if (switchVal != buttonState)   
       
    buttonState = switchVal;         // save the new state in our variable  
   } // END if (switchVal == switchVal2)  
    
  switch (lightMode) {  
    case 0:  
      musicEQ();  
    break;  
    case 1:  
      colorControl();  
    break;
    case 2:   
     //1000ms = 1s
     cycleColor(250);  
    break;  
    case 3:  
      lightsOff();  
    break;  
   }*/
 } // END loop()  
 
    
  // lightMode 1   
  void colorControl() {  
    
    // read the potentiometer position  
    getPot();  
    potVal = average;   
     
   // RED > ORANGE > YELLOW  
    if (potVal > 0 && potVal < 170) {  
     redPwr = 255;  
     bluePwr = 0;  
     greenPwr = map(potVal, 0, 170, 0, 255);  
    }  
     
    // YELLOW > LIME?? > GREEN   
    if (potVal > 170 && potVal < 341) {  
     greenPwr = 255;  
     bluePwr = 0;  
     redPwr = map(potVal, 341, 170, 0, 255);  
    }  
    // GREEN > TURQOUISE  
    if (potVal > 341 && potVal < 511) {  
     greenPwr = 255;  
     redPwr = 0;  
     bluePwr = map(potVal, 341, 511, 0, 255);  
    }  
     
    // TURQOUISE > BLUE   
    if (potVal > 511 && potVal < 682) {  
     bluePwr = 255;  
     redPwr = 0;  
     greenPwr = map(potVal, 682, 511, 0, 255);  
    }  
     
    // BLUE > PURPLE   
    if (potVal > 682 && potVal < 852) {  
     bluePwr = 255;  
     greenPwr = 0;  
     redPwr = map(potVal, 682, 852, 0, 255);  
    }  
     
    // PURPLE > RED  
    if (potVal > 852 && potVal < 1023) {  
     redPwr = 255;  
     greenPwr = 0;  
     bluePwr = map(potVal, 1023, 852, 0, 255);  
    }   
      
    redFloat = float(redPwr);  
    greenFloat = float(greenPwr);  
    blueFloat = float(bluePwr);  
      
    redKoff = redFloat / 255;  
    greenKoff = greenFloat / 255;  
    blueKoff = blueFloat / 255;  
      
    redPulse = redPwr;  
    greenPulse = greenPwr;  
    bluePulse = bluePwr;   
      
   /*  
   // Debug   
   Serial.print("redFLoat: ");  
   Serial.print(redFloat, DEC);  
   Serial.print(" redPwr: ");  
   Serial.print(redPwr, DEC);  
   Serial.print(" greenFloat: ");  
   Serial.print(greenFloat, DEC);  
   Serial.print(" greenPwr: ");  
   Serial.print(greenPwr, DEC);  
   Serial.print(" blueFloat: ");  
   Serial.print(blueFloat, DEC);  
   Serial.print(" bluePwr: ");  
   Serial.println(bluePwr, DEC);  
   // End debug  
   */  
     
   // Display colors   
   colorDisplay();  
  }      
    
  // lightMode 2  
  void cycleColor() {  // Cycles through colors  
    
   switch(truColor) {  
   // RED > ORANGE > YELLOW    
    case 0:  
     redPwr = 255;  
     bluePwr = 0;  
     greenPwr++;  
     if (greenPwr > 254) {  
      truColor = 1;  
     }  
     break;  
      
    // YELLOW > LIME?? > GREEN   
    case 1:  
     greenPwr = 255;  
     bluePwr = 0;  
     redPwr--;  
     if (redPwr < 1) {  
      truColor = 2;  
     }  
     break;  
    
    // GREEN > TURQOUISE  
    case 2:  
     greenPwr = 255;  
     bluePwr++;  
     redPwr = 0;  
     if (bluePwr > 254) {  
      truColor = 3;  
     }    
    break;  
      
    // TURQOUISE > BLUE   
    case 3:  
     greenPwr--;  
     bluePwr = 255;  
     redPwr = 0;  
     if (greenPwr < 1) {  
      truColor = 4;  
     }  
     break;  
       
    // BLUE > PURPLE   
    case 4:  
     greenPwr = 0;  
     bluePwr = 255;  
     redPwr++;  
     if (redPwr > 254) {  
      truColor = 5;  
     }  
     break;  
       
    // PURPLE > RED  
    case 5:  
     greenPwr = 0;  
     bluePwr--;  
     redPwr = 255;  
     if (bluePwr < 1) {  
      truColor = 0;  
     }    
     break;  
   }  
   // START SPEED   
   getPot();  
   potVal = average;   
   //pulseSpeed = map(potVal, 0, 1023, 0, 200);  
     
   //display the colors  
   colorDisplay();  
   // set speed of change  
   delay(pulseSpeed);
   //delay(delaytime);  
   // END SPEED  
     
  } // END cycleColor   
    
  // lightMode 3  
  void lightsOff() {  
   redPwr = 0;  
   greenPwr = 0;  
   bluePwr = 0;  
   colorDisplay();  
  }  
    
  // lightMode 0   
  void musicEQ() {  
   getPot();  
     
   filterMax = map(average, 0, 1023, 0, 255);  
     
   // Set reset pin low to enable strobe  
   digitalWrite(resetPin, HIGH);  
   digitalWrite(resetPin, LOW);  
     
   int specval;
   // Get all 7 spectrum values from the MSGEQ7  
   for (int i = 0; i < 7; i++)  
   {  
      digitalWrite(strobePin, LOW);  
      delayMicroseconds(30); // Allow output to settle  
     
      specval = analogRead(analogPin);  
     
      // Constrain any value above 1023 or below filterValue  
      specval = constrain(specval, filterValue, 1023);  
       
      // Remap the value to a number between 0 and 255  
      specval = map(specval, filterValue, 1023, 0, filterMax);
    
      spectrumValue[i][spectrumIndex] = specval;
     
      // Remove serial stuff after debugging  
      //Serial.print(spectrumValue[i]);  
      //Serial.print(" ");  
     
      digitalWrite(strobePin, HIGH);}   
    //Serial.println();  
       
    /* Spectrum Value Frequencies:  
     0: 63 Hz  
     1: 160 Hz  
     2: 400 Hz  
     3: 1000 Hz  
     4: 2500 Hz  
     5: 6250 Hz  
     6: 16000 Hz  
       
     1, 4, and 6 seem to offer best combination using only 3 LEDs  
     If using 6 LEDs, 1-6 or 1-3 and 4-6 depending on music.  
    */  
    
   if (spectrumIndex == numSpectrumReadings) {
     spectrumIndex = 0;
     for (int i=0; i<numSpectrums; i++) {
       spectrumAvg[i] = 0;
     }
     for (int i=0; i<numSpectrums; i++) {
       for (int j=0; j<numSpectrumReadings; j++) {
         spectrumAvg[i] += spectrumValue[i][j];
       }
     }
     
     int intensity = baseIntensity;
     for (int i=0; i<numSpectrums; i++) {
       spectrumAvg[i] /= numSpectrumReadings;
       intensity += pow(spectrumAvg[i], spectrumPow[i]);
     }
     
     /*if (intensity + threshold > oldIntensity) {
       intensity = intensity - oldIntensity/intensityDiv;
     } else if (intensity - threshold < oldIntensity) {
       intensity = intensity + oldIntensity/intensityDiv;
     } else if ((intensity-oldIntensity) > threshold) {
       intensity = intensity - intensityStepDown;
     }
     oldIntensity = intensity;*/
     
     intensity *= intensityMult;
     if (intensity>maxIntensity)
       intensity = maxIntensity;

     redPwr = colorMult[0]*intensity;
     greenPwr = colorMult[1]*intensity;
     bluePwr = colorMult[2]*intensity;
     colorDisplay();
   }
   spectrumIndex++;
  }  
  
  ////////////////////FUNCTIONS////////////////////////
    
  // Displays the colors when called from other functions  
  void colorDisplay() {  
   analogWrite(ledRed, redPwr);  
   analogWrite(ledGreen, greenPwr);  
   analogWrite(ledBlue, bluePwr);  
  }  
    
    
  // Smooths out the pot readings.   
  void getPot() {  
   // subtract the last reading:  
   total= total - readings[index];       
   // read from the sensor:   
   readings[index] = analogRead(potPin);   
   // add the reading to the total:  
   total= total + readings[index];      
   // advance to the next position in the array:   
   index = index + 1;            
    
   // if we're at the end of the array...  
   if (index >= numReadings)         
    // ...wrap around to the beginning:   
    index = 0;                
    
   // calculate the average:  
   average = total / numReadings;       
   // send it to the computer as ASCII digits  
   // Serial.println(average);    
   delay(1);    // delay in between reads for stability        
  }
