#include "Gate.h"
#include "EditingTool.h"
#include "VectorOperations.h"

Gate::Gate(const Gate &other)
    :   QObject()
    ,   QSFML::Objects::CanvasObject(other.getName(),nullptr)
{

}
Gate::Gate(const std::string &name,
           CanvasObject *parent)
    :   QObject()
    ,   QSFML::Objects::CanvasObject(name,parent)
{
    m_button = new QSFML::Components::Button("button",QSFML::Utilities::AABB({0,0},m_size));
    connect(m_button, &QSFML::Components::Button::fallingEdge, this, &Gate::onButtonFallingEdge);
    connect(m_button, &QSFML::Components::Button::risingEdge, this, &Gate::onButtonRisingEdge);

    m_mouseFollower = new QSFML::Components::MouseFollower("MouseFollower");

    connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged ,this,&Gate::onGateDragTo);
    m_isDragging = false;

    m_mousePressEvent = new QSFML::Components::MousePressEvent("MousePressEvent",sf::Mouse::Button::Right);
    connect(m_mousePressEvent, &QSFML::Components::MousePressEvent::fallingEdge, this, &Gate::onRightMouseButtonPressed);

    m_symbolicText = new QSFML::Components::Text(getName());
    m_symbolicText->setCharacterSize(80);

    m_gateDrawable = new GateDrawable();
    m_gateDrawable->m_color = sf::Color(130,130,130);
    m_gateDrawable->m_gate = this;

    m_orientation = Orientation::right;
    m_size = sf::Vector2f(30,60);
    m_autoSize = true;
    m_autoSizeInterval = 15.f;

    addComponent(m_gateDrawable);
    addComponent(m_button);
    addComponent(m_mouseFollower);
    addComponent(m_mousePressEvent);
    addComponent(m_symbolicText);

    enableMouseDrag(false);

    m_minMovingDistance = 5;

}

void Gate::update()
{
    /*static bool wasPressed = false;
    bool pressed = false;
    if(m_isDragging)
    {

        pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
        if(!wasPressed && pressed)
        {

        }

    }
    wasPressed = pressed;*/
}
void Gate::setText(const std::string &text)
{
    m_symbolicText->setText(text);
    updatePosition();
}
QSFML::Components::Text *Gate::getText() const
{
    return m_symbolicText;
}
void  Gate::setCharacterSize(unsigned int size)
{
    m_symbolicText->setCharacterSize(size);
    updatePosition();
}
unsigned int  Gate::getCharacterSize() const
{
    return m_symbolicText->getCharacterSize();
}

void Gate::enableAutoSize(bool enable)
{
    m_autoSize = enable;
}
bool Gate::autoSizeEnabled() const
{
    return m_autoSize;
}
void Gate::setAutoSizeInterval(float interval)
{
    m_autoSizeInterval = interval;
}
float Gate::getAutoSizeInterval() const
{
    return m_autoSizeInterval;
}
void Gate::setOrientation(Orientation orientation)
{
    m_orientation = orientation;
    updateGeometry();
}
Orientation Gate::getOrientation() const
{
    return m_orientation;
}
void Gate::setPosition(const sf::Vector2f &pos)
{
    m_origin = pos;
    updatePosition();
}
const sf::Vector2f &Gate::getPosition() const
{
    return m_origin;
}
void Gate::setSize(const sf::Vector2f &size)
{
    m_size = size;
    if(m_size.x < 0)
        m_size.x = -m_size.x;
    if(m_size.y < 0)
        m_size.y = -m_size.y;
    updateGeometry();
}
const sf::Vector2f &Gate::getSize() const
{
    return m_size;
}
void Gate::enableMouseDrag(bool enable)
{
    m_draggingIsEnabled = enable;
    if(!enable)
    {
        m_isDragging = false;
        m_mouseFollower->setEnabled(m_isDragging);
        m_mousePressEvent->setEnabled(m_isDragging);
    }
}
bool Gate::mouseDragEnabled() const
{
    return m_mouseFollower->isEnabled();
}
void Gate::snapToMouse(bool enable)
{
    m_draggingIsEnabled |= enable;
    if(enable)
    {
        setPosition(getMouseWorldPosition());
        m_isDragging = true;
        m_mouseFollower->setEnabled(m_isDragging);
        m_mousePressEvent->setEnabled(m_isDragging);
    }
    else
    {
        m_isDragging = false;
        m_mouseFollower->setEnabled(m_isDragging);
        m_mousePressEvent->setEnabled(m_isDragging);
    }
}
void Gate::setInputCount(size_t inputs)
{
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        ISerializable::removeChild(m_inputs[i]);
        deleteChild(m_inputs[i]);
    }
    m_inputs.clear();
    m_inputs.reserve(inputs);


    for(size_t i=0; i<inputs; ++i)
    {
        Pin *pin = new Pin("IN "+std::to_string(i+1));
        pin->setType(Pin::Type::input);
        pin->setPinNr(i);
        m_inputs.push_back(pin);
        CanvasObject::addChild(pin);
        ISerializable::addChild(pin);
    }
    updateGeometry();
}
size_t Gate::getInputCount() const
{
    return m_inputs.size();
}
void Gate::setOutputCount(size_t outputs)
{
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        ISerializable::removeChild(m_outputs[i]);
        deleteChild(m_outputs[i]);
    }
    m_outputs.clear();
    m_outputs.reserve(outputs);


    for(size_t i=0; i<outputs; ++i)
    {
        Pin *pin = new Pin("OUT "+std::to_string(i+1));
        pin->setType(Pin::Type::output);
        pin->setPinNr(i);
        m_outputs.push_back(pin);
        CanvasObject::addChild(pin);
        ISerializable::addChild(pin);
    }
    updateGeometry();
}
size_t Gate::getOutputCount() const
{
    return m_outputs.size();
}
Pin* Gate::getInputPin(size_t pinNr) const
{
    Pin *pin = nullptr;
    bool search = false;
    if(m_inputs.size() > pinNr)
    {
        pin = m_inputs[pinNr];
        if(pin->getPinNr() != pinNr)
            search = true;
    }
    else
        search = true;

    if(search)
    {
        for(size_t i=0; i<m_inputs.size(); ++i)
        {
            pin = m_inputs[pinNr];
            if(pin->getPinNr() == pinNr)
                return pin;
        }
        return nullptr;
    }
    return pin;
}
Pin* Gate::getOutputPin(size_t pinNr) const
{
    Pin *pin = nullptr;
    bool search = false;
    if(m_outputs.size() > pinNr)
    {
        pin = m_outputs[pinNr];
        if(pin->getPinNr() != pinNr)
            search = true;
    }
    else
        search = true;

    if(search)
    {
        for(size_t i=0; i<m_outputs.size(); ++i)
        {
            pin = m_outputs[pinNr];
            if(pin->getPinNr() == pinNr)
                return pin;
        }
        return nullptr;
    }
    return pin;
}

