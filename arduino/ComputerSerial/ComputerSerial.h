#ifndef COMPUTER_SERIAL_H
#define COMPUTER_SERIAL_H

#include <Arduino.h>

#define START_BYTE (byte)0xFF
#define NULL_BYTE (byte)0x00

#define CONTENT_SIZE 250

class ComputerSerial{
	// SerialEvent state enum
	typedef enum {
		STATE_START,
		STATE_SIZE,
		STATE_OPCODE,
		STATE_FLAG,
		STATE_CONTENT
	};
	
	void commandHandler(uint8_t size, uint8_t opcode, uint8_t flag, uint8_t content[]);
	void ack(uint8_t opcode);
	void ack(uint8_t opcode, uint8_t content[], uint8_t contentSize);
	void ping();
	void text(uint8_t size, uint8_t flag, uint8_t content[]);
	void sensor(uint8_t number);
	void pinToggle(uint8_t pin);
	void pinRead(uint8_t pin);
	void pinWrite(uint8_t pin, uint8_t value);
	void reset();
	
	
	
	unsigned int bytesReceived;
	
public:
	ComputerSerial();
	static void placeHolder(uint8_t flag, uint8_t content[], uint8_t contentSize);
	void serialEvent();
	void begin(int baud);
	void attachFunction(uint8_t opcode, 
		void (*handler)(uint8_t flag, uint8_t content[], uint8_t contentSize));
	
	unsigned int getBytesReceived();
	
	// Enum for protocol OPCodes
	typedef enum {
		OPCODE_PING,
		OPCODE_TEXT,
		OPCODE_SENSOR,
		OPCODE_PIN_T,
		OPCODE_PIN_R,
		OPCODE_PIN_W,
		OPCODE_RESPONSE = 0xFE,
		OPCODE_RESET = 0xFF
	};

private: 
	static const uint8_t NUM_OPCODES = 6;
	void (*functions[NUM_OPCODES]) (uint8_t flag, uint8_t content[], uint8_t contentSize);
};

#endif