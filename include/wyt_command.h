#ifndef __WYT_COMMAND_H__
#define __WYT_COMMAND_H__

#include <stdint.h>
#include <assert.h>
#include "wyt_response.h"
#define HEADER_SIZE 5
#define STATE_COMMAND_SIZE 35
#define QUERY_COMMAND_SIZE 8

namespace pioneer_uart
{
    namespace command
    {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

        enum class Source : uint16_t
        {
            Controller = 0x0001,
            Appliance = 0x0100,
        };

        enum class Command : uint8_t
        {
            SetState = 0x03,
            QueryState = 0x04,
        };

        enum class OpMode : uint8_t
        {
            Heat = 0x01,
            Dehumidify = 0x02,
            Cool = 0x03,
            Fan = 0x07,
            Auto = 0x08,
        };

        enum class FanSpeed : uint8_t
        {
            Auto = 0x00,
            Low = 0x02,
            Medium = 0x03,
            High = 0x05,
            MidLow = 0x06,
            MidHigh = 0x07,
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
            Auto = 0x80,
            LeftFix = 0x81,
            MiddleLeftFix = 0x82,
            MiddleFix = 0x83,
            MiddleRightFix = 0x84,
            RightFix = 0x85,
            LeftRightFlow = 0x88,
            LeftFlow = 0x90,
            MiddleFlow = 0x98,
            RightFlow = 0xa0,
        };

        union WytCommandHeader
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
                uint8_t length;
            } __attribute__((packed));
            uint8_t bytes[HEADER_SIZE];
        };

        union WytQueryCommand
        {
            struct
            {
                // 00..04
                WytCommandHeader header;
                // 05..06
                uint8_t command;
                // 07
                uint8_t checksum;
            } __attribute__((packed));
            uint8_t bytes[QUERY_COMMAND_SIZE];
        };

        WytQueryCommand query_command();

        typedef union
        {
            struct
            {
                // 00..04
                WytCommandHeader header;
                // 05..06
                uint8_t unknown1[2];
                // 07
                bool eco : 1;
                bool display : 1;
                bool beeper : 1;
                bool unknown2 : 2;
                bool power : 1;
                uint8_t unknown3 : 2;
                // 08
                bool mute : 1;
                bool strong : 1;
                bool unknown4 : 1;
                bool health : 1;
                OpMode mode : 4;
                // 09
                uint8_t set_temperature_whole;
                // 0a
                bool antifreeze : 1;
                bool unknown5 : 1;
                uint8_t vertical_flow : 3;
                FanSpeed fan_speed : 3;
                // 0b
                uint8_t unknown6 : 5;
                bool set_temperature_half : 1;
                uint8_t unknown7 : 2;
                // 0c..12
                uint8_t unknown8[7];
                // 13
                SleepMode sleep;
                // 14..1f
                uint8_t unknown9[12];
                // 20
                UpDownFlow up_down_flow;
                // 21
                LeftRightFlow left_right_flow;
                // 22
                uint8_t checksum;
            } __attribute__((packed));
            uint8_t bytes[STATE_COMMAND_SIZE];
        } WytStateCommand;

        inline void set_chosen_temperature_degrees_c(WytStateCommand &command, const float temp)
        {
            assert(temp >= 10.0 && temp <= 30.0);

            uint8_t temp_double = static_cast<uint8_t>(temp * 2);
            uint8_t temp_whole = temp_double / 2;
            bool temp_half = temp_double % 2;
            command.set_temperature_whole = temp_whole + 111;
            command.set_temperature_half = temp_half;
        }
        uint8_t checksum(const WytStateCommand &command);
        void set_checksum(WytStateCommand *command);
        WytStateCommand from_bytes(const uint8_t buffer[STATE_COMMAND_SIZE]);
        WytStateCommand from_response(const pioneer_uart::response::WytResponse &response);

        WytCommandHeader new_header(const Source &source, const Command &command, const uint8_t size);
    }
#else
#error "Only little-endian architecture supported right now"
#endif

}
#endif