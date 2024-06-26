// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// JeVois Smart Embedded Machine Vision Toolkit - Copyright (C) 2022 by Laurent Itti, the University of Southern
// California (USC), and iLab at USC. See http://iLab.usc.edu and http://jevois.org for information about this project.
//
// This file is part of the JeVois Smart Embedded Machine Vision Toolkit.  This program is free software; you can
// redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software
// Foundation, version 2.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.  You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// Contact information: Laurent Itti - 3641 Watt Way, HNB-07A - Los Angeles, CA 90089-2520 - USA.
// Tel: +1 213 740 3527 - itti@pollux.usc.edu - http://iLab.usc.edu - http://jevois.org
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file */

#pragma once

// Special register, cause a delay instead
#define AR0234_REG_DELAY 0xffff

// End of sequence marker
#define AR0234_SEQUENCE_END {0x0000, 0x0000, 0x0000, 0x0000}

// ########## BANK 0 (0x30xx) ################################################################################
#define AR0234_REG_CHIP_VERSION_REG  0x3000
#define AR0234_REG_Y_ADDR_START  0x3002
#define AR0234_REG_X_ADDR_START  0x3004
#define AR0234_REG_Y_ADDR_END  0x3006
#define AR0234_REG_X_ADDR_END  0x3008
#define AR0234_REG_FRAME_LENGTH_LINES  0x300A
#define AR0234_REG_LINE_LENGTH_PCK  0x300C
#define AR0234_REG_REVISION_NUMBER  0x300E
#define AR0234_REG_LOCK_CONTROL  0x3010
#define AR0234_REG_COARSE_INTEGRATION_TIME  0x3012
#define AR0234_REG_FINE_INTEGRATION_TIME  0x3014
#define AR0234_REG_COARSE_INTEGRATION_TIME_CB  0x3016
#define AR0234_REG_FINE_INTEGRATION_TIME_CB  0x3018
#define AR0234_REG_RESET_REGISTER  0x301A
#define AR0234_REG_MODE_SELECT_  0x301C
#define AR0234_REG_IMAGE_ORIENTATION_  0x301D
#define AR0234_REG_DATA_PEDESTAL  0x301E
#define AR0234_REG_SOFTWARE_RESET_  0x3021
#define AR0234_REG_GROUPED_PARAMETER_HOLD_  0x3022
#define AR0234_REG_MASK_CORRUPTED_FRAMES_  0x3023
#define AR0234_REG_PIXEL_ORDER_  0x3024
#define AR0234_REG_GPI_STATUS  0x3026
#define AR0234_REG_ROW_SPEED  0x3028
#define AR0234_REG_VT_PIX_CLK_DIV  0x302A
#define AR0234_REG_VT_SYS_CLK_DIV  0x302C
#define AR0234_REG_PRE_PLL_CLK_DIV  0x302E
#define AR0234_REG_PLL_MULTIPLIER  0x3030
#define AR0234_REG_CTX_CONTROL_REG  0x3034
#define AR0234_REG_OP_PIX_CLK_DIV  0x3036
#define AR0234_REG_OP_SYS_CLK_DIV  0x3038
#define AR0234_REG_FRAME_COUNT  0x303A
#define AR0234_REG_FRAME_STATUS  0x303C
#define AR0234_REG_LINE_LENGTH_PCK_CB  0x303E
#define AR0234_REG_READ_MODE  0x3040
#define AR0234_REG_EXTRA_DELAY  0x3042
#define AR0234_REG_GREEN1_GAIN  0x3056
#define AR0234_REG_BLUE_GAIN  0x3058
#define AR0234_REG_RED_GAIN  0x305A
#define AR0234_REG_GREEN2_GAIN  0x305C
#define AR0234_REG_GLOBAL_GAIN  0x305E
#define AR0234_REG_ANALOG_GAIN  0x3060
#define AR0234_REG_SMIA_TEST  0x3064
#define AR0234_REG_CTX_WR_DATA_REG  0x3066
#define AR0234_REG_CTX_RD_DATA_REG  0x3068
#define AR0234_REG_DATAPATH_SELECT  0x306E
#define AR0234_REG_TEST_PATTERN_MODE  0x3070
#define AR0234_REG_TEST_DATA_RED  0x3072
#define AR0234_REG_TEST_DATA_GREENR  0x3074
#define AR0234_REG_TEST_DATA_BLUE  0x3076
#define AR0234_REG_TEST_DATA_GREENB  0x3078
#define AR0234_REG_OPERATION_MODE_CTRL  0x3082
#define AR0234_REG_OPERATION_MODE_CTRL_CB  0x3084
#define AR0234_REG_SEQ_DATA_PORT  0x3086
#define AR0234_REG_SEQ_CTRL_PORT  0x3088
#define AR0234_REG_X_ADDR_START_CB  0x308A
#define AR0234_REG_Y_ADDR_START_CB  0x308C
#define AR0234_REG_X_ADDR_END_CB  0x308E
#define AR0234_REG_Y_ADDR_END_CB  0x3090
#define AR0234_REG_X_EVEN_INC  0x30A0
#define AR0234_REG_X_ODD_INC  0x30A2
#define AR0234_REG_Y_EVEN_INC  0x30A4
#define AR0234_REG_Y_ODD_INC  0x30A6
#define AR0234_REG_Y_ODD_INC_CB  0x30A8
#define AR0234_REG_FRAME_LENGTH_LINES_CB  0x30AA
#define AR0234_REG_X_ODD_INC_CB  0x30AE
#define AR0234_REG_DIGITAL_TEST  0x30B0
#define AR0234_REG_TEMPSENS_DATA_REG  0x30B2
#define AR0234_REG_TEMPSENS_CTRL_REG  0x30B4
#define AR0234_REG_GREEN1_GAIN_CB  0x30BC
#define AR0234_REG_BLUE_GAIN_CB  0x30BE
#define AR0234_REG_RED_GAIN_CB  0x30C0
#define AR0234_REG_GREEN2_GAIN_CB  0x30C2
#define AR0234_REG_GLOBAL_GAIN_CB  0x30C4
#define AR0234_REG_TEMPSENS_CALIB1  0x30C6
#define AR0234_REG_TEMPSENS_CALIB2  0x30C8
#define AR0234_REG_GRR_CONTROL1  0x30CE
#define AR0234_REG_NOISE_PEDESTAL  0x30FE
#define AR0234_REG_AECTRLREG  0x3100
#define AR0234_REG_AE_LUMA_TARGET_REG  0x3102
#define AR0234_REG_AE_MIN_EV_STEP_REG  0x3108
#define AR0234_REG_AE_MAX_EV_STEP_REG  0x310A
#define AR0234_REG_AE_DAMP_OFFSET_REG  0x310C
#define AR0234_REG_AE_DAMP_GAIN_REG  0x310E
#define AR0234_REG_AE_DAMP_MAX_REG  0x3110
#define AR0234_REG_AE_MAX_EXPOSURE_REG  0x311C
#define AR0234_REG_AE_MIN_EXPOSURE_REG  0x311E
#define AR0234_REG_AE_DARK_CUR_THRESH_REG  0x3124
#define AR0234_REG_AE_CURRENT_GAINS  0x312A
#define AR0234_REG_ACTUAL_FRAME_LENGTH_LINES  0x312E
#define AR0234_REG_AE_ROI_X_START_OFFSET  0x3140
#define AR0234_REG_AE_ROI_Y_START_OFFSET  0x3142
#define AR0234_REG_AE_ROI_X_SIZE  0x3144
#define AR0234_REG_AE_ROI_Y_SIZE  0x3146
#define AR0234_REG_AE_HIST_BEGIN_PERC  0x3148
#define AR0234_REG_AE_HIST_END_PERC  0x314A
#define AR0234_REG_AE_HIST_DIV  0x314C
#define AR0234_REG_AE_NORM_WIDTH_MIN  0x314E
#define AR0234_REG_AE_MEAN_H  0x3150
#define AR0234_REG_AE_MEAN_L  0x3152
#define AR0234_REG_AE_HIST_BEGIN_H  0x3154
#define AR0234_REG_AE_HIST_BEGIN_L  0x3156
#define AR0234_REG_AE_HIST_END_H  0x3158
#define AR0234_REG_AE_HIST_END_L  0x315A
#define AR0234_REG_AE_LOW_END_MEAN_H  0x315C
#define AR0234_REG_AE_LOW_END_MEAN_L  0x315E
#define AR0234_REG_AE_PERC_LOW_END  0x3160
#define AR0234_REG_AE_NORM_ABS_DEV  0x3162
#define AR0234_REG_AE_COARSE_INTEGRATION_TIME  0x3164
#define AR0234_REG_AE_AG_EXPOSURE_HI  0x3166
#define AR0234_REG_AE_AG_EXPOSURE_LO  0x3168
#define AR0234_REG_DELTA_DK_CONTROL  0x3180
#define AR0234_REG_DATA_FORMAT_BITS  0x31AC
#define AR0234_REG_SERIAL_FORMAT  0x31AE
#define AR0234_REG_FRAME_PREAMBLE  0x31B0
#define AR0234_REG_LINE_PREAMBLE  0x31B2
#define AR0234_REG_MIPI_TIMING_0  0x31B4
#define AR0234_REG_MIPI_TIMING_1  0x31B6
#define AR0234_REG_MIPI_TIMING_2  0x31B8
#define AR0234_REG_MIPI_TIMING_3  0x31BA
#define AR0234_REG_MIPI_TIMING_4  0x31BC
#define AR0234_REG_SERIAL_CONFIG_STATUS  0x31BE
#define AR0234_REG_SERIAL_CONTROL_STATUS  0x31C6
#define AR0234_REG_SERIAL_CRC_0  0x31C8
#define AR0234_REG_COMPANDING  0x31D0
#define AR0234_REG_STAT_FRAME_ID  0x31D2
#define AR0234_REG_I2C_WRT_CHECKSUM  0x31D6
#define AR0234_REG_SERIAL_TEST  0x31D8
#define AR0234_REG_PIX_DEF_ID  0x31E0
#define AR0234_REG_HORIZONTAL_CURSOR_POSITION  0x31E8
#define AR0234_REG_VERTICAL_CURSOR_POSITION  0x31EA
#define AR0234_REG_HORIZONTAL_CURSOR_WIDTH  0x31EC
#define AR0234_REG_VERTICAL_CURSOR_WIDTH  0x31EE
#define AR0234_REG_AE_STATS_CONTROL  0x31F0
#define AR0234_REG_CCI_IDS  0x31FC
#define AR0234_REG_CUSTOMER_REV  0x31FE
#define AR0234_REG_AE_X1_START_OFFSET  0x3240
#define AR0234_REG_AE_X2_START_OFFSET  0x3242
#define AR0234_REG_AE_X3_START_OFFSET  0x3244
#define AR0234_REG_AE_X4_START_OFFSET  0x3246
#define AR0234_REG_AE_Y1_START_OFFSET  0x3248
#define AR0234_REG_AE_Y2_START_OFFSET  0x324A
#define AR0234_REG_AE_Y3_START_OFFSET  0x324C
#define AR0234_REG_AE_Y4_START_OFFSET  0x324E
#define AR0234_REG_AE_GRID_SEL_LO  0x3250
#define AR0234_REG_AE_GRID_SEL_HI  0x3252
#define AR0234_REG_LED_FLASH_CONTROL  0x3270
#define AR0234_REG_MIPI_TEST_CNTRL  0x3338
#define AR0234_REG_MIPI_COMPRESS_8_DATA_TYPE  0x333A
#define AR0234_REG_MIPI_COMPRESS_7_DATA_TYPE  0x333C
#define AR0234_REG_MIPI_COMPRESS_6_DATA_TYPE  0x333E
#define AR0234_REG_MIPI_JPEG_PN9_DATA_TYPE  0x3340
#define AR0234_REG_MIPI_CNTRL  0x3354
#define AR0234_REG_MIPI_TEST_PATTERN_CNTRL  0x3356
#define AR0234_REG_MIPI_TEST_PATTERN_STATUS  0x3358
#define AR0234_REG_P_GR_P0Q0  0x3600
#define AR0234_REG_P_GR_P0Q1  0x3602
#define AR0234_REG_P_GR_P0Q2  0x3604
#define AR0234_REG_P_GR_P0Q3  0x3606
#define AR0234_REG_P_GR_P0Q4  0x3608
#define AR0234_REG_P_RD_P0Q0  0x360A
#define AR0234_REG_P_RD_P0Q1  0x360C
#define AR0234_REG_P_RD_P0Q2  0x360E
#define AR0234_REG_P_RD_P0Q3  0x3610
#define AR0234_REG_P_RD_P0Q4  0x3612
#define AR0234_REG_P_BL_P0Q0  0x3614
#define AR0234_REG_P_BL_P0Q1  0x3616
#define AR0234_REG_P_BL_P0Q2  0x3618
#define AR0234_REG_P_BL_P0Q3  0x361A
#define AR0234_REG_P_BL_P0Q4  0x361C
#define AR0234_REG_P_GB_P0Q0  0x361E
#define AR0234_REG_P_GB_P0Q1  0x3620
#define AR0234_REG_P_GB_P0Q2  0x3622
#define AR0234_REG_P_GB_P0Q3  0x3624
#define AR0234_REG_P_GB_P0Q4  0x3626
#define AR0234_REG_P_GR_P1Q0  0x3640
#define AR0234_REG_P_GR_P1Q1  0x3642
#define AR0234_REG_P_GR_P1Q2  0x3644
#define AR0234_REG_P_GR_P1Q3  0x3646
#define AR0234_REG_P_GR_P1Q4  0x3648
#define AR0234_REG_P_RD_P1Q0  0x364A
#define AR0234_REG_P_RD_P1Q1  0x364C
#define AR0234_REG_P_RD_P1Q2  0x364E
#define AR0234_REG_P_RD_P1Q3  0x3650
#define AR0234_REG_P_RD_P1Q4  0x3652
#define AR0234_REG_P_BL_P1Q0  0x3654
#define AR0234_REG_P_BL_P1Q1  0x3656
#define AR0234_REG_P_BL_P1Q2  0x3658
#define AR0234_REG_P_BL_P1Q3  0x365A
#define AR0234_REG_P_BL_P1Q4  0x365C
#define AR0234_REG_P_GB_P1Q0  0x365E
#define AR0234_REG_P_GB_P1Q1  0x3660
#define AR0234_REG_P_GB_P1Q2  0x3662
#define AR0234_REG_P_GB_P1Q3  0x3664
#define AR0234_REG_P_GB_P1Q4  0x3666
#define AR0234_REG_P_GR_P2Q0  0x3680
#define AR0234_REG_P_GR_P2Q1  0x3682
#define AR0234_REG_P_GR_P2Q2  0x3684
#define AR0234_REG_P_GR_P2Q3  0x3686
#define AR0234_REG_P_GR_P2Q4  0x3688
#define AR0234_REG_P_RD_P2Q0  0x368A
#define AR0234_REG_P_RD_P2Q1  0x368C
#define AR0234_REG_P_RD_P2Q2  0x368E
#define AR0234_REG_P_RD_P2Q3  0x3690
#define AR0234_REG_P_RD_P2Q4  0x3692
#define AR0234_REG_P_BL_P2Q0  0x3694
#define AR0234_REG_P_BL_P2Q1  0x3696
#define AR0234_REG_P_BL_P2Q2  0x3698
#define AR0234_REG_P_BL_P2Q3  0x369A
#define AR0234_REG_P_BL_P2Q4  0x369C
#define AR0234_REG_P_GB_P2Q0  0x369E
#define AR0234_REG_P_GB_P2Q1  0x36A0
#define AR0234_REG_P_GB_P2Q2  0x36A2
#define AR0234_REG_P_GB_P2Q3  0x36A4
#define AR0234_REG_P_GB_P2Q4  0x36A6
#define AR0234_REG_P_GR_P3Q0  0x36C0
#define AR0234_REG_P_GR_P3Q1  0x36C2
#define AR0234_REG_P_GR_P3Q2  0x36C4
#define AR0234_REG_P_GR_P3Q3  0x36C6
#define AR0234_REG_P_GR_P3Q4  0x36C8
#define AR0234_REG_P_RD_P3Q0  0x36CA
#define AR0234_REG_P_RD_P3Q1  0x36CC
#define AR0234_REG_P_RD_P3Q2  0x36CE
#define AR0234_REG_P_RD_P3Q3  0x36D0
#define AR0234_REG_P_RD_P3Q4  0x36D2
#define AR0234_REG_P_BL_P3Q0  0x36D4
#define AR0234_REG_P_BL_P3Q1  0x36D6
#define AR0234_REG_P_BL_P3Q2  0x36D8
#define AR0234_REG_P_BL_P3Q3  0x36DA
#define AR0234_REG_P_BL_P3Q4  0x36DC
#define AR0234_REG_P_GB_P3Q0  0x36DE
#define AR0234_REG_P_GB_P3Q1  0x36E0
#define AR0234_REG_P_GB_P3Q2  0x36E2
#define AR0234_REG_P_GB_P3Q3  0x36E4
#define AR0234_REG_P_GB_P3Q4  0x36E6
#define AR0234_REG_P_GR_P4Q0  0x3700
#define AR0234_REG_P_GR_P4Q1  0x3702
#define AR0234_REG_P_GR_P4Q2  0x3704
#define AR0234_REG_P_GR_P4Q3  0x3706
#define AR0234_REG_P_GR_P4Q4  0x3708
#define AR0234_REG_P_RD_P4Q0  0x370A
#define AR0234_REG_P_RD_P4Q1  0x370C
#define AR0234_REG_P_RD_P4Q2  0x370E
#define AR0234_REG_P_RD_P4Q3  0x3710
#define AR0234_REG_P_RD_P4Q4  0x3712
#define AR0234_REG_P_BL_P4Q0  0x3714
#define AR0234_REG_P_BL_P4Q1  0x3716
#define AR0234_REG_P_BL_P4Q2  0x3718
#define AR0234_REG_P_BL_P4Q3  0x371A
#define AR0234_REG_P_BL_P4Q4  0x371C
#define AR0234_REG_P_GB_P4Q0  0x371E
#define AR0234_REG_P_GB_P4Q1  0x3720
#define AR0234_REG_P_GB_P4Q2  0x3722
#define AR0234_REG_P_GB_P4Q3  0x3724
#define AR0234_REG_P_GB_P4Q4  0x3726
#define AR0234_REG_POLY_SC_ENABLE  0x3780
#define AR0234_REG_POLY_ORIGIN_C  0x3782
#define AR0234_REG_POLY_ORIGIN_R  0x3784
#define AR0234_REG_DIGITAL_CTRL_1  0x3786
#define AR0234_REG_P_GR_Q5  0x37C0
#define AR0234_REG_P_RD_Q5  0x37C2
#define AR0234_REG_P_BL_Q5  0x37C4
#define AR0234_REG_P_GB_Q5  0x37C6
