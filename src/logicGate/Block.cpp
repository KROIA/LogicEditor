#include "Block.h"
#include "BlockPin.h"
#include "BlockGate.h"


Block::Block(const std::string &name,
             CanvasObject *parent)
    : QObject()
    , CanvasObject(name,parent)
    , ISerializable()
{
    m_innerParent = new QSFML::Objects::CanvasObject("inner");
    m_gate = new BlockGate(getName());
    m_gate->setBlock(this);

    m_inputs = 0;
    m_inputs = 0;

    CanvasObject::addChild(m_innerParent);
    CanvasObject::addChild(m_gate);
    setEditMode(true);
}
Block::Block(const Block &other)
    : QObject()
    , CanvasObject(other)
    , ISerializable(other)
{
    m_innerParent = new QSFML::Objects::CanvasObject("inner");
    m_gate = new BlockGate(getName());
    m_gate->setBlock(this);
    CanvasObject::addChild(m_innerParent);
    CanvasObject::addChild(m_gate);

    QJsonObject otherData = other.save();
    read(otherData);
    postLoad();
    setEditMode(false);
}

void Block::setEditMode(bool enable)
{
    m_isInEditMode = enable;
    if(m_isInEditMode)
    {
        m_gate->setEditMode(true);
        m_innerParent->setEnabled(true);
    }
    else
    {
        m_gate->setEditMode(false);
        m_innerParent->setEnabled(false);
    }
    std::vector<Gate* > gates = m_innerParent->getChilds<Gate>();
    for(size_t i=0; i<gates.size(); ++i)
    {
        gates[i]->setEditMode(enable);
    }
}
bool Block::isInEditMode() const
{
    return m_isInEditMode;
}

void Block::addGate(Gate *gate)
{
    if(!m_isInEditMode)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Can't edit "<<getName().c_str()<< " switch in edit mode first.";
        return;
    }
    if(!gate) return;
    if(!m_innerParent->childExists(gate))
    {
        connectGateSignals(gate);
        BlockPin *pin = dynamic_cast<BlockPin*>(gate);
        if(pin)
        {
            if(pin->getPinType() == Pin::Type::input)
                pin->setPinNr(m_inputs++);
            else
                pin->setPinNr(m_outputs++);
        }
        m_innerParent->addChild(gate);
    }
}
void Block::removeGate(Gate *gate)
{
    if(!m_isInEditMode)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Can't edit "<<getName().c_str()<< " switch in edit mode first.";
        return;
    }
    if(!gate) return;
    if(m_innerParent->childExists(gate))
    {
        disconnectGateSignals(gate);
        m_innerParent->removeChild(gate);
        BlockPin *pin = dynamic_cast<BlockPin*>(gate);
        if(pin)
        {
            if(pin->getPinType() == Pin::Type::input)
                m_inputs--;
            else
                m_outputs--;
        }
    }
}
void Block::deleteGate(Gate *gate)
{
    if(!m_isInEditMode)
    {
        qDebug() << __PRETTY_FUNCTION__ << " Can't edit "<<getName().c_str()<< " switch in edit mode first.";
        return;
    }
    if(!gate) return;
    if(m_innerParent->childExists(gate))
    {
        disconnectGateSignals(gate);

        BlockPin *pin = dynamic_cast<BlockPin*>(gate);
        if(pin)
        {
            if(pin->getPinType() == Pin::Type::input)
                m_inputs--;
            else
                m_outputs--;
        }
        m_innerParent->deleteChild(gate);
    }
}

void  Block::update()
{

}

