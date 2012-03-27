#include "WiFly.h"
#include "WiFlyDevice.h"
#include "String.h"
#include "IPAddress.h"

#define SOFTWARE_REBOOT_RETRY_ATTEMPTS 5

WiFlyDevice WiFly;

WiFlyDevice::WiFlyDevice() {
  serverConnected = false;
}

void  WiFlyDevice::setUart(Stream* newUart) {
  uart = newUart;
}

void WiFlyDevice::begin() {
	String fullVersion;
	int index = 10;

	// need a successful reboot before we're ready
	while(!reboot()) {
		delay(250);
	};

	fullVersion = getCommandResponse("ver");

	while (fullVersion[index] != ',') {
		++index;
	}

	firmwareVersion = (char*)malloc(index - 7);
	fullVersion = fullVersion.substring(10, index);
	fullVersion = "<" + fullVersion + ">";

	fullVersion.toCharArray(firmwareVersion, index - 7);
}

void WiFlyDevice::sendBareCommand(const __FlashStringHelper* command) {
	if (commandModeFlag) {
		exitCommandMode();
	}

	enterCommandMode();

	uart->flush();

	const prog_char *p = (const prog_char *)command;

	while (1) {
		unsigned char c = pgm_read_byte(p++);
		if (c == 0) break;
		uart->write(c);
  	}

	uart->write(13);
	uart->find("\n");
}

String WiFlyDevice::getCommandResponse(const __FlashStringHelper* command) {	
	String response = "";


	sendBareCommand(command);

	// wait for response
	while (!uart->available()) {
		delay(10);
	}

	while (uart->available()) {
		response += (char)uart->read();
		delay(1);
	}

	return response;
}

boolean WiFlyDevice::sendCommand(const __FlashStringHelper* command) {
	return sendCommand(command, "AOK", 2000, true);
}

boolean WiFlyDevice::sendCommand(const __FlashStringHelper* command, char* response) {
	return sendCommand(command, response, 2000, true);
}

boolean WiFlyDevice::sendCommand(const __FlashStringHelper* command, char* response, int timeout) {
	return sendCommand(command, response, timeout, true);
}

boolean WiFlyDevice::sendCommand(const __FlashStringHelper* command, char* response, int timeout, boolean exit) {
	boolean result;

	sendBareCommand(command);

	// wait until the WiFly responds
	result = waitForResponse(response, timeout);

	if (exit) {
		uart->flush();
		exitCommandMode();
	}

	return result;
}

void WiFlyDevice::sendBareCommand(char* command) {
	if (commandModeFlag) {
		exitCommandMode();
	}

	enterCommandMode();

	uart->flush();
	uart->print(command);
	uart->write(13);
	uart->find("\n");
}

String WiFlyDevice::getCommandResponse(char* command) {	
	String response = "";


	sendBareCommand(command);

	// wait for response
	while (!uart->available()) {
		delay(10);
	}

	while (uart->available()) {
		response += (char)uart->read();
		delay(1);
	}

	return response;
}

boolean WiFlyDevice::sendCommand(char* command) {
	return sendCommand(command, "AOK");
}

boolean WiFlyDevice::sendCommand(char* command, char* response) {
	return sendCommand(command, response, 2000);
}

boolean WiFlyDevice::sendCommand(char* command, char* response, int timeout) {
	return sendCommand(command, response, timeout, true);
}

boolean WiFlyDevice::sendCommand(char* command, char* response, int timeout, boolean exit) {
	boolean result;

	sendBareCommand(command);

	// wait until the WiFly responds
	result = waitForResponse(response, timeout);

	if (exit) {
		uart->flush();
		exitCommandMode();
	}

	return result;
}

IPAddress WiFlyDevice::localIP() {
  String result = getCommandResponse(F("get ip"));
  int start;
  int end;

  if (start = result.indexOf("IP=")) {
  	end = result.indexOf(':', start);
  }

  exitCommandMode();

  return stringToIPAddress(result.substring(start + 3, end));
}

IPAddress WiFlyDevice::subnetMask() {
  String result = getCommandResponse(F("get ip"));
  int start;
  int end;

  if (start = result.indexOf("NM=")) {
  	end = result.indexOf('\n', start);
  }

  exitCommandMode();

  return stringToIPAddress(result.substring(start + 3, end));
}

IPAddress WiFlyDevice::gatewayIP() {
  String result = getCommandResponse(F("get ip"));
  int start;
  int end;

  if (start = result.indexOf("GW=")) {
  	end = result.indexOf('\n', start);
  }

  exitCommandMode();

  return stringToIPAddress(result.substring(start + 3, end));
}

IPAddress WiFlyDevice::dnsServerIP() {
  String result = getCommandResponse(F("get dns"));
  int start;
  int end;

  if (start = result.indexOf("DNS=")) {
  	end = result.indexOf('\n', start);
  }

  exitCommandMode();

  return stringToIPAddress(result.substring(start + 4, end));	
}

boolean WiFlyDevice::reboot() {
	boolean result;

	enterCommandMode();

	uart->setTimeout(4000);

	uart->print("reboot");
	uart->write(13);
	uart->find("*Reboot*");

	// wait a bit until we have data coming in
	while (!uart->available()) {
		delay(10);
	}
	uart->flush();

	result = uart->find("*READY*");
	commandModeFlag = false;

	return result;
}

void WiFlyDevice::enterCommandMode(){

	uart->setTimeout(2000);
	uart->flush();
	uart->print(F("$$$"));

	uart->find("CMD");
	uart->write(13);

	// wait a bit until we have data coming in
	while (!uart->available()) {
		delay(10);
	}

	// make sure we're in command mode

	if (firmwareVersion == NULL) {
		uart->find("<");
		uart->find(">");		
	} else {
		uart->find(firmwareVersion);
	}

	uart->flush();

	commandModeFlag = true;
}

void WiFlyDevice::exitCommandMode() {
	uart->print("exit");
	uart->write(13);
	uart->find("EXIT");
	uart->flush();

	commandModeFlag = false;
}

boolean WiFlyDevice::waitForResponse(char* response) {
	waitForResponse(response, 2000);
}

boolean WiFlyDevice::waitForResponse(char* response, int timeout) {
	uart->setTimeout(timeout);

	while (!uart->available()) {
		delay(10);
	}

	return uart->find(response);
}

IPAddress WiFlyDevice::stringToIPAddress(String str) {
  int max = str.length() + 1;
  char* raw;
  char *i;  
  IPAddress ip = IPAddress();

  raw = (char*)malloc(max);
  str.toCharArray(raw, max);

  ip[0] = atoi(strtok_r(raw, ".", &i));
  ip[1] = atoi(strtok_r(NULL, ".", &i));
  ip[2] = atoi(strtok_r(NULL, ".", &i));
  ip[3] = atoi(strtok_r(NULL, ".", &i));

  return ip;
}