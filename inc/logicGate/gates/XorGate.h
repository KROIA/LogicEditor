#pragma once

#include "Gate.h"

class XorGate: public Gate
{
    public:
        XorGate(const std::string &name = "XorGate",
                CanvasObject *parent = nullptr);
        void update() override;

        void setInputCount(size_t inputs) override;


    private slots:

    protected:
        void setOutputCount(size_t outputs) override;

    private:

};
