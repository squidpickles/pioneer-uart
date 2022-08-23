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

    /**
     * Main interface for interacting with a Pioneer WYT control MCU.
     * The internal object state, accessed via `is*` and `get*` methods, is
     * populated by `pollState()` or `setStateFromBytes()`.
     * The pending command state, updated using the `set*` methods, is seeded
     * from the internal object state when the first `set*` method is called,
     * and then gets sent to the Pioneer unit on `applySettings()`, or by using
     * `getCommandBytes()` and sending the command manually.
     */
    class PioneerWYT
    {
    public:
        /** Constructs a new Pioneer control object without any serial communications capabilities. */
        PioneerWYT();

        /** Destructor cleans up any pending state command that might have been created, without sending */
        ~PioneerWYT();
#ifdef USE_ARDUINO
        /**
         * Constructs a new Pioneer control object that can communicate with a Pioneer WYT MCU over a serial connection.
         * Note the serial connection must be 9600 baud, `8E1`.
         */
        PioneerWYT(Stream &serial);
        /**
         * Requests a report of the current state from the WYT's MCU over the serial connection, and updates
         * this object's internal state from the response.
         *
         * @return true on success, false on errors
         */
        bool pollState();
        /**
         * Sends the desired new state to the WYT's MCU in order to change settings.
         * Commands work by sending the entire desired state. Pending command states are built up from the last polled
         * state and any `set*` methods on this module. The caller is responsible for ensuring there has been a recent
         * successful `pollState()` call before changing any settings and calling this method.
         * On a successful call, the existing pending command will be cleared, and `pollState()` will be called to
         * pick up the new state changes from the MCU.
         *
         * @return true on success, false on errors (including those from the `pollState()` call)
         */
        bool applySettings();
#endif
        /** Returns whether the unit's power is on, as of the last state update. */
        bool isPowerOn() const;
        /** Returns whether the unit's "eco" mode is on, as of the last state update. */
        bool isEco() const;
        /** Returns whether the unit's LED display is on, as of the last state update. */
        bool isDisplayOn() const;
        /** Returns whether the unit's "strong" mode is on, as of the last state update. */
        bool isStrong() const;
        /** Returns whether the unit's "health" mode is on, as of the last state update. */
        bool isHealth() const;
        /** Returns whether the unit's beeper is muted, as of the last state update. */
        bool isMute() const;
        /** Returns whether the unit's vent louvers are moving vertically, as of the last state update. */
        bool isVerticalFlow() const;
        /** Returns whether the unit's vent louvers are moving horizontally, as of the last state update. */
        bool isHorizontalFlow() const;
        /** Returns whether the unit's 4-way valve is powered up, as of the last state update. */
        bool isFourWayValveOn() const;
        /** Returns whether the unit's anti-freeze mode is on, as of the last state update. */
        bool isAntifreeze() const;
        /** Returns whether the unit is currently heating, as of the last state update. */
        bool isHeatMode() const;
        /**
         * Fills the array with a byte representation of the pending new state command, if any.
         *
         * @return false if there is no new state command pending.
         */
        bool getCommandBytes(uint8_t bytes[STATE_COMMAND_SIZE]) const;
        /**
         * Sets the state of this object from bytes returned from the serial line.
         * This is analogous to the `pollState()` function, if this instance is managing serial connections.
         * Caller will likely want to call `clearPendingCommand()` after sending any state commands to the unit.
         *
         * @param bytes response from the Pioneer MCU containing the current device state
         */
        void setStateFromBytes(const uint8_t bytes[RESPONSE_SIZE]);
        /** Returns which mode (heat, cooling, fan only, etc) the unit is in, as of the last state update. */
        OpMode getMode() const;
        /** Returns the fan speed setting for the unit, as of the last state update. */
        FanSpeed getChosenFanSpeed() const;
        /** Returns the temperature setting for the unit, as of the last state update. */
        DegreesC getChosenTemperature() const;
        /** Returns the unit's measured indoor temperature, as of the last state update. */
        DegreesC getIndoorTemperature() const;
        /** Returns the unit's measured temperature at the heat exchanger, as of the last state update. */
        DegreesC getIndoorHeatExchangerTemperature() const;
        /** Returns the unit's measured outdoor temperature, as of the last state update. */
        DegreesC getOutdoorTemperature() const;
        /** Returns the unit's measured temperature at the condenser coil, as of the last state update. */
        DegreesC getCondenserCoilTemperature() const;
        /** Returns the unit's measured temperature at the compressor discharge site, as of the last state update. */
        DegreesC getCompressorDischargeTemperature() const;
        /** Returns the unit's measured compressor operation frequency, as of the last state update. */
        uint8_t getCompressorFrequency() const;
        /** Returns the unit's actual fan speed on the indoor unit, as of the last state update. */
        IndoorFanSpeed getIndoorFanSpeed() const;
        /** Returns the unit's actual fan speed on the outdoor unit, as of the last state update. */
        uint8_t getOutdoorFanSpeed() const;
        /** Returns the unit's measured input voltage, as of the last state update. */
        uint8_t getSupplyVoltage() const;
        /** Returns the unit's measured current draw, in amperes, as of the last state update. */
        uint8_t getCurrentUsedAmps() const;
        /** Returns which mode the indoor unit's vent louvers are moving in the vertical direction, as of the last state update. */
        UpDownFlow getUpDownFlow() const;
        /** Returns which mode the indoor unit's vent louvers are moving in the horizontal direction, as of the last state update. */
        LeftRightFlow getLeftRightFlow() const;
        /** Returns which sleep mode is currently active on the unit, as of the last state update. */
        SleepMode getSleepMode() const;

        /** Sets whether the unit's power should be on.
         *
         * @param power `true` to turn on, `false` to turn off.
         */
        void setPowerOn(bool power);
        /** Sets whether the unit's "eco" mode should be on.
         *
         * @param eco `true` to turn on eco mode, `false` to turn off.
         */
        void setEco(bool eco);
        /** Sets whether the unit's LED display should be on.
         *
         * @param display `true` to turn on display, `false` to turn off.
         */
        void setDisplayOn(bool display);
        /** Sets whether the unit's "strong" mode should be on.
         *
         * @param strong `true` to turn on strong mode, `false` to turn off.
         */
        void setStrong(bool strong);
        /** Sets whether the unit's "health" mode should be on.
         *
         * @param health `true` to turn on health mode, `false` to turn off.
         */
        void setHealth(bool health);
        /** Sets whether the unit's beeper (which makes noise when settings change) should be silenced.
         *
         * @param mute `true` to silence the beeper, `false` to enable the beeper.
         */
        void setMute(bool mute);
        /** Sets which mode (heat, cooling, fan only, etc) the unit should be in. */
        void setMode(OpMode mode);
        /** Sets which fan speed the indoor unit should run at. */
        void setChosenFanSpeed(FanSpeed speed);
        /** Sets the desired temperature for the room. */
        void setChosenTemperature(DegreesC temperature);
        /** Sets how the vent louvers should move vertically. */
        void setUpDownFlow(UpDownFlow flow);
        /** Sets how the vent louvers should move horizontally. */
        void setLeftRightFlow(LeftRightFlow flow);
        /** Sets the sleep mode, or turns it off. */
        void setSleepMode(SleepMode sleep);
        /** Clears any settings that have been set, but not applied (sent to the unit). */
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