#include <Shifty.h>

#include "Talkie.h"
#include "Vocab_US_Clock.h"
#include "Vocab_Special.h"
#include "Vocab_US_Large.h"
#include "Vocab_US_TI99.h"

#include "uRTCLib.h"
#include <stdlib.h>
#include "ClickButton.h"

#include "DHT.h"

DHT dht;
// Declare the shift register
Shifty shift; 
Talkie Voice(true, false);
uRTCLib rtc(0x68);

const int ledPin = 9;

const int buttonPin2 = 7;   // Button connected to digital pin 7
const int buttonPin3 = 8;   // Button connected to digital pin 8
const int buttonPin1 = 10;  // Button connected to digital pin 10

ClickButton button1(buttonPin1, HIGH);
ClickButton button2(buttonPin2, HIGH);
ClickButton button3(buttonPin3, HIGH);


void setup() {
  // Set the number of bits you have (multiples of 8)
  shift.setBitCount(8*6);
  // Set the clock, data, and latch pins you are using
  // This also sets the pinMode for these pins
  shift.setPins(2, 5, 4); 

  pinMode(ledPin, OUTPUT); //cotrols the LEDs of the clock
  digitalWrite(ledPin, HIGH);

  pinMode(6, OUTPUT); //controls the relay of the talkie module
  digitalWrite(6, LOW); //sets the relay to closed at the startup

  setLEDBrightness(0); //disables the LEDs while setting all of the Shift Registers to LOW (LOW means HIGH here, the logic is inverted)
  for (int i = 1; i <= 48; i++) {
    shift.writeBit(i, HIGH);
  }
  Dots(); //set Dot segments on
  dht.setup(12); 	/* set pin for data communication */
  //setTime(30, 28, 19, 4, 6, 6, 24);
  URTCLIB_WIRE.begin(); //Needed for RTC module
  //rtc.set(15, 16, 12, 5, 7, 6, 24);
  //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
  //setTime(0, 0, 12, 0, 0, 0, 24) {
  button1.debounceTime = 25;   // Debounce timer in ms
  button1.multiclickTime = 350;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // Time until "held-down clicks" register

  button2.debounceTime = 25;   // Debounce timer in ms
  button2.multiclickTime = 350;  // Time limit for multi clicks
  button2.longClickTime  = 1000; // Time until "held-down clicks" register

  button3.debounceTime = 25;   // Debounce timer in ms
  button3.multiclickTime = 350;  // Time limit for multi clicks
  button3.longClickTime  = 1000; // Time until "held-down clicks" register

  setLEDBrightness(10); //set full brightness
}

void setTime(byte second, byte minute, byte hour, byte week, byte monthDay, byte month, byte year) {
  	rtc.set(second, minute, hour, week, monthDay, month, year);
	  //RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
}

void displayTime(){ //updating time to LEDs
  rtc.refresh();


  int hour = rtc.hour();
  int minute = rtc.minute();
  int second = rtc.second();

  if(hour < 10) {
    displayNumber(6, 0);  // Tens place of the hour
    displayNumber(5, hour);  // Units place of the hour
  }
  else {
    displayNumber(6, hour / 10);  // Tens place of the hour
    displayNumber(5, hour % 10);  // Units place of the hour
  }

  // Display minutes
  if(minute < 10) {
    displayNumber(4, 0);  // Tens place of the minute
    displayNumber(3, minute);  // Units place of the minute
  }
  else {
    displayNumber(4, minute / 10);  // Tens place of the minute
    displayNumber(3, minute % 10);  // Units place of the minute
  }

  // Display seconds
  if(second < 10) {
    displayNumber(2, 0);  // Tens place of the second
    displayNumber(1, second);  // Units place of the second
  }
  else {
    displayNumber(2, second / 10);  // Tens place of the second
    displayNumber(1, second % 10);  // Units place of the second
  }

  const long interval = 1000;
  unsigned long millis(interval);
}




