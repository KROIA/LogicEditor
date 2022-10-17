#pragma once

#include "Gate.h"

class InputGate: public Gate
{
    Q_OBJECT
    public:
        InputGate(const std::string &name = "InputGate",
                CanvasObject *parent = nullptr);

        InputGate(const InputGate &other);
        ~InputGate();
        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(InputGate);

        void update() override;

        void setPin(Pin *pin);
        Pin *getPin() const;

        //void switchOn();
        //void switchOff();

        const LogicSignal &getSignal();

        // Serializer
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

    signals:
        void getsDeleted(InputGate *gate, Pin *pin);
    private slots:
       // void onGateButtonFallingEdge();
    protected:
        void setInputCount(size_t inputs) override;
        void setOutputCount(size_t outputs) override;

    private:
        //bool m_on;
        Pin *m_pin;
        Pin *m_externPin;
};
