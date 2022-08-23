#include "wyt_response.h"
#include <string.h>

namespace pioneer_uart
{
  namespace response
  {
    WytResponse from_bytes(const uint8_t buffer[RESPONSE_SIZE])
    {
      WytResponse response;
      memcpy(response.bytes, buffer, RESPONSE_SIZE);
      return response;
    }
  }
}