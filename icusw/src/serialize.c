#include "serialize.h"

uint16_t deserialize_uint16(uint8_t data_bytes[]) {

    uint16_t var;

    var = data_bytes[0];
    var = var << 8;
    var = var | data_bytes[1];

    return var;

}

uint32_t deserialize_uint32(uint8_t data_bytes[]) {

    uint32_t var;

    var = data_bytes[0];
    var = var << 8;
    var = var | data_bytes[1];
    var = var << 8;
    var = var | data_bytes[2];
    var = var << 8;
    var = var | data_bytes[3];

    return var;

}

void serialize_uint16(uint16_t var, uint8_t data_bytes[]) {

    data_bytes[0] = (var & 0xFF00) >> 8;
    data_bytes[1] = (var & 0x00FF);

}

void serialize_uint32(uint32_t var, uint8_t data_bytes[]) {

    data_bytes[0] = (var & 0xFF000000) >> 24;
    data_bytes[1] = (var & 0x00FF0000) >> 16;
    data_bytes[2] = (var & 0x0000FF00) >> 8;
    data_bytes[3] = (var & 0x000000FF);

}
