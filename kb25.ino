/*
Project: Key Scan Sample.for Roland K-25m
Author : Takao Watase

Copyright 2015 Takao Watase

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// #define _SERIAL_DEBUG_

// Pin Assignment
const int DSC0 = 2;   // Data lines for the secondely switches
const int DSC1 = 3;
const int DSC2 = 4;
const int DSC3 = 5;
const int DPR0 = 6;    // Data lines for the primary switches
const int DPR1 = 7;
const int DPR2 = 8;
const int DPR3 = 9;
const int SL0 = 10;    // Scan lines for switch matrix
const int SL1 = 11;
const int SL2 = 12;
const int SL3 = 13;
const int SL4 = 14; // A0
const int SL5 = 15; // A1
const int SL6 = 16; // A2
const int SL7 = 17; // A3

#define NM_KEY 25        // number of key
#define KEY_OFF 0        // Key status = OFF (released)
#define KEY_ON 1         // Key status = ON (pressed)
#define OFFSET_KEY 48   // key #0 : C3
#define DEFAULT_VEL 100 // velocity is fixed

// key status map
class KeyMap
{
  private:
  byte mapKey[NM_KEY];    // status of each key.

  public:
  KeyMap() { Init(); }
  void Init() {
   for ( int i = 0; i < NM_KEY; i++) { mapKey[i] = 1; }
  }
  bool Check(int key, int state) { return mapKey[key] != (byte)state; }
  // return true if key status is differnt with map.
  void Set(int key, int state) { mapKey[key] = (byte)state; }
};


int midiKey;    // 
int arrSl[8];   // Stores pin number of scan lines
KeyMap theMap;  // Key map

void setup() {
  // Use secondely switches for input
  pinMode(DSC0, INPUT_PULLUP);
  pinMode(DSC1, INPUT_PULLUP);
  pinMode(DSC2, INPUT_PULLUP);
  pinMode(DSC3, INPUT_PULLUP);

  // Set scan lines are output
  pinMode(SL0, OUTPUT);
  pinMode(SL1, OUTPUT);
  pinMode(SL2, OUTPUT);
  pinMode(SL3, OUTPUT);
  pinMode(SL4, OUTPUT);
  pinMode(SL5, OUTPUT);
  pinMode(SL6, OUTPUT);
  pinMode(SL7, OUTPUT);

  // Set scan lines as high.
  digitalWrite(SL0, HIGH);
  digitalWrite(SL1, HIGH);
  digitalWrite(SL2, HIGH);
  digitalWrite(SL3, HIGH);
  digitalWrite(SL4, HIGH);
  digitalWrite(SL5, HIGH);
  digitalWrite(SL6, HIGH);
  digitalWrite(SL7, HIGH);

  // Store pin number of scan lines
  arrSl[0] = SL0;
  arrSl[1] = SL1;
  arrSl[2] = SL2;
  arrSl[3] = SL3;
  arrSl[4] = SL4;
  arrSl[5] = SL5;
  arrSl[6] = SL6;
  arrSl[7] = SL7;
  
  theMap.Init();
  
#ifdef _SERIAL_DEBUG_
  Serial.begin(115200);
#else
  // Initialize and start MIDI recieving
  MIDI.begin();
#endif

}

void loop() {
  // iterate scan lines and read status of sitches
  for (int line = 0; line < 8; line++) {
    digitalWrite(arrSl[line], LOW);      // set scan line to low to read status
    
    int s0 = digitalRead(DSC0);        // read status of switches
    int s1 = digitalRead(DSC1);        //   0:on (pressed). 1:off (released)
    int s2 = digitalRead(DSC2);
    int s3 = digitalRead(DSC3);

    digitalWrite(arrSl[line], HIGH);    // back the scan line to high

    // give the key number and process the event
    chkKey(line+0, s0);    
    chkKey(line+8, s1);
    chkKey(line+16, s2);
    chkKey(line+24, s3);
  }
}

bool chkKey(int key, int val)
{
  // Check and update the status of the keymap.
  // Output midi message. 
  // key = key number (0-24)
  // val = key status 0:on, 1, off 

  if (key >= NM_KEY) { return false; }    // check key range

  if (theMap.Check(key, val)) {           // detect status change

    #ifdef _SERIAL_DEBUG_
    Serial.print("detect change : ");
    Serial.print(key);
    Serial.print(", ");
    Serial.print(val);
    Serial.println();
    #endif
    
    if (val) { // 1... key has released 
      SendOff(key);
    } else {    // 0... key has pressed
      SendOn(key);
    }
    theMap.Set(key, val);  // update key map
    return true;
  }
  return false;
}

void SendOff(int key) 
{
    // send note off message to midi
#ifdef _SERIAL_DEBUG_
  Serial.print("OFF: 90 ");
  Serial.print(key);
  Serial.println(" 0");
#else
  MIDI.sendNoteOn(key + OFFSET_KEY, 0, 1);  // key number, velocity = 0, channel = 1
#endif
}

void SendOn(int key) 
{
  // send note on message to MIDI.
#ifdef _SERIAL_DEBUG_
  Serial.print("ON : 90 ");
  Serial.print(key);
  Serial.println(" 100");
#else
  MIDI.sendNoteOn(key + OFFSET_KEY, DEFAULT_VEL, 1);  // key number, velocity = 100, channel = 1
#endif
}


// end of file

