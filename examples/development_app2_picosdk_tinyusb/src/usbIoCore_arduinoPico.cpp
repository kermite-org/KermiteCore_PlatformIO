#include "RP2040USB.h"
#include "class/hid/hid_device.h"
#include "tusb.h"

void __USBInstallKeyboard() {}
void __USBInstallMouse() {}
void __USBInstallSecondHID_RawHID() {}

static uint8_t hidKeyboardStatusLedFlags = 0;

static uint8_t rawHidRxBuf[64];
static bool rawHidRxHasData = false;

void hidSetReportCallbackFn(uint8_t instance, uint8_t reportId, uint8_t reportType, uint8_t const *buffer, uint16_t bufsize) {
  const int instanceShared = __USBGetHIDInstanceIndexForSharedHID();
  const int instanceRawHid = __USBGetHIDInstanceIndexForRawHID();
  if (instance == instanceShared) {
    const int reportIdHidKeyboard = __USBGetKeyboardReportID();
    if (reportId == reportIdHidKeyboard && bufsize == 1) {
      hidKeyboardStatusLedFlags = buffer[0];
    }
  }
  if (instance == instanceRawHid && bufsize == 64) {
    memcpy(rawHidRxBuf, buffer, 64);
    rawHidRxHasData = true;
  }
}

void usbIoCore_initialize() {
  __USBSubscribeHIDSetReportCallback(hidSetReportCallbackFn);
}

void usbIoCore_hidKeyboard_writeReport(uint8_t *pReportBytes8) {
  const int instance = __USBGetHIDInstanceIndexForSharedHID();
  const int reportId = __USBGetKeyboardReportID();
  if (tud_hid_n_ready(instance)) {
    tud_hid_n_report(instance, reportId, pReportBytes8, 8);
  }
}

uint8_t usbIoCore_hidKeyboard_getStatusLedFlags() {
  return hidKeyboardStatusLedFlags;
}

void usbIoCore_hidMouse_writeReport(uint8_t *pReportBytes7) {
  const int instance = __USBGetHIDInstanceIndexForSharedHID();
  const int reportId = __USBGetMouseReportID();
  if (tud_hid_n_ready(instance)) {
    tud_hid_n_report(instance, reportId, pReportBytes7, 7);
  }
}

void usbIoCore_rawHid_writeData(uint8_t *pDataBytes64) {
  const int instance = __USBGetHIDInstanceIndexForRawHID();
  if (tud_hid_n_ready(instance)) {
    tud_hid_n_report(instance, 0, pDataBytes64, 64);
  }
}

bool usbIoCore_rawHid_readDataIfExists(uint8_t *pDataBytes64) {
  if (rawHidRxHasData) {
    memcpy(pDataBytes64, rawHidRxBuf, 64);
    rawHidRxHasData = false;
    return true;
  }
  return false;
}