QJsonObject Gate::save() const
{
    // Combine the QJsonObject with the base object of this
    qDebug() << "save() "<<getName().c_str();
    QJsonObject obj = ISerializable::save();

    obj["inputs"] = (int)m_inputs.size();
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        obj["INP_"+QString::number(i)] = m_inputs[i]->save();
    }
    obj["outputs"] = (int)m_outputs.size();
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        obj["OUT_"+QString::number(i)] = m_outputs[i]->save();
    }

    return combine(obj,
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"TL", sfVector2fToString(m_topLeft)},
        {"origin", sfVector2fToString(m_origin)},
        {"size", sfVector2fToString(m_size)},
        {"orientation", m_orientation},
        {"dragEN", m_draggingIsEnabled},
        {"autoSize", m_autoSize},
        {"autoSizeInterval", m_autoSizeInterval},
    });
}
bool Gate::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = ISerializable::read(reader);

    std::string TL;
    std::string origin;
    std::string size;

    success &= extract(reader,TL, "TL");
    success &= extract(reader,origin, "origin");
    success &= extract(reader,size, "size");

    m_topLeft = stringToSfVector2f(TL.c_str());
    m_origin  = stringToSfVector2f(origin.c_str());
    m_size    = stringToSfVector2f(size.c_str());

    int orient = 0;
    success &= extract(reader, orient, "orientation");
    m_orientation = (Orientation)orient;

    success &= extract(reader,m_draggingIsEnabled, "dragEN");
    success &= extract(reader,m_autoSize, "autoSize");
    success &= extract(reader,m_autoSizeInterval, "autoSizeInterval");

    int inputs = 0;
    int outputs = 0;
    success &= extract(reader,inputs, "inputs");
    success &= extract(reader,outputs, "outputs");
    setInputCount(inputs);
    setOutputCount(outputs);

    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        QJsonObject pinObj = reader["INP_"+QString::number(i)].toObject();
        m_inputs[i]->read(pinObj);
    }
    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        QJsonObject pinObj = reader["OUT_"+QString::number(i)].toObject();
        m_outputs[i]->read(pinObj);
    }

    return success;
}
void Gate::postLoad()
{
    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        m_inputs[i]->postLoad();
    }

    updateGeometry();

}

void Gate::onGateDragTo(const sf::Vector2f &worldPos,
                        const sf::Vector2i &)
{

    if(EditingTool::getCurrentTool() == EditingTool::Tool::none)
    {
        m_currentMovingVectorFromStart = m_currentMovingStart - worldPos;
        float length = getVectorLength(m_currentMovingVectorFromStart);
      //  qDebug() <<length;
        if(length >= m_minMovingDistance)
        {
            EditingTool::setCurrentTool(EditingTool::Tool::moveGate);
            EditingTool::setCurrentlyMoving(this);
            m_isDragging = true;
        }
    }
    if(EditingTool::getCurrentlyMoving() == this)
        setPosition(worldPos);
}
void Gate::onRightMouseButtonPressed()
{
    setOrientation((Orientation)((getOrientation()+1)%4));
}

