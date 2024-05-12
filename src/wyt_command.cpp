#include "wyt_command.h"
#include <string.h>

namespace pioneer_uart
{
  namespace command
  {
    void set_checksum(WytSetStateCommand *command)
    {
      if (!command)
      {
        return;
      }
      uint8_t sum = checksum(*command);
      command->checksum = sum;
    }

    uint8_t checksum(const WytSetStateCommand &command)
    {
      uint8_t result = 0;
      for (size_t idx = 0; idx < STATE_COMMAND_SIZE - 1; ++idx)
      {
        result ^= command.bytes[idx];
      }
      return result;
    }

    WytCommandHeader new_header(const Source &source, const Command &command, const uint8_t size)
    {
      WytCommandHeader header;
      header.magic = 0xbb;
      header.source = source;
      header.command = command;
      header.length = size;
      return header;
    }

    WytQueryCommand query_command()
    {
      static const uint8_t command_bytes[] = {0xbb, 0x00, 0x01, 0x04, 0x02, 0x01, 0x00, 0xbd};
      WytQueryCommand command;
      memcpy(command.bytes, command_bytes, QUERY_COMMAND_SIZE);
      return command;
    }

    WytSetStateCommand from_bytes(const uint8_t buffer[STATE_COMMAND_SIZE])
    {
      WytSetStateCommand command;
      memcpy(command.bytes, buffer, STATE_COMMAND_SIZE);
      return command;
    }
    WytSetStateCommand from_response(const response::WytResponse &response)
    {
      WytSetStateCommand command;
      WytCommandHeader header = new_header(Source::Controller, Command::SetState, 0x1d);
      command.header = header;
      command.eco = response.eco;
      command.display = response.display;
      command.beeper = !response.mute;
      command.power = response.power;
      command.mute = response.mute;
      command.strong = response.strong;
      command.health = response.health;
      switch (response.mode)
      {
      case response::OpMode::Auto:
        command.mode = OpMode::Auto;
        break;
      case response::OpMode::Cool:
        command.mode = OpMode::Cool;
        break;
      case response::OpMode::Dehumidify:
        command.mode = OpMode::Dehumidify;
        break;
      case response::OpMode::Fan:
        command.mode = OpMode::Fan;
        break;
      case response::OpMode::Heat:
        command.mode = OpMode::Heat;
        break;
      }
      command.antifreeze = response.antifreeze;
      command.vertical_flow = response.vertical_flow;
      switch (response.fan_speed)
      {
      case response::FanSpeed::Auto:
        command.fan_speed = FanSpeed::Auto;
        break;
      case response::FanSpeed::High:
        command.fan_speed = FanSpeed::High;
        break;
      case response::FanSpeed::Low:
        command.fan_speed = FanSpeed::Low;
        break;
      case response::FanSpeed::MidHigh:
        command.fan_speed = FanSpeed::MidHigh;
        break;
      case response::FanSpeed::MidLow:
        command.fan_speed = FanSpeed::MidLow;
        break;
      }
      set_chosen_temperature(command, response::get_chosen_temperature_degrees_c(response));
      command.unknown8[0] = 0x80;
      command.sleep = static_cast<SleepMode>(response.sleep);
      command.up_down_flow = static_cast<UpDownFlow>(response.up_down_flow);
      command.left_right_flow = static_cast<LeftRightFlow>(static_cast<uint8_t>(response.left_right_flow) + 0x80);
      command.checksum = checksum(command);
      return command;
    }
  }
}
