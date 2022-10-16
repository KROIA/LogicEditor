#pragma once

#include "CanvasObject.h"
#include "MouseFollower.h"
#include "Line.h"
#include "LogicSignal.h"


class Connection : public QObject, public QSFML::Objects::CanvasObject
{
    Q_OBJECT
        class ConnectionPainter;
        friend ConnectionPainter;
    public:
        Connection(const std::string &name = "Connection",
                   CanvasObject *parent = nullptr);
        Connection(const Connection &other);
        ~Connection();



        void setStartPos(const sf::Vector2f &pos);
        const sf::Vector2f &getStartPos() const;

        void setEndPos(const sf::Vector2f &pos);
        const sf::Vector2f &getEndPos() const;

        void setThickness(float thickness);
        float getThickness() const;

        void setColor(const sf::Color &color);
        const sf::Color &getColor() const;

        void setStartPin(Pin *start);
        void setEndPin(Pin *end);

        void setSignal(const LogicSignal *signal);
        const LogicSignal *getSignal() const;

        Pin* getStartPin() const;
        Pin* getEndPin() const;

        void update() override;



        static Connection *currentlyConnecting;

    signals:

    private slots:
        void mousePosChanged(const sf::Vector2f &worldPos,
                             const sf::Vector2i &pixelPos);

        void onSignalDeleted();
    protected:

    private:
        //bool m_draggingIsEnabled;
        QSFML::Components::MouseFollower *m_mouseFollower;
        QSFML::Components::Line *m_line;

        Pin *m_startPin;
        Pin *m_endPin;
        const LogicSignal *m_signal;
      //  ConnectionPainter *m_painter;

        bool m_followMouse;



};