void setLEDBrightness(int level) { //PWM Brightness control
  // Constrain the level to be within 0 to 10
  level = constrain(level, 0, 10);
  
  // Map the level (0-10) to the PWM value (0-255)
  int pwmValue = map(level, 0, 10, 0, 255);

  // Set the PWM value on the LED pin
  analogWrite(ledPin, pwmValue);
}

void displayNumber(int i, int number) { //mapping numbers to shift registers

  if (i == 1){
    i = 0;
  }
  else if (i == 2){
    i = 8;
  }
  else if (i == 3){
    i = 16;
  }
  else if (i == 4){
    i = 24;
  }
  else if (i == 5){
    i = 32;
  }
  else if (i == 6){
    i = 40;
  }

  
  // Set specific bits to LOW based on the number to display
  switch (number) {
    case 0:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 4, LOW);
      shift.writeBit(i + 5, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 7, LOW);
      shift.writeBit(i + 2, HIGH);
      break;
    case 1:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 3, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 2, HIGH);
      shift.writeBit(i + 4, HIGH);
      shift.writeBit(i + 5, HIGH);
      shift.writeBit(i + 6, HIGH);
      shift.writeBit(i + 7, HIGH);
      break;
    case 2:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 5, LOW);
      shift.writeBit(i + 4, LOW);
      shift.writeBit(i + 7, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 3, HIGH);
      shift.writeBit(i + 6, HIGH);
      break;
    case 3:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 4, LOW);
      shift.writeBit(i + 7, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 5, HIGH);
      shift.writeBit(i + 6, HIGH);
      break;
    case 4:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 4, HIGH);
      shift.writeBit(i + 5, HIGH);
      shift.writeBit(i + 7, HIGH);
      break;
    case 5:
      shift.writeBit(i + 7, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 4, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 1, HIGH);
      shift.writeBit(i + 5, HIGH);
      break;
    case 6:
      shift.writeBit(i + 7, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 5, LOW);
      shift.writeBit(i + 4, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 1, HIGH);
      break;
    case 7:
      shift.writeBit(i + 7, LOW);
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 3, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 2, HIGH);
      shift.writeBit(i + 4, HIGH);
      shift.writeBit(i + 5, HIGH);
      shift.writeBit(i + 6, HIGH);
      break;
    case 8:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 4, LOW);
      shift.writeBit(i + 5, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 7, LOW);
      // Set unused bits to HIGH
      break;
    case 9:
      shift.writeBit(i + 1, LOW);
      shift.writeBit(i + 2, LOW);
      shift.writeBit(i + 3, LOW);
      shift.writeBit(i + 6, LOW);
      shift.writeBit(i + 7, LOW);
      // Set unused bits to HIGH
      shift.writeBit(i + 4, HIGH);
      shift.writeBit(i + 5, HIGH);
      
      break;
        default:
            shift.writeBit(i + 7, HIGH);
            shift.writeBit(i + 1, HIGH);
            shift.writeBit(i + 2, HIGH);
            shift.writeBit(i + 3, HIGH);
            shift.writeBit(i + 4, HIGH);
            shift.writeBit(i + 5, HIGH);
            shift.writeBit(i + 6, HIGH);
            break;
    }
}

