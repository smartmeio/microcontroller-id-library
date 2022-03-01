#include "MicrocontrollerID.h"

/*
This function will provide the microcontroller's unique ID. If provided size is greater than microcontroller's ID size, then
it will be zero padded.
*/
void MicrocontrollerID::getUniqueID(uint8_t* id_out, int size){
	if (size < IDSIZE){
		_zeroPad(id_out, 0, size);
		return;
	}

#if defined(ARDUINO_ARCH_RP2040)
  #if defined(ARDUINO_ARCH_MBED)
  flash_get_unique_id((uint8_t*) id_out);
  #else
  pico_unique_board_id_t pico;
  pico_get_unique_board_id(&pico);
	for (int i = 0; i < IDSIZE; i++) {
		id_out[i] = pico.id[i];
	}
	#endif

#elif defined(ARDUINO_ARCH_AVR)
    for (int i = 0; i < IDSIZE; i++) {
        id_out[i] = boot_signature_byte_get(0x0E + i + (IDSIZE == 9 && i > 5 ? 1 : 0));
    }

#elif defined(ARDUINO_ARCH_ESP8266)
    uint32_t _id = ESP.getChipId();
    for (int i = 0; i < 4; i++){
        id_out [i] = 0;
    }
    id_out[4] = _id >> 24;
    id_out[5] = _id >> 16;
    id_out[6] = _id >> 8;
    id_out[7] = _id;

#elif defined(ARDUINO_ARCH_ESP32)
    uint64_t _id = ESP.getEfuseMac();
    id_out[0] = 0;
    id_out[1] = 0;
    int reg_shift = 0;
    for (int i = 2; i<8; i++) {
        id_out [i] = _id >> 8*reg_shift;
        reg_shift++;
    }

#elif defined(ARDUINO_ARCH_SAM) //From: https://github.com/ricaun/ArduinoUniqueID
	unsigned int status ;
	/* Send the Start Read unique Identifier command (STUI) by writing the Flash Command Register with the STUI command.*/
	EFC1->EEFC_FCR = (0x5A << 24) | EFC_FCMD_STUI;
	do
	{
		status = EFC1->EEFC_FSR ;
	} while ( (status & EEFC_FSR_FRDY) == EEFC_FSR_FRDY ) ;

	/* The Unique Identifier is located in the first 128 bits of the Flash memory mapping. So, at the address 0x400000-0x400003. */
	uint32_t _id[4];
	_id[0] = *(uint32_t *)IFLASH1_ADDR;
	_id[1] = *(uint32_t *)(IFLASH1_ADDR + 4);
	_id[2] = *(uint32_t *)(IFLASH1_ADDR + 8);
	_id[3] = *(uint32_t *)(IFLASH1_ADDR + 12);
	for (int i = 0; i < 4; i++)
	{
		id_out[i*4+0] = (uint8_t)(_id[i] >> 24);
		id_out[i*4+1] = (uint8_t)(_id[i] >> 16);
		id_out[i*4+2] = (uint8_t)(_id[i] >> 8);
		id_out[i*4+3] = (uint8_t)(_id[i] >> 0);
	}

	/* To stop the Unique Identifier mode, the user needs to send the Stop Read unique Identifier
	command (SPUI) by writing the Flash Command Register with the SPUI command. */
	EFC1->EEFC_FCR = (0x5A << 24) | EFC_FCMD_SPUI ;

	/* When the Stop read Unique Unique Identifier command (SPUI) has been performed, the
	FRDY bit in the Flash Programming Status Register (EEFC_FSR) rises. */
	do
	{
		status = EFC1->EEFC_FSR ;
	} while ( (status & EEFC_FSR_FRDY) != EEFC_FSR_FRDY );

#elif defined(ARDUINO_ARCH_SAMD)
    #if defined (__SAMD51__)
        // SAMD51 from section 9.6 of the datasheet
        #define SERIAL_NUMBER_WORD_0	*(volatile uint32_t*)(0x008061FC)
        #define SERIAL_NUMBER_WORD_1	*(volatile uint32_t*)(0x00806010)
        #define SERIAL_NUMBER_WORD_2	*(volatile uint32_t*)(0x00806014)
        #define SERIAL_NUMBER_WORD_3	*(volatile uint32_t*)(0x00806018)
    #else
        // SAMD21 from section 9.3.3 of the datasheet
        #define SERIAL_NUMBER_WORD_0	*(volatile uint32_t*)(0x0080A00C)
        #define SERIAL_NUMBER_WORD_1	*(volatile uint32_t*)(0x0080A040)
        #define SERIAL_NUMBER_WORD_2	*(volatile uint32_t*)(0x0080A044)
        #define SERIAL_NUMBER_WORD_3	*(volatile uint32_t*)(0x0080A048)
    #endif

    uint32_t _id[4];
	_id[0] = SERIAL_NUMBER_WORD_0;
	_id[1] = SERIAL_NUMBER_WORD_1;
	_id[2] = SERIAL_NUMBER_WORD_2;
	_id[3] = SERIAL_NUMBER_WORD_3;

	for (int i = 0; i < 4; i++)
	{
		id_out[i*4+0] = (uint8_t)(_id[i] >> 24);
		id_out[i*4+1] = (uint8_t)(_id[i] >> 16);
		id_out[i*4+2] = (uint8_t)(_id[i] >> 8);
		id_out[i*4+3] = (uint8_t)(_id[i] >> 0);
	}

#elif defined(ARDUINO_ARCH_STM32)
	uint32_t _id[3];
	#if defined (ARDUINO_ARANCINOV12_H743ZI2)
		#define UID_BASE       (0x1FF1E800)    /*!< Unique device ID register base address for H743 */
		
		_id[0] =  (uint32_t)(READ_REG(*((uint32_t *)UID_BASE ))) ;
		_id[1] =  (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE + 4U)))) ;
		_id[2] =  (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE + 8U)))) ;
		
	#else
		_id[0] = HAL_GetUIDw0();
		_id[1] = HAL_GetUIDw1();
		_id[2] = HAL_GetUIDw2();
    
	#endif
	for (int i = 0; i < 3; i++)
	{
		id_out[i*4+0] = (uint8_t)(_id[i] >> 24);
		id_out[i*4+1] = (uint8_t)(_id[i] >> 16);
		id_out[i*4+2] = (uint8_t)(_id[i] >> 8);
		id_out[i*4+3] = (uint8_t)(_id[i] >> 0);
	}

