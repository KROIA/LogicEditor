#include "Pin.h"
#include "Connection.h"
#include "Gate.h"

//bool Pin::m_connectorToolActive = false;
Pin::Pin(const std::string &name,
         CanvasObject *parent)
    : CanvasObject(name, parent)
    , ISerializable()
{
    m_size = sf::Vector2f(15,4);
    m_value = new LogicSignal(LogicSignal::Digital::low);
    m_originalValue = m_value;
    m_type = Type::input;
    m_orientation = Orientation::left;
    m_isInverted = false;
    m_pinNr = 0;

    m_painter = new PinPainter("PinPainter");
    m_painter->m_pin = this;

    m_button = new QSFML::Components::Button("Button",QSFML::Utilities::AABB({0,0},m_size));
    connect(m_button,&QSFML::Components::Button::fallingEdge,this, &Pin::onButtonFallingEdge);
    connect(m_button,&QSFML::Components::Button::down,this, &Pin::onButtonDown);
    connect(m_button,&QSFML::Components::Button::risingEdge,this, &Pin::onButtonRisingEdge);

    m_inverterButton = new QSFML::Components::Button("InverterButton",QSFML::Utilities::AABB({0,0},sf::Vector2f(m_size.x/2.f,m_size.x)));
    connect(m_inverterButton,&QSFML::Components::Button::fallingEdge,this, &Pin::onInverterButtonFallingEdge);

    addComponent(m_painter);
    addComponent(m_button);
    addComponent(m_inverterButton);



    m_inputConnection = nullptr;
}
Pin::Pin(const Pin &other)
    : CanvasObject(other)
{
    m_type = other.m_type;
    m_orientation = other.m_orientation;
    m_value = new LogicSignal(other.m_value->getValue());
    m_originalValue = m_value;
    m_size = other.m_size;
    m_isInverted = other.m_isInverted;
    m_pinNr = other.m_pinNr;

    m_painter = new PinPainter(*other.m_painter);
    m_painter->m_pin = this;

    m_button = new QSFML::Components::Button(*other.m_button);
    connect(m_button,&QSFML::Components::Button::fallingEdge,this, &Pin::onButtonFallingEdge);
    connect(m_button,&QSFML::Components::Button::down,this, &Pin::onButtonDown);
    connect(m_button,&QSFML::Components::Button::risingEdge,this, &Pin::onButtonRisingEdge);

    m_inverterButton = new QSFML::Components::Button(*other.m_inverterButton);
    connect(m_inverterButton,&QSFML::Components::Button::fallingEdge,this, &Pin::onInverterButtonFallingEdge);

    addComponent(m_painter);
    addComponent(m_button);
    addComponent(m_inverterButton);

    setPosition(other.m_pos);
}

Pin::~Pin()
{
    delete m_originalValue;

    qDebug() << "delete Pin";
    if(m_inputConnection)
        m_inputConnection->getParent()->deleteChild(m_inputConnection);
    m_inputConnection = nullptr;
}

void Pin::update()
{

}

/*void Pin::enableConnecterTool(bool enable)
{
    m_connectorToolActive = enable;
}
bool Pin::isConnectorToolEnabled()
{
    return m_connectorToolActive;
}
*/
void Pin::setPinNr(size_t nr)
{
    m_pinNr = nr;
}
size_t Pin::getPinNr() const
{
    return m_pinNr;
}
void Pin::setPosition(const sf::Vector2f &pos)
{
    m_pos = pos;
    updateGeomoetry();


}
const sf::Vector2f &Pin::getPosition() const
{
    return m_pos;
}
sf::Vector2f Pin::getConnectionAnchorPoint() const
{
    switch(m_orientation)
    {
        case Orientation::left:
        {
            return m_pos - sf::Vector2f(m_size.x*0.9f,0);
        }
        case Orientation::up:
        {
            return m_pos - sf::Vector2f(0,m_size.x*0.9f);
        }
        case Orientation::right:
        {
            return m_pos + sf::Vector2f(m_size.x*0.9f,0);
        }
        case Orientation::down:
        {
            return m_pos + sf::Vector2f(0,m_size.x*0.9f);
        }
    }
}
void Pin::setType(Type type)
{
    if(m_type == type) return;
    if(m_type == Type::input)
    {
        setInputConnection(nullptr);
    }
    else
    {
        removeAllOutputConnections();
    }
    m_type = type;

}
Pin::Type Pin::getType() const
{
    return m_type;
}
void Pin::setOrientation(Orientation orientation)
{
    m_orientation = orientation;
    updateGeomoetry();
}
Orientation Pin::getOrientation() const
{
    return m_orientation;
}