void sayNumber(int num) { //Talkie numbers to be used in the future
  switch (num) {
    case 0:
      Voice.say(sp3_ZERO);
      break;
    case 1:
      Voice.say(sp3_ONE);
      break;
    case 2:
      Voice.say(sp3_TWO);
      break;
    case 3:
      Voice.say(sp3_THREE);
      break;
    case 4:
      Voice.say(sp3_FOUR);
      break;
    case 5:
      Voice.say(sp3_FIVE);
      break;
    case 6:
      Voice.say(sp3_SIX);
      break;
    case 7:
      Voice.say(sp3_SEVEN);
      break;
    case 8:
      Voice.say(sp3_EIGHT);
      break;
    case 9:
      Voice.say(sp3_NINE);
      break;
    case 10:
      Voice.say(sp3_TEN);
      break;
    case 11:
      Voice.say(sp3_ELEVEN);
      break;
    case 12:
      Voice.say(sp3_TWELVE);
      break;
    case 13:
      Voice.say(sp3_THIRTEEN);
      break;
    case 14:
      Voice.say(sp3_FOURTEEN);
      break;
    case 15:
      Voice.say(sp3_FIFTEEN);
      break;
    case 16:
      Voice.say(sp3_SIXTEEN);
      break;
    case 17:
      Voice.say(sp3_SEVENTEEN);
      break;
    case 18:
      Voice.say(sp3_EIGHTEEN);
      break;
    case 19:
      Voice.say(sp3_NINETEEN);
      break;
    case 20:
      Voice.say(sp3_TWENTY);
      break;
    case 30:
      Voice.say(sp3_THIRTY);
      break;
    case 40:
      Voice.say(sp3_FOURTY);
      break;
    case 50:
      Voice.say(sp3_FIFTY);
      break;
    case 60:
      Voice.say(sp3_SIXTY);
      break;
    case 70:
      Voice.say(sp3_SEVENTY);
      break;
    case 80:
      Voice.say(sp3_EIGHTY);
      break;
    case 90:
      Voice.say(sp3_NINETY);
      break;
    case 100:
      Voice.say(sp3_HUNDRED);
      break;
    case 1000:
      Voice.say(sp3_THOUSAND);
      break;
    case 1000000:
      Voice.say(sp3_MILLION);
      break;
    default:
      //Serial.println("Number out of range");
      break;
  }
}

bool Dot = false;
void Dots() {
  if (Dot) {
    shift.writeBit(8, HIGH);  // turns on dot segments between the numbers
    shift.writeBit(16, HIGH);
    shift.writeBit(24, HIGH);
    shift.writeBit(32, HIGH);
    Dot = false;
  } else {
    shift.writeBit(8, LOW); // turns off dot segments between the numbers
    shift.writeBit(16, LOW);
    shift.writeBit(24, LOW);
    shift.writeBit(32, LOW);
    Dot = true;
  }
}
void DotsOn() {

  shift.writeBit(8, LOW);  // turns on dot segments between the numbers
  shift.writeBit(16, LOW);
  shift.writeBit(24, LOW);
  shift.writeBit(32, LOW);

}

void RandomDisplay() {
  for (int i = 0; i < 48; i++) {
    bool state = rand() % 2;  // Generate a random number (0 or 1)
    shift.writeBit(i, state ? HIGH : LOW);
  }
}

void displayTemperatureMoisture() {
    Dots();

    int temperature = dht.getTemperature();
    int humidity = dht.getHumidity();
    displayNumber(4, -1);
    displayNumber(3, -1);

    if (temperature < 10) {
        displayNumber(6, 0);  // Tens place of the hour
        displayNumber(5, temperature);  // Units place of the hour
    }
    else {
        displayNumber(6, temperature / 10);  // Tens place of the hour
        displayNumber(5, temperature % 10);  // Units place of the hour
    }

    if (humidity < 10) {
        displayNumber(2, 0);  // Tens place of the hour
        displayNumber(1, humidity);  // Units place of the hour
    }
    else {
        displayNumber(2, humidity / 10);  // Tens place of the hour
        displayNumber(1, humidity % 10);  // Units place of the hour
    }
}

int RandomInteger(int lowerLimit, int upperLimit) {
  // Ensure that the random function is seeded
  randomSeed(analogRead(0));

  // Generate a random integer between lowerLimit (inclusive) and upperLimit (exclusive)
  int randomNumber = random(lowerLimit, upperLimit + 1);

  return randomNumber;
}

