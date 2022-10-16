#include "Clock.h"

Clock::Clock(const std::string &name,
        CanvasObject *parent)
    : Gate(name,parent)
{
    connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
            this, &Clock::onGateButtonFallingEdge);
    setInputCount(0);
    setOutputCount(1);
    m_on = false;
    m_toggle = false;

    qDebug() << "Create "<<name.c_str()<<" gate";

    setText("CLK");



    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &Clock::onTimer);
    setFrequency(2);
}
Clock::Clock(const Clock &other)
    : Gate(other)
{
    connect(getGateButton(),&QSFML::Components::Button::fallingEdge,
            this, &Clock::onGateButtonFallingEdge);
    setInputCount(other.getInputCount());
    setOutputCount(other.getOutputCount());
    m_on = other.m_on;
    m_toggle = other.m_toggle;

    qDebug() << "Create "<<getName().c_str()<<" gate";

    setText(other.getText()->getText());



    m_timer = new QTimer();
    connect(m_timer, &QTimer::timeout, this, &Clock::onTimer);
    setInterval(other.getInterval());
    if(m_on)
        startClock();
    else
        stopClock();
}
Clock::~Clock()
{
    delete m_timer;
}

void Clock::setFrequency(float clkPerSeconds)
{
    if(clkPerSeconds == 0)
        clkPerSeconds = 1;
    m_interval = 500.f/clkPerSeconds;
    m_timer->setInterval((int)m_interval);
}
void Clock::setInterval(float intervalMs)
{
    m_interval = intervalMs;
    if(m_interval < 0)
        m_interval = -m_interval;
    m_timer->setInterval((int)m_interval);
}
float Clock::getFrequency() const
{
    if(m_interval == 0)
        return 9999;
    return 500.f/m_interval;
}
float Clock::getInterval() const
{
    return m_interval;
}
// Serializer
QJsonObject Clock::save() const
{
    // Combine the QJsonObject with the base object of this
    return combine(Gate::save(),
    QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"isOn" ,m_on},
        {"toggle", m_toggle},
        {"interval", m_interval},
    });
}
bool Clock::read(const QJsonObject &reader)
{
    bool success = true;
    // Read the value for the base class
    success = Gate::read(reader);

    // Read the values for this class
    success &= extract(reader,m_on, "isOn");
    success &= extract(reader,m_toggle,"toggle");
    success &= extract(reader,m_interval,"interval");

    setInterval(m_interval);
    if(m_on)
        startClock();
    else
        stopClock();
    return success;
}
void Clock::postLoad()
{

}
void Clock::setInputCount(size_t inputs)
{
    Gate::setInputCount(inputs);
}
void Clock::setOutputCount(size_t outputs)
{
    Gate::setOutputCount(outputs);
}

void Clock::startClock()
{
    m_on = true;
    m_timer->start();
}
void Clock::stopClock()
{
    m_on = false;
    m_timer->stop();
}

void Clock::onGateButtonFallingEdge()
{
    //setOrientation((Orientation)((getOrientation()+1)%4));
    m_on = !m_on;

    if(m_on)
        m_timer->start();
    else
        m_timer->stop();
}
void Clock::onTimer()
{
    m_toggle = !m_toggle;
    std::vector<Pin*> out = getOutputPins();
    for(size_t i=0; i<out.size(); ++i)
    {
        out[i]->setValue((LogicSignal::Digital)m_toggle);
    }
}
