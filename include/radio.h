#ifndef CC2538_RADIO_H_
#define CC2538_RADIO_H_


#include "cc2538-reg.h"


/*	RADIO FUNCTION */
#define IEEE_EUI64                              0x00280028  // Address of IEEE EUI-64 address

#define RFCORE_XREG_SRCMATCH                    0x40088608  // Source address matching and pending bits
#define RFCORE_XREG_SRCMATCH_EN                 0x00000001  // SRCMATCH.SRC_MATCH_EN(1)
#define RFCORE_XREG_SRCMATCH_AUTOPEND           0x00000002  // SRCMATCH.AUTOPEND(1)
#define RFCORE_XREG_SRCMATCH_PEND_DATAREQ_ONLY  0x00000004  // SRCMATCH.PEND_DATAREQ_ONLY(1)
#define RFCORE_XREG_SRCMATCH_ENABLE_STATUS_SIZE 3           // Num of register for source match enable status
#define RFCORE_XREG_SRCMATCH_SHORT_ENTRIES      24          // 24 short address entries in maximum
#define RFCORE_XREG_SRCMATCH_EXT_ENTRIES        12          // 12 extended address entries in maximum
#define RFCORE_XREG_SRCMATCH_SHORT_ENTRY_OFFSET 4           // address offset for one short address entry
#define RFCORE_XREG_SRCMATCH_EXT_ENTRY_OFFSET   8           // address offset for one extended address entry
#define RFCORE_XREG_SRCSHORTEN0                 0x4008860C  // Short address matching
#define RFCORE_XREG_SRCEXTEN0                   0x40088618  // Extended address matching
#define RFCORE_XREG_FRMCTRL0                    0x40088624  // Frame handling
#define RFCORE_XREG_FRMCTRL0_AUTOACK            0x00000020
#define RFCORE_XREG_FRMCTRL0_AUTOCRC            0x00000040
#define RFCORE_XREG_FRMCTRL0_INFINITY_RX        0x00000008
#define RFCORE_XREG_FRMCTRL1                    0x40088628  // Frame handling
#define RFCORE_XREG_FRMCTRL1_PENDING_OR         0x00000004
#define RFCORE_XREG_RXENABLE                    0x4008862C  // RX enabling
#define RFCORE_XREG_FREQCTRL                    0x4008863C  // Controls the RF frequency
#define RFCORE_XREG_TXPOWER                     0x40088640  // Controls the output power
#define RFCORE_XREG_FSMSTAT1                    0x4008864C  // Radio status register
#define RFCORE_XREG_FSMSTAT1_TX_ACTIVE          0x00000002
#define RFCORE_XREG_FSMSTAT1_CCA                0x00000010  // Clear channel assessment
#define RFCORE_XREG_FSMSTAT1_SFD                0x00000020
#define RFCORE_XREG_FSMSTAT1_FIFOP              0x00000040
#define RFCORE_XREG_FSMSTAT1_FIFO               0x00000080
#define RFCORE_XREG_FIFOPCTRL                   0x40088650  // FIFOP threshold
#define RFCORE_XREG_CCACTRL0                    0x40088658  // CCA threshold
#define RFCORE_XREG_RSSISTAT                    0x40088664  // RSSI valid status register
#define RFCORE_XREG_RSSISTAT_RSSI_VALID         0x00000001  // RSSI value is valid.
#define RFCORE_XREG_AGCCTRL1                    0x400886C8  // AGC reference level
#define RFCORE_XREG_RFC_OBS_CTRL0				0x400887AC	// RF observation mux control		
#define RFCORE_XREG_RFC_OBS_CTRL1				0x400887B0	// RF observation mux control
#define CCTEST_OBSSEL2							0x4401001C	//Select output signal on observation output 2
#define CCTEST_OBSSEL3							0x44010020	//Select output signal on observation output 2


#define RFCORE_XREG_TXFILTCFG                   0x400887E8  // TX filter configuration
#define RFCORE_XREG_RFRND                       0x4008869C  // Random data
#define RFCORE_XREG_RFRND_IRND                  0x00000001
#define RFCORE_XREG_FRMFILT0                    0x40088600  // The frame filtering function
#define RFCORE_XREG_FRMFILT0_FRAME_FILTER_EN    0x00000001  // Enables frame filtering

#define RFCORE_FFSM_SRCADDRESS_TABLE            0x40088400  // Source Address Table
#define RFCORE_FFSM_SRCEXTPENDEN0               0x40088590  // Enable/Disable automatic pending per extended address
#define RFCORE_FFSM_SRCSHORTPENDEN0             0x4008859C  // Enable/Disable automatic pending per short address
#define RFCORE_FFSM_EXT_ADDR0                   0x400885A8  // Local address information
#define RFCORE_FFSM_PAN_ID0                     0x400885C8  // Local address information
#define RFCORE_FFSM_PAN_ID1                     0x400885CC  // Local address information
#define RFCORE_FFSM_SHORT_ADDR0                 0x400885D0  // Local address information
#define RFCORE_FFSM_SHORT_ADDR1                 0x400885D4  // Local address information

#define RFCORE_SFR_RFDATA                       0x40088828  // The TX FIFO and RX FIFO
#define RFCORE_SFR_RFERRF                       0x4008882C  // RF error interrupt flags
#define RFCORE_SFR_RFERRF_RXOVERF               0x00000004  // RX FIFO overflowed.
#define RFCORE_SFR_RFIRQF0                      0x40088834  // RF interrupt flags
#define RFCORE_SFR_RFST                         0x40088838  // RF CSMA-CA/strobe processor
#define RFCORE_SFR_RFST_INSTR_RXON              0xE3        // Instruction set RX on
#define RFCORE_SFR_RFST_INSTR_TXON              0xE9        // Instruction set TX on
#define RFCORE_SFR_RFST_INSTR_RFOFF             0xEF        // Instruction set RF off
#define RFCORE_SFR_RFST_INSTR_FLUSHRX           0xED        // Instruction set flush rx buffer
#define RFCORE_SFR_RFST_INSTR_FLUSHTX           0xEE        // Instruction set flush tx buffer

#define ANALOG_O_IVCTRL                       0x00000004  // Analog control register


#endif