#elif defined(ARDUINO_ARCH_NRF52)
	uint32_t _id[2]; 
	_id[0] = NRF_FICR->DEVICEID[0]; 
	_id[1] = NRF_FICR->DEVICEID[1];
	for (int i = 0; i < 2; i++)
		{
			id_out[i*4+0] = (uint8_t)(_id[i] >> 24);
			id_out[i*4+1] = (uint8_t)(_id[i] >> 16);
			id_out[i*4+2] = (uint8_t)(_id[i] >> 8);
			id_out[i*4+3] = (uint8_t)(_id[i] >> 0);
		}
#endif

    _zeroPad(id_out, IDSIZE, size);
}


/*
Microcontroller's ID will be provided as string. If provided, size will set a fixed number of bytes the ID will be returned
(as usual it will be zero padded if not long enough).
Please keep in mind that the buffer should at least double+1 the ID size.
*/
void MicrocontrollerID::getUniqueIDString(char* id_out, int size){
	size = size == 0 ? IDSIZE : size;
	uint8_t _id[size];
	getUniqueID(_id, IDSIZE);
	_zeroPad(_id, IDSIZE, size);
	_array_to_string(_id, size, id_out);
}

void MicrocontrollerID::_zeroPad(uint8_t* buf, int start, int stop) {
    for (int i = start; i < stop; i++) {
        buf[i] = 0;
    }
}

void MicrocontrollerID::_array_to_string(byte array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

MicrocontrollerID MicroID;