QJsonObject Block::save() const
{
    std::vector<ISerializable* > gates = m_innerParent->getChilds<ISerializable>();
    QJsonObject thisData = ISerializable::save();


    int gateCount = 0;
    int blockCount = 0;
    for(size_t i=0; i<gates.size(); ++i)
    {
        Block *block = dynamic_cast<Block*>(gates[i]);
        Gate  *gate = dynamic_cast<Gate*>(gates[i]);
        if(block)
        {
            thisData["B"+QString::number(blockCount)] = block->getName().c_str();
            ++blockCount;
        }else if(gate)
        {
            thisData["G"+QString::number(gateCount)] = gate->save();
            ++gateCount;
        }
    }
    thisData["gateCount"] = gateCount;
    thisData["blockCount"] = blockCount;
    thisData["BlockGage"] = m_gate->save();

    // Get the QJsonObject of the member object
    //thisData["myObject"] = myObject.save();

    return thisData;
}
bool Block::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = ISerializable::read(reader);

    // Read the value for the member object
    //success &= myObject.read(thisData["myObject"]);

    // Read the values for this class
    //success &= extract(reader,m_name, "name");
    //success &= extract(reader,m_color,"color");
    return success;
}
void Block::postLoad()
{

    generateBlock();
}


void Block::generateBlock()
{
    return;
    std::vector<BlockPin* > pins = m_innerParent->getChilds<BlockPin>();

    m_inputsGates.clear();
    m_outputsGates.clear();

    m_inputsGates.reserve(pins.size());
    m_outputsGates.reserve(pins.size());

    for(size_t i=0; i<pins.size(); ++i)
    {
        if(pins[i]->getPinType() == Pin::Type::input)
        {
            m_inputsGates.push_back(pins[i]);
        } else if(pins[i]->getPinType() == Pin::Type::output)
        {
            m_outputsGates.push_back(pins[i]);
        }
    }

    m_inputs = m_inputsGates.size();
    m_outputs = m_outputsGates.size();

    std::sort( m_inputsGates.begin( ), m_inputsGates.end( ), [ ]( const BlockPin* lhs, const BlockPin* rhs )
    {
        return lhs->getPinNr() < rhs->getPinNr();
    });
    std::sort( m_outputsGates.begin( ), m_outputsGates.end( ), [ ]( const BlockPin* lhs, const BlockPin* rhs )
    {
        return lhs->getPinNr() < rhs->getPinNr();
    });

    m_gate->setInputCount(m_inputs);
    m_gate->setOutputCount(m_outputs);
}
void Block::onChildGateDeleted()
{

}
void Block::setInputSignal(unsigned int pinNr, const LogicSignal &signal)
{
    BlockPin *pin = nullptr;
    if(m_inputsGates.size() > pinNr)
    {
        if(m_inputsGates[pinNr]->getPinNr() == pinNr)
            pin = m_inputsGates[pinNr];
    }
    if(!pin)
    {
        for(size_t i=0; i<m_inputsGates.size(); ++i)
        {
            if(m_inputsGates[i]->getPinNr() == pinNr)
            {
                pin = m_inputsGates[i];
                break;
            }
        }
    }
    if(pin)
    {
        pin->setSignal(signal);
    }
}
const LogicSignal &Block::GetOutputSignal(unsigned int pinNr) const
{
    BlockPin *pin = nullptr;
    if(m_inputsGates.size() > pinNr)
    {
        if(m_inputsGates[pinNr]->getPinNr() == pinNr)
            pin = m_inputsGates[pinNr];
    }
    if(!pin)
    {
        for(size_t i=0; i<m_inputsGates.size(); ++i)
        {
            if(m_inputsGates[i]->getPinNr() == pinNr)
            {
                pin = m_inputsGates[i];
                break;
            }
        }
    }
    if(pin)
    {
        return pin->getSignal();
    }
    const static LogicSignal dummy(LogicSignal::Digital::low);
    return dummy;
}


void Block::connectGateSignals(Gate *gate)
{
    connect(gate, &Gate::destroyed, this, &Block::onChildGateDeleted);
}
void Block::disconnectGateSignals(Gate *gate)
{
    disconnect(gate, &Gate::destroyed, this, &Block::onChildGateDeleted);
}
