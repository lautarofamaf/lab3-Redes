//
// Generated file, do not edit! Created by opp_msgtool 6.0 from Packet.msg.
//

#ifndef PACKET_H
#define PACKET_H


#include <omnetpp.h>

enum Kind{
    Undefined=0,
    Data=1,
    Feedback=2,
};

class DataPkt;
class FeedbackPkt;
// cplusplus {{
#include <string>
// }}

/**
 * Class generated from <tt>Packet.msg:6</tt> by opp_msgtool.
 * <pre>
 * // Definición de DataPkt
 * packet DataPkt
 * {
 *     int id;               // Identificador del paquete
 *     int remainingBuffer;  // Espacio restante en el buffer
 *     int byteLength;      // Tamaño en bytes
 *     bool ack;             // Indica si es un paquete de ACK
 * }
 * </pre>
 */
class DataPkt : public ::omnetpp::cPacket
{
  protected:
    int id = 0;
    int remainingBuffer = 0;
    int byteLength = 0;
    bool ack = false;

  private:
    void copy(const DataPkt& other);

  protected:
    bool operator==(const DataPkt&) = delete;

  public:
    DataPkt(const char *name=nullptr, short kind=0);
    DataPkt(const DataPkt& other);
    virtual ~DataPkt();
    DataPkt& operator=(const DataPkt& other);
    virtual DataPkt *dup() const override {return new DataPkt(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getId() const;
    virtual void setId(int id);

    virtual int getRemainingBuffer() const;
    virtual void setRemainingBuffer(int remainingBuffer);

    virtual bool getAck() const;
    virtual void setAck(bool ack);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const DataPkt& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, DataPkt& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>Packet.msg:14</tt> by opp_msgtool.
 * <pre>
 * // Definición de FeedbackPkt
 * packet FeedbackPkt
 * {
 *     int id;               // Identificador del paquete
 *     int Lose_Packet;      // Número del paquete perdido
 *     float delay;          // Retardo asociado
 *     bool isCongestion;      // Indica si el buffer está lleno
 * }
 * </pre>
 */
class FeedbackPkt : public ::omnetpp::cPacket
{
  protected:
    int id = 0;
    int Lose_Packet = 0;
    float delay = 0;
    bool isCongestion_ = false;

  private:
    void copy(const FeedbackPkt& other);

  protected:
    bool operator==(const FeedbackPkt&) = delete;

  public:
    FeedbackPkt(const char *name=nullptr, short kind=0);
    FeedbackPkt(const FeedbackPkt& other);
    virtual ~FeedbackPkt();
    FeedbackPkt& operator=(const FeedbackPkt& other);
    virtual FeedbackPkt *dup() const override {return new FeedbackPkt(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getId() const;
    virtual void setId(int id);

    virtual int getLose_Packet() const;
    virtual void setLose_Packet(int Lose_Packet);

    virtual float getDelay() const;
    virtual void setDelay(float delay);

    virtual bool isCongestion() const;
    virtual void setIsCongestion(bool isCongestion);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const FeedbackPkt& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, FeedbackPkt& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline DataPkt *fromAnyPtr(any_ptr ptr) { return check_and_cast<DataPkt*>(ptr.get<cObject>()); }
template<> inline FeedbackPkt *fromAnyPtr(any_ptr ptr) { return check_and_cast<FeedbackPkt*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef PACKET_H

