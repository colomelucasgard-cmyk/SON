#include <Audio.h>
#include "test.h" // Ton effet Faust
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioInputUSB      usb1;           

BitCrusherEffectv7 mycrusher;      // Ton effet
AudioOutputI2S     out;            // Sortie Casque (Shield)
AudioControlSGTL5000 audioShield;


AudioConnection patchCord1(usb1, 0, mycrusher, 0);

AudioConnection patchCord2(mycrusher, 0, out, 0);
AudioConnection patchCord3(mycrusher, 0, out, 1);

const int PIN_BOUTON = 3;    
int etatBoutonPrecedent = HIGH; 
bool vinylactif = false;

const int PIN_BOUTON0 = 0;    
int etatBoutonPrecedent0 = HIGH; 
bool B0actif = false;

const int PIN_BOUTON1 = 1;    
int etatBoutonPrecedent1 = HIGH; 
bool B1actif = false;

const int PIN_BOUTON2 = 2;    
int etatBoutonPrecedent2 = HIGH; 
bool B2actif = false;
void setup() {
  // Allouer assez de mémoire pour le tampon USB
  Serial.begin(9600);
  AudioMemory(80); 
  
  audioShield.enable();
  audioShield.volume(0.5); 
  pinMode(PIN_BOUTON, INPUT);
  pinMode(PIN_BOUTON0, INPUT_PULLUP);
  pinMode(PIN_BOUTON1, INPUT_PULLUP);
  pinMode(PIN_BOUTON2, INPUT_PULLUP);
  
  
 
}

void loop() {
  // --- GESTION DES POTENTIOMÈTRES (Inchangé) ---
  
  int lecturepot1 = analogRead(A0);
  float bitsvalue = (float)map(lecturepot1, 0, 1023, 16, 2); 

  int lecturepot3 = analogRead(A8);
  float freeze = (float)map(lecturepot3, 0, 1023, 0.5, 0.001); 
  
  int lecturepot2 = analogRead(A2); 
  float Dsvalue = (float)map(lecturepot2, 0, 1023, 1, 50);


  int lectureBouton = digitalRead(PIN_BOUTON);
  int lectureBouton0 = digitalRead(PIN_BOUTON0);
  int lectureBouton1 = digitalRead(PIN_BOUTON1);
  int lectureBouton2 = digitalRead(PIN_BOUTON2);
  //Serial.println(lectureBouton); 

  mycrusher.setParamValue("Bits", bitsvalue);       
  mycrusher.setParamValue("Downsample", Dsvalue);
  mycrusher.setParamValue("Freeze_Len", freeze);
  mycrusher.setParamValue("Vinyl_Wear", 1.0);



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
  
  if (lectureBouton0 == LOW && etatBoutonPrecedent0 == HIGH) {
    // 1. On inverse l'état du filtre
    B0actif = !B0actif; 
    
    // 2. On envoie la commande à Faust (0 ou 1)
    if (B0actif) {
      mycrusher.setParamValue("Flanger_Mode", 1.0);
      Serial.println("B0: ON"); // Utile pour vérifier
    } else {
      mycrusher.setParamValue("Flanger_Mode", 0.0);
      Serial.println("B0: OFF");
    }
  delay(50);
  }
  if (lectureBouton1 == LOW && etatBoutonPrecedent1 == HIGH) {
    // 1. On inverse l'état du filtre
    B1actif = !B1actif; 
    
    // 2. On envoie la commande à Faust (0 ou 1)
    if (B1actif) {
      mycrusher.setParamValue("Freeze_Trig", 1.0);
      Serial.println("B1: ON"); // Utile pour vérifier
    } else {
      mycrusher.setParamValue("Freeze_Trig", 0.0);
      Serial.println("B1: OFF");
    }
  delay(50);
  }

   /*if (lectureBouton2 == LOW && etatBoutonPrecedent2 == HIGH) {
    // 1. On inverse l'état du filtre
    B2actif = !B2actif; 
    
    // 2. On envoie la commande à Faust (0 ou 1)
    if (B2actif) {
      mycrusher.setParamValue("B2", 1.0);
      Serial.println("B2: ON"); // Utile pour vérifier
    } else {
      mycrusher.setParamValue("B2", 0.0);
      Serial.println("B2: OFF");
    }
  delay(50);
  }*/
  
  etatBoutonPrecedent = lectureBouton;
  etatBoutonPrecedent0 = lectureBouton0;
  etatBoutonPrecedent1 = lectureBouton1;  
  etatBoutonPrecedent2 = lectureBouton2;
  
  delay(20);
}
