#ifndef __WYT_RESPONSE_H__
#define __WYT_RESPONSE_H__

#include <stdint.h>
#define RESPONSE_SIZE 61

namespace pioneer_uart
{
    namespace response
    {

        enum class Source : uint16_t
        {
            Controller = 0x0001,
            Appliance = 0x0100,
        };

        enum class Command : uint8_t
        {
            ResponseToCommand = 0x03,
            ResponseToQuery = 0x04,
        };

        enum class OpMode : uint8_t
        {
            Cool = 0x01,
            Fan = 0x02,
            Dehumidify = 0x03,
            Heat = 0x04,
            Auto = 0x05,
        };

        enum class FanSpeed : uint8_t
        {
            Auto = 0x00,
            Low = 0x01,
            Medium = 0x02,
            MidLow = 0x04,
            MidHigh = 0x05,
            High = 0x03,
        };

        enum class SleepMode : uint8_t
        {
            Off = 0x00,
            Standard = 0x01,
            Elderly = 0x02,
            Child = 0x03,
        };

        enum class UpDownFlow : uint8_t
        {
            Auto = 0x00,
            TopFix = 0x01,
            UpperFix = 0x02,
            MiddleFix = 0x03,
            LowerFix = 0x04,
            BottomFix = 0x05,
            UpDownFlow = 0x08,
            UpFlow = 0x10,
            DownFlow = 0x18,
        };

        enum class LeftRightFlow : uint8_t
        {
            Auto = 0x00,
            LeftFix = 0x01,
            MiddleLeftFix = 0x02,
            MiddleFix = 0x03,
            MiddleRightFix = 0x04,
            RightFix = 0x05,
            LeftRightFlow = 0x08,
            LeftFlow = 0x10,
            MiddleFlow = 0x18,
            RightFlow = 0x20,
        };

        enum class IndoorFanSpeed : uint8_t
        {
            Off = 0x00,
            Low = 0x3c,
            Medium = 0x55,
            High = 0x62,
        };

        enum class OutdoorStatus : uint8_t
        {
            No = 0x00,
            Yes = 0x0a,
        };

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        /** Holds a response from the WYT MCU describing its current operating state. */
        typedef union
        {
            struct
            {
                // 00
                uint8_t magic;
                // 01..02
                Source source;
                // 03
                Command command;
                // 04
                uint8_t command_length;
                // 05..06
                uint8_t unknown1[2];
                // 07
                OpMode mode : 3;
                bool unknown2 : 1;
                bool power : 1;
                bool display : 1;
                bool eco : 1;
                bool strong : 1;
                // 08
                uint8_t set_temperature_whole : 4;
                FanSpeed fan_speed : 3;
                bool unknown3 : 1;
                // 09
                bool set_temperature_half : 1;
                bool unknown7 : 1;
                bool health : 1;
                uint8_t unknown6 : 5;
                // 0A
                uint8_t unknown10 : 5;
                bool horizontal_flow : 1;
                bool vertical_flow : 1;
                bool unknown9 : 1;
                // 0B..10
                uint8_t unknown11[6];
                // 11
                uint8_t indoor_temp_base;
                // 12
                uint8_t unknown12;
                // 13
                SleepMode sleep : 2;
                uint8_t unknown13 : 5;
                bool four_way_valve_on : 1;
                // 14..1d
                uint8_t unknown14[10];
                // 1e
                uint8_t indoor_heat_exchanger_temp;
                // 1f
                uint8_t unknown15;
                // 20
                uint8_t unknown16 : 7;
                bool antifreeze : 1;
                // 21
                uint8_t unknown17 : 7;
                bool mute : 1;
                // 22
                IndoorFanSpeed indoor_fan_speed;
                // 23
                uint8_t outdoor_temp;
                // 24
                uint8_t condenser_coil_temp;
                // 25
                uint8_t compressor_discharge_temp;
                // 26
                uint8_t compressor_frequency;
                // 27
                uint8_t outdoor_fan_speed;
                // 28
                OutdoorStatus outdoor_stuff_running : 4;
                uint8_t unknown19 : 2;
                bool heat_mode : 1;
                bool unknown18 : 1;
                // 29..2c
                uint8_t unknown20[4];
                // 2d
                uint8_t supply_voltage;
                // 2e
                uint8_t current_used_amps;
                // 2f..32
                uint8_t unknown21[4];
                // 33
                UpDownFlow up_down_flow;
                // 34
                LeftRightFlow left_right_flow;
                // 35..3b
                uint8_t unknown22[8];
            } __attribute__((packed));
            uint8_t bytes[RESPONSE_SIZE];
        } WytResponse;

#else
#error "Only little-endian architecture supported right now"
#endif

        inline float get_chosen_temperature_degrees_c(const WytResponse &state)
        {
            return 16 + state.set_temperature_whole + (state.set_temperature_half ? 0.5 : 0);
        }

        WytResponse from_bytes(const uint8_t buffer[RESPONSE_SIZE]);

    }
}
#endif