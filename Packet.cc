
#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "Packet.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(DataPkt)

DataPkt::DataPkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

DataPkt::DataPkt(const DataPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

DataPkt::~DataPkt()
{
}

DataPkt& DataPkt::operator=(const DataPkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void DataPkt::copy(const DataPkt& other)
{
    this->id = other.id;
    this->remainingBuffer = other.remainingBuffer;
    this->byteLength = other.byteLength;
    this->ack = other.ack;
}

void DataPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->id);
    doParsimPacking(b,this->remainingBuffer);
    doParsimPacking(b,this->byteLength);
    doParsimPacking(b,this->ack);
}

void DataPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->id);
    doParsimUnpacking(b,this->remainingBuffer);
    doParsimUnpacking(b,this->byteLength);
    doParsimUnpacking(b,this->ack);
}

int DataPkt::getId() const
{
    return this->id;
}

void DataPkt::setId(int id)
{
    this->id = id;
}

int DataPkt::getRemainingBuffer() const
{
    return this->remainingBuffer;
}

void DataPkt::setRemainingBuffer(int remainingBuffer)
{
    this->remainingBuffer = remainingBuffer;
}


bool DataPkt::getAck() const
{
    return this->ack;
}

void DataPkt::setAck(bool ack)
{
    this->ack = ack;
}

class DataPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_id,
        FIELD_remainingBuffer,
        FIELD_byteLength,
        FIELD_ack,
    };
  public:
    DataPktDescriptor();
    virtual ~DataPktDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(DataPktDescriptor)

DataPktDescriptor::DataPktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(DataPkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

DataPktDescriptor::~DataPktDescriptor()
{
    delete[] propertyNames;
}

bool DataPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<DataPkt *>(obj)!=nullptr;
}

const char **DataPktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *DataPktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int DataPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int DataPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_id
        FD_ISEDITABLE,    // FIELD_remainingBuffer
        FD_ISEDITABLE,    // FIELD_byteLength
        FD_ISEDITABLE,    // FIELD_ack
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *DataPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "id",
        "remainingBuffer",
        "byteLength",
        "ack",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int DataPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "id") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "remainingBuffer") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "byteLength") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "ack") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *DataPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_id
        "int",    // FIELD_remainingBuffer
        "int",    // FIELD_byteLength
        "bool",    // FIELD_ack
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **DataPktDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *DataPktDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int DataPktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void DataPktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'DataPkt'", field);
    }
}

const char *DataPktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string DataPktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: return long2string(pp->getId());
        case FIELD_remainingBuffer: return long2string(pp->getRemainingBuffer());
        case FIELD_byteLength: return long2string(pp->getByteLength());
        case FIELD_ack: return bool2string(pp->getAck());
        default: return "";
    }
}

void DataPktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: pp->setId(string2long(value)); break;
        case FIELD_remainingBuffer: pp->setRemainingBuffer(string2long(value)); break;
        case FIELD_byteLength: pp->setByteLength(string2long(value)); break;
        case FIELD_ack: pp->setAck(string2bool(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

omnetpp::cValue DataPktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: return pp->getId();
        case FIELD_remainingBuffer: return pp->getRemainingBuffer();
        case FIELD_byteLength: return pp->getByteLength();
        case FIELD_ack: return pp->getAck();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'DataPkt' as cValue -- field index out of range?", field);
    }
}

void DataPktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: pp->setId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_remainingBuffer: pp->setRemainingBuffer(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_byteLength: pp->setByteLength(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_ack: pp->setAck(value.boolValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

const char *DataPktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr DataPktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void DataPktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    DataPkt *pp = omnetpp::fromAnyPtr<DataPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'DataPkt'", field);
    }
}

Register_Class(FeedbackPkt)

FeedbackPkt::FeedbackPkt(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

FeedbackPkt::FeedbackPkt(const FeedbackPkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

FeedbackPkt::~FeedbackPkt()
{
}

FeedbackPkt& FeedbackPkt::operator=(const FeedbackPkt& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void FeedbackPkt::copy(const FeedbackPkt& other)
{
    this->id = other.id;
    this->Lose_Packet = other.Lose_Packet;
    this->delay = other.delay;
    this->isCongestion_ = other.isCongestion_;
}

void FeedbackPkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->id);
    doParsimPacking(b,this->Lose_Packet);
    doParsimPacking(b,this->delay);
    doParsimPacking(b,this->isCongestion_);
}

void FeedbackPkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->id);
    doParsimUnpacking(b,this->Lose_Packet);
    doParsimUnpacking(b,this->delay);
    doParsimUnpacking(b,this->isCongestion_);
}

