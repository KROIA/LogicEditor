#pragma once

#include "Gate.h"

class NotGate: public Gate
{
    public:
        NotGate(const std::string &name = "NotGate",
                CanvasObject *parent = nullptr);

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(NotGate);

        void update() override;

        void setInputCount(size_t inputs) override;


    private slots:

    protected:
        void setOutputCount(size_t outputs) override;

    private:

};