void Gate::onButtonFallingEdge()
{
    if(EditingTool::getCurrentTool() == EditingTool::Tool::removeGate)
    {
        deleteThis();
        return;
    }else if(EditingTool::getCurrentTool() != EditingTool::Tool::moveGate &&
             m_draggingIsEnabled)
    {
        //qDebug() <<"Falling";
        m_currentMovingStart = getPosition();
        m_mouseFollower->setEnabled(true);
        m_mousePressEvent->setEnabled(true);
    }
}
/*
void Gate::onButtonDown()
{

}*/
void Gate::onButtonRisingEdge()
{
    //if(EditingTool::getCurrentTool() == EditingTool::Tool::moveGate &&
    //   EditingTool::getCurrentlyMoving() == this)
    {
        if(EditingTool::getCurrentlyMoving() == this)
        {

            EditingTool::clear();
        }
        //qDebug() << "Rising";

        snapToMouse(false);
    }

}
/*void Gate::onPinButtonFallingEdge()
{
    Pin *sender = qobject_cast<Pin*>(QObject::sender());
    if(sender)
        sender->setValue(!sender->getValue());
}
*/

QSFML::Components::Button *Gate::getGateButton() const
{
    return m_button;
}
Gate::GateDrawable *Gate::getGateDrawable() const
{
    return m_gateDrawable;
}
const std::vector<Pin*> &Gate::getInputPins() const
{
    return m_inputs;
}
const std::vector<Pin*> &Gate::getOutputPins() const
{
    return m_outputs;
}

void Gate::updateGeometry()
{
    // Setze die Orientierung anhand der ausrichtung des Gates.
    Orientation inputOrientation = (Orientation)((m_orientation + 2) %4);


    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        m_inputs[i]->setOrientation(inputOrientation);
    }

    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        m_outputs[i]->setOrientation(m_orientation);
    }
    if(m_autoSize)
    {
        size_t maximum = m_inputs.size();
        if(maximum < m_outputs.size())
            maximum = m_outputs.size();
        m_size.y = (float)maximum * m_autoSizeInterval;
    }

    switch(m_orientation)
    {
        case Orientation::left:
        case Orientation::right:
        {
            m_button->setSize(m_size);
            break;
        }
        case Orientation::up:
        case Orientation::down:
        {
            m_button->setSize(sf::Vector2f(m_size.y,m_size.x));
            break;
        }
    }

    updatePosition();
}
void Gate::updatePosition()
{
    float inputSpacing  = m_size.y / (float)(m_inputs.size()+1);
    float outputSpacing = m_size.y / (float)(m_outputs.size()+1);

    switch(m_orientation)
    {
        case Orientation::left:
        case Orientation::right:
        {
            m_topLeft = m_origin - m_size/2.f;
            m_button->setPos(m_topLeft);

            break;
        }
        case Orientation::up:
        case Orientation::down:
        {
            m_topLeft = m_origin - sf::Vector2f(m_size.y/2.f,m_size.x/2.f);
            m_button->setPos(m_topLeft);
            break;
        }
    }
    m_symbolicText->setPosition(m_origin);
    //qDebug() << "Pos: "<<m_symbolicText->getPosition().x << " "<<m_symbolicText->getPosition().y;

    for(size_t i=0; i<m_inputs.size(); ++i)
    {
        switch(m_orientation)
        {
            case Orientation::left:
            {
                m_inputs[i]->setPosition(m_topLeft + sf::Vector2f(m_size.x,inputSpacing*(i+1)));
                break;
            }
            case Orientation::up:
            {
                m_inputs[i]->setPosition(m_topLeft + sf::Vector2f(inputSpacing*(i+1), m_size.x));
                break;
            }
            case Orientation::right:
            {
                m_inputs[i]->setPosition(m_topLeft + sf::Vector2f(0,inputSpacing*(i+1)));
                break;
            }
            case Orientation::down:
            {
                m_inputs[i]->setPosition(m_topLeft + sf::Vector2f(inputSpacing*(i+1), 0));
                break;
            }
        }
    }

    for(size_t i=0; i<m_outputs.size(); ++i)
    {
        switch(m_orientation)
        {
            case Orientation::left:
            {
                m_outputs[i]->setPosition(m_topLeft + sf::Vector2f(0,outputSpacing*(i+1)));
                break;
            }
            case Orientation::up:
            {
                m_outputs[i]->setPosition(m_topLeft + sf::Vector2f(outputSpacing*(i+1), 0));
                break;
            }
            case Orientation::right:
            {
                m_outputs[i]->setPosition(m_topLeft + sf::Vector2f(m_size.x,outputSpacing*(i+1)));
                break;
            }
            case Orientation::down:
            {
                m_outputs[i]->setPosition(m_topLeft + sf::Vector2f(outputSpacing*(i+1), m_size.x));
                break;
            }
        }
    }

}



Gate::GateDrawable::GateDrawable(const std::string &name)
    : QSFML::Components::Drawable(name)
{

}

void Gate::GateDrawable::draw(sf::RenderTarget& target,
                              sf::RenderStates) const
{
    sf::RectangleShape rectangle;

    rectangle.setPosition(m_gate->m_button->TL());
    rectangle.setSize(m_gate->m_button->getSize());
    rectangle.setFillColor(m_color);

    target.draw(rectangle);
}
