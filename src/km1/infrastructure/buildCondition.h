#if defined(ARDUINO_ARCH_RP2040) && !defined(KERMITECORE_MANUAL_STACK_CONFIG)
//default stack configuration for RP2040

#if defined(USE_TINYUSB)
#define KEMRITECORE_USE_USBIOCORE_ADAFRUIT_TINYUSB
#else
#define KEMRITECORE_USE_USBIOCORE_RP2040_ARDUINO_PICO
#endif

#define KERMITECORE_USE_FLASHSTORAGE_RP2040_PICO_SDK

#endif
