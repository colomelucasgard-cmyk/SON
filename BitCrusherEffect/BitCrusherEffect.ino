#include <Audio.h>
#include "BitCrusherEffectv2.h" // Ton effet Faust
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputUSB      usb1;           

BitCrusherEffectv2 mycrusher;      // Ton effet
AudioOutputI2S     out;            // Sortie Casque (Shield)
AudioControlSGTL5000 audioShield;


AudioConnection patchCord1(usb1, 0, mycrusher, 0);

AudioConnection patchCord2(mycrusher, 0, out, 0);
AudioConnection patchCord3(mycrusher, 0, out, 1);

const int PIN_BOUTON = 3;    
int etatBoutonPrecedent = HIGH; 
bool filtreActif = false;

void setup() {
  // Allouer assez de mémoire pour le tampon USB
  Serial.begin(9600);
  AudioMemory(20); 
  
  audioShield.enable();
  audioShield.volume(0.5); 
  pinMode(PIN_BOUTON, INPUT);
  
  
 
}

void loop() {
  // --- GESTION DES POTENTIOMÈTRES (Inchangé) ---
  
  int lecturepot1 = analogRead(A0);
  float bitsvalue = (float)map(lecturepot1, 0, 1023, 16, 2); 
  
  int lecturepot2 = analogRead(A2); 
  float Dsvalue = (float)map(lecturepot2, 0, 1023, 1, 50);

  int lectureBouton = digitalRead(PIN_BOUTON);
  //Serial.println(lectureBouton); 

  mycrusher.setParamValue("Bits", bitsvalue);       
  mycrusher.setParamValue("Downsample", Dsvalue);

  if (lectureBouton == LOW && etatBoutonPrecedent == HIGH) {
    // 1. On inverse l'état du filtre
    filtreActif = !filtreActif; 
    
    // 2. On envoie la commande à Faust (0 ou 1)
    if (filtreActif) {
      mycrusher.setParamValue("AntiAlias", 1.0);
      Serial.println("Filtre: ON"); // Utile pour vérifier
    } else {
      mycrusher.setParamValue("AntiAlias", 0.0);
      Serial.println("Filtre: OFF");
    }
  delay(50);
  }
  
  etatBoutonPrecedent = lectureBouton;
  
  
  
  delay(20);
}
