#include "rapidfire.h"
#include "logging.h"

void Rapidfire::Init() {
  ModuleBase::Init();

  delay(VRX_BOOT_DELAY);

  // https://github.com/ExpressLRS/ExpressLRS/pull/1489 &
  // https://github.com/ExpressLRS/Backpack/pull/65

  // *rapidfire = rapidFIRE_SPI(PIN_CLK, PIN_MOSI, PIN_CS);
  EnableSPIMode();
  SetPinsInput();

  DBGLN("Rapid Fire init");
}

void Rapidfire::SetPinsOutput() {
  // Set pins to output to configure rapidfire in SPI mode
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
}

void Rapidfire::SetPinsInput() {
  // Set pins to input to configure rapidfire in SPI mode
  pinMode(PIN_MOSI, INPUT);
  pinMode(PIN_CLK, INPUT);
  pinMode(PIN_CS, INPUT);
}

void Rapidfire::ResetSPIPins() {
  digitalWrite(PIN_MOSI, LOW);
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_CS, HIGH);
}

void Rapidfire::EnableSPIMode() {

  SetPinsOutput();

  // put the RF into SPI mode by pulling all 3 pins high,
  // then low within 100-400ms
  digitalWrite(PIN_MOSI, HIGH);
  digitalWrite(PIN_CLK, HIGH);
  digitalWrite(PIN_CS, HIGH);

  delay(RAPIDFIRE_SPI_MODE_ENABLE_DELAY);

  digitalWrite(PIN_MOSI, LOW);
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_CS, LOW);

  delay(RAPIDFIRE_SPI_MODE_ENABLE_DELAY);

  ResetSPIPins();

  SPIModeEnabled = true;

  DBGLN("SPI config complete");
}

void Rapidfire::SendBuzzerCmd() {
  DBGLN("Beep!");

  uint8_t cmd[4];
  cmd[0] = RF_API_BEEP_CMD;   // 'S'
  cmd[1] = RF_API_DIR_GRTHAN; // '>'
  cmd[2] = 0x00;              // len
  cmd[3] = crc8(cmd, 3);

  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(cmd, 4);
  }
}

void Rapidfire::SetOSD(mspPacket_t *packet) {
  // MSP msp;
  // msp.sendPacket(packet, m_port);

  // packet->readByte();

  // SendBuzzerCmd();
  // SendUserTextCmd();
}

void Rapidfire::SetRecordingState(uint8_t recordingState, uint16_t delay) {
  // SendBuzzerCmd();
  SendUserTextCmd();
}

void Rapidfire::SendHeadTrackingEnableCmd(bool enable) {
  // SendBuzzerCmd();
  SendBuzzerCmd();
  // SendUserTextCmd();
}

void Rapidfire::SendUserTextCmd() {

  uint8_t mode_cmd[5];
  mode_cmd[0] = RF_API_OSD_MODE_CMD;
  mode_cmd[1] = RF_API_DIR_EQUAL;
  mode_cmd[2] = 0x01; // len
  mode_cmd[3] = (byte)Rapidfire::OSDMODE::USERTEXT;
  mode_cmd[3] = crc8(mode_cmd, 4);
  mode_cmd[4] = (byte)Rapidfire::OSDMODE::USERTEXT;

  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(mode_cmd, 5);
  }

  // char str[] = "Hello";

  /*
  uint8_t text_cmd[9];
  text_cmd[0] = 0x54; // RF_API_TEXT_CMD;
  text_cmd[1] = 0x3d; // RF_API_DIR_EQUAL;
  text_cmd[2] = 0x05; //(byte)strlen(str); // len // MAX 25!
  text_cmd[3] = 0x8a;
  text_cmd[4] = 0x48;
  text_cmd[5] = 0x65;
  text_cmd[6] = 0x6c;
  text_cmd[7] = 0x6c;
  text_cmd[8] = 0x6f;
  */

  /*text_cmd[3] = (byte)str[0]; // 'H'
  text_cmd[4] = (byte)str[1]; // 'e'
  text_cmd[5] = (byte)str[2]; // 'l'
  text_cmd[6] = (byte)str[3]; // 'l'
  text_cmd[7] = (byte)str[4]; // 'o'
  text_cmd[3] = crc8(text_cmd, 8);
  text_cmd[4] = (byte)str[0];
  text_cmd[5] = (byte)str[1];
  text_cmd[6] = (byte)str[2];
  text_cmd[7] = (byte)str[3];
  text_cmd[8] = (byte)str[4];
  */

  /*
  for (int i = 0; i < 1; i++) {
    SendSPI(text_cmd, 9);
  }


  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(mode_cmd, 5);
  }
        */

  delay(1000);

  
  mode_cmd[3] = (byte)Rapidfire::OSDMODE::DEFAULTMODE;
  mode_cmd[3] = crc8(mode_cmd, 4);
  mode_cmd[4] = (byte)Rapidfire::OSDMODE::DEFAULTMODE;

  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(mode_cmd, 5);
  }
  
}

