#include "NotGate.h"

NotGate::NotGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
    setInputCount(1);
    setOutputCount(1);

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("NOT");
}
void NotGate::update()
{
    std::vector<Pin*> inp = getInputPins();
    std::vector<Pin*> out = getOutputPins();

    size_t smallest = inp.size();
    if(smallest > out.size())
        smallest = out.size();


    // Process logic
    for(size_t i=0; i<smallest; ++i)
    {
        out[i]->setValue((LogicSignal::Digital)!(bool)inp[i]->getOutputValue());
    }
}
void NotGate::setInputCount(size_t inputs)
{
    Gate::setInputCount(inputs);
    Gate::setOutputCount(inputs);
}
void NotGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}

