#include "LogicSignal.h"

sf::Color LogicSignal::m_highColor = sf::Color::Red;
sf::Color LogicSignal::m_lowColor  = sf::Color(40,40,40);

LogicSignal::LogicSignal(Digital signal)
   // :   QObject()
{
    m_signal = signal;
    //m_inverted = false;
}
LogicSignal::LogicSignal(const LogicSignal &other)
   // :   QObject()
{
    m_signal = other.m_signal;
    //m_inverted = other.m_inverted;
}

const LogicSignal& LogicSignal::operator=(const LogicSignal &other)
{
    m_signal = other.m_signal;
  //  m_inverted = other.m_inverted;
    return *this;
}
const LogicSignal& LogicSignal::operator=(Digital signal)
{
    m_signal = signal;
    return *this;
}

/*void LogicSignal::setInverted(bool inverted)
{
    m_inverted = inverted;
}
bool LogicSignal::isInverted() const
{
    return m_inverted;
}*/
void LogicSignal::setValue(Digital signal)
{
    m_signal = signal;
}
LogicSignal::Digital LogicSignal::getValue() const
{
    //if(m_inverted)
    //    return (Digital)!(bool)m_signal;
    return m_signal;
}
LogicSignal LogicSignal::getInverted() const
{
    return LogicSignal((Digital)!(bool)m_signal);
}
const sf::Color &LogicSignal::getColor() const
{
    return getColor(m_signal);
}
const sf::Color &LogicSignal::getColor(Digital value)
{
    if(value == Digital::high)
        return m_highColor;
    return m_lowColor;
}

void LogicSignal::setHighColor(const sf::Color &color)
{
    m_highColor = color;
}
const sf::Color &LogicSignal::getHighColor() const
{
    return m_highColor;
}

void LogicSignal::setLowColor(const sf::Color &color)
{
    m_lowColor = color;
}
const sf::Color &LogicSignal::getLowColor() const
{
    return m_lowColor;
}
