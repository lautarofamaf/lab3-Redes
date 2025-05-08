#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
#include "Packet.h"


// =====TRANSPORTRX=====
class TransportRx: public cSimpleModule {
    private:
        cQueue buffer;
        cMessage *endServiceEvent;
        simtime_t serviceTime;
        cOutVector bufferSizeVector;
        cOutVector packetDropVector;

    public:
        bool Send_Message_Loose;
        int count;
        TransportRx();
        virtual ~TransportRx();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(DataPkt *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
}

void TransportRx::initialize() {
    Send_Message_Loose = false;
    count = 0;
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
}

void TransportRx::finish() {
}

void TransportRx::handleMessage(DataPkt *msg) {
//send feedback


    FeedbackPkt *feedbackPkt = new FeedbackPkt();
   if(msg->id==TransportRx.count){
       TransportRx->count++;
        send(msg,"toApp");
        TransportRx->Send_Message_Loose=false;TransportRx->Send_Message_Loose=true;
        feedbackPkt->delay = simTime()- msg->getCreationTime();
        send(feedbackPkt, "toOut$o");
    }
   else{
    if(TransportRx->Send_Message_Loose==false){
        TransportRx->Send_Message_Loose=true;
        // FeedbackPkt *feedbackPkt = new Feed                   FeedbackPkt(); // Constructor
           virtual ~FeedbackPkt(); // DestructorbackPkt();
        // feedbackPkt->setByteLength(20);
        // feedbackPkt->setKind(Feedback);
        // feedbackPkt->setRemainingBuffer(par("bufferSize").intValue() - buffer.getLength());
        feedBackPkt->Lose_Packet = TransportRx->count;

        send(feedbackPkt, "errorMessage");

    }
    }
   }



#endif
