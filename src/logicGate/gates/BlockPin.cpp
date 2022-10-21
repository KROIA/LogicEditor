#include "BlockPin.h"

BlockPin::BlockPin(const std::string &name,
                   CanvasObject *parent)
    :   Gate(name, parent)
   // ,   m_signal(LogicSignal::Digital::low)
{
    setPinNr(0);
    setType(Pin::Type::input);
}

BlockPin::BlockPin(const BlockPin &other)
    :   Gate(other)
    //,   m_signal(LogicSignal::Digital::low)
{
    setType(other.m_type);
    setPinNr(other.m_pinNr);
}

void BlockPin::update()
{

}

void BlockPin::setPinNr(unsigned int nr)
{
    m_pinNr = nr;
    if(m_inputs.size() > 0)
        m_inputs[0]->setPinNr(m_pinNr);
    if(m_outputs.size() > 0)
        m_outputs[0]->setPinNr(m_pinNr);
}
unsigned int BlockPin::getPinNr() const
{
    return m_pinNr;
}

void BlockPin::setType(Pin::Type type)
{
    m_type = type;
    if(type == Pin::Type::input)
    {
        if(m_outputs.size() > 0)
            return;
        if(m_inputs.size() > 0)
        {
            ISerializable::removeChild(m_inputs[0]);
            deleteChild(m_inputs[0]);
            m_inputs.clear();
        }
        Pin *pin = new Pin("OUT "+std::to_string(m_pinNr));
        pin->setType(Pin::Type::output);
        pin->setPinNr(m_pinNr);
        m_outputs.push_back(pin);
        CanvasObject::addChild(pin);
        ISerializable::addChild(pin);
    }
    else
    {
        if(m_inputs.size() > 0)
            return;
        if(m_outputs.size() > 0)
        {
            ISerializable::removeChild(m_outputs[0]);
            deleteChild(m_outputs[0]);
            m_outputs.clear();
        }
        Pin *pin = new Pin("IN "+std::to_string(m_pinNr));
        pin->setType(Pin::Type::input);
        pin->setPinNr(m_pinNr);
        m_inputs.push_back(pin);
        CanvasObject::addChild(pin);
        ISerializable::addChild(pin);
    }
    updateGeometry();
}
Pin::Type BlockPin::getPinType() const
{
    return m_type;
}

void BlockPin::setSignal(const LogicSignal &signal)
{
    //m_signal = signal;
    if(m_type == Pin::Type::input)
    {
        m_outputs[0]->setSignal(signal.getValue());
    }
   /* else
    {
        getInputPin(0)->setSignal(signal.getValue());
    }*/
}
const LogicSignal &BlockPin::getSignal() const
{
    if(m_type == Pin::Type::input)
    {
        return *m_outputs[0]->getOutputSignal();
    }
    else
    {
        return *m_inputs[0]->getOutputSignal();
    }
}

// Serializer
QJsonObject BlockPin::save() const
{
    QJsonObject obj = Gate::save();
    obj["type"] = m_type;
    obj["pinNr"] = (int)m_pinNr;
    return obj;
}
bool BlockPin::read(const QJsonObject &reader)
{
    bool success = Gate::read(reader);
    m_type = (Pin::Type)reader["type"].toInt(0);
    m_pinNr = (Pin::Type)reader["pinNr"].toInt(0);
    return success;
}
void BlockPin::postLoad()
{
    Gate::postLoad();
}

void BlockPin::setInputCount(size_t)
{

}
void BlockPin::setOutputCount(size_t)
{

}
