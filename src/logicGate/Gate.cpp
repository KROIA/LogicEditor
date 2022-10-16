#include "Gate.h"

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
}
QSFML::Components::Text *Gate::getText() const
{
    return m_symbolicText;
}
void  Gate::setCharacterSize(unsigned int size)
{
    m_symbolicText->setCharacterSize(size);
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
    updateGeomoetry();
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
    updateGeomoetry();
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
    enableMouseDrag(enable);
    if(enable)
    {
        m_isDragging = true;
        m_mouseFollower->setEnabled(m_isDragging);
        m_mousePressEvent->setEnabled(m_isDragging);
    }    
}
void Gate::setInputCount(size_t inputs)
{
    for(size_t i=0; i<m_inputs.size(); ++i)
        deleteChild(m_inputs[i]);
    m_inputs.clear();
    m_inputs.reserve(inputs);


    for(size_t i=0; i<inputs; ++i)
    {
        Pin *pin = new Pin("IN "+std::to_string(i+1));
        pin->setType(Pin::Type::input);
        m_inputs.push_back(pin);
        addChild(pin);
    }
    updateGeomoetry();
}
void Gate::setOutputCount(size_t outputs)
{
    for(size_t i=0; i<m_outputs.size(); ++i)
        deleteChild(m_outputs[i]);
    m_outputs.clear();
    m_outputs.reserve(outputs);


    for(size_t i=0; i<outputs; ++i)
    {
        Pin *pin = new Pin("OUT "+std::to_string(i+1));
        pin->setType(Pin::Type::output);

        m_outputs.push_back(pin);
        addChild(pin);
    }
    updateGeomoetry();
}

void Gate::onGateDragTo(const sf::Vector2f &worldPos,
                        const sf::Vector2i &)
{
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
    }
    if(m_draggingIsEnabled)
    {
        m_isDragging = true;
        m_mouseFollower->setEnabled(m_isDragging);
        m_mousePressEvent->setEnabled(m_isDragging);
    }
}
/*
void Gate::onButtonDown()
{

}*/
void Gate::onButtonRisingEdge()
{
    m_isDragging = false;
    m_mouseFollower->setEnabled(m_isDragging);
    m_mousePressEvent->setEnabled(m_isDragging);
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

void Gate::updateGeomoetry()
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
