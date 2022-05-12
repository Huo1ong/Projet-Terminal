/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe MyStone et MySerial

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran
 * */

#include <iostream>
#include <string>

#include <Arduino.h>
#include <ArduinoJson.h>

//BOUTON START
int BoutonStart; //évènement bouton
int EtatSysteme = 0; //0 = arrêt, 1 = démarrage, 2 = pause ...

//COMPTE A REBOURS
int counter = 0;

// TEMPÉRATURE
#include "MyTemp.h"
#define DHTPIN 21
#define DHTTYPE DHT22
float temperature =0;
MyTemp *myTemp = NULL;

#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>

#include "MyButton.h"
MyButton *myButtonT4 = new MyButton();
MyButton *myButtonT5 = new MyButton();

#include "MyStone.h"
MyStone *myStone;

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};



//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";

  switch(rd.id){

      case 0x1001: { //Bouton
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneName = rd.name;
          std::cout << "Bouton : " <<  stoneName.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));
          if(strcmp(stoneName.c_str(), "button") == 0){
            BoutonStart = 1;
          }
          break;
          }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}


void setup() {
  
  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);


  myButtonT4->init(T4);
  myButtonT4->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myButtonT5->init(T5);
  myButtonT5->autoSensibilisation(); //Trouve la sensibilité automatiquement

  cout << std::string("Début de l'exemple Stone de base pour le ESP32")  << "\n";

  myTemp=new MyTemp();
    myTemp->init(DHTPIN, DHTTYPE);
    Serial.println("ça marche !");

     //Affichage des données
     //Partie des caractéristiques
          myStone->setLabel("bois", "Érable");
delay(100);
          myStone->setLabel("type", "Dur");
delay(100);
          myStone->setLabel("origine", "US");
delay(100);         
          myStone->setLabel("tempsechage", "20 secondes");
delay(100);        
          myStone->setLabel("tempmin", "25 Celcius");              
delay(100);
  //Partie des informations
          myStone->setLabel("bois2", "Érable");
delay(100);
          myStone->setLabel("tempmin2", "(min : 25 Celcius)");

BoutonStart = 0;
EtatSysteme = 0;

}

void loop() { 
  char buffer[10];
  readStoneData();

  //Boucle pour le compte à rebours
  if(BoutonStart == 1){
    Serial.print("fonctionnement");
    if(temperature >= 25*0.90 && temperature <= 25*1.19){
      while (counter < 20 && (temperature >= 25*0.90 && temperature <= 25*1.19)){
        delay(500);

        Serial.print(20);
        counter++;
        Serial.print(counter);
        char tempsSecondes[20];
        sprintf(tempsSecondes, "%.0f s / %.1f s", counter, 20);
        myStone->setLabel("temps", tempsSecondes);

        sprintf(buffer, "%.1f °C", temperature);
          myStone->setLabel("temperature", buffer);
      }
    }
    BoutonStart = 0;
  }

  int buttonActionT4 = myButtonT4->checkMyButton();
      if(buttonActionT4 > 2)  {  //Si appuyé plus de 0.2 secondes
          Serial.println("Button T4 pressed");

         // temperature = myTemp->getTemperature();
          Serial.print("Temperature : ");
          Serial.println(temperature);
          }

  int buttonActionT5 = myButtonT5->checkMyButton();
      if(buttonActionT5 > 2)  {  //Si appuyé plus de 0.2 secondes
          Serial.println("Button T5 pressed");

          //char cmdFormat2[99];
         // strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
         // std::cout << cmdFormat2 << "\n";
         // myStone->writeIt((char*)cmdFormat2);
        if(myStone) myStone->getVersion();

          
          }
  sprintf(buffer, "%.1f °C", temperature);
          myStone->setLabel("temperature", buffer);

  temperature = myTemp->getTemperature(); //récupération de la température

  if(BoutonStart == 3){
    BoutonStart = 0;
    EtatSysteme = 1;
  }

  if(EtatSysteme == 1){ //si démarrage -> Compte à rebours commence
      counter ++;
  }

  delay(100);
  }