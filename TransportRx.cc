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
        TransportRx();
        virtual ~TransportRx();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx() {
    endServiceEvent = NULL;
}

TransportRx::~TransportRx() {
    cancelAndDelete(endServiceEvent);
}

void TransportRx::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
}

void TransportRx::finish() {
}

void TransportRx::handleMessage(cMessage *msg) {
//send feedback
   FeedbackPkt *feedbackPkt = new FeedbackPkt();
   feedbackPkt->setByteLength(20);
   feedbackPkt->setKind(2);
   feedbackPkt->setRemainingBuffer(par("bufferSize").intValue() - buffer.getLength());
   send(feedbackPkt, "toOut$o");

   if(msg->getKind()==Data){
       send(msg,"toApp");
   }

}

#endif