void Pin::setSignal(const LogicSignal &val)
{
    if(m_isInverted)
        *m_value = val.getInverted();
    else
        *m_value = val;
}
const LogicSignal *Pin::getSignal() const
{
    if(m_inputConnection)
        return m_inputConnection->getSignal();
    return m_value;
}
void Pin::setValue(LogicSignal::Digital val)
{
    if(m_isInverted)
        *m_value = LogicSignal(val).getInverted();
    else
        *m_value = val;
}
LogicSignal::Digital Pin::getValue() const
{
    if(m_inputConnection)
        return m_inputConnection->getSignal()->getValue();
    return m_value->getValue();
}
void Pin::setInverted(bool enable)
{
    if(m_isInverted == enable)
        return;
    m_isInverted = enable;
    *m_value = m_value->getInverted();
}
bool Pin::isInverted() const
{
    return m_isInverted;
}

bool Pin::setInputConnection(Connection *con)
{
    if(con && (m_inputConnection || m_type == Type::output))
        return false; // Allready connected

    if(con)
    {
        connect(con, &Connection::destroyed, this, &Pin::onConnectionDeleted);
        con->setEndPin(this);
    }
    else if(m_inputConnection)
    {
        disconnect(m_inputConnection, &Connection::destroyed, this, &Pin::onConnectionDeleted);
        m_inputConnection->setEndPin(nullptr);
    }
    m_inputConnection = con;

    return true;
}
bool Pin::addOutputConnection(Connection *con)
{
    if(m_type == Type::input)
        return false;
    for(size_t i=0; i<m_outputConnections.size(); ++i)
    {
        if(m_outputConnections[i] == con)
            return false;
    }
    m_outputConnections.push_back(con);
    con->setStartPin(this);
    connect(con, &Connection::destroyed, this, &Pin::onConnectionDeleted);
    CanvasObject::addChild(con);
    return true;
}
void Pin::removeOutputConnection(Connection *con)
{
    for(size_t i=0; i<m_outputConnections.size(); ++i)
    {
        if(m_outputConnections[i] == con)
        {
            CanvasObject::removeChild(con);
            con->setStartPin(nullptr);
            disconnect(con, &Connection::destroyed, this, &Pin::onConnectionDeleted);
            m_outputConnections.erase(m_outputConnections.begin() +i);
            return;
        }
    }
}
void Pin::removeAllOutputConnections()
{
    for(size_t i=0; i<m_outputConnections.size(); ++i)
    {

        m_outputConnections[i]->setStartPin(nullptr);
        disconnect(m_outputConnections[i], &Connection::destroyed, this, &Pin::onConnectionDeleted);
        deleteChild(m_outputConnections[i]);
    }
    m_outputConnections.clear();
}

