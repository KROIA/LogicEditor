#pragma once

#include "Gate.h"

class InputGate: public Gate
{
    public:
        InputGate(const std::string &name = "InputGate",
                CanvasObject *parent = nullptr);

        InputGate(const InputGate &other);
        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(InputGate);

        void switchOn();
        void switchOff();

        // Serializer
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

    private slots:
        void onGateButtonFallingEdge();
        void onGateButtonRisingEdge();
    protected:
        void setInputCount(size_t inputs) override;
        void setOutputCount(size_t outputs) override;

    private:
        bool m_on;

       // bool m_tryToSwitch;
};
