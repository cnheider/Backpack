#pragma once

#include "module_base.h"
#include "msp.h"
#include "msptypes.h"
#include "rapidFIRE_SPI.h"
#include "rapidFIRE_SPI_Protocol.h"
#include <Arduino.h>
#include <SPI.h>

#define VRX_BOOT_DELAY 2000

#define BIT_BANG_FREQ 1000

#define DELAY_BETWEEN_SPI_PKT 100

#define SPAM_COUNT 3 // rapidfire sometimes missed a pkt, so send 3x

#define RF_API_DIR_GRTHAN 0x3E // '>'
#define RF_API_BEEP_CMD 0x53   // 'S'

#define RF_API_DIR_QUEST 0x3F    // '?'
#define RF_API_RSSI_CMD 0x52     // 'R'
#define RF_API_VOLTAGE_CMD 0x56  // 'V'
#define RF_API_FIRMWARE_CMD 0x46 // 'F'

#define RF_API_DIR_EQUAL 0x3D     // '='
#define RF_API_TEXT_CMD 0x54      // 'T'
#define RF_API_CHANNEL_CMD 0x43   // 'C'
#define RF_API_BAND_CMD 0x42      // 'B'
#define RF_API_MODULE_CMD 0x4D    // 'M'
#define RF_API_DIVERSITY_CMD 0x44 // 'D'

// #define RF_API_DIR_MISTAKE_CMD 0x00 // ???
#define RF_API_OSD_MODE_CMD 0x4F // 'O'

class Rapidfire : public ModuleBase {
public:
  void Init();
  void SendIndexCmd(uint8_t index);
  void SendChannelCmd(uint8_t channel);
  void SendBandCmd(uint8_t band);
  void SetOSD(mspPacket_t *packet);
  void SetRecordingState(uint8_t recordingState, uint16_t delay);
  // void SetRTC();
  void SendHeadTrackingEnableCmd(bool enable);
  // void SendLinkTelemetry(uint8_t *rawCrsfPacket);
  // void SendBatteryTelemetry(uint8_t *rawCrsfPacket);
  // void SendGpsTelemetry(crsf_packet_gps_t *packet) {}

  enum BAND {
    BAND_F = 0x01, // - ImmersionRC / FatShark
    BAND_R = 0x02, // - RaceBand
    BAND_E = 0x03, // - Boscam E
    BAND_B = 0x04, // - Boscam B
    BAND_A = 0x05, // - Boscam A
    BAND_L = 0x06, // - LowRace
    BAND_X = 0x07, // - Band X
  };

  enum RXMODULE {
    BOTH = 0x00,  // - Both
    UPPER = 0x01, // - Upper
    LOWER = 0x02, // - Lower
  };

  enum OSDMODE {
    OFF = 0,             // - Off
    LOCKONLY = 1,        // - LockOnly
    DEFAULTMODE = 2,     // - Default
    LOCKANDSTANDARD = 3, // - LockAndStandard
    RSSIBARSLITE = 4,    // - RSSIBarsLite
    RSSIBARS = 5,        // - RSSIBars
    UNIQUE_ID = 6,      // - Unique ID
    INTERNAL_USE = 7,    // - Internal use
    USERTEXT = 8,        // - UserText
  };

  /*
  enum OSDMODE {
    OFF = 0,             // - Off
    DEFAULTMODE = 1,     // - Default
    LOCKONLY = 2,        // - LockOnly
    USERTEXT = 3,        // - UserText
    RSSIBARS = 4,        // - RSSIBars
    LOCKANDSTANDARD = 5, // - LockAndStandard
    RSSIBARSLITE = 6,    // - RSSIBarsLite
    INTERNAL_USE = 7,    // - Internal use
    INTERNAL_USE2 = 8,   // - Internal use
    UNIQUE_ID = 9,       // - Unique ID
    LONG_RANGE = 10,     // - Long Range
  };
    */

  enum RAPIDFIREMODE {
    RAPIDFIRE_1 = 0x00, // - RapidFIRE #1
    RAPIDFIRE_2 = 0x01, // - RapidFIRE #2
    LEGACY = 0x02,      // - Legacy
  };

private:
  void SendBuzzerCmd();
  void SendUserTextCmd();
  void SetPinsOutput();
  void SetPinsInput();
  void ResetSPIPins();
  // void SendOSDModeCmd();
  // void SendRSSICmd();
  // void SendVoltageCmd();
  // void SendFirmwareVersionCmd();
  // void SendModuleCmd();
  // void SendRapidfireModeCmd();

  void SendSPI(uint8_t *buf, uint8_t bufLen);
  void EnableSPIMode();

  uint8_t crc8(uint8_t *buf, uint8_t bufLen);

  // rapidFIRE_SPI *rapidfire;
  bool SPIModeEnabled = false;
};
