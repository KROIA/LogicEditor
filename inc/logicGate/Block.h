#pragma once

#include "CanvasObject.h"
#include "ISerializable.h"
#include "Gate.h"

class BlockGate;
class Block: public QObject, public QSFML::Objects::CanvasObject, public ISerializable
{
    Q_OBJECT
        friend BlockGate;
    public:
        Block(const std::string &name = "Block",
             CanvasObject *parent = nullptr);
        Block(const Block &other);

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(Block);

        void setEditMode(bool enable);
        bool isInEditMode() const;

        void addGate(Gate *gate);
        void removeGate(Gate *gate);
        void deleteGate(Gate *gate);

        void update() override;

        // Iserializable
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;

        void generateBlock();
    private slots:
        void onChildGateDeleted();

    private:
        void setInputSignal(unsigned int pinNr, const LogicSignal &signal);
        const LogicSignal &GetOutputSignal(unsigned int pinNr) const;


        void connectGateSignals(Gate *gate);
        void disconnectGateSignals(Gate *gate);

        BlockGate *m_gate;
        QSFML::Objects::CanvasObject *m_innerParent;
        size_t m_inputs, m_outputs;
        std::string m_name;
        bool m_isInEditMode;

        // generated
        std::vector<BlockPin* > m_inputsGates;
        std::vector<BlockPin* > m_outputsGates;
};
