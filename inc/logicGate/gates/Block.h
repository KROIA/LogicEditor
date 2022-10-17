#pragma once

#include "Gate.h"
#include "InputGate.h"
#include "OutputGate.h"


class Block: public QObject, public QSFML::Objects::CanvasObject, public ISerializable
{
    Q_OBJECT
public:
    Block(const std::string &name = "Block",
            CanvasObject *parent = nullptr);
    Block(const Block &other);
    void update() override;

    IMPLEMENT_ISERIALIZABLE_CONST_FUNC(Block);

    void switchViewToOutside(bool toOutside);
    bool isViewOutside() const;

    void addGate(Gate *gate);

    QJsonObject save() const override;
    bool read(const QJsonObject &reader) override;
    void postLoad() override;


    static Block *currentEditingBlock;
private slots:
    void onGateDeleted();
    void onInputDeleted(InputGate *gate, Pin *pin);
    void onOutputDeleted(OutputGate *gate, Pin *pin);
    //void onGateButtonFallingEdge();
    //void onPinButtonFallingEdge();
protected:
    //void setOutputCount(size_t outputs) override;

private:
    void updateOutsideGate();

    bool m_isOutside;
    Gate *m_thisOutsideGate;
    QSFML::Objects::CanvasObject *m_insideObj;
    //ISerializable *m_insideSerializable;

    std::vector<std::string> m_loadingInputIDs;
    std::vector<std::string> m_loadingOutputIDs;

};
