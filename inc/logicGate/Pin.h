#pragma once

#include "CanvasObject.h"
#include "Button.h"
#include "Drawable.h"
#include "LogicSignal.h"
#include "EditingTool.h"
#include "ISerializable.h"
#include "sfSerializer.h"

enum Orientation
{
    left,
    up,
    right,
    down
};
class Pin : public QObject, public QSFML::Objects::CanvasObject, public ISerializable
{
        Q_OBJECT
        class PinPainter;
        friend PinPainter;
    public:
        enum Type
        {
            input,
            output
        };


        Pin(const std::string &name = "MyObject",
            CanvasObject *parent = nullptr);
        Pin(const Pin &other);
        ~Pin();

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(Pin)

        void update() override;

       // static void enableConnecterTool(bool enable);
       // static bool isConnectorToolEnabled();

        void setPinNr(size_t nr);
        size_t getPinNr() const;

        void setPosition(const sf::Vector2f &pos);
        const sf::Vector2f &getPosition() const;
        sf::Vector2f getConnectionAnchorPoint() const;
        void setType(Type type);
        Type getType() const;
        void setOrientation(Orientation orientation);
        Orientation getOrientation() const;

        void setSignal(const LogicSignal &val);
        const LogicSignal *getInputSignal();
        const LogicSignal *getOutputSignal();
        void setValue(LogicSignal::Digital val);
        LogicSignal::Digital getInputValue();
        LogicSignal::Digital getOutputValue();

        void setInverted(bool enable);
        bool isInverted() const;

        bool setInputConnection(Connection *con);
        bool addOutputConnection(Connection *con);
        void removeOutputConnection(Connection *con);
        void removeAllOutputConnections();

        Connection *getInputConnection() const;
        std::vector<Connection*> getOutputConnections() const;

        // Serialize
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

        void updateGeometry();

    signals:
        void pinButtonFallingEdge();
        void pinButtonDown();
        void pinButtonRisingEdge();

    private slots:
        void onButtonFallingEdge();
        void onButtonDown();
        void onButtonRisingEdge();

        void onInverterButtonFallingEdge();

        void onConnectionDeleted();
    protected:

    private:


        Type m_type;
        Orientation m_orientation;
        LogicSignal *m_inputValue;
        LogicSignal *m_outputValue;
        sf::Vector2f m_pos;
        sf::Vector2f m_size;
        sf::Vector2f m_topLeft;
        size_t m_pinNr;
        bool m_isInverted;

        //static bool m_connectorToolActive;
        Connection *m_inputConnection;
        std::vector<Connection *> m_outputConnections;

        PinPainter *m_painter;
        QSFML::Components::Button *m_button;
        QSFML::Components::Button *m_inverterButton;

        // Reloading data
        bool m__hasPinConnection;
        std::string m__pinConGateID;
        size_t m__pinConPinNr;

        class PinPainter : public QSFML::Components::Drawable
        {
            public:
                PinPainter(const std::string &name = "PinPainter");

                void draw(sf::RenderTarget& target,
                          sf::RenderStates states) const override;
              //  sf::Color m_frameColor;

                Pin *m_pin;
        };
};
