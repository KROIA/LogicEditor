#pragma once

#include <vector>
#include "CanvasObject.h"
#include "Button.h"
#include "Pin.h"
#include "MouseFollower.h"
#include "MousePressEvent.h"
#include "Text.h"
#include "ISerializable.h"

class Gate : public QObject, public QSFML::Objects::CanvasObject, public ISerializable
{
    Q_OBJECT
        class GateDrawable;
        friend GateDrawable;
    public:
        Gate(const Gate &other);
        Gate(const std::string &name = "Gate",
             CanvasObject *parent = nullptr);

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(Gate);

        void update() override;

        void setText(const std::string &text);
        QSFML::Components::Text *getText() const;

        void setCharacterSize(unsigned int size);
        unsigned int getCharacterSize() const;

        void enableAutoSize(bool enable);
        bool autoSizeEnabled() const;

        void setAutoSizeInterval(float interval);
        float getAutoSizeInterval() const;

        void setOrientation(Orientation orientation);
        Orientation getOrientation() const;

        void setPosition(const sf::Vector2f &pos);
        const sf::Vector2f &getPosition() const;

        void setSize(const sf::Vector2f &size);
        const sf::Vector2f &getSize() const;

        void enableMouseDrag(bool enable);
        bool mouseDragEnabled() const;
        void snapToMouse(bool enable);

        virtual void setInputCount(size_t inputs);
        size_t getInputCount() const;
        virtual void setOutputCount(size_t outputs);
        size_t getOutputCount() const;

        Pin* getInputPin(size_t pinNr) const;
        Pin* getOutputPin(size_t pinNr) const;

        // Serializer
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

    private slots:
       void onButtonFallingEdge();
       //void onButtonDown();
       void onButtonRisingEdge();

       //void onPinButtonFallingEdge();
        void onGateDragTo(const sf::Vector2f &worldPos,
                          const sf::Vector2i &pixelPos);

        void onRightMouseButtonPressed();

    protected:

        QSFML::Components::Button *getGateButton() const;
        GateDrawable *getGateDrawable() const;

        const std::vector<Pin*> &getInputPins() const;
        const std::vector<Pin*> &getOutputPins() const;
    private:
        void updateGeomoetry();
        void updatePosition();

        QSFML::Components::Button *m_button;
        QSFML::Components::MouseFollower *m_mouseFollower;
        QSFML::Components::MousePressEvent *m_mousePressEvent;
        QSFML::Components::Text *m_symbolicText;
        GateDrawable *m_gateDrawable;


        sf::Vector2f m_topLeft;
        sf::Vector2f m_origin;
        sf::Vector2f m_size;
        Orientation m_orientation;

        std::vector<Pin*> m_inputs;
        std::vector<Pin*> m_outputs;

        bool m_draggingIsEnabled;
        bool m_isDragging;
        bool m_autoSize;
        float m_autoSizeInterval;

        sf::Vector2f m_currentMovingVectorFromStart;
        sf::Vector2f m_currentMovingStart;
        float m_minMovingDistance;

        class GateDrawable : public QSFML::Components::Drawable
        {
            public:
                GateDrawable(const std::string &name = "GateDrawable");

                void draw(sf::RenderTarget& target,
                          sf::RenderStates states) const override;

                sf::Color m_color;

                Gate *m_gate;
        };

};
