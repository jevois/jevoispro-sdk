/*
*
* SPDX-License-Identifier: GPL-2.0
*
* Copyright (C) 2011-2018 ARM or its affiliates
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; version 2.
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#if !defined(__IMX290_SENSOR_H__)
#define __IMX290_SENSOR_H__

#include "sensor_init.h"

// ####################################################################################################
// Initialization sequence - JeVois-Pro IMX290
// ####################################################################################################

// Registers that must be set to some preset value according to the datasheet.
// Note: compared to original linear 1080p sequence, this adds: 3134, 313B, 317F, 33B1 and changes value of
// 33B0 from 0x50 to 0x08
#define IMX290_PRESET(x) { IMX290_REG_PRESET ## x, IMX290_VAL_PRESET ## x, 0xff, 1 }

#define IMX290_PRESETS_DATASHEET IMX290_PRESET(300C), IMX290_PRESET(300F), IMX290_PRESET(3010), IMX290_PRESET(3012), \
    IMX290_PRESET(3013), IMX290_PRESET(3016), IMX290_PRESET(301B), IMX290_PRESET(3070), IMX290_PRESET(3071), \
    IMX290_PRESET(309B), IMX290_PRESET(30A2), IMX290_PRESET(30A6), IMX290_PRESET(30A8), IMX290_PRESET(30AA), \
    IMX290_PRESET(30AC), IMX290_PRESET(30B0), IMX290_PRESET(310B), IMX290_PRESET(3119), IMX290_PRESET(311C), \
    IMX290_PRESET(311E), IMX290_PRESET(3128), IMX290_PRESET(3134), IMX290_PRESET(313B), IMX290_PRESET(313D), \
    IMX290_PRESET(3150), IMX290_PRESET(317E), IMX290_PRESET(317F), IMX290_PRESET(32B8), IMX290_PRESET(32B9), \
    IMX290_PRESET(32BA), IMX290_PRESET(32BB), IMX290_PRESET(32C8), IMX290_PRESET(32C9), IMX290_PRESET(32CA), \
    IMX290_PRESET(32CB), IMX290_PRESET(332C), IMX290_PRESET(332D), IMX290_PRESET(332E), IMX290_PRESET(3358), \
    IMX290_PRESET(3359), IMX290_PRESET(335A), IMX290_PRESET(3360), IMX290_PRESET(3361), IMX290_PRESET(3362), \
    IMX290_PRESET(33B0), IMX290_PRESET(33B1), IMX290_PRESET(33B2), IMX290_PRESET(33B3)

// Additional settings found in original linear 1080p sequence, but which are not documented in the datasheet:
#define IMX290_UNDOC_SETTINGS {0x309C, 0x22, 0xff, 1}, {0x3106, 0x00, 0xff, 1}, {0x3415, 0x01, 0xff, 1}, \
     {0x347B, 0x24, 0xff, 1}, {0x3480, 0x49, 0xff, 1}
 
// mclk: 37.125MHz or 74.250MHz
// bayer = BAYER_RGGB
 
// ####################################################################################################
static acam_reg_t linear_1080p_30fps[] =
{
 // IMX290_SOFT_RESET,
 IMX290_ENTER_STANDBY,                                // Enter standby mode so we can adjust settings

 IMX290_PRESETS_DATASHEET,                            // Apply presets recommended by the datasheet
 IMX290_UNDOC_SETTINGS,                               // Apply additional undocumented presets

 IMX290_INCK_37125_1080P,                             // Select master clock rate (37.125MHz or 74.250MHz)
 IMX290_SET_12BIT,                                    // Select pixel data bit width (12 or 10)
 IMX290_SET_DOL_LINEAR,                               // Select DOL-WDR type (LINEAR or FS_LIN)
 IMX290_SET_MIPI_4LANES,                              // Set MIPI output and number of lanes (2 or 4)

 IMX290_SET_WIN_1080P,                                // Set window mode (1080P, 720P, or CROP from 1080P)
 IMX290_SET_WIN_VFLIP(0),                             // Set Vertical image flip (0 or 1)
 IMX290_SET_WIN_HFLIP(0),                             // Set horizontal image flip (0 or 1)
 IMX290_SET_TIMINGS(1080P_4LANES_30FPS),              // Fine timing parameters for selected mode
 IMX290_SET_REPETITION_4LANES_30FPS,                  // Adjust timing for selected mode
 IMX290_SET_FRAME_30FPS,                              // Set frame rate (30FPS, 60FPS, 120FPS, 25FPS, 50FPS, 100FPS)
 IMX290_SET_HCG,                                      // Set conversion gain (HCG or LCG)
 IMX290_SET_VMAX(IMX290_VMAX_1080P),                  // Set vertical span
 IMX290_SET_HMAX(IMX290_HMAX_1080P_30FPS),            // Set horizontal span
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_1080P),  // Set vertical OB size
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_1080P),  // Set Y out size
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_1080P),  // Set X out size
 IMX290_SOUTSEL_NONE,                                 // Set which sync pulses to send out to pins
 
 IMX290_SET_SHS1(1),                                  // Storage time adjustments, in line units
 IMX290_SET_SHS2(0),
 IMX290_SET_RHS1(0),

 IMX290_SET_GAIN(32),                                 // Set gain (0.0 dB to 63.0 dB / 0.3 dB step)

 IMX290_MASTER_START,                                 // Start capturing in MASTER or SLAVE mode
 IMX290_SEQUENCE_END

    /*
    { IMX290_REG_STANDBY, 0x01, 0xff, 1},
    { IMX290_REG_DELAY, 1},

    { IMX290_REG_ADBIT, 0x01, 0xff, 1}, //0:10bit 1:12bit
    { IMX290_REG_WIN, 0x00, 0xff, 1}, //full hd 1080p
    { IMX290_REG_FRAME, 0x12, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0xF0, 0xff, 1}, //black level
    { IMX290_REG_BLKLEVEL_H, 0x00, 0xff, 1},
    { IMX290_REG_PRESET300C, 0x00, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_PRESET3013, 0x00, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},//Gain
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0x65, 0xff, 1},
    { IMX290_REG_VMAX_M, 0x04, 0xff, 1},
    { IMX290_REG_VMAX_H, 0x00, 0xff, 1},
    { IMX290_REG_PRESET301B, 0x00, 0xff, 1},
    { IMX290_REG_HMAX_L, 0x30, 0xff, 1},
    { IMX290_REG_HMAX_H, 0x11, 0xff, 1},
    { IMX290_REG_SHS1_L, 0x01, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS2_L, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_M, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_RHS1_L, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_DOL, 0x01, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0xe1, 0xff, 1},//LANE CHN
    { IMX290_REG_SOUTSEL, 0x00, 0xff, 1},
    //{ IMX290_REG_Y_OUT_SIZE_L, 0x5e, 0xff, 1},//Y_out size, tools should modify from B2 to 9C
    //{ IMX290_REG_Y_OUT_SIZE_H, 0x09, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1}, //must set
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},
    {0x309B, 0x10, 0xff, 1},
    {0x309C, 0x22, 0xff, 1},
    {0x30A2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30B0, 0x43, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},

    {0x3129, 0x00, 0xff, 1},

    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1A, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    { IMX290_REG_INCKSEL6, 0x1A, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    { IMX290_REG_PRESET317C, 0x00, 0xff, 1},
    {0x317E, 0x00, 0xff, 1},

    {0x31EC, 0x0E, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},
    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xD3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0D, 0xff, 1},
    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xE1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},
    { IMX290_REG_PRESET3360, 0x1E, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},
    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33B2, 0x1A, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x20, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0A, 0xff, 1},
    {0x3415, 0x01, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x49, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_H, 0x04, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_L, 0x0C, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0C, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},//mclk :37.125M
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},

    { IMX290_REG_TCLKPOST_L, 0x47, 0xff, 1},//global timming
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x1F, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x17, 0xff, 1},
    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x0F, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_THSTRAIL_L, 0x17, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0x47, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_L, 0x0F, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x0F, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0x9C, 0xff, 1},
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x347B, 0x24, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1},
    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XHSLNG, 0x0A, 0xff, 1},
    {0x0000, 0x0000, 0x0000, 0x0000},
*/

};