void endingSpeech() {
  
  int randomNumber = RandomInteger(0,3);

  switch (randomNumber) {
    case 0:
      Voice.say(sp3_ANSWER);
      break;
    case 1:
      Voice.say(spt_ENTER);
      break;
    case 2:
      Voice.say(sp2_GO);
      break;
    case 3:
      Voice.say(sp2_ENTER);
      break;
    default:
        //Serial.println("Number out of range");
    break;
  }
}
void alarmSpeech() {
  
  int randomNumber = RandomInteger(0,4);

  switch (randomNumber) {
    case 0:
      Voice.say(sp4_EMERGENCY);
      break;
    case 1:
      Voice.say(sp4_WARNING);
      break;
    case 2:
      Voice.say(sp4_MAYDAY);
      break;
    case 3:
      Voice.say(sp2_DANGER);
      break;
    case 4:
      Voice.say(sp2_CAUTION);
      break;
    default:
        //Serial.println("Number out of range");
    break;
  }
}

void selectionSpeech() {
  
  Voice.say(sp4_SELECT);
  
  Voice.say(spt_WITH);
  
  Voice.say(sp2_BUTTON);
  

  Voice.say(spPAUSE1);
  
  Voice.say(sp2_FROM);
  
  Voice.say(sp2_DISPLAY);
 

  Voice.say(spPAUSE1);
  
  Voice.say(sp4_LEFT);
 
  Voice.say(sp4_IS);
 
  Voice.say(sp3_RED);
  
  Voice.say(spPAUSE2);

 
  Voice.say(spt_MIDDLE);
  
  Voice.say(sp4_IS);
  
  Voice.say(spt_BLUE);
 
  Voice.say(spPAUSE2);
  

  Voice.say(sp4_RIGHT);
 
  Voice.say(sp4_IS);
  
  Voice.say(spt_BLACK);

  Voice.say(spPAUSE1);
 
}

void timeSpeech() {
    int hour = rtc.hour();
    int minute = rtc.minute();
    bool isPM = hour >= 12;

    Voice.say(spc_THE);
    displayTime();
    Voice.say(spc_TIME);
    displayTime();
    Voice.say(spc_IS);
    displayTime();
    Voice.say(spPAUSE1);
    displayTime();

    // Say the hour
    if (hour < 10) {
        sayNumber(hour);
    } else if (hour < 20) {
        sayNumber(hour);
    } else {
        sayNumber((hour / 10) * 10); // Tens place
        sayNumber(hour % 10); // Units place
    }
    displayTime();
    // Say the minute
    if (minute < 10) {
        sayNumber(minute);
    } else if (minute < 20) {
        sayNumber(minute);
    } else {
        sayNumber((minute / 10) * 10); // Tens place
        sayNumber(minute % 10); // Units place
    }

    displayTime();
    // Say AM or PM
    if (isPM) {
        Voice.say(spc_P_M_);
    } else {
        Voice.say(spc_A_M_);
    }
    displayTime();

}

void TemperatureMoistureSpeech() {
    int temperature = dht.getTemperature();
    int humidity = dht.getHumidity();
    displayTemperatureMoisture();
    Voice.say(sp4_THE);
    
    Voice.say(sp3_TEMPERATURE);
    
    Voice.say(sp2_IS);
    

    // Say the temperature
    if (temperature < 10) {
        sayNumber(temperature);
    } else if (temperature < 20) {
        sayNumber(temperature);
    } else {
        sayNumber((temperature / 10) * 10); // Tens place
        sayNumber(temperature % 10); // Units place
    }
    
    // Say Celsius
    Voice.say(sp3_CELCIUS);
    

    Voice.say(spPAUSE2);
    

    Voice.say(sp4_THE);
    
    Voice.say(sp3_WEATHER);
    
    Voice.say(sp3_MIST);
    
    Voice.say(sp2_IS);
    

    // Say the humidity
    if (humidity < 10) {
        sayNumber(humidity);
    } else if (humidity < 20) {
        sayNumber(humidity);
    } else {
        sayNumber((humidity / 10) * 10); // Tens place
        sayNumber(humidity % 10); // Units place
    }

    // Say percent
    Voice.say(sp3_PERCENT);
    
}

