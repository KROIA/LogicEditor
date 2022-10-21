#include "BlockGate.h"
#include "Block.h"

BlockGate::BlockGate(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
    m_parentBlock = nullptr;
}
void BlockGate::update()
{
    if(!m_parentBlock) return;
    std::vector<Pin*> inp = getInputPins();
    std::vector<Pin*> out = getOutputPins();

    for(size_t i=0; i<inp.size(); ++i)
    {
        m_parentBlock->setInputSignal(inp[i]->getPinNr(), *inp[i]->getOutputSignal());
    }
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setSignal(m_parentBlock->GetOutputSignal(out[i]->getPinNr()));
    }

   /* // Process logic

    bool outValue = true;
    for(size_t i=0; i<inp.size(); ++i)
    {
        outValue &= inp[i]->getOutputValue();
    }
    out[0]->setValue((LogicSignal::Digital)outValue);*/
}

void BlockGate::setEditMode(bool enable)
{
    Gate::setEditMode(enable);
    m_gateDrawable->setEnabled(!enable);
    m_symbolicText->setEnabled(!enable);
}
void BlockGate::setBlock(Block *block)
{
    m_parentBlock = block;
}
Block *BlockGate::getBlock() const
{
    return m_parentBlock;
}

QJsonObject BlockGate::save() const
{
    return Gate::save();
}
bool BlockGate::read(const QJsonObject &reader)
{
    return Gate::read(reader);
}
void BlockGate::postLoad()
{
    Gate::postLoad();
}
/*void BlockGate::setInputCount(size_t inputs)
{
    Gate::setInputCount(inputs);
}
void BlockGate::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}*/

