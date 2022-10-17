#include "Block.h"

Block *Block::currentEditingBlock = nullptr;
Block::Block(const std::string &name,
             CanvasObject *parent)
    : QObject()
    , CanvasObject(name,parent)
    , ISerializable()
{
    m_insideObj = new QSFML::Objects::CanvasObject("inside");
    m_thisOutsideGate = new Gate("outside");

    CanvasObject::addChild(m_insideObj);
    CanvasObject::addChild(m_thisOutsideGate);

    switchViewToOutside(false);
}
Block::Block(const Block &other)
    : QObject()
    , CanvasObject(other)
    , ISerializable(other)
{
    m_insideObj = new QSFML::Objects::CanvasObject("inside");
    m_thisOutsideGate = new Gate("outside");

    CanvasObject::addChild(m_insideObj);
    CanvasObject::addChild(m_thisOutsideGate);
}
void Block::update()
{

}

void Block::switchViewToOutside(bool toOutside)
{
    m_isOutside = toOutside;
    if(m_isOutside)
    {
        m_thisOutsideGate->setEnabled(true);
        m_insideObj->setEnabled(false);
    }
    else
    {
        m_thisOutsideGate->setEnabled(false);
        m_insideObj->setEnabled(true);
    }
}
bool Block::isViewOutside() const
{
    return m_isOutside;
}

void Block::addGate(Gate *gate)
{
    m_insideObj->addChild(gate);
    ISerializable::addChild(gate);
    connect(gate, &QObject::destroyed, this, &Block::onGateDeleted);

    InputGate *inp = dynamic_cast<InputGate*>(gate);
    OutputGate *out = dynamic_cast<OutputGate*>(gate);

    if(inp)
    {
        Pin *pin = new Pin(inp->getName());
        inp->setPin(pin);
        m_thisOutsideGate->addInput(pin);
        connect(inp, &InputGate::getsDeleted, this, &Block::onInputDeleted);
    }
    if(out)
    {
        Pin *pin = new Pin(out->getName());
        out->setPin(pin);
        m_thisOutsideGate->addOutput(pin);
        connect(out, &OutputGate::getsDeleted, this, &Block::onOutputDeleted);
    }
}
QJsonObject Block::save() const
{
    QJsonObject obj = ISerializable::save();
    std::vector<Gate*> gates = m_insideObj->getChilds<Gate>();
    std::vector<InputGate*> inputs = m_insideObj->getChilds<InputGate>();
    std::vector<OutputGate*> outputs = m_insideObj->getChilds<OutputGate>();
    std::sort( inputs.begin( ), inputs.end( ), [ ]( const InputGate* lhs, const InputGate* rhs )
    {
        return lhs->getPin()->getPinNr() < rhs->getPin()->getPinNr();
    });

    std::sort( outputs.begin( ), outputs.end( ), [ ]( const OutputGate* lhs, const OutputGate* rhs )
    {
        return lhs->getPin()->getPinNr() < rhs->getPin()->getPinNr();
    });

    obj["inputs"] = (int)inputs.size();
    obj["outputs"] = (int)outputs.size();
    for(size_t i=0; i<inputs.size(); ++i)
    {
        obj["INP_"+QString::number(i)] = inputs[i]->getID().c_str();
    }
    for(size_t i=0; i<outputs.size(); ++i)
    {
        obj["OUT_"+QString::number(i)] = outputs[i]->getID().c_str();
    }
    /*QJsonObject gatesObj;
    for(size_t i=0; i<gates.size(); ++i)
    {
        gatesObj[QString::number(i)] = gates[i]->save();
    }
    obj["gates"] = gatesObj;*/



    //obj["outsideObj"] = m_thisOutsideGate->save();

    // Combine the QJsonObject with the base object of this
    return combine(ISerializable::save(),
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"outside" , m_isOutside},
    });
}
bool Block::read(const QJsonObject &reader)
{
    bool success = ISerializable::read(reader);

    int inputs = 0;
    int outputs = 0;
    success &= extract(reader,inputs, "inputs");
    success &= extract(reader,outputs, "outputs");


    /*QJsonObject gatesObj = reader["gates"].toObject();
    for(int i=0; i<gatesCount; ++i)
    {
        DatabaseObject *gateObj = getInstantiated(reader[QString::number(i)].toObject());

        gatesObj[QString::number(i)] = gates[i]->save();
    }*/

    m_loadingInputIDs.reserve(inputs);
    m_loadingOutputIDs.reserve(outputs);
    for(int i=0; i<inputs; ++i)
    {
        std::string id;
        success &= extract(reader,id, "INP_"+std::to_string(i));
        if(id.size())
            m_loadingInputIDs.push_back(id);
    }
    for(int i=0; i<outputs; ++i)
    {
        std::string id;
        success &= extract(reader,id, "OUT_"+std::to_string(i));
        if(id.size())
            m_loadingOutputIDs.push_back(id);
    }

    //QJsonObject outsideObj = reader["outsideObj"].toObject();
    //m_thisOutsideGate->read(outsideObj);


    success &= extract(reader,m_isOutside, "outside");
    return success;
}
void Block::postLoad()
{
    ISerializable::postLoad();
    std::vector<ISerializable*> childs = ISerializable::getChilds();
    for(size_t i=0; i<childs.size(); ++i)
    {
        Gate *gate = dynamic_cast<Gate*>(childs[i]);
        if(gate)
        {
            m_insideObj->addChild(gate);
            connect(gate, &QObject::destroyed, this, &Block::onGateDeleted);

            InputGate *inp = dynamic_cast<InputGate*>(gate);
            OutputGate *out = dynamic_cast<OutputGate*>(gate);

            if(inp)
            {
                Pin *pin = new Pin(inp->getName());
                inp->setPin(pin);
                m_thisOutsideGate->addInput(pin);
                connect(inp, &InputGate::getsDeleted, this, &Block::onInputDeleted);
            }
            if(out)
            {
                Pin *pin = new Pin(out->getName());
                out->setPin(pin);
                m_thisOutsideGate->addOutput(pin);
                connect(out, &OutputGate::getsDeleted, this, &Block::onOutputDeleted);
            }
        }
    }
}

void Block::onGateDeleted()
{
    Gate* obj = qobject_cast<Gate*>(QObject::sender());
    ISerializable::removeChild(obj);
}
void Block::onInputDeleted(InputGate *gate, Pin *pin)
{
    if(!gate) return;
    m_thisOutsideGate->removeInput(pin);
}
void Block::onOutputDeleted(OutputGate *gate, Pin *pin)
{
    if(!gate) return;
    m_thisOutsideGate->removeOutput(pin);
}
