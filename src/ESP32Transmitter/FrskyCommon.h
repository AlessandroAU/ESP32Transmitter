#pragma once
/******************************/
/**  FrSky V, D and X routines  **/
/******************************/
#include "CC2500.h"

//#if defined(FRSKYX_CC2500_INO) || defined(SFHSS_CC2500_INO) || defined(HITEC_CC2500_INO)
uint8_t calData[48];
//#endif

//Serial protocol
uint8_t sub_protocol = 0x01;
//sub_protocol = 0x01;
uint8_t protocol;
uint8_t option;
uint8_t cur_protocol[3];
uint8_t prev_option;
uint8_t prev_power = 0xFD; // unused power value
uint8_t  RX_num;

///////////from Multiprotocol.ino////////////////////////////////////////////////
//Global constants/variables
uint32_t MProtocol_id;//tx id,
uint32_t MProtocol_id_master;
uint32_t blink = 0, last_signal = 0;
//
uint16_t counter;
uint8_t  channel;
uint8_t  packet[40];

#define NUM_CHN 16
// Servo data
uint16_t Channel_data[NUM_CHN];
uint8_t  Channel_AUX;
#ifdef FAILSAFE_ENABLE
uint16_t Failsafe_data[NUM_CHN];
#endif

// Protocol variables
uint8_t  cyrfmfg_id[6];//for dsm2 and devo
uint8_t  rx_tx_addr[5];
uint8_t  rx_id[5];
uint8_t  phase;
uint16_t bind_counter;
uint8_t  bind_phase;
uint8_t  binding_idx;
uint16_t packet_period;
uint8_t  packet_count;
uint8_t  packet_sent;
uint8_t  packet_length;
//uint8_t  hopping_frequency[50];
uint8_t  hopping_frequency[50] = {0,130,25,155,50,180,75,205,100,230,125,20,150,45,175,70,200,95,225,120,15,145,40,170,65,195,92,222,115,10,140,35,165,60,190,85,215,110,5,135,30,160,55,185,80,210,105,0,0,0};
uint8_t  *hopping_frequency_ptr;
uint8_t  hopping_frequency_no = 0;
uint8_t  rf_ch_num;
uint8_t  throttle, rudder, elevator, aileron;
uint8_t  flags;
uint16_t crc;
uint8_t  crc8;
uint16_t seed;
uint16_t failsafe_count;
uint16_t state;
uint8_t  len;
uint8_t  armed, arm_flags, arm_channel_previous;
//////////////////////////////////////////////////////////////////////////////////

