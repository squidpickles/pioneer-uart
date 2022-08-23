#ifndef __PIONEER_UART_H__
#define __PIONEER_UART_H__

#ifdef USE_ARDUINO
#include <Arduino.h>
#endif
#include "wyt_response.h"
#include "wyt_command.h"

namespace pioneer_uart
{

#define WYT_BAUD_RATE 9600

    using DegreesC = float;
    using namespace response;

    class PioneerWYT
    {
    public:
        PioneerWYT();
        ~PioneerWYT();
#ifdef USE_ARDUINO
        PioneerWYT(Stream &serial);
        bool pollState();
        bool applySettings();
#endif
        bool isPowerOn() const;
        bool isEco() const;
        bool isDisplayOn() const;
        bool isStrong() const;
        bool isHealth() const;
        bool isMute() const;
        bool isVerticalFlow() const;
        bool isHorizontalFlow() const;
        bool isFourWayValveOn() const;
        bool isAntifreeze() const;
        bool isHeatMode() const;
        /**
         * @brief Fills the array with a byte representation of the pending new state command, if any.
         *
         * Returns `true` on success, `false` on error or if there is no new state command pending.
         */
        bool getCommandBytes(uint8_t bytes[STATE_COMMAND_SIZE]) const;
        /**
         * @brief Sets the state of this object from bytes returned from the serial line
         *
         * @param bytes response from the Pioneer MCU containing the current device state
         */
        void setStateFromBytes(const uint8_t bytes[RESPONSE_SIZE]);
        OpMode getMode() const;
        FanSpeed getChosenFanSpeed() const;
        DegreesC getChosenTemperature() const;
        DegreesC getIndoorTemperature() const;
        DegreesC getIndoorHeatExchangerTemperature() const;
        DegreesC getOutdoorTemperature() const;
        DegreesC getCondenserCoilTemperature() const;
        DegreesC getCompressorDischargeTemperature() const;
        uint8_t getCompressorFrequency() const;
        IndoorFanSpeed getIndoorFanSpeed() const;
        uint8_t getOutdoorFanSpeed() const;
        uint8_t getSupplyVoltage() const;
        uint8_t getCurrentUsedAmps() const;
        UpDownFlow getUpDownFlow() const;
        LeftRightFlow getLeftRightFlow() const;
        SleepMode getSleepMode() const;

        void setPowerOn(bool power);
        void setEco(bool eco);
        void setDisplayOn(bool display);
        void setStrong(bool strong);
        void setHealth(bool health);
        void setMute(bool mute);
        void setMode(OpMode mode);
        void setChosenFanSpeed(FanSpeed speed);
        void setChosenTemperature(DegreesC temperature);
        void setUpDownFlow(UpDownFlow flow);
        void setLeftRightFlow(LeftRightFlow flow);
        void setSleepMode(SleepMode sleep);
        void clearPendingCommand();

    private:
        WytResponse m_state;
        command::WytStateCommand *m_pending_command;
#ifdef USE_ARDUINO
        Stream *m_serial;
#endif

        void initPendingCommand();
        inline void checkOrInitCommand()
        {
            if (!m_pending_command)
            {
                initPendingCommand();
            }
        }
    };

}
#endif