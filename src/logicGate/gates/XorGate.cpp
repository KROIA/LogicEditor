#include "XorGate.h"

XorGate::XorGate(const std::string &name,
                 CanvasObject *parent)
    : Gate(name,parent)
{
    setInputCount(2);
    setOutputCount(1);

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("XOR");
}
void XorGate::update()
{
    std::vector<Pin*> inp = getInputPins();
    std::vector<Pin*> out = getOutputPins();

    if(inp.size() != 2)
        return; // Xor must have 2 inputs

    // Process logic
    bool outValue = false;
    if(inp[0]->getValue() != inp[1]->getValue())
        outValue = true;
    out[0]->setValue((LogicSignal::Digital)outValue);
}
void XorGate::setInputCount(size_t inputs)
{
    if(inputs < 2)
        return;
    Gate::setInputCount(inputs);
}
void XorGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}