// ####################################################################################################
static acam_reg_t linear_1080p_60fps[] =
{
 // IMX290_SOFT_RESET,
 IMX290_ENTER_STANDBY,                                // Enter standby mode so we can adjust settings

 IMX290_PRESETS_DATASHEET,                            // Apply presets recommended by the datasheet
 IMX290_UNDOC_SETTINGS,                               // Apply additional undocumented presets

 IMX290_INCK_37125_1080P,                             // Select master clock rate (37.125MHz or 74.250MHz)
 IMX290_SET_12BIT,                                    // Select pixel data bit width (12 or 10)
 IMX290_SET_DOL_LINEAR,                               // Select DOL-WDR type (LINEAR or FS_LIN)
 IMX290_SET_MIPI_4LANES,                              // Set MIPI output and number of lanes (2 or 4)

 IMX290_SET_WIN_1080P,                                // Set window mode (1080P, 720P, or CROP from 1080P)
 IMX290_SET_WIN_VFLIP(0),                             // Set Vertical image flip (0 or 1)
 IMX290_SET_WIN_HFLIP(0),                             // Set horizontal image flip (0 or 1)
 IMX290_SET_TIMINGS(1080P_4LANES_60FPS),              // Fine timing parameters for selected mode
 IMX290_SET_REPETITION_4LANES_60FPS,                  // Adjust timing for selected mode
 IMX290_SET_FRAME_60FPS,                              // Set frame rate (30FPS, 60FPS, 120FPS, 25FPS, 50FPS, 100FPS)
 IMX290_SET_HCG,                                      // Set conversion gain (HCG or LCG)
 IMX290_SET_VMAX(IMX290_VMAX_1080P),                  // Set vertical span
 IMX290_SET_HMAX(IMX290_HMAX_1080P_60FPS),            // Set horizontal span
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_1080P),  // Set vertical OB size
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_1080P),  // Set Y out size
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_1080P),  // Set X out size
 IMX290_SOUTSEL_NONE,                                 // Set which sync pulses to send out to pins
 
 IMX290_SET_SHS1(1),                                  // Storage time adjustments, in line units
 IMX290_SET_SHS2(0),
 IMX290_SET_RHS1(0),

 IMX290_SET_GAIN(32),                                 // Set gain (0.0 dB to 63.0 dB / 0.3 dB step)

 IMX290_MASTER_START,                                 // Start capturing in MASTER or SLAVE mode
 IMX290_SEQUENCE_END
};

// ####################################################################################################
static acam_reg_t linear_1080p_120fps[] =
{
 // IMX290_SOFT_RESET,
 IMX290_ENTER_STANDBY,                                // Enter standby mode so we can adjust settings

 IMX290_PRESETS_DATASHEET,                            // Apply presets recommended by the datasheet
 IMX290_UNDOC_SETTINGS,                               // Apply additional undocumented presets

 IMX290_INCK_37125_1080P,                             // Select master clock rate (37.125MHz or 74.250MHz)
 IMX290_SET_10BIT,                                    // Select pixel data bit width (12 or 10)
 IMX290_SET_DOL_LINEAR,                               // Select DOL-WDR type (LINEAR or FS_LIN)
 IMX290_SET_MIPI_4LANES,                              // Set MIPI output and number of lanes (2 or 4)

 IMX290_SET_WIN_1080P,                                // Set window mode (1080P, 720P, or CROP from 1080P)
 IMX290_SET_WIN_VFLIP(0),                             // Set Vertical image flip (0 or 1)
 IMX290_SET_WIN_HFLIP(0),                             // Set horizontal image flip (0 or 1)
 IMX290_SET_TIMINGS(1080P_4LANES_120FPS),             // Fine timing parameters for selected mode
 IMX290_SET_REPETITION_4LANES_120FPS,                 // Adjust timing for selected mode
 IMX290_SET_FRAME_120FPS,                             // Set frame rate (30FPS, 60FPS, 120FPS, 25FPS, 50FPS, 100FPS)
 IMX290_SET_HCG,                                      // Set conversion gain (HCG or LCG)
 IMX290_SET_VMAX(IMX290_VMAX_1080P),                  // Set vertical span
 IMX290_SET_HMAX(IMX290_HMAX_1080P_120FPS),           // Set horizontal span
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_1080P),  // Set vertical OB size
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_1080P),  // Set Y out size
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_1080P),  // Set X out size
 IMX290_SOUTSEL_NONE,                                 // Set which sync pulses to send out to pins
 
 IMX290_SET_SHS1(1),                                  // Storage time adjustments, in line units
 IMX290_SET_SHS2(0),
 IMX290_SET_RHS1(0),

 IMX290_SET_GAIN(32),                                 // Set gain (0.0 dB to 63.0 dB / 0.3 dB step)

 // IMX290_SET_BLKLEVEL(IMX290_VAL_BLKLEVEL_12BIT),
 IMX290_SET_BLKLEVEL(0xe0),


 
 IMX290_MASTER_START,                                 // Start capturing in MASTER or SLAVE mode
 IMX290_SEQUENCE_END
};

// ####################################################################################################
static acam_reg_t linear_720p_30fps[] =
{
 // FIXME: frame collision issue. So we configure for half the HMAX
 
 IMX290_ENTER_STANDBY,                                // Enter standby mode so we can adjust settings

 IMX290_PRESETS_DATASHEET,                            // Apply presets recommended by the datasheet
 IMX290_UNDOC_SETTINGS,                               // Apply additional undocumented presets

 IMX290_INCK_37125_720P,                              // Select master clock rate (37.125MHz or 74.250MHz)
 IMX290_SET_12BIT,                                    // Select pixel data bit width (12 or 10)
 IMX290_SET_DOL_LINEAR,                               // Select DOL-WDR type (LINEAR or FS_LIN)
 IMX290_SET_MIPI_4LANES,                              // Set MIPI output and number of lanes (2 or 4)

 IMX290_SET_WIN_720P,                                 // Set window mode (1080P, 720P, or CROP from 1080P)
 IMX290_SET_TIMINGS(720P_4LANES_30FPS),               // Fine timing parameters for selected mode
 IMX290_SET_REPETITION_4LANES_30FPS,                  // Adjust timing for selected mode
 IMX290_SET_FRAME_30FPS,                              // Set frame rate (30FPS, 60FPS, 120FPS, 25FPS, 50FPS, 100FPS)
 IMX290_SET_HCG,                                      // Set conversion gain (HCG or LCG)
 IMX290_SET_VMAX(IMX290_VMAX_720P),                   // Set vertical span
 IMX290_SET_HMAX(4400),             // Set horizontal span
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_720P),   // Set vertical OB size
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_720P),   // Set Y out size
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_720P),   // Set X out size
 
 IMX290_SET_SHS1(100),                                // Storage time adjustments, in line units
 IMX290_SET_GAIN(32),                                 // Set gain (0.0 dB to 63.0 dB / 0.3 dB step)

 IMX290_MASTER_START,                                 // Start capturing in MASTER or SLAVE mode
 IMX290_SEQUENCE_END
};

