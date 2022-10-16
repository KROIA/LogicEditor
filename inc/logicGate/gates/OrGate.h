#pragma once

#include "Gate.h"

class OrGate: public Gate
{
    public:
        OrGate(const std::string &name = "OrGate",
                CanvasObject *parent = nullptr);
        void update() override;

        void setInputCount(size_t inputs) override;


    private slots:

    protected:
        void setOutputCount(size_t outputs) override;

    private:

};