Connection *Pin::getInputConnection() const
{
    return m_inputConnection;
}
std::vector<Connection*> Pin::getOutputConnections() const
{
    return m_outputConnections;
}
QJsonObject Pin::save() const
{
    std::string conSourceID;
    size_t conSourceIndex = 0;
    bool hasConnection = false;
    if(m_inputConnection)
    {
        qDebug() << "Get parent from input";
        Gate *parent = dynamic_cast<Gate*>(m_inputConnection->getParent()->getParent());
        if(parent)
        {
            conSourceID = parent->getID();
            if(m_inputConnection->getStartPin() != this)
                conSourceIndex = m_inputConnection->getStartPin()->getPinNr();
            else
                conSourceIndex = m_inputConnection->getEndPin()->getPinNr();
            hasConnection = true;
        }
        else
            qDebug() << "No parent from input";
    }

    // Combine the QJsonObject with the base object of this
    return combine(ISerializable::save(),
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"pinNr", (int)m_pinNr},
     //   {"type" , m_type},
     //   {"orientation", m_orientation},
        {"value", m_value->getValue()},
     //   {"pos", sfVector2fToString(m_pos)},
     //   {"size", sfVector2fToString(m_size)},
        {"inverted", m_isInverted},
        {"hasInpCon", hasConnection},
        {"conSourceID", conSourceID.c_str()},
        {"conPinNr", (int)conSourceIndex},
    });
}
bool Pin::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = ISerializable::read(reader);

    int pinNr = 0;
    success &= extract(reader, pinNr, "pinNr");
    m_pinNr = pinNr;
    int value;
    success &= extract(reader, value, "value");
    m_value->setValue((LogicSignal::Digital)value);
    success &= extract(reader,m_isInverted, "inverted");

    int pinIndex = 0;
    success &= extract(reader,m__hasPinConnection, "hasInpCon");
    success &= extract(reader,m__pinConGateID, "conSourceID");
    success &= extract(reader,pinIndex, "conPinNr");
    m__pinConPinNr = pinIndex;


    return success;
}
void Pin::postLoad()
{
    if(m__hasPinConnection)
    {
        Gate *gate = databaseGetObject<Gate>(m__pinConGateID);
        if(gate)
        {
            Connection::currentlyConnecting = new Connection("Connection");
            Pin *otherGateOutP = gate->getOutputPin(m__pinConPinNr);
            if(!otherGateOutP)
            {
                qDebug() << "Pin::read() Can't find outputPin of source Gate ID: "<<m__pinConGateID.c_str();
                return;
            }
            if(otherGateOutP->addOutputConnection(Connection::currentlyConnecting))
            {
                if(setInputConnection(Connection::currentlyConnecting))
                {
                    Connection::currentlyConnecting = nullptr;
                }
                else
                {
                    qDebug() << "can't add connection";
                }
            }
            else
            {
                delete Connection::currentlyConnecting;
                Connection::currentlyConnecting = nullptr;
                qDebug() << "Pin::read() Can't create connection from Gate ID: "<<m__pinConGateID.c_str();
                return;
            }
        }
        else
        {
            qDebug() << "Pin::read() Can't load inputConnection from Gate ID: "<<m__pinConGateID.c_str();
        }
    }
}

void Pin::updateGeomoetry()
{
    switch(m_orientation)
    {
        case Orientation::left:
        {
            m_button->setSize(m_size);
            m_button->setPos(m_pos - sf::Vector2f(m_size.x,m_size.y/2.f));
            m_inverterButton->setSize(sf::Vector2f(m_size.x/2.f,m_size.y));
            m_inverterButton->setPos(m_pos - sf::Vector2f(m_size.x/2.f,m_size.y/2.f));
            break;
        }
        case Orientation::up:
        {
            m_button->setSize(sf::Vector2f(m_size.y,m_size.x));
            m_button->setPos(m_pos - sf::Vector2f(m_size.y/2.f,m_size.x));
            m_inverterButton->setSize(sf::Vector2f(m_size.y/2.f,m_size.x));
            m_inverterButton->setPos(m_pos - sf::Vector2f(m_size.y/2.f,m_size.x/2.f));

            break;
        }
        case Orientation::right:
        {
            m_button->setSize(m_size);
            m_button->setPos(m_pos - sf::Vector2f(0,m_size.y/2.f));
            m_inverterButton->setSize(sf::Vector2f(m_size.x/2.f,m_size.y));
            m_inverterButton->setPos(m_pos - sf::Vector2f(0,m_size.y/2.f));

            break;
        }
        case Orientation::down:
        {
            m_button->setSize(sf::Vector2f(m_size.y,m_size.x));
            m_button->setPos(m_pos - sf::Vector2f(m_size.y/2.f,0));
            m_inverterButton->setSize(sf::Vector2f(m_size.x/2.f,m_size.y));
            m_inverterButton->setPos(m_pos - sf::Vector2f(m_size.x/2.f,0));

            break;
        }
    }
}

