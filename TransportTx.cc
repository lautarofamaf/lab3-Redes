#ifndef TRANSPORTTX
#define TRANSPORTTX

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
#include "Packet.h"

class TransportTx: public cSimpleModule {
    private:
        cQueue buffer;
        cMessage *endServiceEvent;
        simtime_t serviceTime;
        cOutVector bufferSizeVector;
        cOutVector packetDropVector;


    public:
        float last_delay = 50;
        bool is_bussy = false;
        flaot delay = 0;
        int count=0;
        TransportTx();
        virtual ~TransportTx();
    protected:
        virtual void initialize();
        virtual void finish();
        virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx() {
    endServiceEvent = NULL;
}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
}

void TransportTx::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");

}

void TransportTx::finish() {
}

void TransportTx::handleMessage(cMesgetKindsage *msg) {

    if(msg->getKind()==Undefined){
         //     msg->setKind(Data);
        DataPkt *dataPkt = (DataPkt*) msg;
        msg->id = TransportTx->count;
        TransportTx->count++;
    }
    else if(msg->getKind()==Feedback){
       if(FeedbackPkt->Lose_Packet==-1){
        if(FeedbackPkt->delay>TransportTx->last_delay+50){
            TransportTx->last_delay=FeedbackPkt->delay;
            TransportTx->is_bussy=true;
            delete msg;
            return;
        }
        else {
            TransportTx->last_delay=FeedbackPkt->delay;
            TransportTx->is_bussy=false;
            delete msg;
            return;
        }

        else{
            TransportTx->is_bussy=true;
            delete msg;
            return;

        }
        delete msg;
        return;

       }
       else{
        TransportTx->count=FeedbackPkt->Lose_Packet;
        TransportTx->is_bussy=true;
        delete msg;
        return;

       }
        // // si me dicen que el buffer ta lleno, me rescato
        // // si no, mantengo velocidad
        // FeedbackPkt *feedbackPkt = (FeedbackPkt*) msg;
        // // hacer algo con la info de feedback pkt
        // int remainingBuffer = feedbackPkt->getRemainingBuffer();
        // // code
 // un poco polemico pero funciona
    }
    else if(msg->getKind()==Data){
        //
    }



    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {

            cPacket *pkt = (cPacket*) buffer.pop();

            send(pkt, "toOut$o");

            serviceTime = pkt->getDuration();                     // <- serviceTime: En cuanto tiempo se mandara
                                                                  //                 el paquete por el medio
                                                                  // Omnet ya sabe que el paquete tardara
                                                                  // getDuration() = Tamano / Datarate_canal
                                                                  // en viajar por el medio
            scheduleAt(simTime() + serviceTime, endServiceEvent); // <- scheduleAt(<pkt_llegada_momento>, endServiceEvent):

        }

    } else { // if msg is a data packet
        if(buffer.getLength() >= par("bufferSize").intValue()){
            //drop the packet
            delete msg;
            this->bubble("packet dropped");
            packetDropVector.record(1);

        }else{
            // enqueue the packet
            buffer.insert(msg);
            bufferSizeVector.record(buffer.getLength());

            // if the server is idle
            if (!endServiceEvent->isScheduled()) {
                // start the service
                if(is_bussy){
                    TransportTx->delay += 10;
                }
                else{
                    if(transportTx->delay>10){
                        TransportTx->delay -=10 ;
                    }
                    else{
                        TransportTx->delay = 0;
                    }
                    }
                scheduleAt(simTime() + TransportTx->delay, endServiceEvent);
            }
        }
    }
}
#endif
