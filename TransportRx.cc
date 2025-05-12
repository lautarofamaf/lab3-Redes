#ifndef TRANSPORT_RX
#define TRANSPORT_RX

#include "Packet.h"
#include <omnetpp.h>

using namespace omnetpp;

class TransportRx : public cSimpleModule {
private:
    unsigned int expectedSeqNum;      // Número de secuencia esperado (contador sincronizado)
    unsigned int lastFeedbackSeqNum;  // Último número de secuencia para el que se envió un FeedbackPkt
    cQueue buffer;               // Cola de paquetes en espera para ser transmitidos
    cMessage* endTransmissionEvent;   // Evento para manejar el final de la transmisión
    unsigned int bufferSize;          // Capacidad máxima del buffer (opcional para monitoreo)
    cOutVector packetDropVector;
    cOutVector bufferSizeVector;

public:
    TransportRx();
    ~TransportRx() override;

protected:
    void initialize() override;
    void handleMessage(cMessage* msg) override;

private:
    void handleDataPacket(DataPkt* pkt);      // Maneja paquetes entrantes de tipo DataPkt
    void sendFeedbackPacket(unsigned int seqNum, double bufferOccupancyRatio = -1.0, bool isCongestion = false);
};

Define_Module(TransportRx);

TransportRx::TransportRx() : expectedSeqNum(0), lastFeedbackSeqNum(UINT_MAX), endTransmissionEvent(nullptr), bufferSize(10) {}

TransportRx::~TransportRx() {
    cancelAndDelete(endTransmissionEvent);
    while (!buffer.isEmpty()) {
        delete buffer.pop();
    }
}

void TransportRx::initialize() {
    buffer.setName("buffer");
    expectedSeqNum = 0; // Inicializa el número esperado en 0
    lastFeedbackSeqNum = UINT_MAX; // Inicializa el seguimiento de feedback
    endTransmissionEvent = new cMessage("endTransmission");
    bufferSize = par("bufferSize"); // Capacidad máxima del buffer
    packetDropVector.setName("packetDrops");
    bufferSizeVector.setName("bufferUsage");
}

void TransportRx::handleMessage(cMessage* msg) {
    if (msg == endTransmissionEvent) {
        // Intentar enviar el siguiente paquete en la cola
        if (!buffer.isEmpty()) {
            auto* pkt = check_and_cast<cPacket*>(buffer.pop());
            send(pkt, "toApp");
        }

        // Si todavía hay paquetes en la cola, programa el próximo envío
        if (!buffer.isEmpty()) {
            simtime_t nextAvailableTime = gate("toApp")->getTransmissionChannel()->getTransmissionFinishTime();
            scheduleAt(nextAvailableTime, endTransmissionEvent);
        }
    } else if (auto* dataPkt = dynamic_cast<DataPkt*>(msg)) {
        handleDataPacket(dataPkt);
    } else {
        EV_ERROR << "[TransportRx] Mensaje desconocido, descartando.\n";
        delete msg;
    }
}

void TransportRx::handleDataPacket(DataPkt* pkt) {
    unsigned int seqNum = pkt->getId(); // Obtener el número de secuencia del paquete

    EV_INFO << "[TransportRx] Paquete recibido con seqNum: " << seqNum << "\n";

    if (seqNum == expectedSeqNum) {
        // Paquete esperado
        EV_INFO << "[TransportRx] Paquete recibido correctamente: " << seqNum << "\n";
        expectedSeqNum++;

        // Simula el envío del paquete a la aplicación
        if (gate("toApp")->getTransmissionChannel()->getTransmissionFinishTime() <= simTime()) {
            // Canal libre, enviar inmediatamente
            send(pkt, "toApp");
        } else {
            // Canal ocupado, verificar si la cola está llena
            if (buffer.getLength() >= bufferSize) {
                EV_WARN << "[TransportRx] Buffer lleno, descartando paquete con seqNum: " << seqNum << "\n";
                delete pkt; // Dropear el paquete
                this->bubble("packet dropped");
                packetDropVector.record(1);

            } else {
                // Agregar el paquete a la cola
                buffer.insert(pkt);
                bufferSizeVector.record(buffer.getLength());
                // Si no hay un evento programado, programa uno
                if (!endTransmissionEvent->isScheduled()) {
                    simtime_t nextAvailableTime = gate("toApp")->getTransmissionChannel()->getTransmissionFinishTime();
                    scheduleAt(nextAvailableTime, endTransmissionEvent);
                }
            }
        }

        // Envía feedback si el buffer está casi lleno
        if (buffer.getLength() >= bufferSize * 0.8) { // Por ejemplo, si el buffer está 80% lleno
            double bufferOccupancyRatio = (double)buffer.getLength() / bufferSize;
            sendFeedbackPacket(expectedSeqNum, bufferOccupancyRatio, true); // true indica congestión
        }
    } else if (seqNum > expectedSeqNum) {
        // Paquete fuera de orden, se ha perdido al menos un paquete
        EV_WARN << "[TransportRx] Paquete fuera de orden. Esperado: " << expectedSeqNum
                << ", recibido: " << seqNum << "\n";

        // Verificar si ya se envió un FeedbackPkt para este número de secuencia
        if (lastFeedbackSeqNum != expectedSeqNum) {
            sendFeedbackPacket(expectedSeqNum, -1.0, false); // false indica pérdida de paquetes
            lastFeedbackSeqNum = expectedSeqNum; // Actualizar el último número de feedback enviado
        }

        delete pkt; // Eliminar el paquete porque no es el esperado
    } else {
        // Paquete duplicado o ya procesado
        EV_WARN << "[TransportRx] Paquete duplicado o ya procesado, descartando: " << seqNum << "\n";
        delete pkt;
    }
}


void TransportRx::sendFeedbackPacket(unsigned int seqNum, double bufferOccupancyRatio, bool isCongestion) {
    auto* feedbackPkt = new FeedbackPkt();
    feedbackPkt->setId(seqNum); // Informa al transmisor el número de paquete que se espera
    feedbackPkt->setLose_Packet(seqNum); // Utilizar Lose_Packet para indicar el paquete perdido
    feedbackPkt->setIsCongestion(isCongestion); // Flag para distinguir congestión
    feedbackPkt->setName(("Feedback for " + std::to_string(seqNum)).c_str());

    // Si se proporciona el estado del buffer, incluirlo en el feedback
    if (bufferOccupancyRatio >= 0.0) {
        EV_INFO << "[TransportRx] Feedback enviado: retransmitir desde " << seqNum
                << " con bufferOccupancy=" << bufferOccupancyRatio
                << " (isCongestion=" << isCongestion << ")\n";
    } else {
        EV_INFO << "[TransportRx] Feedback enviado: retransmitir desde " << seqNum
                << " (isCongestion=" << isCongestion << ")\n";
    }

    send(feedbackPkt, "toOut$o");
}
#endif /* TRANSPORT_RX */
