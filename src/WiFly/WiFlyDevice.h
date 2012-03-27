#ifndef __WIFLY_DEVICE_H__
#define __WIFLY_DEVICE_H__

#include <Arduino.h>
#include <Stream.h>
#include "String.h"
#include "IPAddress.h"

class WiFlyDevice {
  public:
    WiFlyDevice();

    void begin();
    void setUart(Stream* newUart);

    void sendBareCommand(const __FlashStringHelper* command);
    String getCommandResponse(const __FlashStringHelper* command);

    boolean sendCommand(const __FlashStringHelper* command);
    boolean sendCommand(const __FlashStringHelper* command, char* response);
    boolean sendCommand(const __FlashStringHelper* command, char* response, int timeout);
    boolean sendCommand(const __FlashStringHelper* command, char* response, int timeout, boolean exit);

    void sendBareCommand(char* command);
    String getCommandResponse(char* command);

    boolean sendCommand(char* command);
    boolean sendCommand(char* command, char* response);
    boolean sendCommand(char* command, char* response, int timeout);
    boolean sendCommand(char* command, char* response, int timeout, boolean exit);

    void enterCommandMode();
    void exitCommandMode();    

    boolean waitForResponse(char* response);
    boolean waitForResponse(char* response, int timeout);

    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsServerIP();

    boolean reboot();

    Stream* uart;
    char* firmwareVersion;
    boolean serverConnected;

  protected:
    IPAddress stringToIPAddress(String str);

  private:
    boolean commandModeFlag;
};

#endif