void Rapidfire::SendIndexCmd(uint8_t index) {
  uint8_t newBand = index / 8 + 1;
  uint8_t newChannel = index % 8;

  SendBandCmd(newBand);
  SendChannelCmd(newChannel);
}

void Rapidfire::SendChannelCmd(uint8_t channel) {
  // ELRS channel is zero based, need to add 1
  channel++;

  DBG("Setting new channel ");
  DBGLN("%x", channel);

  uint8_t cmd[5];
  cmd[0] = RF_API_CHANNEL_CMD; // 'C'
  cmd[1] = RF_API_DIR_EQUAL;   // '='
  cmd[2] = 0x01;               // len
  cmd[3] = channel;            // temporarily set byte 4 to channel for crc calc
  cmd[3] = crc8(cmd, 4);       // reset byte 4 to crc
  cmd[4] = channel;            // assign channel to correct byte 5

  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(cmd, 5);
  }
}

void Rapidfire::SendBandCmd(uint8_t band) {
  DBG("Setting new band ");
  DBGLN("%x", band);

  uint8_t imrcBand; // convert ELRS band index to IMRC band index:
  switch (band) {
  case 0x01: // Boscam A
    imrcBand = Rapidfire::BAND::BAND_A;
    break;
  case 0x02: // Boscam B
    imrcBand = Rapidfire::BAND::BAND_B;
    break;
  case 0x03: // Boscam E
    imrcBand = Rapidfire::BAND::BAND_E;
    break;
  case 0x04: // ImmersionRC/FatShark
    imrcBand = Rapidfire::BAND::BAND_F;
    break;
  case 0x05: // RaceBand
    imrcBand = Rapidfire::BAND::BAND_R;
    break;
  case 0x06: // LowRace
    imrcBand = Rapidfire::BAND::BAND_L;
    break;
  default: // ImmersionRC/FatShark
    imrcBand = Rapidfire::BAND::BAND_F;
    break;
  }

  uint8_t cmd[5];
  cmd[0] = RF_API_BAND_CMD;  // 'C'
  cmd[1] = RF_API_DIR_EQUAL; // '='
  cmd[2] = 0x01;             // len
  cmd[3] = imrcBand;         // temporarily set byte 4 to band for crc calc
  cmd[3] = crc8(cmd, 4);     // reset byte 4 to crc
  cmd[4] = imrcBand;         // assign band to correct byte 5

  for (int i = 0; i < SPAM_COUNT; i++) {
    SendSPI(cmd, 5);
  }
}

void Rapidfire::SendSPI(uint8_t *buf, uint8_t bufLen) {
  if (!SPIModeEnabled)
    EnableSPIMode();

  uint32_t periodMicroSec = 1000000 / BIT_BANG_FREQ;

  SetPinsOutput();
  ResetSPIPins();

  delayMicroseconds(periodMicroSec);

  digitalWrite(PIN_CS, LOW);

  delay(DELAY_BETWEEN_SPI_PKT);

  // debug code for printing SPI pkt
  for (int i = 0; i < bufLen; ++i) {
    uint8_t bufByte = buf[i];

    DBG("%x", bufByte);
    DBG(",");

    for (uint8_t k = 0; k < 8; k++) {
      // digitalWrite takes about 0.5us, so it is not taken into account with
      // delays.
      digitalWrite(PIN_CLK, LOW);
      delayMicroseconds(periodMicroSec / 4);

      digitalWrite(PIN_MOSI, bufByte & 0x80);
      delayMicroseconds(periodMicroSec / 4);

      digitalWrite(PIN_CLK, HIGH);
      delayMicroseconds(periodMicroSec / 2);

      bufByte <<= 1;
    }
  }

  DBGLN("");

  ResetSPIPins();

  delay(DELAY_BETWEEN_SPI_PKT);

  SetPinsInput();
}

// CRC function for IMRC rapidfire API
// Input: byte array, array length
// Output: crc byte
uint8_t Rapidfire::crc8(uint8_t *buf, uint8_t bufLen) {
  uint32_t sum = 0;

  for (uint8_t i = 0; i < bufLen; ++i) {
    sum += buf[i];
  }

  return sum & 0xFF;
}
