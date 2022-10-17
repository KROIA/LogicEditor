#include "OutputGate.h"



OutputGate::OutputGate(const std::string &name,
                       CanvasObject *parent)
    : Gate(name, parent)
{
    Gate::setInputCount(1);
    m_pin = Gate::getInputPin(0);
    m_pin = getInputPins()[0];
    setText("Output");
    m_externPin = nullptr;
}

OutputGate::OutputGate(const OutputGate &other)
    : Gate(other)
{
    Gate::setInputCount(1);
    m_pin = Gate::getInputPin(0);
    m_pin = getInputPins()[0];
    m_externPin = nullptr;
}
OutputGate::~OutputGate()
{
    emit getsDeleted(this, m_pin);
}

void OutputGate::update()
{
    if(m_externPin)
    {
        m_pin->setValue(m_externPin->getValue());
    }
}

void OutputGate::setPin(Pin *pin)
{
    m_externPin = pin;
    if(!m_externPin) return;
    setText("Output "+std::to_string(m_externPin->getPinNr()));
}
Pin *OutputGate::getPin() const
{
    return m_pin;
}

const LogicSignal &OutputGate::getSignal()
{
    return *m_pin->getSignal();
}

// Serializer
QJsonObject OutputGate::save() const
{
    int pinNr = 0;
    if(m_pin)
        pinNr = m_pin->getPinNr();
    // Combine the QJsonObject with the base object of this
    return combine(Gate::save(),
    QJsonObject
    {
    // Add the properties of this object here
    // Do not take the same keyvalues two times,
    // also not the keys of the base class
        {"nr", pinNr},
    });
}
bool OutputGate::OutputGate::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = Gate::read(reader);

    int nr = 0;
    success &= extract(reader,nr, "nr");
    if(m_pin)
        m_pin->setPinNr(nr);


    return success;
}
void OutputGate::OutputGate::postLoad()
{
    Gate::postLoad();
}

void OutputGate::setInputCount(size_t)
{

}
void OutputGate::setOutputCount(size_t)
{

}
