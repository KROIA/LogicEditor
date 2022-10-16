#pragma once

#include <QTimer>
#include "Gate.h"

class Clock: public Gate
{
    public:
        Clock(const std::string &name = "Clock",
                CanvasObject *parent = nullptr);
        ~Clock();

        void setFrequency(float clkPerSeconds);
        void setInterval(float intervalMs);
        float getFrequency() const;
        float getInterval() const;

    public slots:
        void startClock();
        void stopClock();

    private slots:
        void onGateButtonFallingEdge();
        void onTimer();

    protected:
        void setInputCount(size_t inputs) override;
        void setOutputCount(size_t outputs) override;

    private:
        bool m_on;
        bool m_toggle;
        float m_interval;
        QTimer *m_timer;
};
