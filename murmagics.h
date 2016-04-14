#ifndef MURMAGICS_H
#define MURMAGICS_H

#include <cstdint>

namespace {

const uint32_t baudrate = 115200;

//! Devices id
const uint8_t main_id = 250;
const uint8_t broadcast_id = 0;

//! Package bytes location
const size_t first_package_header_position = 0;
const size_t second_package_header_position = 1;
const size_t package_size_position = 2;
const size_t transmitter_byte_position = 3;
const size_t receiver_byte_position = 4;
const size_t confirm_flag_position = 5;
const size_t message_id_position = 6;

//! Package bytes values
const uint8_t first_package_header_value = 0x7D;
const uint8_t second_package_header_value = 0x7F;
const uint8_t end_of_message_value = 0x7E;

//! Size of service information (preamble, crc, ids, ...) in package
const uint8_t service_info_size = 0x0A;

//! Offset for pdu (package data unit)
const size_t data_offset = 7;

//! Output messages ids
const uint8_t request_altimeter_status = 0xA0;
const uint8_t request_device_type = 0x04;
const uint8_t setup_new_address =  0x01;

//! Input messages ids
const uint8_t error_message = 0xFF;
const uint8_t device_type_message = 0x05;
const uint8_t altimeter_status = 0xA1;

//! Time delay for uart transfer in milliseconds
const uint8_t response_wait = 5;

//! Undefined behavior
const uint8_t undefined_device = 0xFF;
const uint8_t undefined_address = 0x00;

//! Package confirmation types
const uint8_t without_confirmation = 0x00;
const uint8_t with_confirmation = 0x0A;
const uint8_t is_replay = 0x0B;

//! CRC values
const uint16_t poly_val = 0x1021;
const uint16_t seed_val = 0xFFFF;

}

#endif // MURMAGICS_H
