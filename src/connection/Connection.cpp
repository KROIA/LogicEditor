
#include "Connection.h"
#include "Pin.h"
#include "EditingTool.h"

Connection *Connection::currentlyConnecting = nullptr;
Connection::Connection(const std::string &name,
                       CanvasObject *parent)
    : CanvasObject(name, parent)
{
    m_mouseFollower = new QSFML::Components::MouseFollower("MouseFollower");
    connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
            this, &Connection::mousePosChanged);

   // m_painter = new ConnectionPainter("ConnectionPainter");
    m_line = new QSFML::Components::Line("Line");

    addComponent(m_mouseFollower);
    addComponent(m_line);

    setThickness(1);


    m_followMouse = true;
    m_signal = nullptr;
    m_startPin = nullptr;
    m_endPin = nullptr;

    m_line->setEnabled(false);
}
Connection::Connection(const Connection &other)
    : CanvasObject(other)
{
    m_followMouse = other.m_followMouse;
    m_signal = nullptr;
    m_startPin = nullptr;
    m_endPin = nullptr;
    m_line = new QSFML::Components::Line(*other.m_line);

    m_mouseFollower = new QSFML::Components::MouseFollower(*other.m_mouseFollower);
    connect(m_mouseFollower, &QSFML::Components::MouseFollower::mousePosChanged,
            this, &Connection::mousePosChanged);

    setThickness(other.getThickness());
    addComponent(m_mouseFollower);
    addComponent(m_line);
}
Connection::~Connection()
{
    qDebug() << "delete Connection";
}


void Connection::setStartPos(const sf::Vector2f &pos)
{
    m_line->setStartPos(pos);
}
const sf::Vector2f &Connection::getStartPos() const
{
    return m_line->getStartPos();
}

void Connection::setEndPos(const sf::Vector2f &pos)
{
    m_line->setEndPos(pos);
}
const sf::Vector2f &Connection::getEndPos() const
{
    return m_line->getEndPos();
}
void Connection::setThickness(float thickness)
{
    m_line->setThickness(thickness);
}
float Connection::getThickness() const
{
    return m_line->getThickness();
}

void Connection::setColor(const sf::Color &color)
{
    m_line->setColor(color);
}
const sf::Color &Connection::getColor() const
{
    return m_line->getColor();
}
void Connection::setStartPin(Pin *start)
{
    m_startPin = start;
    if(m_startPin)
        setSignal(m_startPin->getSignal());
}
void Connection::setEndPin(Pin *end)
{
    m_endPin = end;
    m_followMouse = false;
    m_mouseFollower->setEnabled(false);
    if(!m_line->isEnabled())
        m_line->setEnabled(true);
}
void Connection::setSignal(const LogicSignal *signal)
{
    if(m_signal)
        disconnect(m_signal, &LogicSignal::destroyed, this, &Connection::onSignalDeleted);
    m_signal = signal;
    if(m_signal)
        connect(m_signal, &LogicSignal::destroyed, this, &Connection::onSignalDeleted);
}
const LogicSignal *Connection::getSignal() const
{
    return m_signal;
}
Pin* Connection::getStartPin() const
{
    return m_startPin;
}
Pin* Connection::getEndPin() const
{
    return m_endPin;
}
void Connection::update()
{
    if(!m_endPin)
    {
        if(EditingTool::getCurrentTool() != EditingTool::Tool::addConnection &&
           currentlyConnecting == this)
        {
            currentlyConnecting = nullptr;
            deleteThis();
        }
    }
    if(m_startPin)
        m_line->setStartPos(m_startPin->getConnectionAnchorPoint());

    if(m_endPin && !m_followMouse)
        m_line->setEndPos(m_endPin->getConnectionAnchorPoint());

    if(m_signal)
        m_line->setColor(m_signal->getColor());
}
void Connection::mousePosChanged(const sf::Vector2f &worldPos,
                                 const sf::Vector2i &)
{
    //qDebug() << "MousePos: "<<worldPos.x << " "<<worldPos.y;
    setEndPos(worldPos);
    if(!m_line->isEnabled())
        m_line->setEnabled(true);
}
void Connection::onSignalDeleted()
{
    deleteThis();
    //QObject::deleteLater();
}




