#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Queue: public cSimpleModule {
private:
    cQueue buffer;
    cMessage *endServiceEvent;
    simtime_t serviceTime;
    cOutVector bufferSizeVector;
    cOutVector packetDropVector;
    cOutVector packetEnqueuedVector;

public:
    Queue();
    virtual ~Queue();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue() {
    endServiceEvent = NULL;
}

Queue::~Queue() {
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize() {
    buffer.setName("buffer");
    endServiceEvent = new cMessage("endService");
    bufferSizeVector.setName("BufferSize");
    packetDropVector.setName("PacketDropCount");
    packetEnqueuedVector.setName("PacketEnqueuedCount");

}

void Queue::finish() {
}

void Queue::handleMessage(cMessage *msg) {
    // if msg is signaling an endServiceEvent
    if (msg == endServiceEvent) {
        // if packet in buffer, send next one
        if (!buffer.isEmpty()) {
            // dequeue packet
            // -> [p1,p2,p3] ->
            cPacket *pkt = (cPacket*) buffer.pop();
            // -> [p1,p2,  ] ->
            //           p3 <- pkt
            // send packet
            send(pkt, "out");
            // -> [p1,p2, ] -> p3
            //                 ^-- pkt.getDuration

            // triggereo el evento endServiceEvent para auto-avisarme que el paquete popeado ha sido
            // entregado (finalizo su viaje por el medio), cuando llega sigo enviando paquetes
            serviceTime = pkt->getDuration();                     // <- serviceTime: En cuanto tiempo se mandara
                                                                  //                 el paquete por el medio
                                                                  // Omnet ya sabe que el paquete tardara
                                                                  // getDuration() = Tamano / Datarate_canal
                                                                  // en viajar por el medio
            scheduleAt(simTime() + serviceTime, endServiceEvent); // <- scheduleAt(<pkt_llegada_momento>, endServiceEvent):
                                                                  // Agendo el evento al momento en el que llegara el pkt
                                                                  // pkt_llegada_momento = simTime() + serviceTime
            //                            _______
            //                           |       | <-- Es un canal out con un datarate D
            // Tx -> p3 -> Queue: [p1p2] ->    ->
            //                               ^-- pkt
            //                               pkt.getDuration = D/pkt.Size
        }
        /*
        pkt sale de Tx y comienza su viaje por el medio
                               (*)          (**)
        Tx -> <     > -> Queue ->            -> Rx
                                ^-- T (marco el tiempo actual T == simTime())
            (*):
                - Apenas sale hago send(out,pkt)
                - D = pkt.getDuration = pkt.Size / out.Datarate
                - Scheduleo el endServiceEvent en T + D = (**)
        */
    } else { // if msg is a data packet
        if(buffer.getLength() >= par("bufferSize").intValue()){
            //drop the packet
            delete msg;
            this->bubble("packet dropped");
            packetDropVector.record(packetDropVector.getValuesStored() + 1);  // sumo uno a los valores recolectados por el vector.
        }else{
            // enqueue the packet
            buffer.insert(msg);
            bufferSizeVector.record(buffer.getLength());
            packetEnqueuedVector.record(packetEnqueuedVector.getValuesStored() + 1); // sumo uno a los valores recolectados por el vector.
            // if the server is idle
            if (!endServiceEvent->isScheduled()) {
                // start the service
                scheduleAt(simTime() + 0, endServiceEvent);
            }
        }
    }
}

#endif /* QUEUE */
