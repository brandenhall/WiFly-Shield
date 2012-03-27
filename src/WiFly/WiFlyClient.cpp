#include "WiFly.h"
#include "WiFlyClient.h"


WiFlyClient::WiFlyClient() : _WiFly (WiFly) {
  connections = 0;
}

// handle connect from server class
int WiFlyClient::connect() {
  connections = 1;
  return connections;
}

int WiFlyClient::connect(const char* host, uint16_t port) {
  String cmd = String("open ");
  cmd += host;
  cmd += " ";
  cmd += port;

  int len = cmd.length() + 1;
  char cmdbuf[len];
  cmd.toCharArray(cmdbuf, len);

  connections = _WiFly.sendCommand(cmdbuf, "*OPEN*", 10000, false);

  return connections;
}

int WiFlyClient::connect(IPAddress ip, uint16_t port) {
  String cmd = String("open ");
  cmd += ip[0];
  cmd += ".";
  cmd += ip[1];
  cmd += ".";
  cmd += ip[2];
  cmd += ".";
  cmd += ip[3];      
  cmd += " ";
  cmd += port;

  int len = cmd.length() + 1;
  char cmdbuf[len];
  cmd.toCharArray(cmdbuf, len);

  connections = _WiFly.sendCommand(cmdbuf, "*OPEN*", 10000, false);

  return connections;
}

size_t WiFlyClient::write(uint8_t value) {
  return _WiFly.uart->write((byte)value);
}

size_t WiFlyClient::write(const uint8_t *buf, size_t size) {
  return _WiFly.uart->write(buf, size);
}

int WiFlyClient::available() {
  if (connections < 1) {
    return 0;
  }

  return _WiFly.uart->available();
}

int WiFlyClient::read() {
  if (connections < 1) {
    return -1;
  }

  return _WiFly.uart->read();
}

int WiFlyClient::read(uint8_t *buf, size_t size) {
  uint8_t value;
  uint8_t count = 0;

  if (connections < 1) {
    return -1;
  }  

  for (uint8_t i=0; i<size; ++i) {
    value = _WiFly.uart->read();

    if (value > -1) {
      buf[count] = value;
      ++count;

    } else {
      break;
    }
  }

  return count;
}

int WiFlyClient::peek() {
  if (connections < 1) {
    return -1;
  }  
  return _WiFly.uart->peek();
}

void WiFlyClient::flush() {
  if (connections < 1) {
    return;
  }
  
  _WiFly.uart->flush();
}

void WiFlyClient::stop() {
  _WiFly.sendCommand(F("close"), _WiFly.firmwareVersion);

  reset();

  _WiFly.uart->flush();

  _WiFly.serverConnected = false;
}

void WiFlyClient::reset() {
  
  connections = 0;
}

uint8_t WiFlyClient::connected() {
  return connections;
}

WiFlyClient::operator bool() {
  return connections == 0;
}
