#ifndef TRANSPORT_TX
#define TRANSPORT_TX

#include <omnetpp.h>
#include "Packet.h"
using namespace omnetpp;

class TransportTx : public cSimpleModule {
private:
    cQueue buffer;                // Cola de paquetes
    cMessage* endServiceEvent;    // Evento para manejar envíos
    unsigned int count;           // Contador de secuencia sincronizado
    unsigned int expectedAck;     // Último número de secuencia reconocido
    unsigned int bufferSize;      // Tamaño máximo del buffer
    double feedbackDelay;         // Retraso aplicado al enviar paquetes tras recibir feedback
    double defaultDelay;          // Valor base del retraso
    cOutVector bufferSizeVector;  // Vector para monitorear el tamaño del buffer

public:
    TransportTx();
    ~TransportTx() override;

protected:
    void initialize() override;
    void handleMessage(cMessage* msg) override;

private:
    void handleEndServiceEvent();      // Maneja el evento de envío
    void handleDataPacket(DataPkt* pkt); // Maneja paquetes entrantes de tipo DataPkt
    void handleFeedbackPacket(FeedbackPkt* pkt); // Maneja feedback del receptor

};

Define_Module(TransportTx);

TransportTx::TransportTx() : endServiceEvent(nullptr), count(0), expectedAck(0), bufferSize(0) {}

TransportTx::~TransportTx() {
    cancelAndDelete(endServiceEvent);
    while (!buffer.isEmpty()) {
        delete buffer.pop();
    }
}

void TransportTx::initialize() {
    endServiceEvent = new cMessage("endService");
    bufferSize = par("bufferSize"); // Tamaño máximo del buffer definido

    // Inicializa el retraso
    feedbackDelay = 0.0;
    defaultDelay = 0.0; // Retraso base en segundos

    // Configura vectores para monitorear estadísticas
    bufferSizeVector.setName("bufferSize");
    bufferSizeVector.record(buffer.getLength());
}

void TransportTx::handleMessage(cMessage* msg) {
    if (msg == endServiceEvent) {
        handleEndServiceEvent();
    } else if (auto* dataPkt = dynamic_cast<DataPkt*>(msg)) {
        handleDataPacket(dataPkt);
    } else if (auto* feedbackPkt = dynamic_cast<FeedbackPkt*>(msg)) {
        handleFeedbackPacket(feedbackPkt);
    } else {
        EV_ERROR << "[TransportTx] Mensaje desconocido, descartando.\n";
        delete msg;
    }
}

void TransportTx::handleDataPacket(DataPkt* pkt) {
    if (buffer.getLength() >= bufferSize) {
        EV_WARN << "[TransportTx] Buffer lleno, descartando paquete.\n";
        delete pkt;
        return;
    }

    pkt->setId(count++);
    buffer.insert(pkt);
    bufferSizeVector.record(buffer.getLength());

    EV_INFO << "[TransportTx] Paquete recibido y asignado seqNum: " << pkt->getId() << "\n";

    // Programa el envío inmediato si no hay eventos programados
    if (!endServiceEvent->isScheduled()) {
        scheduleAt(simTime(), endServiceEvent);
    }
}

void TransportTx::handleEndServiceEvent() {
    EV_INFO << "[TransportTx] Intentando enviar el siguiente paquete." << std::endl;

    // Verificar si el buffer está vacío
    if (buffer.isEmpty()) {
        EV_WARN << "[TransportTx] No hay paquetes para enviar." << std::endl;
        return;
    }

    // Buscar el siguiente paquete listo para enviar
    DataPkt* pktToSend = nullptr;
    for (int i = 0; i < buffer.getLength(); ++i) {
        auto* pkt = check_and_cast<DataPkt*>(buffer.get(i)); // Obtener el elemento i del buffer
        if (pkt->getId() >= expectedAck) {  // Enviar desde expectedAck en adelante
            pktToSend = pkt;
            break;
        }
    }
    if (!pktToSend) {
        EV_WARN << "[TransportTx] No hay paquetes listos para enviar." << std::endl;
        return;
    }

    EV_INFO << "[TransportTx] Enviando paquete con seqNum: " << pktToSend->getId() << std::endl;

    // Duplicar el paquete para enviarlo
    auto* dupPkt = pktToSend->dup();

    // Enviar el paquete al siguiente módulo
    send(dupPkt, "toOut$o");

    // Actualizar el buffer eliminando el paquete enviado
    buffer.remove(pktToSend); // Eliminar el paquete del buffer
    delete pktToSend;         // Liberar memoria del paquete original
    bufferSizeVector.record(buffer.getLength());

    EV_INFO << "[TransportTx] Paquete enviado. Buffer actual: " << buffer.getLength() << " paquetes." << std::endl;

    // Reducir gradualmente el retraso si el sistema se estabiliza
    if (feedbackDelay > defaultDelay) {
        feedbackDelay -= 0.025; // Reduce el retraso en 5 ms después de cada envío exitoso
        if (feedbackDelay < defaultDelay) {
            feedbackDelay = defaultDelay; // No permitir que el retraso sea menor al valor base
        }
    }

    // Programar el próximo evento de servicio
    auto serviceTime = dupPkt->getDuration();  // Obtener la duración del paquete
    auto endServiceTime = simTime() + serviceTime + feedbackDelay;
    scheduleAt(endServiceTime, endServiceEvent);
}


void TransportTx::handleFeedbackPacket(FeedbackPkt* pkt) {
    unsigned int ackNumber = pkt->getLose_Packet(); // Usar el campo Lose_Packet como el número de ACK
    bool isCongestion = pkt->isCongestion(); // Revisar si el feedback indica congestión
    delete pkt;

    if (isCongestion) {
        // Caso de congestión: incrementar el retraso
        feedbackDelay += 1; // Incrementa el retraso en 50 ms
        EV_INFO << "[TransportTx] Congestión detectada, aumentando retraso a " << feedbackDelay << " segundos.\n";
    } else {
        // Caso de pérdida de paquetes: retransmitir desde el paquete perdido
        EV_INFO << "[TransportTx] Pérdida detectada, retransmitiendo desde ackNumber: " << ackNumber << "\n";

        if (ackNumber >= expectedAck) {
            expectedAck = ackNumber;  // Reinicia el contador a partir del paquete perdido
            count = expectedAck;     // Sincroniza el contador con el nuevo expectedAck

            // Limpia el buffer de paquetes confirmados
            while (!buffer.isEmpty() && static_cast<DataPkt*>(buffer.front())->getId() < expectedAck) {
                delete buffer.pop();
            }
            bufferSizeVector.record(buffer.getLength());
        }

        // Incrementa el retraso para dar tiempo a la cola de vaciarse
        feedbackDelay += 1; // Incrementa el retraso en caso de pérdida
    }

    // Si hay más paquetes para enviar, ajusta la programación con el nuevo retraso
    if (!endServiceEvent->isScheduled() && !buffer.isEmpty()) {
        scheduleAt(simTime() + feedbackDelay, endServiceEvent);
    }
}



#endif /* TRANSPORT_TX */
