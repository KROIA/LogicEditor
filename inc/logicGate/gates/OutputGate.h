#pragma once

#include "Gate.h"
#include "LogicSignal.h"

class OutputGate: public Gate
{
    Q_OBJECT
public:
    OutputGate(const std::string &name = "OutputGate",
              CanvasObject *parent = nullptr);

    OutputGate(const OutputGate &other);
    ~OutputGate();
    IMPLEMENT_ISERIALIZABLE_CONST_FUNC(OutputGate);

    void update() override;

    void setPin(Pin *pin);
    Pin *getPin() const;

    const LogicSignal &getSignal();

    // Serializer
    QJsonObject save() const override;
    bool read(const QJsonObject &reader) override;
    void postLoad() override;

signals:
    void getsDeleted(OutputGate *gate, Pin *pin);
private slots:

protected:
    void setInputCount(size_t inputs) override;
    void setOutputCount(size_t outputs) override;

private:
    Pin *m_pin;
    Pin *m_externPin;
};
