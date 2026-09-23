/*
 * debug.h
 *
 * Copyright 2021 Victor Chew
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Syslog.h>
#include "soc/rtc.h"

char *getClockName();

#ifdef DEBUG

void init_debug()
{
  Serial.begin(115200);

  rtc_slow_freq_t src = rtc_clk_slow_freq_get();
    switch (src) {
        case RTC_SLOW_FREQ_RTC:
            Serial.println("Slow Clock Source: Internal RC Oscillator (150kHz/136kHz)\n");
            break;
        case RTC_SLOW_FREQ_32K_XTAL:
            Serial.println("Slow Clock Source: External 32kHz Crystal\n");
            break;
        case RTC_SLOW_FREQ_8MD256:
            Serial.println("Slow Clock Source: Internal 8MHz/256 (Divided fast clock)\n");
            break;
        default:
            Serial.println("Slow Clock Source: Unknown\n");
            break;
    }

}

void netdebug(const char *format, ...)
{
  WiFiUDP udpClient;
  Syslog syslog(udpClient, "192.168.1.3", 514, getClockName(), "", LOG_KERN, SYSLOG_PROTO_BSD);

  char buf[1024];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  Serial.println(buf);

  if (WiFi.status() == WL_CONNECTED)
  {
    syslog.log(buf);
    delay(500);
  }
}

void debug(const char *format, ...)
{
  char buf[300];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  va_end(ap);
  Serial.println(buf);
}

#else // !DEBUG

void init_debug() {}
void netdebug(const char *format, ...) {}
void debug(const char *format, ...) {}

#endif // DEBUG

void status(const char *format, ...)
{
  WiFiUDP udpClient;
  Syslog syslog(udpClient, "192.168.1.3", 514, getClockName(), "", LOG_KERN, SYSLOG_PROTO_BSD);

  char buf[1024];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, sizeof(buf), format, ap);
  va_end(ap);

  Serial.println(buf);

  if (WiFi.status() == WL_CONNECTED)
  {
    syslog.log(buf);
    delay(500);
  }
}