// ####################################################################################################
static acam_reg_t linear_720p_30fps_10bits[] =
{
 // FIXME: frame collision issue. So we configure for half the HMAX
 IMX290_ENTER_STANDBY,                                // Enter standby mode so we can adjust settings

 IMX290_PRESETS_DATASHEET,                            // Apply presets recommended by the datasheet
 IMX290_UNDOC_SETTINGS,                               // Apply additional undocumented presets

 IMX290_INCK_37125_720P,                              // Select master clock rate (37.125MHz or 74.250MHz)
 IMX290_SET_10BIT,                                    // Select pixel data bit width (12 or 10)
 IMX290_SET_DOL_LINEAR,                               // Select DOL-WDR type (LINEAR or FS_LIN)
 IMX290_SET_MIPI_4LANES,                              // Set MIPI output and number of lanes (2 or 4)

 IMX290_SET_WIN_720P,                                 // Set window mode (1080P, 720P, or CROP from 1080P)
 IMX290_SET_TIMINGS(720P_4LANES_30FPS),               // Fine timing parameters for selected mode
 IMX290_SET_REPETITION_4LANES_30FPS,                  // Adjust timing for selected mode
 IMX290_SET_FRAME_30FPS,                              // Set frame rate (30FPS, 60FPS, 120FPS, 25FPS, 50FPS, 100FPS)
 IMX290_SET_HCG,                                      // Set conversion gain (HCG or LCG)
 IMX290_SET_VMAX(IMX290_VMAX_720P),                   // Set vertical span
 IMX290_SET_HMAX(IMX290_HMAX_720P_30FPS / 2),         // Set horizontal span
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_720P),   // Set vertical OB size
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_720P),   // Set Y out size
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_720P),   // Set X out size
 
 IMX290_SET_SHS1(100),                                // Storage time adjustments, in line units
 IMX290_SET_GAIN(32),                                 // Set gain (0.0 dB to 63.0 dB / 0.3 dB step)

 IMX290_MASTER_START,                                 // Start capturing in MASTER or SLAVE mode
 IMX290_SEQUENCE_END
};

// ####################################################################################################
#define IMX290_CROPPED_SEQ(name, w, h, f)                               \
  static acam_reg_t linear_##name##_##f##fps[] =                        \
  {                                                                     \
   IMX290_SOFT_RESET, IMX290_ENTER_STANDBY, IMX290_PRESETS_DATASHEET, IMX290_UNDOC_SETTINGS, \
   IMX290_INCK_37125_1080P, IMX290_SET_12BIT, IMX290_SET_DOL_LINEAR, IMX290_SET_MIPI_4LANES, \
   IMX290_SET_CROP(w, h), IMX290_SET_TIMINGS(1080P_4LANES_##f##FPS), IMX290_SET_REPETITION_4LANES_##f##FPS, \
   IMX290_SET_FRAME_##f##FPS, IMX290_SET_HCG, IMX290_SET_HMAX(IMX290_HMAX_1080P_##f##FPS), \
   IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_1080P), IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_1080P), \
   IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_1080P), IMX290_SET_SHS1(h>>1), IMX290_SET_GAIN(32), \
   IMX290_MASTER_START, IMX290_SEQUENCE_END                             \
  }

IMX290_CROPPED_SEQ(sxga, 1280, 1024, 30);
IMX290_CROPPED_SEQ(wxga, 1280,  800, 30);
IMX290_CROPPED_SEQ(xga,  1024,  768, 30);
IMX290_CROPPED_SEQ(svga,  800,  600, 30);
IMX290_CROPPED_SEQ(vga,   640,  480, 30);
IMX290_CROPPED_SEQ(cif,   352,  288, 30);
IMX290_CROPPED_SEQ(qvga,  320,  240, 30);
IMX290_CROPPED_SEQ(vga1080, 1440,  1080, 30);
IMX290_CROPPED_SEQ(sxga1080, 1350,  1080, 30);
IMX290_CROPPED_SEQ(cif1080, 1320,  1080, 30);

/*
static acam_reg_t linear_gogo_30fps[] =
{
 IMX290_SOFT_RESET, IMX290_ENTER_STANDBY, IMX290_PRESETS_DATASHEET, IMX290_UNDOC_SETTINGS,
 IMX290_INCK_37125_1080P, IMX290_SET_12BIT, IMX290_SET_DOL_LINEAR, IMX290_SET_MIPI_4LANES,
 IMX290_SET_CROP(1408, 1080),
 IMX290_SET_TIMINGS(1080P_4LANES_30FPS), IMX290_SET_REPETITION_4LANES_30FPS,
 IMX290_SET_FRAME_30FPS, IMX290_SET_HCG,
 IMX290_SET_HMAX(IMX290_HMAX_1080P_30FPS),
 IMX290_SET_OPB_SIZE_V(IMX290_VAL_OPB_SIZE_V_1080P),
 IMX290_SET_Y_OUT_SIZE(IMX290_VAL_Y_OUT_SIZE_1080P),
 IMX290_SET_X_OUT_SIZE(IMX290_VAL_X_OUT_SIZE_1080P),
 IMX290_SET_SHS1(100), IMX290_SET_GAIN(32),
 
 IMX290_MASTER_START, IMX290_SEQUENCE_END
};
*/

static acam_reg_t linear_1080p_50fps_446Mbps_4lane_10bits[] = {
    //{ IMX290_REG_RESET, 0x01, 0xff, 1}, /* sw_reset */
    //{ IMX290_REG_DELAY, 200},
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* XTMSTA */

    { IMX290_REG_ADBIT, 0x00, 0xff, 1},
    { IMX290_REG_WIN, 0x00, 0xff, 1},
    { IMX290_REG_FRAME, 0x01, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0x3c, 0xff, 1},
    //{ IMX290_REG_PRESET300C, 0x00, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0xB8, 0xff, 1},//VMAX change from 04C4 to 05B8, 50fps
    { IMX290_REG_VMAX_M, 0x05, 0xff, 1},//VMAX

    { IMX290_REG_HMAX_L, 0xEC, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    { IMX290_REG_HMAX_H, 0x07, 0xff, 1},//* HMAX */
    { IMX290_REG_SHS1_L, 0x02, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    //{ IMX290_REG_SHS2_L, 0xC9, 0xff, 1},//SHS2
    //{ IMX290_REG_SHS2_M, 0x07, 0xff, 1},//SHS2
    //{ IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    //{ IMX290_REG_RHS1_L, 0x0B, 0xff, 1},//RHS1
    //{ IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    //{ IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    //{ IMX290_REG_DOL, 0x05, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    { IMX290_REG_SOUTSEL, 0x0a, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x49, 0xff, 1},//Y_out size,1097
    { IMX290_REG_Y_OUT_SIZE_H, 0x04, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1},
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},

    {0x309b, 0x10, 0xff, 1},
    {0x309c, 0x22, 0xff, 1},

    {0x30a2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30b0, 0x43, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},
    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1a, 0xff, 1},
    { IMX290_REG_INCKSEL6, 0x1a, 0xff, 1},
    { IMX290_REG_ADBIT2, 0x12, 0xff, 1},
    {0x317e, 0x00, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xd3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0d, 0xff, 1},

    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xe1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},

    { IMX290_REG_PRESET3360, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},

    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33b2, 0x1a, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x10, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},

    { IMX290_REG_CSI_DT_FMT_L, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},
    { IMX290_REG_TCLKPOST_L, 0x57, 0xff, 1},
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x37, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x1f, 0xff, 1},

    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},

    { IMX290_REG_THSTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0x77, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},

    { IMX290_REG_TCLKPREPARE_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x17, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0x9c, 0xff, 1},
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000}
};

/*
 * bayer = BAYER_GBRG
 */
