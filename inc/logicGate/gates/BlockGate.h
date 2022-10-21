#pragma once

#include "Gate.h"
#include "Block.h"

//class Block;
class BlockGate: public Gate
{
      //  friend Block;
    public:
        BlockGate(const std::string &name = "BlockGate",
                CanvasObject *parent = nullptr);
        void update() override;

        IMPLEMENT_ISERIALIZABLE_CONST_FUNC(BlockGate);

        void setEditMode(bool enable) override;

        void setBlock(Block *block);
        Block *getBlock() const;

        // Serializer
        QJsonObject save() const override;
        bool read(const QJsonObject &reader) override;
        void postLoad() override;


    private slots:

    protected:
       // void setInputCount(size_t inputs) override;
       // void setOutputCount(size_t outputs) override;

    private:
        Block *m_parentBlock;

};
