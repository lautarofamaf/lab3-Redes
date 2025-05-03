#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

typedef long long ll;
// ===== FEEDBACKPKT =====
class FeedbackPkt: public cMessage {
    private:
        int remainingBuffer;
        ll byteLength;
        bool ack;
        ll seqNumber;
        ll ackNumber;
    public:
        FeedbackPkt();
        virtual ~FeedbackPkt();
        virtual void setRemainingBuffer(int r);
        virtual int getRemainingBuffer();
        virtual void setByteLength(ll b);
    protected:
        virtual void initialize();
        virtual void finish();


};
//Constructor
FeedbackPkt::FeedbackPkt() {
}
//Destructor
FeedbackPkt::~FeedbackPkt() {
}
void FeedbackPkt::initialize(){
        this->setKind(0);
        this->remainingBuffer = -1;
        this->ack = false;
        this->ackNumber = -1;
        this->seqNumber = -1;
        this->byteLength= 0;
}

void FeedbackPkt::finish(){
}

void FeedbackPkt::setRemainingBuffer(int r){
    this->remainingBuffer = r;
}
int FeedbackPkt::getRemainingBuffer(){
    return this->remainingBuffer;
}
void FeedbackPkt::setByteLength(ll b){
    this->byteLength = b;
}

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


   send(msg,"toApp");
}

#endif
