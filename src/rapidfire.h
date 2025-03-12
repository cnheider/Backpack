#pragma once

#include "module_base.h"
#include "msp.h"
#include "msptypes.h"
#include "rapidFIRE_SPI.h"
#include "rapidFIRE_SPI_Protocol.h"
#include <Arduino.h>
#include <SPI.h>

#define VRX_BOOT_DELAY 2000

#define DELAY_BETWEEN_SPI_PKT 100

#define SPAM_COUNT 3 // rapidfire sometimes missed a pkt, so send 3x

#define MAX_LENGTH_TEXT 25

#define TIMEOUT_SET_OSD 3000

class Rapidfire : public ModuleBase {
public:
  void Init();
  void Loop(uint32_t now);
  void SendIndexCmd(uint8_t index);
  void SendChannelCmd(uint8_t channel);
  void SendBandCmd(uint8_t band);
  void SendHeadTrackingEnableCmd(bool enable);
  void SetRecordingState(uint8_t recordingState, uint16_t delay);
  void SetOSD(mspPacket_t *packet);

private:
    uint32_t m_writeStartMillis = 0;
    void Rapidfire::Write(char text);
};
