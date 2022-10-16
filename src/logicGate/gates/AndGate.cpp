#include "AndGate.h"

AndGate::AndGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
   // connect(getGateButton(),&QSFML::Components::Button::fallingEdge,this, &AndGate::onGateButtonFallingEdge);

    setInputCount(2);
    setOutputCount(1);

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("AND");
}
void AndGate::update()
{
    std::vector<Pin*> inp = getInputPins();
    std::vector<Pin*> out = getOutputPins();

    // Process logic

    bool outValue = true;
    for(size_t i=0; i<inp.size(); ++i)
    {
        outValue &= inp[i]->getValue();
    }
    out[0]->setValue((LogicSignal::Digital)outValue);
}
void AndGate::setInputCount(size_t inputs)
{
    if(inputs < 2)
        return;
    Gate::setInputCount(inputs);
    std::vector<Pin*> inp = getInputPins();
   /* for(size_t i=0; i<inp.size(); ++i)
    {
        connect(inp[i],&Pin::pinButtonFallingEdge,this, &AndGate::onPinButtonFallingEdge);
    }*/
}
void AndGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}
/*void AndGate::onGateButtonFallingEdge()
{
    //setOrientation((Orientation)((getOrientation()+1)%4));
}*/
/*void AndGate::onPinButtonFallingEdge()
{
    Pin *sender = qobject_cast<Pin*>(QObject::sender());
    if(sender)
        if(!sender->getInputConnection())
            sender->setValue((LogicSignal::Digital)!(bool)sender->getValue());
}
*/