void displayTwoDigitNumber(int number, int position) {
    if (number < 10) {
        displayNumber(position + 1, 0);  // Tens place
        displayNumber(position, number);  // Units place
    } else {
        displayNumber(position + 1, number / 10);  // Tens place
        displayNumber(position, number % 10);  // Units place
    }
}

void speakTwoDigitNumber(int number) {
    if (number < 10) {
        sayNumber(number);
    } else if (number < 20) {
        sayNumber(number);
    } else {
        sayNumber((number / 10) * 10); // Tens place
        sayNumber(number % 10); // Units place
    }
}


int question = 0;

void loop() {
  

  //Dots();

  //displayTime();
  button1.Update();
  button2.Update();
  button3.Update();

  if (button3.clicks == 0) {
    displayTime();
    DotsOn();
  }
  else if (button3.clicks == 1) {
    displayTemperatureMoisture();
    delay(3000);
  }

  if (button2.clicks == 1) {

    digitalWrite(6, HIGH); //Relay on
    
    Voice.say(spPAUSE2); //Wait for amplifier to work

    timeSpeech();
    Voice.say(spPAUSE2);
    TemperatureMoistureSpeech();

    digitalWrite(6, LOW);
    Dots();
  }
  else if (button2.clicks == 2) {

    digitalWrite(6, HIGH); //Relay on
    Voice.say(spPAUSE2); //Wait for amplifier to work

    Voice.say(spc_THE);
    RandomDisplay();
    Voice.say(spc_TIME);
    RandomDisplay();
    Voice.say(spc_IS);
    RandomDisplay();
    Voice.say(spPAUSE1);
    RandomDisplay();
    Voice.say(sp3_INCREASING);
    RandomDisplay();

    Voice.say(spPAUSE1);

    RandomDisplay();
    Voice.say(sp4_THE);
    RandomDisplay();
    Voice.say(sp3_TEMPERATURE);
    RandomDisplay();
    Voice.say(sp2_IS);
    RandomDisplay();
    Voice.say(spPAUSE1);
    RandomDisplay();
    Voice.say(spt_SOME);
    RandomDisplay();
    Voice.say(spt_THING);

    
    RandomDisplay();
    Voice.say(spPAUSE1);

    bool sad = rand() % 2;  // Generate a random number (0 or 1)
    if (sad) {
      Voice.say(spt_SO);
      RandomDisplay();
      Voice.say(spt_SORRY);
      RandomDisplay();
      Voice.say(spPAUSE1);
      RandomDisplay();
      Voice.say(spt_PLEASE);
      RandomDisplay();
      Voice.say(spt_TRY_AGAIN);
      RandomDisplay();
    }
    else {
      Voice.say(spt_NOT);
      RandomDisplay();
      Voice.say(spt_SORRY);
      RandomDisplay();
      Voice.say(spPAUSE1);
      RandomDisplay();
      Voice.say(spt_THAT_IS_RIGHT);
      RandomDisplay();
      Voice.say(spt_BYE);
      RandomDisplay();
    }

    digitalWrite(6, LOW);
    Dots();
  }

  if (button1.clicks == 1) {
    RandomDisplay();

    //question = RandomInteger(1, 3); //RANDOM QUESTION
    
    if (question == 4) {
      question = 1;
    }
    question++;

    digitalWrite(6, HIGH); //Relay on

    RandomDisplay();
    Voice.say(spPAUSE2); //Wait for amplifier to work

    int numIterations = RandomInteger(2,4);
    for (int i = 0; i < numIterations; i++) {
      alarmSpeech();
      Voice.say(spPAUSE1);
      RandomDisplay();
      Serial.println(i + 1);
    }
  
    Voice.say(spPAUSE1);
    RandomDisplay();
    Voice.say(spt_WHAT);
    RandomDisplay();

    Voice.say(spt_IS);
    RandomDisplay();

    switch (question) {
      case 1: {
        int first = RandomInteger(3, 20);
        
        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();

        Voice.say(spPAUSE1);
        speakTwoDigitNumber(first);

        displayTwoDigitNumber(first, 5);

        Voice.say(sp3_TIMES);

        int second = RandomInteger(1, 99/first);
        speakTwoDigitNumber(second);

        displayTwoDigitNumber(second, 1);

        Voice.say(spPAUSE2);

        endingSpeech();

        Voice.say(spPAUSE2);

        selectionSpeech();

        digitalWrite(6, LOW);
        int answer = first * second;
        int incorrect_answer = 0;

        if (answer <= 5) {
          // If answer is 3 or smaller, use range +6
          incorrect_answer = RandomInteger(answer, answer + 6);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        } else {
          // If answer is bigger than 3, use range -3 to +3
          incorrect_answer = RandomInteger(answer - 3, answer + 3);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        }

        int incorrect_answer2 = RandomInteger(incorrect_answer-3, incorrect_answer+3);

        if ((incorrect_answer2 == answer)&&(incorrect_answer2 == answer)) {
          incorrect_answer2++;
        }
        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();

        int randomNumber = RandomInteger(0, 2); // Generate a random number between 0 and 3

        // Map the random number to 1, 3, or 5
        int selectedNumber;
        int number2 = 1;
        int number1 = 1;
        switch (randomNumber) {
          case 0:
            selectedNumber = 1;
            number2 = 3;
            number1 = 5;
            break;
          case 1:
            selectedNumber = 3;
            number2 = 1;
            number1 = 5;
            break;
          case 2:
            selectedNumber = 5;
            number2 = 1;
            number1 = 3;
            break;
        }
        displayTwoDigitNumber(answer, selectedNumber);
        displayTwoDigitNumber(incorrect_answer, number1);
        displayTwoDigitNumber(incorrect_answer2, number2);

        
        bool correct = true;
        while(correct) {

          button1.Update();
          button2.Update();
          button3.Update();

          if (button1.clicks == 1){
            int buttonNumber = 5;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          
          }

          if (button2.clicks == 1){
            int buttonNumber = 3;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }

          if (button3.clicks == 1){
            int buttonNumber = 1;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }
          
        }
        
        digitalWrite(6, LOW);
        break;
      }
      case 2: {
        int first = RandomInteger(5, 99);

        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();

        Voice.say(spPAUSE1);
        speakTwoDigitNumber(first);
        
        displayTwoDigitNumber(first, 5);

        int second = RandomInteger(0, first);
        Voice.say(sp3_MINUS);
        
        speakTwoDigitNumber(second);
        
        displayTwoDigitNumber(second, 1);

        Voice.say(spPAUSE2);
        endingSpeech();
        

        Voice.say(spPAUSE2);
        selectionSpeech();

        digitalWrite(6, LOW);
        int answer = first - second;
        int incorrect_answer = 0;

        if (answer <= 5) {
          // If answer is 3 or smaller, use range +6
          incorrect_answer = RandomInteger(answer, answer + 6);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        } else {
          // If answer is bigger than 3, use range -3 to +3
          incorrect_answer = RandomInteger(answer - 3, answer + 3);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        }

        int incorrect_answer2 = RandomInteger(incorrect_answer-3, incorrect_answer+3);

        if ((incorrect_answer2 == answer)&&(incorrect_answer2 == answer)) {
          incorrect_answer2++;
        }
        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();

        int randomNumber = RandomInteger(0, 2); // Generate a random number between 0 and 3

        // Map the random number to 1, 3, or 5
        int selectedNumber;
        int number2 = 1;
        int number1 = 1;
        switch (randomNumber) {
          case 0:
            selectedNumber = 1;
            number2 = 3;
            number1 = 5;
            break;
          case 1:
            selectedNumber = 3;
            number2 = 1;
            number1 = 5;
            break;
          case 2:
            selectedNumber = 5;
            number2 = 1;
            number1 = 3;
            break;
        }
        displayTwoDigitNumber(answer, selectedNumber);
        displayTwoDigitNumber(incorrect_answer, number1);
        displayTwoDigitNumber(incorrect_answer2, number2);

        
        bool correct = true;
        while(correct) {

          button1.Update();
          button2.Update();
          button3.Update();

          if (button1.clicks == 1){
            int buttonNumber = 5;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          
          }

          if (button2.clicks == 1){
            int buttonNumber = 3;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }

          if (button3.clicks == 1){
            int buttonNumber = 1;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }
          
        }
        
        digitalWrite(6, LOW);      
        break;
      }
      case 3: {
        int first = RandomInteger(0, 99);
        int second = RandomInteger(0, 99-first);

        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();

        Voice.say(spPAUSE1);
        speakTwoDigitNumber(first);

        displayTwoDigitNumber(first, 5);

        Voice.say(sp3_PLUS);

        speakTwoDigitNumber(second);

        displayTwoDigitNumber(second, 1);

        
        Voice.say(spPAUSE2);
        endingSpeech();
        

        Voice.say(spPAUSE2);
        selectionSpeech();

        digitalWrite(6, LOW);
        int answer = first + second;
        int incorrect_answer = 0;

        if (answer <= 5) {
          // If answer is 3 or smaller, use range +6
          incorrect_answer = RandomInteger(answer, answer + 6);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        } else {
          // If answer is bigger than 3, use range -3 to +3
          incorrect_answer = RandomInteger(answer - 3, answer + 3);
          if (incorrect_answer == answer){
            incorrect_answer++;
          }
        }

        int incorrect_answer2 = RandomInteger(incorrect_answer-3, incorrect_answer+3);

        if ((incorrect_answer2 == answer)&&(incorrect_answer2 == answer)) {
          incorrect_answer2++;
        }
        displayNumber(1, -1);
        displayNumber(2, -1);
        displayNumber(3, -1);
        displayNumber(4, -1);
        displayNumber(5, -1);
        displayNumber(6, -1);
        displayNumber(7, -1);
        DotsOn();
        
        int randomNumber = RandomInteger(0, 2); // Generate a random number between 0 and 3

        // Map the random number to 1, 3, or 5
        int selectedNumber;
        int number2 = 1;
        int number1 = 1;
        switch (randomNumber) {
          case 0:
            selectedNumber = 1;
            number2 = 3;
            number1 = 5;
            break;
          case 1:
            selectedNumber = 3;
            number2 = 1;
            number1 = 5;
            break;
          case 2:
            selectedNumber = 5;
            number2 = 1;
            number1 = 3;
            break;
        }
        displayTwoDigitNumber(answer, selectedNumber);
        displayTwoDigitNumber(incorrect_answer, number1);
        displayTwoDigitNumber(incorrect_answer2, number2);

        
        bool correct = true;
        while(correct) {

          button1.Update();
          button2.Update();
          button3.Update();

          if (button1.clicks == 1){
            int buttonNumber = 5;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          
          }

          if (button2.clicks == 1){
            int buttonNumber = 3;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }

          if (button3.clicks == 1){
            int buttonNumber = 1;
            if(selectedNumber == buttonNumber){
              correct = false;
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_YOU_WIN);
            }
            else {
              digitalWrite(6, HIGH);
              Voice.say(spPAUSE2);
              Voice.say(spt_THAT_IS_INCORRECT);
              Voice.say(spPAUSE2);
              Voice.say(spt_TRY_AGAIN);
              digitalWrite(6, LOW);
            }
          }
          
        }
        
        digitalWrite(6, LOW);
        break;
      }
    }
    
    digitalWrite(6, LOW); // Relay off
  }
  


  

  //delay(2 * dht.getMinimumSamplingPeriod());

  /*
  digitalWrite(6, HIGH);
  

  sayNumber(1);
  
  RandomDisplay();
  sayNumber(2);
  
  RandomDisplay();
  sayNumber(3);
  
  RandomDisplay();
  sayNumber(4);
  
  RandomDisplay();

  digitalWrite(6, LOW);
  
  //delay(1000); 
*/
}