static acam_reg_t linear_1080p_60fps_446Mbps_4lane_10bits[] = {
    //{ IMX290_REG_RESET, 0x01, 0xff, 1}, /* sw_reset */
    //{ IMX290_REG_DELAY, 200},
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* XTMSTA */

    { IMX290_REG_ADBIT, 0x00, 0xff, 1},
    { IMX290_REG_WIN, 0x00, 0xff, 1},
    { IMX290_REG_FRAME, 0x01, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0x3c, 0xff, 1},
    //{ IMX290_REG_PRESET300C, 0x00, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0xC4, 0xff, 1},//VMAX change to 04C4  60fps
    { IMX290_REG_VMAX_M, 0x04, 0xff, 1},//VMAX

    { IMX290_REG_HMAX_L, 0xEC, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    { IMX290_REG_HMAX_H, 0x07, 0xff, 1},//* HMAX */
    { IMX290_REG_SHS1_L, 0x02, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    //{ IMX290_REG_SHS2_L, 0xC9, 0xff, 1},//SHS2
    //{ IMX290_REG_SHS2_M, 0x07, 0xff, 1},//SHS2
    //{ IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    //{ IMX290_REG_RHS1_L, 0x0B, 0xff, 1},//RHS1
    //{ IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    //{ IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    //{ IMX290_REG_DOL, 0x05, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    { IMX290_REG_SOUTSEL, 0x0a, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x49, 0xff, 1},//Y_out size,1097
    { IMX290_REG_Y_OUT_SIZE_H, 0x04, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1},
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},

    {0x309b, 0x10, 0xff, 1},
    {0x309c, 0x22, 0xff, 1},

    {0x30a2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30b0, 0x43, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},
    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1a, 0xff, 1},
    { IMX290_REG_INCKSEL6, 0x1a, 0xff, 1},
    { IMX290_REG_ADBIT2, 0x12, 0xff, 1},
    {0x317e, 0x00, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xd3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0d, 0xff, 1},

    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xe1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},

    { IMX290_REG_PRESET3360, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},

    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33b2, 0x1a, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x10, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},

    { IMX290_REG_CSI_DT_FMT_L, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},
    { IMX290_REG_TCLKPOST_L, 0x57, 0xff, 1},
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x37, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x1f, 0xff, 1},

    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},

    { IMX290_REG_THSTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0x77, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},

    { IMX290_REG_TCLKPREPARE_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x17, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0x9c, 0xff, 1},
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t dol_1080p_25fps_4lane_10bits[] = {
    //{ IMX290_REG_RESET, 0x01, 0xff, 1}, /* sw_reset */
    //{ IMX290_REG_DELAY, 200},
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* XTMSTA */

    { IMX290_REG_ADBIT, 0x00, 0xff, 1},
    { IMX290_REG_WIN, 0x00, 0xff, 1},
    { IMX290_REG_FRAME, 0x01, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0x3c, 0xff, 1},
    { IMX290_REG_PRESET300C, 0x11, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0xB8, 0xff, 1},//VMAX change from 04C4 to 05B8, 25fps
    { IMX290_REG_VMAX_M, 0x05, 0xff, 1},//VMAX

    { IMX290_REG_HMAX_L, 0xEC, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    { IMX290_REG_HMAX_H, 0x07, 0xff, 1},//* HMAX */
    { IMX290_REG_SHS1_L, 0x02, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS2_L, 0xC9, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_M, 0x07, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_RHS1_L, 0x0B, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_DOL, 0x05, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    { IMX290_REG_SOUTSEL, 0x0a, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x9C, 0xff, 1},//Y_out size, tools should modify from B2 to 9C
    { IMX290_REG_Y_OUT_SIZE_H, 0x08, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1},
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},

    {0x309b, 0x10, 0xff, 1},
    {0x309c, 0x22, 0xff, 1},

    {0x30a2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30b0, 0x43, 0xff, 1},

    {0x3106, 0x11, 0xff, 1}, //Need double confirm, H company 11h
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},
    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1a, 0xff, 1},
    { IMX290_REG_INCKSEL6, 0x1a, 0xff, 1},
    { IMX290_REG_ADBIT2, 0x12, 0xff, 1},
    {0x317e, 0x00, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xd3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0d, 0xff, 1},

    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xe1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},

    { IMX290_REG_PRESET3360, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},

    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33b2, 0x1a, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x10, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},

    { IMX290_REG_CSI_DT_FMT_L, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},
    { IMX290_REG_TCLKPOST_L, 0x57, 0xff, 1},
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x37, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x1f, 0xff, 1},

    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},

    { IMX290_REG_THSTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0x77, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},

    { IMX290_REG_TCLKPREPARE_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x17, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0xA0, 0xff, 1},//Xout size from 079c to 07A0,8/3th's info
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x347B, 0x23, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t dol_1080p_30fps_4lane_10bits[] = {
    //{ IMX290_REG_RESET, 0x01, 0xff, 1}, /* sw_reset */
    //{ IMX290_REG_DELAY, 200},
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* XTMSTA */

    { IMX290_REG_ADBIT, 0x00, 0xff, 1},
    { IMX290_REG_WIN, 0x00, 0xff, 1},
    { IMX290_REG_FRAME, 0x01, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0x3c, 0xff, 1},
    { IMX290_REG_PRESET300C, 0x11, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0xC4, 0xff, 1},//VMAX change from 0465 to 04C4
    { IMX290_REG_VMAX_M, 0x04, 0xff, 1},//VMAX

    { IMX290_REG_HMAX_L, 0xEC, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    { IMX290_REG_HMAX_H, 0x07, 0xff, 1},//* HMAX */
    { IMX290_REG_SHS1_L, 0x3c, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS2_L, 0xcb, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_M, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_RHS1_L, 0xc9, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_DOL, 0x05, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    { IMX290_REG_SOUTSEL, 0x0a, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x5e, 0xff, 1},//Y_out size, tools should modify from B2 to 9C
    { IMX290_REG_Y_OUT_SIZE_H, 0x09, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1},
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},

    {0x309b, 0x10, 0xff, 1},
    {0x309c, 0x22, 0xff, 1},

    {0x30a2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30b0, 0x43, 0xff, 1},

    {0x3106, 0x11, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},
    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1a, 0xff, 1},
    { IMX290_REG_INCKSEL6, 0x1a, 0xff, 1},
    { IMX290_REG_ADBIT2, 0x12, 0xff, 1},
    {0x317e, 0x00, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xd3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0d, 0xff, 1},

    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xe1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},

    { IMX290_REG_PRESET3360, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},

    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33b2, 0x1a, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x10, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},

    { IMX290_REG_CSI_DT_FMT_L, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},
    { IMX290_REG_TCLKPOST_L, 0x57, 0xff, 1},
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x37, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x1f, 0xff, 1},

    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},

    { IMX290_REG_THSTRAIL_L, 0x1f, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0x77, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},

    { IMX290_REG_TCLKPREPARE_L, 0x1f, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x17, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0xA0, 0xff, 1},//Xout size from 079c to 07A0,8/3th's info
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x347B, 0x23, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},

    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t dol_1080p_60fps_4lane_10bits[] = {
    //{ IMX290_REG_RESET, 0x01, 0xff, 1}, /* sw_reset */
    //{ IMX290_REG_DELAY, 200},
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_DELAY, 1},
    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* XTMSTA */

    { IMX290_REG_ADBIT, 0x00, 0xff, 1},
    { IMX290_REG_WIN, 0x00, 0xff, 1},
    { IMX290_REG_FRAME, 0x00, 0xff, 1},
    { IMX290_REG_BLKLEVEL_L, 0x3c, 0xff, 1},
    { IMX290_REG_PRESET300C, 0x11, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    { IMX290_REG_PRESET3010, 0x21, 0xff, 1},
    { IMX290_REG_PRESET3012, 0x64, 0xff, 1},
    { IMX290_REG_GAIN, 0x20, 0xff, 1},
    { IMX290_REG_PRESET3016, 0x09, 0xff, 1},
    { IMX290_REG_VMAX_L, 0xc4, 0xff, 1},//VMAX change from 0465 to 04C4
    { IMX290_REG_VMAX_M, 0x04, 0xff, 1},//VMAX

    { IMX290_REG_HMAX_L, 0xf6, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    { IMX290_REG_HMAX_H, 0x03, 0xff, 1},//* HMAX */
    { IMX290_REG_SHS1_L, 0x02, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_M, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS1_H, 0x00, 0xff, 1},//SHS1
    { IMX290_REG_SHS2_L, 0x49, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_M, 0x08, 0xff, 1},//SHS2
    { IMX290_REG_SHS2_H, 0x00, 0xff, 1},//SHS2
    { IMX290_REG_RHS1_L, 0xc9, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_M, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_RHS1_H, 0x00, 0xff, 1},//RHS1
    { IMX290_REG_DOL, 0x05, 0xff, 1},//DOL
    { IMX290_REG_OPORT, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    { IMX290_REG_SOUTSEL, 0x0a, 0xff, 1},
    { IMX290_REG_Y_OUT_SIZE_L, 0x5e, 0xff, 1},//Y_out size, tools should modify from B2 to 9C
    { IMX290_REG_Y_OUT_SIZE_H, 0x09, 0xff, 1},//Y_out size

    { IMX290_REG_INCKSEL1, 0x18, 0xff, 1},
    { IMX290_REG_INCKSEL2, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1},
    { IMX290_REG_INCKSEL4, 0x01, 0xff, 1},

    { IMX290_REG_PRESET3070, 0x02, 0xff, 1},
    { IMX290_REG_PRESET3071, 0x11, 0xff, 1},

    {0x309b, 0x10, 0xff, 1},
    {0x309c, 0x22, 0xff, 1},

    {0x30a2, 0x02, 0xff, 1},
    { IMX290_REG_PRESET30A6, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30A8, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AA, 0x20, 0xff, 1},
    { IMX290_REG_PRESET30AC, 0x20, 0xff, 1},
    {0x30b0, 0x43, 0xff, 1},

    {0x3106, 0x11, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    { IMX290_REG_PRESET3119, 0x9e, 0xff, 1},
    {0x311c, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET311E, 0x08, 0xff, 1},

    { IMX290_REG_PRESET3128, 0x05, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},
    { IMX290_REG_PRESET313D, 0x83, 0xff, 1},
    {0x3150, 0x03, 0xff, 1},
    { IMX290_REG_INCKSEL5, 0x1a, 0xff, 1},
    { IMX290_REG_INCKSEL6, 0x1a, 0xff, 1},
    { IMX290_REG_ADBIT2, 0x12, 0xff, 1},
    {0x317e, 0x00, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    { IMX290_REG_PRESET32B8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32B9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32BA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32BB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET32C8, 0x50, 0xff, 1},
    { IMX290_REG_PRESET32C9, 0x10, 0xff, 1},
    { IMX290_REG_PRESET32CA, 0x00, 0xff, 1},
    { IMX290_REG_PRESET32CB, 0x04, 0xff, 1},

    { IMX290_REG_PRESET332C, 0xd3, 0xff, 1},
    { IMX290_REG_PRESET332D, 0x10, 0xff, 1},
    { IMX290_REG_PRESET332E, 0x0d, 0xff, 1},

    { IMX290_REG_PRESET3358, 0x06, 0xff, 1},
    { IMX290_REG_PRESET3359, 0xe1, 0xff, 1},
    { IMX290_REG_PRESET335A, 0x11, 0xff, 1},

    { IMX290_REG_PRESET3360, 0x1e, 0xff, 1},
    { IMX290_REG_PRESET3361, 0x61, 0xff, 1},
    { IMX290_REG_PRESET3362, 0x10, 0xff, 1},

    { IMX290_REG_PRESET33B0, 0x50, 0xff, 1},
    {0x33b2, 0x1a, 0xff, 1},
    { IMX290_REG_PRESET33B3, 0x04, 0xff, 1},

    { IMX290_REG_REPETITION, 0x00, 0xff, 1},
    { IMX290_REG_PHY_LANES, 0x03, 0xff, 1},
    { IMX290_REG_OPB_SIZE_V, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},

    { IMX290_REG_CSI_DT_FMT_L, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_DT_FMT_H, 0x0a, 0xff, 1},
    { IMX290_REG_CSI_LANE_MODE, 0x03, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_L, 0x20, 0xff, 1},
    { IMX290_REG_EXTCK_FREQ_H, 0x25, 0xff, 1},
    { IMX290_REG_TCLKPOST_L, 0x77, 0xff, 1},
    { IMX290_REG_TCLKPOST_H, 0x00, 0xff, 1},
    { IMX290_REG_THSZERO_L, 0x67, 0xff, 1},
    { IMX290_REG_THSZERO_H, 0x00, 0xff, 1},
    { IMX290_REG_THSPREPARE_L, 0x47, 0xff, 1},

    { IMX290_REG_THSPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_L, 0x37, 0xff, 1},
    { IMX290_REG_TCLKTRAIL_H, 0x00, 0xff, 1},

    { IMX290_REG_THSTRAIL_L, 0x3f, 0xff, 1},
    { IMX290_REG_THSTRAIL_H, 0x00, 0xff, 1},
    { IMX290_REG_TCLKZERO_L, 0xff, 0xff, 1},
    { IMX290_REG_TCLKZERO_H, 0x00, 0xff, 1},

    { IMX290_REG_TCLKPREPARE_L, 0x3f, 0xff, 1},
    { IMX290_REG_TCLKPREPARE_H, 0x00, 0xff, 1},
    { IMX290_REG_TLPX_L, 0x37, 0xff, 1},
    { IMX290_REG_TLPX_H, 0x00, 0xff, 1},

    { IMX290_REG_X_OUT_SIZE_L, 0xA0, 0xff, 1},//Xout size from 079c to 07A0,8/3th's info
    { IMX290_REG_X_OUT_SIZE_H, 0x07, 0xff, 1},
    {0x347B, 0x23, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    { IMX290_REG_DELAY, 1},

    { IMX290_REG_XMSTA, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t imx290_test_pattern[] = {
    { IMX290_REG_STANDBY, 0x01, 0xff, 1}, /* standby */
    { IMX290_REG_BLKLEVEL_L, 0x00, 0xff, 1},
    {0x300e, 0x00, 0xff, 1},
    { IMX290_REG_PRESET300F, 0x00, 0xff, 1},
    {0x308c, 0x21, 0xff, 1},
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t settings_context_imx290[] = {
    { 0x19000, 0xf000f0L, 0xfff0fff,4 },
    { 0x1ae7c, 0xb2b4b4c8L, 0x0,4 },
    { 0x1b28c, 0x0L, 0x0,2 },
    { 0x1abb0, 0x1L, 0x31,1 },
    { 0x19004, 0xf000L, 0xfffff,4 },
    { 0x1b104, 0x0L, 0xfff,2 },
    { 0x19008, 0x6000180L, 0xfff0fff,4 },
    { 0x192ac, 0xffffL, 0x0,2 },
    { 0x1bbac, 0x1L, 0x77,1 },
    { 0x1900c, 0xc000600L, 0xfff0fff,4 },
    { 0x19010, 0x1800180L, 0xfff0fff,4 },
    { 0x19358, 0x6eaL, 0x0,2 },
    { 0x19014, 0x1000100L, 0xfff0fff,4 },
    { 0x1b144, 0xfffL, 0xfff,2 },
    { 0x1b038, 0x5dcL, 0xfff,2 },
    { 0x19018, 0x100L, 0xfff,2 },
    { 0x1b004, 0x40L, 0xfff0fff,4 },
    { 0x1aeb0, 0x81d461cL, 0x0,4 },
    { 0x1901c, 0x80000L, 0xfffff,4 },
    { 0x1b0e8, 0x800L, 0xfff,2 },
    { 0x1b138, 0xfffL, 0xfff,2 },
    { 0x19020, 0x21400L, 0x3f3f3f,4 },
    { 0x192b0, 0x400L, 0x0,2 },
    { 0x19024, 0x2000040L, 0xfff0fff,4 },
    { 0x1b0f4, 0x333L, 0x0,2 },
    { 0x1b0d8, 0x0L, 0x0,2 },
    { 0x19028, 0x0L, 0x3fffff,4 },
    { 0x1935c, 0x0L, 0x0,4 },
    { 0x1902c, 0x0L, 0xfff00ff,4 },
    { 0x1c0f0, 0x4380780L, 0x0,4 },
    { 0x1b2cc, 0x0L, 0xfff,2 },
    { 0x1ac20, 0x100L, 0xfff,2 },
    { 0x19030, 0x0L, 0x3fffff,4 },
    { 0x1b008, 0x200L, 0xfff,2 },
    { 0x1b100, 0xfbd8L, 0x0,2 },
    { 0x19034, 0x0L, 0xfffff,4 },
    { 0x1b028, 0xc83L, 0xfff,2 },
    { 0x19038, 0x0L, 0xfff,2 },
    { 0x1abb4, 0x400040L, 0x7f007f,4 },
    { 0x1b2b0, 0x1ffL, 0xfff,2 },
    { 0x1903c, 0x0L, 0x3fffff,4 },
    { 0x1afe4, 0x10a5L, 0x1fff,2 },
    { 0x1c070, 0x0L, 0xfff0fff,4 },
    { 0x1b128, 0x0L, 0xfff,2 },
    { 0x19040, 0x0L, 0xfffff,4 },
    { 0x1ac60, 0x0L, 0xfffff,4 },
    { 0x19044, 0x0L, 0xfff0fff,4 },
    { 0x1b048, 0xfffL, 0xfff,2 },
    { 0x1c124, 0x0L, 0x1ff,2 },
    { 0x19048, 0x400100L, 0xff0fff,4 },
    { 0x1b00c, 0xf8L, 0xfff0fff,4 },
    { 0x1b0d4, 0x1L, 0x1,1 },
    { 0x1904c, 0x3L, 0x10107,4 },
    { 0x19050, 0xdac0dacL, 0xfff0fff,4 },
    { 0x1abb8, 0x10000040L, 0x1fff1fff,4 },
    { 0x19054, 0xdac0dacL, 0xfff0fff,4 },
    { 0x1c1e0, 0x100L, 0xfff,2 },
    { 0x1ac5c, 0x0L, 0x3,1 },
    { 0x19058, 0x8000800L, 0xfff0fff,4 },
    { 0x1b2d0, 0xfffL, 0xfff,2 },
    { 0x1b2a0, 0x3ffL, 0x3ff,2 },
    { 0x1905c, 0x200800L, 0xfff0fff,4 },
    { 0x1afcc, 0x300L, 0xfff,2 },
    { 0x1b118, 0xe7b4L, 0x0,2 },
    { 0x19060, 0x800040L, 0xfff0fff,4 },
    { 0x1b010, 0x7adL, 0xfff,2 },
    { 0x1ac64, 0x1000100L, 0xfff0fff,4 },
    { 0x1ac54, 0x100L, 0x3ffff,4 },
    { 0x1ac68, 0x1000100L, 0xfff0fff,4 },
    { 0x1abbc, 0x0L, 0x1,1 },
    { 0x1926c, 0x100L, 0x1fff,2 },
    { 0x1b148, 0xffffL, 0x0,2 },
    { 0x1ac3c, 0x700202L, 0x3ff0303,4 },
    { 0x19270, 0xf000L, 0xfffff,4 },
    { 0x19368, 0x1e02L, 0xff07,2 },
    { 0x1c08c, 0x6a40000L, 0xffff00ff,4 },
    { 0x19274, 0x0L, 0xfffff,4 },
    { 0x1aecc, 0x3330333L, 0x1fff1fff,4 },
    { 0x1ac4c, 0xb4dc0a01L, 0xffff0f01,4 },
    { 0x19278, 0x0L, 0xfffff,4 },
    { 0x1b014, 0x320L, 0xfff0fff,4 },
    { 0x1b290, 0x5L, 0x0,2 },
    { 0x1927c, 0x0L, 0xfffff,4 },
    { 0x1b108, 0xf0L, 0xfff,2 },
    { 0x19280, 0x0L, 0xfffff,4 },
    { 0x1abc0, 0x7d0L, 0x0,2 },
    { 0x1b160, 0x100L, 0xfff,2 },
    { 0x19284, 0xf000L, 0xfffff,4 },
    { 0x1ac44, 0xffff0000L, 0x0,4 },
    { 0x1ac24, 0x0L, 0xfffff,4 },
    { 0x18e88, 0x4380780L, 0x0,4 },
    { 0x1936c, 0x0L, 0x0,2 },
    { 0x18e8c, 0x0L, 0x3030303,4 },
    { 0x1b03c, 0x190L, 0xfff0fff,4 },
    { 0x19290, 0x0L, 0x0,2 },
    { 0x1b018, 0x960L, 0xfff,2 },
    { 0x1aeb4, 0x1f8bL, 0x1fff,2 },
    { 0x19294, 0xfc300aaL, 0xfff0fff,4 },
    { 0x1b2bc, 0xc4L, 0xfff,2 },
    { 0x1b13c, 0xffffL, 0x0,2 },
    { 0x19298, 0x8000L, 0xffffff,4 },
    { 0x1abc4, 0xf000L, 0xfffff,4 },
    { 0x1b280, 0xd0L, 0xfff,2 },
    { 0x1929c, 0xaa0040L, 0xffff00ff,4 },
    { 0x1b04c, 0x20L, 0xfff0fff,4 },
    { 0x192a0, 0x80011dL, 0xff0fff,4 },
    { 0x19370, 0x4000L, 0x0,2 },
    { 0x192a4, 0x0L, 0x0,4 },
    { 0x1c0f4, 0x5000000L, 0x0,4 },
    { 0x192a8, 0x0L, 0x0,2 },
    { 0x1aa1c, 0x1L, 0x3,1 },
    { 0x1c078, 0x1L, 0x1,1 },
    { 0x18eac, 0x30L, 0x3f,1 },
    { 0x1aec0, 0x808b2L, 0xffffff,4 },
    { 0x1b02c, 0x6eL, 0xfff0fff,4 },
    { 0x18eb0, 0x2L, 0x1f,1 },
    { 0x1abc8, 0xf000L, 0xfffff,4 },
    { 0x1b2b4, 0x40L, 0xfff,2 },
    { 0x192b4, 0x0L, 0x17f,2 },
    { 0x1afe8, 0x1016L, 0x1fff,2 },
    { 0x1b12c, 0xf0L, 0xfff,2 },
    { 0x18eb8, 0x0L, 0x7,1 },
    { 0x19374, 0x8000L, 0x0,2 },
    { 0x1b278, 0x10L, 0xfff,2 },
    { 0x1c074, 0x0L, 0xfff,2 },
    { 0x18ebc, 0x0L, 0x7f,1 },
    { 0x1afdc, 0x21c03c0L, 0x0,4 },
    { 0x1b0dc, 0x0L, 0x0,1 },
    { 0x1c128, 0x0L, 0x0,4 },
    { 0x18ec0, 0xc5L, 0x0,1 },
    { 0x1aa20, 0x2L, 0x1f,1 },
    { 0x18ec4, 0x0L, 0xf,1 },
    { 0x1b14c, 0x1010L, 0x3f3f,2 },
    { 0x18ec8, 0x2L, 0x1f,1 },
    { 0x1abcc, 0xf000L, 0xfffff,4 },
    { 0x1c1e8, 0x0L, 0xfff,2 },
    { 0x18ecc, 0x0L, 0x301,2 },
    { 0x1c1e4, 0x0L, 0xfff0fff,4 },
    { 0x1b01c, 0x100L, 0xfff0fff,4 },
    { 0x18ed0, 0x1000000L, 0x1010226,4 },
    { 0x19378, 0xc000L, 0x0,2 },
    { 0x1b2a4, 0x0L, 0x3ff,2 },
    { 0x18ed4, 0x3010200L, 0x3030303,4 },
    { 0x1b11c, 0x0L, 0xfff,2 },
    { 0x18ed8, 0x0L, 0x1,1 },
    { 0x1aa24, 0x2L, 0xff0f,2 },
    { 0x1b164, 0xed8L, 0x0,2 },
    { 0x18edc, 0x3L, 0x0,1 },
    { 0x1ac28, 0x1L, 0x1,1 },
    { 0x18ee0, 0xfffffL, 0xfffff,4 },
    { 0x1abd0, 0xf000L, 0xfffff,4 },
    { 0x18ee4, 0xfffffL, 0xfffff,4 },
    { 0x1b124, 0xe7b4L, 0x0,2 },
    { 0x18ee8, 0xfffffL, 0xfffff,4 },
    { 0x1937c, 0x40000000L, 0x0,4 },
    { 0x1c090, 0x1180118L, 0x3fff3fff,4 },
    { 0x18eec, 0x8ffffL, 0xfffff,4 },
    { 0x1ac6c, 0x0L, 0x1,1 },
    { 0x18ef0, 0x8ffffL, 0xfffff,4 },
    { 0x1aa28, 0x7800L, 0xff07,2 },
    { 0x1b294, 0xfffaL, 0x0,2 },
    { 0x18ef4, 0x8ffffL, 0xfffff,4 },
    { 0x1b050, 0x0L, 0xfff,2 },
    { 0x1b0d0, 0x1L, 0x1,1 },
    { 0x18ef8, 0x0L, 0x0,2 },
    { 0x1aed4, 0x4000L, 0xfffff,4 },
    { 0x18efc, 0xfffffL, 0xfffff,4 },
    { 0x1b2c4, 0x2121L, 0x0,2 },
    { 0x1c100, 0x0L, 0x0,4 },
    { 0x1ae80, 0x5dL, 0x0,1 },
    { 0x1ac58, 0x1000L, 0x3ffff,4 },
    { 0x1b158, 0xed8L, 0x0,2 },
    { 0x1c144, 0x0L, 0x21f,2 },
    { 0x1ac30, 0x0L, 0xfffff,4 },
    { 0x18f08, 0x0L, 0x1,1 },
    { 0x1aa2c, 0x0L, 0x0,2 },
    { 0x1c080, 0xa004cL, 0xff00ff,4 },
    { 0x1aeb8, 0x1fa4L, 0x1fff,2 },
    { 0x18f0c, 0x3L, 0x0,1 },
    { 0x1c104, 0x0L, 0x0,4 },
    { 0x18f10, 0xfffffL, 0xfffff,4 },
    { 0x1aed8, 0xfa00000L, 0xfff0000,4 },
    { 0x1b27c, 0x20L, 0xfff,2 },
    { 0x18f14, 0xfffffL, 0xfffff,4 },
    { 0x1b0fc, 0x258L, 0xfff,2 },
    { 0x18f18, 0xfffffL, 0xfffff,4 },
    { 0x1ae84, 0x140064L, 0xfff0fff,4 },
    { 0x18f1c, 0x8ffffL, 0xfffff,4 },
    { 0x1c0f8, 0x0L, 0x0,4 },
    { 0x18f20, 0x8ffffL, 0xfffff,4 },
    { 0x1aa30, 0x0L, 0x0,2 },
    { 0x1c1ec, 0x1L, 0x1,1 },
    { 0x18f24, 0x8ffffL, 0xfffff,4 },
    { 0x1b030, 0xcdL, 0xfff,2 },
    { 0x18f28, 0x0L, 0x0,2 },
    { 0x1aedc, 0x4000L, 0xfffff,4 },
    { 0x1b2b8, 0xc7L, 0xfff,2 },
    { 0x18f2c, 0xfffffL, 0xfffff,4 },
    { 0x1afec, 0x101fL, 0x1fff,2 },
    { 0x1c260, 0x0L, 0x21f,2 },
    { 0x1b130, 0xe7b4L, 0x0,2 },
    { 0x1afc8, 0x1L, 0x1,1 },
    { 0x19288, 0x7d0L, 0x0,2 },
    { 0x1b134, 0x0L, 0xfff,2 },
    { 0x1b0ec, 0x800L, 0xfff,2 },
    { 0x1c064, 0x1L, 0x1,1 },
    { 0x1ac2c, 0x40807fL, 0xffffff,4 },
    { 0x18f38, 0x0L, 0x1,1 },
    { 0x1aa34, 0x0L, 0x0,2 },
    { 0x1b10c, 0xe7b4L, 0x0,2 },
    { 0x18f3c, 0x3L, 0x0,1 },
    { 0x18f40, 0xfffffL, 0xfffff,4 },
    { 0x1aee0, 0x4eL, 0xff0fff,4 },
    { 0x18f44, 0xfffffL, 0xfffff,4 },
    { 0x1aec4, 0xf0L, 0xfff0fff,4 },
    { 0x1b020, 0x2d0L, 0xfff,2 },
    { 0x18f48, 0xfffffL, 0xfffff,4 },
    { 0x1928c, 0x5L, 0xcd,1 },
    { 0x1b2a8, 0x1ffL, 0xfff,2 },
    { 0x18f4c, 0x8ffffL, 0xfffff,4 },
    { 0x1b054, 0x0L, 0x0,1 },
    { 0x1b120, 0xf0L, 0xfff,2 },
    { 0x18f50, 0x8ffffL, 0xfffff,4 },
    { 0x1aa38, 0x0L, 0x0,2 },
    { 0x18f54, 0x8ffffL, 0xfffff,4 },
    { 0x1c11c, 0xde78adf4L, 0x0,4 },
    { 0x18f58, 0x0L, 0x0,2 },
    { 0x1aee4, 0x4000L, 0xfffff,4 },
    { 0x18f5c, 0xfffffL, 0xfffff,4 },
    { 0x1b2d4, 0x0L, 0xfff,2 },
    { 0x1b730, 0x1L, 0x77,1 },
    { 0x19360, 0x0L, 0x0,4 },
    { 0x1ae90, 0x8000800L, 0xfff0fff,4 },
    { 0x1c094, 0x0L, 0xf,1 },
    { 0x19364, 0xffffffffL, 0x0,4 },
    { 0x1c1d8, 0x1L, 0x1,1 },
    { 0x1c108, 0x0L, 0xf,1 },
    { 0x18f68, 0x0L, 0x1,1 },
    { 0x1aa3c, 0x40L, 0x0,4 },
    { 0x1ae88, 0xf00064L, 0xfff0fff,4 },
    { 0x18f6c, 0x3L, 0x0,1 },
    { 0x1b298, 0x2121L, 0x0,2 },
    { 0x1ac50, 0x400L, 0x0,2 },
    { 0x18f70, 0xfffffL, 0xfffff,4 },
    { 0x1aee8, 0xfa0L, 0xfff,2 },
    { 0x18f74, 0xfffffL, 0xfffff,4 },
    { 0x1b154, 0x100L, 0xfff,2 },
    { 0x1c10c, 0x1e00L, 0x0,4 },
    { 0x18f78, 0xfffffL, 0xfffff,4 },
    { 0x1ae94, 0x800L, 0xfff0fff,4 },
    { 0x1b0f8, 0x0L, 0xfff,2 },
    { 0x18f7c, 0x8ffffL, 0xfffff,4 },
    { 0x1ac48, 0xffff0000L, 0x0,4 },
    { 0x1ac34, 0xd9999L, 0xfffff,4 },
    { 0x18f80, 0x8ffffL, 0xfffff,4 },
    { 0x1b040, 0xd48L, 0xfff,2 },
    { 0x1c084, 0x12cL, 0xff03ff,4 },
    { 0x1aebc, 0x1L, 0x1,1 },
    { 0x18f84, 0x8ffffL, 0xfffff,4 },
    { 0x1b000, 0x171L, 0x1fff,2 },
    { 0x18f88, 0x0L, 0x0,2 },
    { 0x1aeec, 0x4000L, 0xfffff,4 },
    { 0x1b288, 0x0L, 0x0,2 },
    { 0x18f8c, 0xfffffL, 0xfffff,4 },
    { 0x1afd4, 0x200fffL, 0xff0fff,4 },
    { 0x1ac40, 0x200L, 0x3ff,2 },
    { 0x1ae98, 0x0L, 0xfff0fff,4 },
    { 0x1b110, 0x0L, 0xfff,2 },
    { 0x1c0fc, 0x0L, 0x0,4 },
    { 0x1b2c8, 0xfffL, 0xfff,2 },
    { 0x18f98, 0x20000L, 0x70007,4 },
    { 0x1b044, 0x5dcL, 0xfff0fff,4 },
    { 0x18f9c, 0x0L, 0x3ffff,4 },
    { 0x1b034, 0x46L, 0xfff0fff,4 },
    { 0x18fa0, 0x0L, 0x1fff,2 },
    { 0x1aff0, 0x159L, 0x1fff,2 },
    { 0x1aeac, 0x0L, 0x0,1 },
    { 0x18fa4, 0x0L, 0xfff,2 },
    { 0x18fa8, 0x0L, 0x0,4 },
    { 0x1ae9c, 0x11d1cL, 0xffffff,4 },
    { 0x1c2b8, 0x0L, 0x21f,2 },
    { 0x18fac, 0x0L, 0x0,2 },
    { 0x1b0f0, 0xcccL, 0xfff,2 },
    { 0x18fb0, 0x0L, 0xf0f0f0f,4 },
    { 0x19348, 0x5cL, 0x0,1 },
    { 0x1b15c, 0x0L, 0xfff,2 },
    { 0x1b140, 0x0L, 0xfff,2 },
    { 0x18fb4, 0x0L, 0xfff0fff,4 },
    { 0x1c0ec, 0x202L, 0xbdf,2 },
    { 0x18fb8, 0x0L, 0xfff0fff,4 },
    { 0x1aff4, 0x103aL, 0x1fff,2 },
    { 0x18fbc, 0x0L, 0xfff0fff,4 },
    { 0x1c1dc, 0x1000100L, 0xfff0fff,4 },
    { 0x1b024, 0xfff01f4L, 0xfff0fff,4 },
    { 0x18fc0, 0x0L, 0xfff0fff,4 },
    { 0x1aea0, 0x0L, 0x0,1 },
    { 0x1b2ac, 0x40L, 0xfff,2 },
    { 0x18fc4, 0x0L, 0xfff0fff,4 },
    { 0x1afe0, 0x1bbL, 0x1fff,2 },
    { 0x1b0e4, 0x3fL, 0x7f,1 },
    { 0x1b284, 0xe0L, 0xfff,2 },
    { 0x18fc8, 0x0L, 0xfff0fff,4 },
    { 0x1934c, 0x3aL, 0x3f,1 },
    { 0x1c068, 0x1000100L, 0xfff0fff,4 },
    { 0x1b764, 0x2121L, 0x0,2 },
    { 0x18fcc, 0x0L, 0xfff0fff,4 },
    { 0x1b0e0, 0x96L, 0xfff,2 },
    { 0x18fd0, 0x0L, 0xfff0fff,4 },
    { 0x1aff8, 0x15L, 0x1fff,2 },
    { 0x1abd4, 0xf000L, 0xfffff,4 },
    { 0x1b29c, 0x0L, 0x1,1 },
    { 0x1afd8, 0xe95L, 0x0,2 },
    { 0x1aea4, 0xcf01b3L, 0xff0fff,4 },
    { 0x1aed0, 0x8L, 0xff0fff,4 },
    { 0x1aec8, 0x157c8000L, 0x0,4 },
    { 0x1afd0, 0x40000c8L, 0xfff0fff,4 },
    { 0x19350, 0x8L, 0x0,1 },
    { 0x1ae8c, 0x960171L, 0xfff0fff,4 },
    { 0x18fe4, 0x2L, 0xff03,2 },
    { 0x1c06c, 0x100L, 0xfff,2 },
    { 0x18fe8, 0x1000100L, 0xfff0fff,4 },
    { 0x1affc, 0x1086L, 0x1fff,2 },
    { 0x1b150, 0x0L, 0xfff,2 },
    { 0x1b114, 0xf0L, 0xfff,2 },
    { 0x18fec, 0x100L, 0xfff,2 },
    { 0x1c110, 0x1L, 0xf,1 },
    { 0x18ff0, 0xc000f00L, 0xfff0fff,4 },
    { 0x1aea8, 0x5596L, 0xffffff,4 },
    { 0x18ff4, 0xc000f00L, 0xfff0fff,4 },
    { 0x1ac38, 0x100L, 0xfff,2 },
    { 0x18ff8, 0xc000f00L, 0xfff0fff,4 },
    { 0x19354, 0x21c03c0L, 0x0,4 },
    { 0x1c088, 0x3e803e8L, 0x3ffffff,4 },
    { 0x18ffc, 0xf000f0L, 0xfff0fff,4 },
    { 0x1b2c0, 0x1b29acL, 0x0,4 },
    { 0x1ab78, 0x40f, 0xfff, 4 }, // set temper bits as 12
    { 0x1ab7c, 6, 0xff, 4 }, // set temper bits as 12
    //stop sequence - address is 0x0000
    { 0x0000, 0x0000, 0x0000, 0x0000 }
};

// ####################################################################################################
// Indices in the table below:
#define IMX290_SEQ_1080P_30FPS 0
#define IMX290_SEQ_1080P_60FPS 1
#define IMX290_SEQ_1080P_120FPS 2
#define IMX290_SEQ_720P_30FPS 3
#define IMX290_SEQ_720P_30FPS_10BITS 4
#define IMX290_SEQ_SXGA_30FPS 5
#define IMX290_SEQ_WXGA_30FPS 6
#define IMX290_SEQ_XGA_30FPS 7
#define IMX290_SEQ_SVGA_30FPS 8
#define IMX290_SEQ_VGA_30FPS 9
#define IMX290_SEQ_CIF_30FPS 10
#define IMX290_SEQ_QVGA_30FPS 11
#define IMX290_SEQ_VGA1080_30FPS 12
#define IMX290_SEQ_SXGA1080_30FPS 13
#define IMX290_SEQ_CIF1080_30FPS 14
#define IMX290_SEQ_TEST_PATTERN 20

static const acam_reg_t *imx290_seq_table[] =
{
 linear_1080p_30fps, // 0
 linear_1080p_60fps, // 1
 linear_1080p_120fps, // 2
 linear_720p_30fps,  // 3
 linear_720p_30fps_10bits, // 4
 linear_sxga_30fps, // 5
 linear_wxga_30fps, // 6
 linear_xga_30fps,  // 7
 linear_svga_30fps, // 8
 linear_vga_30fps,  // 9
 linear_cif_30fps,  // 10
 linear_qvga_30fps, // 11
 linear_vga1080_30fps, // 12
 linear_sxga1080_30fps, // 13
 linear_cif1080_30fps, // 14

 // keep compiler happy
 linear_1080p_50fps_446Mbps_4lane_10bits,
 linear_1080p_60fps_446Mbps_4lane_10bits, // frame mostly grey except purple on edges
 dol_1080p_25fps_4lane_10bits,
 dol_1080p_30fps_4lane_10bits,
 dol_1080p_60fps_4lane_10bits,
 imx290_test_pattern,
};

// ####################################################################################################
#define SENSOR_IMX290_CONTEXT_SEQ 0

static const acam_reg_t *isp_seq_table[] = {
    settings_context_imx290,
};

#endif /* __IMX290_SENSOR_H__ */