// Telemetry
#define MAX_PKT 29
uint8_t pkt[MAX_PKT];//telemetry receiving packets
#if defined(TELEMETRY)
#ifdef INVERT_TELEMETRY
#if not defined(ORANGE_TX) && not defined(STM32_BOARD)
// enable bit bash for serial
#define BASH_SERIAL 1
#endif
#define INVERT_SERIAL 1
#endif
uint8_t pass = 0;
uint8_t pktt[MAX_PKT];//telemetry receiving packets
#ifdef BASH_SERIAL
// For bit-bashed serial output
#define TXBUFFER_SIZE 192
volatile struct t_serial_bash
{
  uint8_t head ;
  uint8_t tail ;
  uint8_t data[TXBUFFER_SIZE] ;
  uint8_t busy ;
  uint8_t speed ;
} SerialControl ;
#else
#define TXBUFFER_SIZE 96
volatile uint8_t tx_buff[TXBUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;
#endif // BASH_SERIAL
uint8_t v_lipo1;
uint8_t v_lipo2;
uint8_t RX_RSSI;
uint8_t TX_RSSI;
uint8_t RX_LQI;
uint8_t TX_LQI;
uint8_t telemetry_link = 0;
uint8_t telemetry_counter = 0;
uint8_t telemetry_lost;
#ifdef SPORT_POLLING
#define MAX_SPORT_BUFFER 64
uint8_t SportData[MAX_SPORT_BUFFER];
bool  ok_to_send = false;
uint8_t sport_idx = 0;
uint8_t sport_index = 0;
#endif
#endif // TELEMETRY


// Channel value for FrSky (PPM is multiplied by 1.5)
uint16_t convert_channel_frsky(uint8_t num)
{
//  uint16_t val = Channel_data[num];
  uint16_t val = map(Channel_data[num], 1000, 2000, 226, 1824);
  
  return ((val * 15) >> 4) + 1290;
}

// Convert 32b id to rx_tx_addr
static void set_rx_tx_addr(uint32_t id)
{ // Used by almost all protocols
  rx_tx_addr[0] = (id >> 24) & 0xFF;
  rx_tx_addr[1] = (id >> 16) & 0xFF;
  rx_tx_addr[2] = (id >>  8) & 0xFF;
  rx_tx_addr[3] = (id >>  0) & 0xFF;
  rx_tx_addr[4] = (rx_tx_addr[2] & 0xF0) | (rx_tx_addr[3] & 0x0F);
}



/******************************/
/**  FrSky D and X routines  **/
/******************************/
//#if defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO)
enum {
  FRSKY_BIND    = 0,
  FRSKY_BIND_DONE = 1000,
  FRSKY_DATA1,
  FRSKY_DATA2,
  FRSKY_DATA3,
  FRSKY_DATA4,
  FRSKY_DATA5
};

void Frsky_init_hop(void)
{
#ifdef USE_BETAFLIGHT_BIND_INFO
////  set frsky_spi_bind_hop_data = 0,130,25,155,50,180,75,205,100,230,125,20,150,45,175,70,200,95,225,120,15,145,40,170,65,195,92,222,115,10,140,35,165,60,190,85,215,110,5,135,30,160,55,185,80,210,105,0,0,0
//hopping_frequency[50] = {0,130,25,155,50,180,75,205,100,230,125,20,150,45,175,70,200,95,225,120,15,145,40,170,65,195,92,222,115,10,140,35,165,60,190,85,215,110,5,135,30,160,55,185,80,210,105,0,0,0};
#else
//  uint8_t val;
//  uint8_t channel = rx_tx_addr[0] & 0x07;
//  uint8_t channel_spacing = rx_tx_addr[1];
//  //Filter bad tables
//  if (channel_spacing < 0x02) channel_spacing += 0x02;
//  if (channel_spacing > 0xE9) channel_spacing -= 0xE7;
//  if (channel_spacing % 0x2F == 0) channel_spacing++;
//
//  hopping_frequency[0] = channel;
//  for (uint8_t i = 1; i < 50; i++)
//  {
//    channel = (channel + channel_spacing) % 0xEB;
//    val = channel;
//    if ((val == 0x00) || (val == 0x5A) || (val == 0xDC))
//      val++;
//    hopping_frequency[i] = i > 46 ? 0 : val;
//  }
#endif
}
//#endif


//#if defined(FRSKYV_CC2500_INO) || defined(FRSKYD_CC2500_INO) || defined(FRSKYX_CC2500_INO)
const uint8_t FRSKY_common_startreg_cc2500_conf[] = {
  CC2500_02_IOCFG0 ,
  CC2500_00_IOCFG2 ,
  CC2500_17_MCSM1 ,
  CC2500_18_MCSM0 ,
  CC2500_06_PKTLEN ,
  CC2500_07_PKTCTRL1 ,
  CC2500_08_PKTCTRL0 ,
  CC2500_3E_PATABLE ,
  CC2500_0B_FSCTRL1 ,
  CC2500_0C_FSCTRL0 ,  // replaced by option value
  CC2500_0D_FREQ2 ,
  CC2500_0E_FREQ1 ,
  CC2500_0F_FREQ0 ,
  CC2500_10_MDMCFG4 ,
  CC2500_11_MDMCFG3 ,
  CC2500_12_MDMCFG2 ,
  CC2500_13_MDMCFG1 ,
  CC2500_14_MDMCFG0 ,
  CC2500_15_DEVIATN
};

//  #if defined(FRSKYV_CC2500_INO)
const uint8_t FRSKYV_cc2500_conf[] = {
  /*02_IOCFG0*/    0x06 ,
  /*00_IOCFG2*/    0x06 ,
  /*17_MCSM1*/     0x0c ,
  /*18_MCSM0*/     0x18 ,
  /*06_PKTLEN*/    0xff ,
  /*07_PKTCTRL1*/  0x04 ,
  /*08_PKTCTRL0*/  0x05 ,
  /*3E_PATABLE*/   0xfe ,
  /*0B_FSCTRL1*/   0x08 ,
  /*0C_FSCTRL0*/   0x00 ,
  /*0D_FREQ2*/     0x5c ,
  /*0E_FREQ1*/     0x58 ,
  /*0F_FREQ0*/     0x9d ,
  /*10_MDMCFG4*/   0xAA ,
  /*11_MDMCFG3*/   0x10 ,
  /*12_MDMCFG2*/   0x93 ,
  /*13_MDMCFG1*/   0x23 ,
  /*14_MDMCFG0*/   0x7a ,
  /*15_DEVIATN*/   0x41
};
// #endif

//#if defined(FRSKYD_CC2500_INO)
const uint8_t FRSKYD_cc2500_conf[] = {
  /*02_IOCFG0*/    0x06 ,
  /*00_IOCFG2*/    0x06 ,
  /*17_MCSM1*/     0x0c ,
  /*18_MCSM0*/     0x18 ,
  /*06_PKTLEN*/    0x19 ,
  /*07_PKTCTRL1*/  0x04 ,
  /*08_PKTCTRL0*/  0x05 ,
  /*3E_PATABLE*/   0xff ,
  /*0B_FSCTRL1*/   0x08 ,
  /*0C_FSCTRL0*/   0x00 ,
  /*0D_FREQ2*/     0x5c ,
  /*0E_FREQ1*/     0x76 ,
  /*0F_FREQ0*/     0x27 ,
  /*10_MDMCFG4*/   0xAA ,
  /*11_MDMCFG3*/   0x39 ,
  /*12_MDMCFG2*/   0x11 ,
  /*13_MDMCFG1*/   0x23 ,
  /*14_MDMCFG0*/   0x7a ,
  /*15_DEVIATN*/   0x42
};
//#endif

//#if defined(FRSKYX_CC2500_INO)
const uint8_t FRSKYX_cc2500_conf[] = {
  //FRSKYX
  /*02_IOCFG0*/    0x06 ,
  /*00_IOCFG2*/    0x06 ,
  /*17_MCSM1*/     0x0c ,
  /*18_MCSM0*/     0x18 ,
  /*06_PKTLEN*/    0x1E ,
  /*07_PKTCTRL1*/  0x04 ,
  /*08_PKTCTRL0*/  0x01 ,
  /*3E_PATABLE*/   0xff ,
  /*0B_FSCTRL1*/   0x0A ,
  /*0C_FSCTRL0*/   0x00 ,
  /*0D_FREQ2*/     0x5c ,
  /*0E_FREQ1*/     0x76 ,
  /*0F_FREQ0*/     0x27 ,
  /*10_MDMCFG4*/   0x7B ,
  /*11_MDMCFG3*/   0x61 ,
  /*12_MDMCFG2*/   0x13 ,
  /*13_MDMCFG1*/   0x23 ,
  /*14_MDMCFG0*/   0x7a ,
  /*15_DEVIATN*/   0x51
};
const uint8_t FRSKYXEU_cc2500_conf[] = {
  /*02_IOCFG0*/    0x06 ,
  /*00_IOCFG2*/    0x06 ,
  /*17_MCSM1*/     0x0E ,
  /*18_MCSM0*/     0x18 ,
  /*06_PKTLEN*/    0x23 ,
  /*07_PKTCTRL1*/  0x04 ,
  /*08_PKTCTRL0*/  0x01 ,
  /*3E_PATABLE*/   0xff ,
  /*0B_FSCTRL1*/   0x08 ,
  /*0C_FSCTRL0*/   0x00 ,
  /*0D_FREQ2*/     0x5c ,
  /*0E_FREQ1*/     0x80 ,
  /*0F_FREQ0*/     0x00 ,
  /*10_MDMCFG4*/   0x7B ,
  /*11_MDMCFG3*/   0xF8 ,
  /*12_MDMCFG2*/   0x03 ,
  /*13_MDMCFG1*/   0x23 ,
  /*14_MDMCFG0*/   0x7a ,
  /*15_DEVIATN*/   0x53
};
//#endif

const uint8_t FRSKY_common_end_cc2500_conf[][2] = {
  { CC2500_19_FOCCFG,   0x16 },
  { CC2500_1A_BSCFG,    0x6c },
  { CC2500_1B_AGCCTRL2, 0x43 },
  { CC2500_1C_AGCCTRL1, 0x40 },
  { CC2500_1D_AGCCTRL0, 0x91 },
  { CC2500_21_FREND1,   0x56 },
  { CC2500_22_FREND0,   0x10 },
  { CC2500_23_FSCAL3,   0xa9 },
  { CC2500_24_FSCAL2,   0x0A },
  { CC2500_25_FSCAL1,   0x00 },
  { CC2500_26_FSCAL0,   0x11 },
  { CC2500_29_FSTEST,   0x59 },
  { CC2500_2C_TEST2,    0x88 },
  { CC2500_2D_TEST1,    0x31 },
  { CC2500_2E_TEST0,    0x0B },
  { CC2500_03_FIFOTHR,  0x07 },
  { CC2500_09_ADDR,     0x00 }
};

void FRSKY_init_cc2500(const uint8_t *ptr)
{
  for (uint8_t i = 0; i < 19; i++)
  {
    uint8_t reg = pgm_read_byte_near(&FRSKY_common_startreg_cc2500_conf[i]);
    uint8_t val = pgm_read_byte_near(&ptr[i]);
    if (reg == CC2500_0C_FSCTRL0)
      val = option;
    CC2500_WriteReg(reg, val);
  }
  prev_option = option ;    // Save option to monitor FSCTRL0 change
  for (uint8_t i = 0; i < 17; i++)
  {
    uint8_t reg = pgm_read_byte_near(&FRSKY_common_end_cc2500_conf[i][0]);
    uint8_t val = pgm_read_byte_near(&FRSKY_common_end_cc2500_conf[i][1]);
    CC2500_WriteReg(reg, val);
  }
  CC2500_SetTxRxMode(TX_EN);
  CC2500_SetPower();
  CC2500_Strobe(CC2500_SIDLE);    // Go to idle...
}
//#endif
