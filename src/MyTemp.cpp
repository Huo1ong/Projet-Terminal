/*
 * MyTemp.cpp
 *
 *  Créé le : 02-05-2022
 *  Auteur : Quentin
 */
#include "MyTemp.h"

void MyTemp::init(int _inputPinUsed, int _typeSensorUsed){
    inputPinUsed = _inputPinUsed;
    typeSensorUsed = _typeSensorUsed;

    dht = new DHT(inputPinUsed, typeSensorUsed);
    dht->begin();
    }

bool MyTemp::setUniteUsed(int _UniteUsed){
   if((_UniteUsed>=0) && (_UniteUsed<=1)){
        UniteUsed = _UniteUsed;
        return(true);
        }
    return(false);
    }

//Return -999 if error
float MyTemp::getTemperature(){
    float t = dht->readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(t)) {
        t = lastTemperatureRead;
        }
    lastTemperatureRead = t;
    return(t);
    }