int FeedbackPkt::getId() const
{
    return this->id;
}

void FeedbackPkt::setId(int id)
{
    this->id = id;
}

int FeedbackPkt::getLose_Packet() const
{
    return this->Lose_Packet;
}

void FeedbackPkt::setLose_Packet(int Lose_Packet)
{
    this->Lose_Packet = Lose_Packet;
}

float FeedbackPkt::getDelay() const
{
    return this->delay;
}

void FeedbackPkt::setDelay(float delay)
{
    this->delay = delay;
}

bool FeedbackPkt::isCongestion() const
{
    return this->isCongestion_;
}

void FeedbackPkt::setIsCongestion(bool isCongestion)
{
    this->isCongestion_ = isCongestion;
}

class FeedbackPktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_id,
        FIELD_Lose_Packet,
        FIELD_delay,
        FIELD_isCongestion,
    };
  public:
    FeedbackPktDescriptor();
    virtual ~FeedbackPktDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(FeedbackPktDescriptor)

FeedbackPktDescriptor::FeedbackPktDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(FeedbackPkt)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

FeedbackPktDescriptor::~FeedbackPktDescriptor()
{
    delete[] propertyNames;
}

bool FeedbackPktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<FeedbackPkt *>(obj)!=nullptr;
}

const char **FeedbackPktDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *FeedbackPktDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int FeedbackPktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int FeedbackPktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_id
        FD_ISEDITABLE,    // FIELD_Lose_Packet
        FD_ISEDITABLE,    // FIELD_delay
        FD_ISEDITABLE,    // FIELD_isCongestion
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *FeedbackPktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "id",
        "Lose_Packet",
        "delay",
        "isCongestion",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int FeedbackPktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "id") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "Lose_Packet") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "delay") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "isCongestion") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *FeedbackPktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_id
        "int",    // FIELD_Lose_Packet
        "float",    // FIELD_delay
        "bool",    // FIELD_isCongestion
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **FeedbackPktDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *FeedbackPktDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int FeedbackPktDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void FeedbackPktDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'FeedbackPkt'", field);
    }
}

const char *FeedbackPktDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string FeedbackPktDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: return long2string(pp->getId());
        case FIELD_Lose_Packet: return long2string(pp->getLose_Packet());
        case FIELD_delay: return double2string(pp->getDelay());
        case FIELD_isCongestion: return bool2string(pp->isCongestion());
        default: return "";
    }
}

void FeedbackPktDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: pp->setId(string2long(value)); break;
        case FIELD_Lose_Packet: pp->setLose_Packet(string2long(value)); break;
        case FIELD_delay: pp->setDelay(string2double(value)); break;
        case FIELD_isCongestion: pp->setIsCongestion(string2bool(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FeedbackPkt'", field);
    }
}

omnetpp::cValue FeedbackPktDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: return pp->getId();
        case FIELD_Lose_Packet: return pp->getLose_Packet();
        case FIELD_delay: return (double)(pp->getDelay());
        case FIELD_isCongestion: return pp->isCongestion();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'FeedbackPkt' as cValue -- field index out of range?", field);
    }
}

void FeedbackPktDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        case FIELD_id: pp->setId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_Lose_Packet: pp->setLose_Packet(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_delay: pp->setDelay(static_cast<float>(value.doubleValue())); break;
        case FIELD_isCongestion: pp->setIsCongestion(value.boolValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FeedbackPkt'", field);
    }
}

const char *FeedbackPktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr FeedbackPktDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void FeedbackPktDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    FeedbackPkt *pp = omnetpp::fromAnyPtr<FeedbackPkt>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'FeedbackPkt'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

