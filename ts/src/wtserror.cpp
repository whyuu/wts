#pragma once

/* TR 101 290 V1.2.1 2001-05 and other stream errors */
struct ts_err {
  int has_level1_error; /* check veriables of "First priority" for detail information */
  int has_level2_error; /* check veriables of "Second priority" for detail information */
  int has_level3_error; /* check veriables of "Third priority" for detail information */
  int has_other_error; /* other stream errors */

  /* First priority: necessary for de-codability (basic monitoring) */
  int TS_sync_loss; /* 1.1 */
  int Sync_byte_error; /* 1.2 */
#define ERR_1_3_0 (1<<0) /* PAT occur more than 500ms */
#define ERR_1_3_1 (1<<1) /* table_id error in PID 0x0000 */
#define ERR_1_3_2 (1<<2) /* PAT Scrambling_control_field is not 00 */
  int PAT_error; /* 1.3 ---- 1.3a of TR 101 290 V1.2.1 2001-05 */
  int Continuity_count_error; /* 1.4 */
#define ERR_1_5_0 (1<<0)
#define ERR_1_5_1 (1<<1)
  int PMT_error; /* 1.5 ---- 1.5a of TR 101 290 V1.2.1 2001-05 */
  int PID_error; /* 1.6 VID|AUD PID occur more than 5s(or user specified period) */

  /* Second priority: recommended for continuous or periodic monitoring */
  int Transport_error; /* 2.1 */
  int CRC_error; /* 2.2 */
  /* int PCR_error; 2.3 == (2.3a | 2.3b), invalid for new implementations! */
  int PCR_repetition_error; /* 2.3a */
  int PCR_discontinuity_indicator_error; /* 2.3b */
  int PCR_accuracy_error; /* 2.4 */
  int PTS_error; /* 2.5 ---- repetition_error */
#define ERR_2_6_0 (1<<0) /* scrambling program without CAT */
#define ERR_2_6_1 (1<<1) /* table_id error in PID 0x0001 */
  int CAT_error; /* 2.6 */

  /* Third priority: application dependant monitoring */
  int NIT_error; /* 3.1 */
  int NIT_actual_error; /* 3.1a */
  int NIT_other_error; /* 3.1b */
  int SI_repetition_error; /* 3.2 */
  int Buffer_error; /* 3.3 */
  int Unreferenced_PID; /* 3.4 */
  int Unreferenced_PID_2; /* 3.4a */
  int SDT_error; /* 3.5 */
  int SDT_actual_error; /* 3.5a */
  int SDT_other_error; /* 3.5b */
  int EIT_error; /* 3.6 */
  int EIT_actual_error; /* 3.6a */
  int EIT_other_error; /* 3.6b */
  int EIT_PF_error; /* 3.6c */
  int RST_error; /* 3.7 */
  int TDT_error; /* 3.8 */
  int Empty_buffer_error; /* 3.9 */
  int Data_delay_error; /* 3.10 */

  /* other errors */
  int adaption_field_control_error; /* 00 */
  int wild_pcr_packet; /* no program use this pcr packet */
  int normal_section_length_error; /* bad normal section length */
  int private_section_length_error; /* bad private section length */
  int pat_pid_error; /* pat table not in 0x0000 */
  int cat_pid_error; /* cat table not in 0x0001 */
  int pmt_pid_error; /* pmt table not in pmt pid of pat */
  int nit_pid_error; /* nit table not in 0x0010 */
  int sdt_pid_error; /* sdt table not in 0x0011 */
  int descriptor_error; /* wrong descriptor */
  int program_info_length_error; /* program_info_length too big */
  int es_info_length_error; /* es_info_length too big */
  int table_id_extension_error; /* table_id_extension != transport_stream_id */
  int pes_pid_error; /* pid of pes is psi/si */
  int pes_elem_error; /* pid of pes is not es in pmt */
  int pes_start_code_error; /* pes start code not 0x000001 */
  int pes_stream_id_error; /* stream_id less than 0xBC in TS */
  int pes_packet_length_error; /* pes_packet_length is too large */
  int pes_header_length_error; /* pes_header_length is too large */
  int pts_dts_flags_error; /* pts_dts_flags is 01 */
  int pmt_section_number_error; /* pmt section_number|last_section_number not 0x00 */
#define ERR_4_0_0 (1<<0) /* PAT changed */
#define ERR_4_0_1 (1<<1) /* CAT changed */
#define ERR_4_0_2 (1<<2) /* PMT changed */
  int section_crc32_error; /* sect->CRC_32 changed */
};