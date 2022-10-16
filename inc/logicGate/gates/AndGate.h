#pragma once

#include "Gate.h"

class AndGate: public Gate
{
    public:
        AndGate(const std::string &name = "AndGate",
                CanvasObject *parent = nullptr);
        void update() override;

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(AndGate);

        void setInputCount(size_t inputs) override;


    private slots:

        //void onGateButtonFallingEdge();
        //void onPinButtonFallingEdge();
    protected:
        void setOutputCount(size_t outputs) override;

    private:

};
