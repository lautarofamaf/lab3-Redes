#ifndef PACKET_H
#define PACKET_H

#include <omnetpp.h> // Para cMessage

typedef long long ll;

enum Kind {
    Undefined =0,
    Data = 1,
    Feedback = 2,
};

using namespace omnetpp;

class FeedbackPkt: public cMessage {
    // Privado  --> Acceso desde la misma clase
    private:
      int  remainingBuffer;
      ll   byteLength;
      bool ack;

    public:
                     FeedbackPkt(); // Constructor
             virtual ~FeedbackPkt(); // Destructor
             // Actualizar tamano del buffer
             virtual void setRemainingBuffer(int n);
             // Consultar tamano del buffer
             virtual int getRemainingBuffer();
             // Setear tamano del paquete
             virtual void setByteLength(ll s);


    protected:
        virtual void initialize();
        virtual void finish();

};


#endif /* PACKET_H */
