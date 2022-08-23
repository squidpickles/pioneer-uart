#include "pioneer_uart.h"

#define SET_TEMPERATURE_BASE 111

namespace pioneer_uart
{
  PioneerWYT::PioneerWYT() {}
  PioneerWYT::~PioneerWYT() {
    clearPendingCommand();
  }
  #ifdef USE_ARDUINO
  PioneerWYT::PioneerWYT(Stream &serial) : m_serial(&serial) {}
  bool PioneerWYT::pollState()
  {
    if (!m_serial)
    {
      return false;
    }
    command::WytQueryCommand query = command::query_command();
    m_serial->write(query.bytes, QUERY_COMMAND_SIZE);
    m_serial->flush();
    uint8_t state_buf[RESPONSE_SIZE];
    size_t bytes_read = m_serial->readBytes(state_buf, RESPONSE_SIZE);
    if (bytes_read < RESPONSE_SIZE)
    {
      return false;
    }
    m_state = from_bytes(state_buf);
    return true;
  }
  bool PioneerWYT::applySettings()
  {
    if (!m_serial)
    {
      return false;
    }
    if (!m_pending_command)
    {
      return false;
    }
    set_checksum(m_pending_command);
    m_serial->write(m_pending_command->bytes, STATE_COMMAND_SIZE);
    m_serial->flush();
    clearPendingCommand();
    return pollState();
  }
  #endif

  bool PioneerWYT::isPowerOn() const { return m_state.power; }
  bool PioneerWYT::isEco() const { return m_state.eco; }
  bool PioneerWYT::isDisplayOn() const { return m_state.display; }
  bool PioneerWYT::isStrong() const { return m_state.strong; }
  bool PioneerWYT::isHealth() const { return m_state.health; }
  bool PioneerWYT::isMute() const { return m_state.mute; };
  bool PioneerWYT::isVerticalFlow() const { return m_state.vertical_flow; }
  bool PioneerWYT::isHorizontalFlow() const { return m_state.horizontal_flow; }
  bool PioneerWYT::isFourWayValveOn() const { return m_state.four_way_valve_on; }
  bool PioneerWYT::isAntifreeze() const { return m_state.antifreeze; }
  bool PioneerWYT::isHeatMode() const { return m_state.heat_mode; }
  OpMode PioneerWYT::getMode() const { return m_state.mode; }
  FanSpeed PioneerWYT::getChosenFanSpeed() const { return m_state.fan_speed; }
  DegreesC PioneerWYT::getChosenTemperature() const
  {
    return SET_TEMPERATURE_BASE - m_state.set_temperature_whole -
           (m_state.set_temperature_half ? 0.5 : 0);
  }
  DegreesC PioneerWYT::getIndoorTemperature() const
  {
    return m_state.indoor_temp_base * 0.3 - 11.5;
  }
  DegreesC PioneerWYT::getIndoorHeatExchangerTemperature() const
  {
    return m_state.indoor_heat_exchanger_temp * 0.3 - 11.5;
  }
  DegreesC PioneerWYT::getOutdoorTemperature() const
  {
    return static_cast<float>(m_state.outdoor_temp);
  }
  DegreesC PioneerWYT::getCondenserCoilTemperature() const
  {
    return static_cast<float>(m_state.condenser_coil_temp);
  }
  DegreesC PioneerWYT::getCompressorDischargeTemperature() const
  {
    return static_cast<float>(m_state.compressor_discharge_temp);
  }
  uint8_t PioneerWYT::getCompressorFrequency() const
  {
    return m_state.compressor_frequency;
  }
  IndoorFanSpeed PioneerWYT::getIndoorFanSpeed() const
  {
    return m_state.indoor_fan_speed;
  }
  uint8_t PioneerWYT::getOutdoorFanSpeed() const
  {
    return m_state.outdoor_fan_speed;
  }
  uint8_t PioneerWYT::getSupplyVoltage() const
  {
    return m_state.supply_voltage;
  }
  uint8_t PioneerWYT::getCurrentUsedAmps() const
  {
    return m_state.current_used_amps;
  }
  UpDownFlow PioneerWYT::getUpDownFlow() const
  {
    return m_state.up_down_flow;
  }
  LeftRightFlow PioneerWYT::getLeftRightFlow() const
  {
    return m_state.left_right_flow;
  }
  SleepMode PioneerWYT::getSleepMode() const { return m_state.sleep; }

  bool PioneerWYT::getCommandBytes(uint8_t bytes[STATE_COMMAND_SIZE]) const
  {
    if (!m_pending_command)
    {
      return false;
    }
    memcpy(bytes, m_pending_command->bytes, STATE_COMMAND_SIZE);
    set_checksum((command::WytStateCommand *)(bytes));
    return true;
  }

  void PioneerWYT::setStateFromBytes(const uint8_t bytes[RESPONSE_SIZE])
  {
    m_state = from_bytes(bytes);
  }

  void PioneerWYT::clearPendingCommand()
  {
    if (m_pending_command) {
      delete m_pending_command;
      m_pending_command = nullptr;
    }
  }

  void PioneerWYT::initPendingCommand()
  {
    clearPendingCommand();
    m_pending_command = new command::WytStateCommand(command::from_response(m_state));
  }

