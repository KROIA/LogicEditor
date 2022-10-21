#pragma once

#include "Pin.h"
#include "Gate.h"

class BlockPin : public Gate
{
    public:
        BlockPin(const std::string &name = "BlockPin",
                CanvasObject *parent = nullptr);
        BlockPin(const BlockPin &other);

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(BlockPin);

        void update() override;

        void setPinNr(unsigned int nr);
        unsigned int getPinNr() const;

        void setType(Pin::Type type);
        Pin::Type getPinType() const;

        void setSignal(const LogicSignal &signal);
        const LogicSignal &getSignal() const;

        // Serializer
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

    private slots:

    protected:
        void setInputCount(size_t inputs) override;
        void setOutputCount(size_t outputs) override;

    private:
        Pin::Type m_type;
        unsigned int m_pinNr;
        //LogicSignal m_signal;
};
