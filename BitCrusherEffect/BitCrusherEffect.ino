#include <Audio.h>
#include "BitCrusherEffect.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioSynthWaveform sine1; 

BitCrusherEffect mycrusher;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;

AudioConnection patchCordInput(sine1, 0, mycrusher, 0);
AudioConnection patchCord0(mycrusher, 0, out, 0);
AudioConnection patchCord1(mycrusher, 0, out, 1);

void setup() {
  AudioMemory(12); 
  audioShield.enable();
  audioShield.volume(0.3);
  
  // SAWTOOTH et une fréquence plus basse (100Hz)
  sine1.begin(0.4, 100, WAVEFORM_SAWTOOTH); 
}

void loop() {
  int lecturepot1 = analogRead(A0);

  float bitsvalue = (float)map(lecturepot1, 0, 1023, 16, 1); 
  
  int lecturepot2 = analogRead(A2); 
  float Dsvalue = (float)map(lecturepot2, 0, 1023, 1, 50);
  
  mycrusher.setParamValue("Bits", bitsvalue);       
  mycrusher.setParamValue("Downsample", Dsvalue);
  
  delay(50);
}
