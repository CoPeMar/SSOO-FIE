#ifndef INCLUDE__EPD_PUS_TMTC_H_
#define INCLUDE__EPD_PUS_TMTC_H_

#include "basic_types.h"

#include "tm_descriptor.h"

/**
 * \brief Generates a Telemetry Packet (1,1) - Accept
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of the serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the accepted telecommand
 * \param tc_packet_seq_ctrl the Packet Sequence Control of the accepted
 *                           telecommand
 */
void epd_pus_build_tm_1_1(tm_descriptor_t *p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl);

/**
 * \brief Generates a Telemetry Packet (1,2) - Reject
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the rejected telecommand
 * \param tc_packet_seq_ctrl the Packet Sequence Control of the rejected
 *                           telecommand
 * \param tc_packet_err_ctrl the Packet Error Control field of the rejected
 *                           telecommand
 * \param calculated_crc the locally-obtained value of the CRC
 */
void epd_pus_build_tm_1_2_crc_error(tm_descriptor_t *p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl,
        uint16_t tc_packet_err_ctrl,
        uint16_t calculated_crc);

/**
 * \brief Generates a Telemetry Packet (1,2) - Reject - Illegal type
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the rejected telecommand
 * \param tc_packet_seq_ctrl the Packet Sequence Control of the rejected
 *                           telecommand
 */
void epd_pus_build_tm_1_2_illegal_type(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl);

/**
 * \brief Generates a Telemetry Packet (1,2) - Reject - Illegal subtype
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the rejected telecommand
 * \param tc_packet_seq_ctrl the Packet Sequence Control of the rejected
 *                           telecommand
 */
void epd_pus_build_tm_1_2_illegal_subtype(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl);

/**
 * \brief Generates a Telemetry Packet (1,7) - Completed TC execution
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the telecommand that was successfully
 *                     executed
 * \param tc_packet_seq_ctrl the Packet Sequence Control of the executed
 *                           telecommand
 */
void epd_pus_build_tm_1_7(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl);

/**
 * \brief Generates a Telemetry Packet (1,8) - Failed TC completion
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param tc_packet_id the Packet ID of the telecommand that failed to be
 *                     executed
 * \param tc_packet_seq_ctrl the Packet Sequence Control of telecommand
 */
void epd_pus_build_tm_1_8(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter,
        uint16_t tc_packet_id,
        uint16_t tc_packet_seq_ctrl);

/**
 * \brief Generates a Telemetry Packet (17,2) - Connection test report
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 */
void epd_pus_build_tm_17_2(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter);

/**
 * \brief Generates a Telemetry Packet (3,25) - HK report SID 0
 *
 * \param p_tm_descriptor pointer to the TM descriptor that will manage the
 *                        storage of serialized generated telemetry
 * \param tm_seq_counter the Sequence Count of the telemetry packet
 * \param hk_parameter the vector of HK parameters
 * \param num_params the number of parameters to be included
 */
void epd_pus_build_tm_3_25_sid0(tm_descriptor_t * p_tm_descriptor,
        uint16_t tm_seq_counter, uint8_t * hk_parameter, uint32_t num_params);


#endif /* INCLUDE__EPD_PUS_TMTC_H_ */
