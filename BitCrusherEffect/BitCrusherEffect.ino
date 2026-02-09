#include <Audio.h>
#include "BitCrusherEffect.h"

BitCrusherEffect mycrusher;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(mycrusher,0,out,0);
AudioConnection patchCord1(mycrusher,0,out,1);

void setup() {
  AudioMemory(2);
  audioShield.enable();
  audioShield.volume(0.5);
  
}

void loop() {
  int lecturepot1 = analogRead(A0);
  float bitsvalue = map(lecturepot1, 0, 1023, 16,4);
  int lecturepot2 = analogRead(A2);
  float Dsvalue = map(lecturepot2, 0, 1023, 1, 50);
  mycrusher.setParamValue("Downsample",bitsvalue);
  mycrusher.setParamValue("Bits",Dsvalue);
  delay(100);
}
