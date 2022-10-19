#include "OrGate.h"

OrGate::OrGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
    setInputCount(2);
    setOutputCount(1);

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("OR");
}
void OrGate::update()
{
    std::vector<Pin*> inp = getInputPins();
    std::vector<Pin*> out = getOutputPins();

    // Process logic
    bool outValue = false;
    for(size_t i=0; i<inp.size(); ++i)
    {
        outValue |= inp[i]->getOutputValue();
    }
    out[0]->setValue((LogicSignal::Digital)outValue);
}
void OrGate::setInputCount(size_t inputs)
{
    if(inputs < 2)
        return;
    Gate::setInputCount(inputs);
}
void OrGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}

