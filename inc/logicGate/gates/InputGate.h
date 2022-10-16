#pragma once

#include "Gate.h"

class InputGate: public Gate
{
    public:
        InputGate(const std::string &name = "InputGate",
                CanvasObject *parent = nullptr);

    private slots:
        void onGateButtonFallingEdge();
    protected:
        void setInputCount(size_t inputs) override;
        void setOutputCount(size_t outputs) override;

    private:
        bool m_on;
};
