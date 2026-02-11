#include <Audio.h>
#include "testv.h" // Ton effet Faust
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputUSB      usb1;           

testv mycrusher;      // Ton effet
AudioOutputI2S     out;            // Sortie Casque (Shield)
AudioControlSGTL5000 audioShield;


AudioConnection patchCord1(usb1, 0, mycrusher, 0);

AudioConnection patchCord2(mycrusher, 0, out, 0);
AudioConnection patchCord3(mycrusher, 0, out, 1);

const int PIN_BOUTON = 3;    
int etatBoutonPrecedent = HIGH; 
bool vinylactif = false;

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

  int lecturepot3 = analogRead(A8);
  float vinylmode = (float)map(lecturepot3, 0, 1023, 1, 0); 
  
  int lecturepot2 = analogRead(A2); 
  float Dsvalue = (float)map(lecturepot2, 0, 1023, 1, 50);

  int lectureBouton = digitalRead(PIN_BOUTON);
  //Serial.println(lectureBouton); 

  mycrusher.setParamValue("Bits", bitsvalue);       
  mycrusher.setParamValue("Downsample", Dsvalue);
  mycrusher.setParamValue("Vinyl_Wear", vinylmode);   

  if (lectureBouton == LOW && etatBoutonPrecedent == HIGH) {
    // 1. On inverse l'état du filtre
    vinylactif = !vinylactif; 
    
    // 2. On envoie la commande à Faust (0 ou 1)
    if (vinylactif) {
      mycrusher.setParamValue("Vinyl_Mode", 1.0);
      Serial.println("Filtre: ON"); // Utile pour vérifier
    } else {
      mycrusher.setParamValue("Vinyl_Mode", 0.0);
      Serial.println("Filtre: OFF");
    }
  delay(50);
  }
  
  etatBoutonPrecedent = lectureBouton;
  
  
  
  delay(20);
}
