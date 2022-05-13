/*
 * MyTemp.cpp
 *
 *  Créé le : 02-05-2022
 *  Auteur : Quentin
 */
#include "MyTemp.h"

//Initialise la température
void MyTemp::init(int _inputPinUsed, int _typeSensorUsed){
    inputPinUsed = _inputPinUsed;
    typeSensorUsed = _typeSensorUsed;

    dht = new DHT(inputPinUsed, typeSensorUsed);
    dht->begin();
    }

//Positionne l'unité de température utilisée
bool MyTemp::setUniteUsed(int _UniteUsed){
   if((_UniteUsed>=0) && (_UniteUsed<=1)){
        UniteUsed = _UniteUsed;
        return(true);
        }
    return(false);
    }

//Récupère la tempérture du capteur thermique
float MyTemp::getTemperature(){
    float t = dht->readTemperature();
    // Vérifiez si des lectures ont échoué et quittez plus tôt (pour réessayer).
    if (isnan(t)) {
        t = lastTemperatureRead;
        }
    lastTemperatureRead = t;
    return(t);
    }