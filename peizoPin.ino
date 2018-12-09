/*
  Arduino Christmas Songs
  
  Based on a project and code by Dipto Pratyaksa, updated on 31/3/13

  Modified for Christmas by Joshi, on Dec 17th, 2017.
*/

#include "pitches.h"

#define melodyPin 9

#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
int selected;
int sleep;
unsigned long CurrentTime;
unsigned long StartTime;

// Jingle Bells

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We wish you a merry Christmas

int wish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// Santa Claus is coming to town

int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};



// dominick the donkey

int dominick_the_donkey[] = {
  
  NOTE_G, NOTE_D, NOTE_G,
  
  NOTE_D, NOTE_E, NOTE_F1,
  
  NOTE_G, NOTE_D, NOTE_G,


};

int dominick_the_donkey_tempo[] = {
  
  8, 8, 8,
  8, 8, 8,
  4, 4, 4,
  
};

int left = 0;
int right = 0;
int ok = 0;

String songDisp[] = {"JINGLE BELLS","SANTA CLAUS","MERRY CHRISTMAS"}; 

void setup(void) {
  pinMode(9, OUTPUT); // Buzzer
  pinMode(10, OUTPUT); // Led indicator when singing a note
  pinMode(11, OUTPUT); //display sleep
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  selected = 1;
  Serial.begin(19200);

  StartTime = millis();
  

  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  digitalWrite(11, HIGH);
  lcd.setCursor(0,0); //First line
  lcd.print("**SELECT SONG**");
  lcd.setCursor(0,1); //Second line
  lcd.print(" <     >     OK ");
  
}

void loop() {
  
  CurrentTime = millis();
  
  unsigned long ElapsedTime = CurrentTime - StartTime;

  if (ElapsedTime > 10000){
    sleep = true;
    digitalWrite(11, LOW);
    
    }
  Serial.println(ElapsedTime,DEC);
      
  digitalWrite(10, HIGH); 
  left = digitalRead(2);
  right = digitalRead(3);
  ok = digitalRead(4);  

   //buzz(melodyPin, 2500, 1000);
  
  if (left == HIGH) {
    if (sleep) digitalWrite(11, HIGH);
    StartTime = millis();
    selected --;    

    if (selected == -1) selected = 2;
    lcd.setCursor(0,0); 
    lcd.print("                ");
    lcd.setCursor(0,0); 
    lcd.print(songDisp[selected]);
    
    Serial.println(selected,DEC);
    Serial.println("PRESSED LEFT");
    Serial.println(songDisp[(selected)]);
    
    delay(500);
        
  } else if (right == HIGH) {
    if (sleep) digitalWrite(11, HIGH);
    StartTime = millis();
    selected ++;    

    if (selected == 3) selected = 0;
    lcd.setCursor(0,0); 
    lcd.print("                ");
    lcd.setCursor(0,0); 
    lcd.print(songDisp[selected]);
    
    Serial.println(selected,DEC);
    Serial.println("PRESSED RIGHT");
    Serial.println(songDisp[(selected)]);
    
    delay(500);

  } else if (ok == HIGH) {
    StartTime = millis();
    if (sleep) digitalWrite(11, HIGH);
    digitalWrite(10, LOW); 

    Serial.println(selected,DEC);
    Serial.println(songDisp[(selected)]);
    sing((selected+1));
  }
}

int song = 0;

void sing(int s) {
  // iterate over the notes of the melody:
  song = s;
  if (song == 3) {
    Serial.println(" 'We wish you a Merry Christmas'");
    int size = sizeof(wish_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / wish_tempo[thisNote];

      buzz(melodyPin, wish_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  } else if (song == 2) {
    Serial.println(" 'Santa Claus is coming to town'");
    int size = sizeof(santa_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 900 / santa_tempo[thisNote];

      buzz(melodyPin, santa_melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  } 
  else if (song == 4) {
    Serial.println(" 'Santa Claus is coming to town'");
    int size = sizeof(dominick_the_donkey) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 900 / dominick_the_donkey_tempo[thisNote];

      buzz(melodyPin, dominick_the_donkey[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

      }
    
    }
    
    else {

    Serial.println(" 'Jingle Bells'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {

      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // stop the tone playing:
      buzz(melodyPin, 0, noteDuration);

    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(10, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(10, LOW);

}

