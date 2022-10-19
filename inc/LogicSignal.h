#pragma once
#include <QObject>
#include "SFML/Graphics/Color.hpp"

class Connection;
class Pin;


class LogicSignal : public QObject
{
        Q_OBJECT
    public:
        enum Digital
        {
            high = 1,
            low = 0
        };
        LogicSignal(Digital signal);
        LogicSignal(const LogicSignal &other);

        const LogicSignal& operator=(const LogicSignal &other);
        const LogicSignal& operator=(Digital signal);

        LogicSignal getInverted() const;


        void setValue(Digital signal);
        Digital getValue() const;
        const sf::Color &getColor() const;
        static const sf::Color &getColor(Digital value);

        void setHighColor(const sf::Color &color);
        const sf::Color &getHighColor() const;

        void setLowColor(const sf::Color &color);
        const sf::Color &getLowColor() const;

    private:
        Digital m_signal;

        static sf::Color m_highColor;
        static sf::Color m_lowColor;
};
