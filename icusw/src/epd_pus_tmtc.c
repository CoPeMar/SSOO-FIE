#include "epd_pus_tmtc.h"
#include "epd_pus_mission.h"
#include "ccsds_pus_format.h"
#include "serialize.h"

void epd_pus_build_tm_1_1(tm_descriptor_t *p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl) {

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x07;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 1;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_id, tm_descriptor.p_tm_bytes+10);

    serialize_uint16(tc_packet_seq_ctrl, p_tm_descriptor->p_tm_bytes + 12);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_seq_ctrl, &tm_descriptor.p_tm_bytes[12]);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;

    return;

}

void epd_pus_build_tm_1_2_crc_error(tm_descriptor_t *p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl,
        uint16_t tc_packet_err_ctrl,
        uint16_t calculated_crc) {

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x0D;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 2;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    serialize_uint16(tc_packet_seq_ctrl, &p_tm_descriptor->p_tm_bytes[12]);
    serialize_uint16(0x2, &p_tm_descriptor->p_tm_bytes[14]);
    serialize_uint16(tc_packet_err_ctrl, &p_tm_descriptor->p_tm_bytes[16]);
    serialize_uint16(calculated_crc, &p_tm_descriptor->p_tm_bytes[18]);

    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;

    return;

}

void epd_pus_build_tm_1_2_illegal_type(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x0B;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 2;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_id, tm_descriptor.p_tm_bytes+10);

    serialize_uint16(tc_packet_seq_ctrl, p_tm_descriptor->p_tm_bytes + 12);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_seq_ctrl, &tm_descriptor.p_tm_bytes[12]);

    serialize_uint16(0x3, &p_tm_descriptor->p_tm_bytes[14]);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}

void epd_pus_build_tm_1_2_illegal_subtype(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x0B;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 2;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_id, tm_descriptor.p_tm_bytes+10);

    serialize_uint16(tc_packet_seq_ctrl, p_tm_descriptor->p_tm_bytes + 12);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_seq_ctrl, &tm_descriptor.p_tm_bytes[12]);

    serialize_uint16(0x4, &p_tm_descriptor->p_tm_bytes[14]);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}

void epd_pus_build_tm_1_7(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x07;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 7;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_id, tm_descriptor.p_tm_bytes+10);

    serialize_uint16(tc_packet_seq_ctrl, p_tm_descriptor->p_tm_bytes + 12);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_seq_ctrl, &tm_descriptor.p_tm_bytes[12]);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}

void epd_pus_build_tm_1_8(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x07;

    df_header.version = 0x10;
    df_header.type = 1;
    df_header.subtype = 8;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    serialize_uint16(tc_packet_id, &p_tm_descriptor->p_tm_bytes[10]);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_id, tm_descriptor.p_tm_bytes+10);

    serialize_uint16(tc_packet_seq_ctrl, p_tm_descriptor->p_tm_bytes + 12);
    // This is equivalent to the previous call:
    // serialize_uint16(tc_packet_seq_ctrl, &tm_descriptor.p_tm_bytes[12]);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}

void epd_pus_build_tm_17_2(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x03;

    df_header.version = 0x10;
    df_header.type = 17;
    df_header.subtype = 2;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}

void epd_pus_build_tm_3_25_sid0(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter, uint8_t * hk_parameter, uint32_t num_params){

    struct ccsds_pus_tmtc_packet_header tm_packet_header;
    struct ccsds_pus_tm_df_header df_header;

    tm_packet_header.packet_id = ccsds_pus_tm_build_packet_id(EPD_APID);

    tm_packet_header.packet_seq_ctrl =
            ccsds_pus_tm_build_packet_seq_ctrl(0x3, tm_seq_counter);

    tm_packet_header.packet_length = 0x14;

    df_header.version = 0x10;
    df_header.type = 3;
    df_header.subtype = 25;
    df_header.destinationID = EPD_DESTINATION_ID;

    ccsds_pus_tm_set_fields(p_tm_descriptor->p_tm_bytes,
            &tm_packet_header, &df_header);
    // This is equivalent to the previous call:
    // ccsds_pus_tm_set_fields(&tm_descriptor.p_tm_bytes[0],
    //         &tm_packet_header, &df_header);

    p_tm_descriptor->p_tm_bytes[10] = 0x0;

    for(uint8_t i = 0; i < num_params; i++){
    	p_tm_descriptor->p_tm_bytes[11+i] = hk_parameter[i];
    }

    // Calculate p_tm_descriptor->tm_num_bytes
    p_tm_descriptor->tm_num_bytes = 6 + tm_packet_header.packet_length + 1;
}