  void PioneerWYT::setPowerOn(bool power)
  {
    checkOrInitCommand();
    m_pending_command->power = power;
  }
  void PioneerWYT::setEco(bool eco)
  {
    checkOrInitCommand();
    m_pending_command->eco = eco;
  }
  void PioneerWYT::setDisplayOn(bool display)
  {
    checkOrInitCommand();
    m_pending_command->display = display;
  }
  void PioneerWYT::setStrong(bool strong)
  {
    checkOrInitCommand();
    m_pending_command->strong = strong;
  }
  void PioneerWYT::setHealth(bool health)
  {
    checkOrInitCommand();
    m_pending_command->health = health;
  }
  void PioneerWYT::setMute(bool mute)
  {
    checkOrInitCommand();
    m_pending_command->mute = mute;
  }
  void PioneerWYT::setMode(OpMode mode)
  {
    checkOrInitCommand();
    switch (mode)
    {
    case OpMode::Auto:
      m_pending_command->mode = command::OpMode::Auto;
      break;
    case OpMode::Heat:
      m_pending_command->mode = command::OpMode::Heat;
      break;
    case OpMode::Cool:
      m_pending_command->mode = command::OpMode::Cool;
      break;
    case OpMode::Dehumidify:
      m_pending_command->mode = command::OpMode::Dehumidify;
      break;
    case OpMode::Fan:
      m_pending_command->mode = command::OpMode::Fan;
      break;
    }
  }
  void PioneerWYT::setChosenFanSpeed(FanSpeed speed)
  {
    checkOrInitCommand();
    switch (speed)
    {
    case FanSpeed::Auto:
      m_pending_command->fan_speed = command::FanSpeed::Auto;
      break;
    case FanSpeed::High:
      m_pending_command->fan_speed = command::FanSpeed::High;
      break;
    case FanSpeed::Low:
      m_pending_command->fan_speed = command::FanSpeed::Low;
      break;
    case FanSpeed::MidHigh:
      m_pending_command->fan_speed = command::FanSpeed::MidHigh;
      break;
    case FanSpeed::MidLow:
      m_pending_command->fan_speed = command::FanSpeed::MidLow;
      break;
    }
  }
  void PioneerWYT::setChosenTemperature(DegreesC temperature)
  {
    checkOrInitCommand();
    command::set_chosen_temperature_degrees_c(*m_pending_command, temperature);
  }
  void PioneerWYT::setUpDownFlow(UpDownFlow flow)
  {
    checkOrInitCommand();
    switch (flow)
    {
    case UpDownFlow::Auto:
      m_pending_command->up_down_flow = command::UpDownFlow::Auto;
      break;
    case UpDownFlow::TopFix:
      m_pending_command->up_down_flow = command::UpDownFlow::TopFix;
      break;
    case UpDownFlow::UpperFix:
      m_pending_command->up_down_flow = command::UpDownFlow::UpperFix;
      break;
    case UpDownFlow::MiddleFix:
      m_pending_command->up_down_flow = command::UpDownFlow::MiddleFix;
      break;
    case UpDownFlow::LowerFix:
      m_pending_command->up_down_flow = command::UpDownFlow::LowerFix;
      break;
    case UpDownFlow::BottomFix:
      m_pending_command->up_down_flow = command::UpDownFlow::BottomFix;
      break;
    case UpDownFlow::UpDownFlow:
      m_pending_command->up_down_flow = command::UpDownFlow::UpDownFlow;
      break;
    case UpDownFlow::UpFlow:
      m_pending_command->up_down_flow = command::UpDownFlow::UpFlow;
      break;
    case UpDownFlow::DownFlow:
      m_pending_command->up_down_flow = command::UpDownFlow::DownFlow;
      break;
    }
  }
  void PioneerWYT::setLeftRightFlow(LeftRightFlow flow)
  {
    checkOrInitCommand();
    switch (flow)
    {
    case LeftRightFlow::Auto:
      m_pending_command->left_right_flow = command::LeftRightFlow::Auto;
      break;
    case LeftRightFlow::LeftFix:
      m_pending_command->left_right_flow = command::LeftRightFlow::LeftFix;
      break;
    case LeftRightFlow::MiddleLeftFix:
      m_pending_command->left_right_flow = command::LeftRightFlow::MiddleLeftFix;
      break;
    case LeftRightFlow::MiddleFix:
      m_pending_command->left_right_flow = command::LeftRightFlow::MiddleFix;
      break;
    case LeftRightFlow::MiddleRightFix:
      m_pending_command->left_right_flow = command::LeftRightFlow::MiddleRightFix;
      break;
    case LeftRightFlow::RightFix:
      m_pending_command->left_right_flow = command::LeftRightFlow::RightFix;
      break;
    case LeftRightFlow::LeftRightFlow:
      m_pending_command->left_right_flow = command::LeftRightFlow::LeftRightFlow;
      break;
    case LeftRightFlow::LeftFlow:
      m_pending_command->left_right_flow = command::LeftRightFlow::LeftFlow;
      break;
    case LeftRightFlow::MiddleFlow:
      m_pending_command->left_right_flow = command::LeftRightFlow::MiddleFlow;
      break;
    case LeftRightFlow::RightFlow:
      m_pending_command->left_right_flow = command::LeftRightFlow::RightFlow;
      break;
    }
  }
  void PioneerWYT::setSleepMode(SleepMode sleep)
  {
    checkOrInitCommand();
    switch (sleep)
    {
    case SleepMode::Off:
      m_pending_command->sleep = command::SleepMode::Off;
      break;
    case SleepMode::Standard:
      m_pending_command->sleep = command::SleepMode::Standard;
      break;
    case SleepMode::Child:
      m_pending_command->sleep = command::SleepMode::Child;
      break;
    case SleepMode::Elderly:
      m_pending_command->sleep = command::SleepMode::Elderly;
      break;
    }
  }

} // namespace pioneer_uart