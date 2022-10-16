#include "InputGate.h"

InputGate::InputGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
    connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
            this, &InputGate::onGateButtonFallingEdge);
    setInputCount(0);
    setOutputCount(1);
    m_on = false;

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("SIGNAL");
    setCharacterSize(40);
}
InputGate::InputGate(const InputGate &other)
    : Gate(other)
{
    m_on = other.m_on;

    connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
            this, &InputGate::onGateButtonFallingEdge);
}

void InputGate::switchOn()
{
    m_on = true;
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }
}
void InputGate::switchOff()
{
    m_on = false;
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }
}

QJsonObject InputGate::save() const
{
    // Combine the QJsonObject with the base object of this
    return combine(Gate::save(),
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"isOn", m_on},
    });
}
bool InputGate::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = Gate::read(reader);


    success &= extract(reader,m_on, "isOn");

    return success;
}
void InputGate::postLoad()
{
    Gate::postLoad();
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }
}

void InputGate::setInputCount(size_t inputs)
{
    Gate::setInputCount(inputs);
}
void InputGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}
void InputGate::onGateButtonFallingEdge()
{
    //setOrientation((Orientation)((getOrientation()+1)%4));
    m_on = !m_on;
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }
}

