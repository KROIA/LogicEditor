#include "LogicSignal.h"

sf::Color LogicSignal::m_highColor = sf::Color::Red;
sf::Color LogicSignal::m_lowColor  = sf::Color(40,40,40);

LogicSignal::LogicSignal(Digital signal)
   // :   QObject()
{
    m_signal = signal;
}
LogicSignal::LogicSignal(const LogicSignal &other)
   // :   QObject()
{
    m_signal = other.m_signal;
}

const LogicSignal& LogicSignal::operator=(const LogicSignal &other)
{
    m_signal = other.m_signal;
    return *this;
}
const LogicSignal& LogicSignal::operator=(Digital signal)
{
    m_signal = signal;
    return *this;
}

void LogicSignal::setValue(Digital signal)
{
    m_signal = signal;
}
LogicSignal::Digital LogicSignal::getValue() const
{
    return m_signal;
}
LogicSignal LogicSignal::getInverted() const
{
    return LogicSignal((Digital)!(bool)m_signal);
}
const sf::Color &LogicSignal::getColor() const
{
    if(m_signal == Digital::high)
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
