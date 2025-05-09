#ifndef PACKET // Evito que el archivo se incluya doblemente en la compilacion
#define PACKET

#include <string.h>  // Para strings
#include <omnetpp.h> // Para cSimplleModule, cMessage, ...

#include "Packet.h"

typedef long long ll;

// Constructor
FeedbackPkt::FeedbackPkt() {}


// Destructor
FeedbackPkt::~FeedbackPkt() {
}
void FeedbackPkt::initialize(){
    this -> setKind(Undefined);
    this -> remainingBuffer = -1;
    this -> ack             =  false;
    this -> byteLength      =  0;
    this -> setName("FeedbackPkt");
    this -> Lose_Packet     =  -1;
    this-> delay          =  0;
}


 void FeedbackPkt::finish(){
 }

// void FeedbackPkt::setRemainingBuffer(int n){
//     this -> remainingBuffer = n;
// }

// int FeedbackPkt::getRemainingBuffer(){
//    return this -> remainingBuffer;
// }

// void FeedbackPkt::setByteLength(ll s){
//     this -> byteLength = s;
// }

DataPkt::DataPkt() {}


// Destructor
DataPkt::~DataPkt() {
}

void DataPkt::initialize(){
    this -> setKind(Undefined);
    this -> remainingBuffer = -1;
    this -> ack             =  false;
    this -> byteLength      =  0;
    this -> setName("FeedbackPkt");
}

 void DataPkt::finish(){
 }

// void DataPkt::setRemainingBuffer(int n){
//     this -> remainingBuffer = n;
// }

// int DataPkt::getRemainingBuffer(){
//    return this -> remainingBuffer;
// }

// void DataPkt::setByteLength(ll s){
//     this -> byteLength = s;
// }








#endif /* PACKET */
