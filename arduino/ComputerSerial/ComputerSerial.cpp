#include <Arduino.h>

#include "ComputerSerial.h"

void ComputerSerial::placeHolder(uint8_t flag, uint8_t content[], uint8_t contentSize) {
	return;
}

ComputerSerial::ComputerSerial(){
	
}

void ComputerSerial::begin(int baud){
	Serial.begin(baud);
	
	for (int i = 0; i < NUM_OPCODES; ++i){
		attachFunction(i, &ComputerSerial::placeHolder);
	}
}

void ComputerSerial::commandHandler(uint8_t size, uint8_t opcode, uint8_t flag, uint8_t content[]) {
	switch (opcode) {
		case OPCODE_PING:
			ping();
			break;
		case OPCODE_TEXT:
			text(size, flag, content);
			break;
		case OPCODE_SENSOR:
			break;
		case OPCODE_PIN_T:
			break;
		case OPCODE_PIN_R:
			break;
		case OPCODE_PIN_W:
			break;
		case OPCODE_RESET:
			break;
		default:
			Serial.println("commandHandler herp derp");
	}
}

void ComputerSerial::ack(uint8_t opcode){
	Serial.write(START_BYTE);
	Serial.write(4);
	Serial.write(OPCODE_RESPONSE);
	Serial.write(opcode);
	Serial.write(NULL_BYTE);
}

void ComputerSerial::ack(uint8_t opcode, uint8_t content[], uint8_t contentSize){
	Serial.write(START_BYTE);
	Serial.write(contentSize + 3);
	Serial.write(OPCODE_RESPONSE);
	Serial.write(opcode);
	for (int i = 0; i < contentSize; ++i){
		Serial.write(content[i]);
	}
}

void ComputerSerial::ping() {
	// Send ping response
	Serial.write((byte)0x00);
	Serial.write((byte)0xFF);
}

void ComputerSerial::text(uint8_t size, uint8_t flag, uint8_t content[]) {
	// Print content on display(flag)
	functions[OPCODE_TEXT](flag, content, size-3);
	ack(OPCODE_TEXT);
}

void ComputerSerial::sensor(uint8_t number) {
	// Send value of sensor(number)
}

void ComputerSerial::pinToggle(uint8_t pin) {
	// Toggle pin(pin)
}

void ComputerSerial::pinRead(uint8_t pin) {
	// Send pin(pin) value
}

void ComputerSerial::pinWrite(uint8_t pin, uint8_t value) {
	// Set value of pin(pin)
}

void ComputerSerial::reset() {
	// Reset arduino
}

void ComputerSerial::attachFunction(uint8_t opcode, 
	void (*handler)(uint8_t flag, uint8_t content[], uint8_t contentSize)){
		functions[opcode] = handler;
}

void ComputerSerial::serialEvent(){
	static int state = STATE_START;
	
	static uint8_t size = 0;
	static uint8_t opcode = 0;
	static uint8_t flag = 0;
	static uint8_t content[CONTENT_SIZE];
	static uint8_t content_counter = 0;
	
	while(Serial.available()){
		
		switch (state){
			case STATE_START:
				if (Serial.read() == START_BYTE){
					state = STATE_SIZE;
				}
				else{
					// DERP
				}
				break;
			case STATE_SIZE:
				size = Serial.read();
				state = STATE_OPCODE;
				break;
			case STATE_OPCODE:
				opcode = Serial.read();
				state = STATE_FLAG;
				break;
			case STATE_FLAG:
				flag = Serial.read();
				state = STATE_CONTENT;
				break;
			case STATE_CONTENT:
				content[content_counter] = Serial.read();
				content_counter++;
				if (content_counter+3 == size) {
					commandHandler(size, opcode, flag, content);
					content_counter = 0;
					state = STATE_START;
				} 
				break;
			default:
				break;
		}
		bytesReceived++;
	}
}

unsigned int ComputerSerial::getBytesReceived(){
	return bytesReceived;
}