void Pin::onButtonFallingEdge()
{
    if(EditingTool::getCurrentTool() == EditingTool::Tool::none)
    {
        EditingTool::setCurrentTool(EditingTool::Tool::addConnection);
        Connection::currentlyConnecting = new Connection("Connection");
        if(addOutputConnection(Connection::currentlyConnecting))
        {
            //  Connection::currentlyConnecting->set
            //qDebug() << "Creaded new connection";
        }
        else
        {
            delete Connection::currentlyConnecting;
            Connection::currentlyConnecting = nullptr;
        }
    }
    else if(EditingTool::getCurrentTool() == EditingTool::addConnection)
    {
        if(setInputConnection(Connection::currentlyConnecting))
        {
            Connection::currentlyConnecting = nullptr;
            EditingTool::setCurrentTool(EditingTool::Tool::none);
        }
        else
        {
            qDebug() << "can't add connection";
        }

    }
    else
        emit pinButtonFallingEdge();
    /*if(EditingTool::getCurrentTool() == EditingTool::Tool::addConnection)
    {
        //qDebug() << "Pin::onButtonFallingEdge()";
        if(Connection::currentlyConnecting)
        {
            if(setInputConnection(Connection::currentlyConnecting))
            {
                Connection::currentlyConnecting = nullptr;
            }
            else
            {
                qDebug() << "can't add connection";
            }

        }
        else
        {
            Connection::currentlyConnecting = new Connection("Connection");
            if(addOutputConnection(Connection::currentlyConnecting))
            {
              //  Connection::currentlyConnecting->set
                //qDebug() << "Creaded new connection";
            }
            else
            {
                delete Connection::currentlyConnecting;
                Connection::currentlyConnecting = nullptr;
            }
        }
    }
    else
        emit pinButtonFallingEdge();*/
}
void Pin::onButtonDown()
{
    if(EditingTool::getCurrentTool() == EditingTool::Tool::addConnection)
    {

    }
    else
        emit pinButtonDown();
}
void Pin::onButtonRisingEdge()
{
    if(EditingTool::getCurrentTool() == EditingTool::Tool::addConnection)
    {

    }
    else
        emit pinButtonRisingEdge();
}
void Pin::onInverterButtonFallingEdge()
{
    if(EditingTool::getCurrentTool() != EditingTool::addConnection)
        setInverted(!isInverted());
}

void Pin::onConnectionDeleted()
{
    Connection *con = qobject_cast<Connection*>(QObject::sender());
    if(!con) return;
    if(con == m_inputConnection)
    {
        m_inputConnection = nullptr;
        return;
    }
    for(size_t i=0; i<m_outputConnections.size(); ++i)
    {
        if(con == m_outputConnections[i])
        {
            m_outputConnections.erase(m_outputConnections.begin() + i);
            return;
        }
    }
}

Pin::PinPainter::PinPainter(const std::string &name)
    : Drawable(name)
{

}

void Pin::PinPainter::draw(sf::RenderTarget& target,
                           sf::RenderStates) const
{
    //qDebug() << m_pin->m_button->TL().x<<" "<<m_pin->m_button->TL().y <<  "  "
    //         << m_pin->m_button->getSize().x<< " "<<m_pin->m_button->getSize().y;
    float colorFac = 0.7f;
    float outlineThichness = 1.f;

    sf::RectangleShape rectangle;



    sf::Vector2f size = m_pin->m_button->getSize();
    rectangle.setPosition(m_pin->m_button->TL());
    //size.x -= outlineThichness;
    //size.y -= outlineThichness;
    rectangle.setSize(size);

    sf::Color col = m_pin->getSignal()->getColor();
    sf::Color outlineColor((float)col.r * colorFac,
                           (float)col.g * colorFac,
                           (float)col.b * colorFac);
    rectangle.setFillColor(col);
    rectangle.setOutlineThickness(outlineThichness);
    rectangle.setOutlineColor(outlineColor);

    target.draw(rectangle);

    if(m_pin->m_isInverted)
    {
        float radius = size.y*0.6f;
        sf::CircleShape invertedShape(radius);
        invertedShape.setOrigin(radius,radius);
        invertedShape.setFillColor(sf::Color(80,80,80));
        invertedShape.setOutlineThickness(outlineThichness);
        invertedShape.setOutlineColor(outlineColor);
        invertedShape.setPosition(m_pin->getPosition());
        target.draw(invertedShape);
    }
}
