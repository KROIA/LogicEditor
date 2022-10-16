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

