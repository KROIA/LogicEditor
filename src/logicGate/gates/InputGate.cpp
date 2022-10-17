#include "InputGate.h"

InputGate::InputGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
   // connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
   //         this, &InputGate::onGateButtonFallingEdge);
    setInputCount(0);
    setOutputCount(1);
    m_externPin = nullptr;
   // m_on = false;


    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("Input");
    //setCharacterSize(40);
    m_pin = getOutputPins()[0];

}
InputGate::InputGate(const InputGate &other)
    : Gate(other)
{
    setInputCount(0);
    setOutputCount(1);
    m_externPin = nullptr;
  //  m_on = other.m_on;

    setText("Input");
    //setCharacterSize(40);

    m_pin = getOutputPins()[0];
   //connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
   //        this, &InputGate::onGateButtonFallingEdge);
}
InputGate::~InputGate()
{
    emit getsDeleted(this, m_pin);
}

void InputGate::update()
{
    if(m_externPin)
    {
        m_pin = getOutputPin(0);
        m_pin->setValue(m_externPin->getValue());
    }
}

void InputGate::setPin(Pin *pin)
{
    m_externPin = pin;
    if(!m_externPin) return;
    setText("Input "+std::to_string(m_externPin->getPinNr()));
}
Pin *InputGate::getPin() const
{
    return m_pin;
}

/*void InputGate::switchOn()
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
}*/

const LogicSignal &InputGate::getSignal()
{
    return *m_pin->getSignal();
}

QJsonObject InputGate::save() const
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
      //  {"isOn", m_on},
        {"nr", pinNr},
    });
}
bool InputGate::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = Gate::read(reader);


    //success &= extract(reader,m_on, "isOn");
    int nr = 0;
    success &= extract(reader,nr, "nr");
    if(m_pin)
        m_pin->setPinNr(nr);

    return success;
}
void InputGate::postLoad()
{
    Gate::postLoad();
    /*std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }*/
}

void InputGate::setInputCount(size_t inputs)
{
    Gate::setInputCount(inputs);
}
void InputGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}
/*
void InputGate::onGateButtonFallingEdge()
{
    //setOrientation((Orientation)((getOrientation()+1)%4));
    m_on = !m_on;
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_on);
    }
}*/

