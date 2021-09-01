// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// JeVois Smart Embedded Machine Vision Toolkit - Copyright (C) 2020 by Laurent Itti, the University of Southern
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

#ifndef JEVOIS_IMX290_REGS
#define JEVOIS_IMX290_REGS


// Special register, cause a delay instead
#define IMX290_REG_DELAY 0xffff

// End of sequence marker
#define IMX290_SEQUENCE_END {0x0000, 0x0000, 0x0000, 0x0000}

// Set registers with size larger than 8-bit  using the high, low and possibly mid bytes:
#define IMX290_SET9(r, v) { IMX290_REG_##r##_L, (v), 0xff, 1 }, { IMX290_REG_##r##_H, (v)>>8, 0x01, 1 }

#define IMX290_SET12(r, v) { IMX290_REG_##r##_L, (v), 0xff, 1 }, { IMX290_REG_##r##_H, (v)>>8, 0x0f, 1 }

#define IMX290_SET16(r, v) { IMX290_REG_##r##_L, (v), 0xff, 1 }, { IMX290_REG_##r##_H, (v)>>8, 0xff, 1 }

#define IMX290_SET17(r, v) { IMX290_REG_##r##_L, (v), 0xff, 1 }, { IMX290_REG_##r##_M, (v)>>8, 0xff, 1 }, \
                           { IMX290_REG_##r##_H, (v)>>16, 0x01, 1 }

// ########## BANK 0 (0x30xx) ################################################################################

#define IMX290_REG_STANDBY              0x3000 /* Standby mode (needed to access most registers) */
  #define IMX290_VAL_STANDBY_OFF        0x00   /*   Operating */
  #define IMX290_VAL_STANDBY_ON         0x01   /*   Standby */
#define IMX290_ENTER_STANDBY { IMX290_REG_STANDBY, IMX290_VAL_STANDBY_ON, 0xff, 1}, { IMX290_REG_DELAY, 1}
#define IMX290_EXIT_STANDBY  { IMX290_REG_STANDBY, IMX290_VAL_STANDBY_OFF, 0xff, 1}, { IMX290_REG_DELAY, 1}

#define IMX290_REG_REGHOLD              0x3001 /* Register hold (to not update V reflection registers) */
  #define IMX290_VAL_REGHOLD_OFF        0x00   /*   Do not hold */
  #define IMX290_VAL_REGHOLD_ON         0x01   /*   Hold */

#define IMX290_REG_XMSTA                0x3002 /* Master mode operation */
  #define IMX290_VAL_MASTER             0x00   /*   Master mode operation start */
  #define IMX290_VAL_SLAVE              0x01   /*   Master mode operation stop */
#define IMX290_MASTER_START { IMX290_REG_XMSTA, IMX290_VAL_MASTER, 0xff, 1}, { IMX290_REG_DELAY, 1}
#define IMX290_SLAVE_START  { IMX290_REG_XMSTA, IMX290_VAL_SLAVE, 0xff, 1}, { IMX290_REG_DELAY, 1}

#define IMX290_REG_RESET                0x3003 /* Software reset */
  #define IMX290_VAL_RESET_OFF          0x00   /*   Operating */
  #define IMX290_VAL_RESET_ON           0x01   /*   Reset (will be cleared after reset complete) */

#define IMX290_REG_ADBIT                0x3005 /* AD conversion bits setting */
  #define IMX290_VAL_ADBIT_10BIT        0x00   /*   10-bit */
  #define IMX290_VAL_ADBIT_12BIT        0x01   /*   12-bit */

#define IMX290_REG_WIN                  0x3007 /* Flip/Mirror/Window */
  #define IMX290_BIT_VFLIP              0      /*   Vertical flip when set */
  #define IMX290_BIT_HFLIP              1      /*   Horizontal flip when set */
  #define IMX290_MSK_WINMODE            0x70   /* Window mode setting */
    #define IMX290_VAL_WIN_1080P        0x00   /*   Full HD1080p */    
    #define IMX290_VAL_WIN_720P         0x10   /*   720p */    
    #define IMX290_VAL_WIN_CROP         0x40   /*   [crop] Window cropping from Full HD 1080p */
#define IMX290_SET_WIN_1080P { IMX290_REG_WIN, IMX290_VAL_WIN_1080P, IMX290_MSK_WINMODE, 1}
#define IMX290_SET_WIN_720P { IMX290_REG_WIN, IMX290_VAL_WIN_720P, IMX290_MSK_WINMODE, 1}
#define IMX290_SET_WIN_CROP { IMX290_REG_WIN, IMX290_VAL_WIN_CROP, IMX290_MSK_WINMODE, 1}
#define IMX290_SET_WIN_VFLIP(x) { IMX290_REG_WIN, (x)<<IMX290_BIT_VFLIP, 1<<IMX290_BIT_VFLIP, 1}
#define IMX290_SET_WIN_HFLIP(x) { IMX290_REG_WIN, (x)<<IMX290_BIT_HFLIP, 1<<IMX290_BIT_HFLIP, 1}

#define IMX290_REG_FRAME                0x3009 /* Frame rate, conversion gain */
  #define IMX290_MSK_FRSEL              0x03   /* Mask for frame rate selection */
    #define IMX290_VAL_FRSEL_30FPS      0x02   /*   25/30fps */
    #define IMX290_VAL_FRSEL_60FPS      0x01   /*   50/60fps */
    #define IMX290_VAL_FRSEL_120FPS     0x00   /*   50/60fps */
  #define IMX290_MSK_FDG_SEL            0x10   /* Mask for conversion gain switching */
    #define IMX290_VAL_LCG              0x00   /*   LCG Mode */
    #define IMX290_VAL_HCG              0x10   /*   HCG Mode */
#define IMX290_SET_FRAME_30FPS { IMX290_REG_FRAME, IMX290_VAL_FRSEL_30FPS, IMX290_MSK_FRSEL, 1}
#define IMX290_SET_FRAME_60FPS { IMX290_REG_FRAME, IMX290_VAL_FRSEL_60FPS, IMX290_MSK_FRSEL, 1}
#define IMX290_SET_FRAME_120FPS { IMX290_REG_FRAME, IMX290_VAL_FRSEL_120FPS, IMX290_MSK_FRSEL, 1}
#define IMX290_SET_LCG { IMX290_REG_FRAME, IMX290_VAL_LCG, IMX290_MSK_FDG_SEL, 1}
#define IMX290_SET_HCG { IMX290_REG_FRAME, IMX290_VAL_HCG, IMX290_MSK_FDG_SEL, 1}

#define IMX290_REG_BLKLEVEL_L           0x300a /* Black level offset value setting, bits [0:7] */
#define IMX290_REG_BLKLEVEL_H           0x300b /* Black level offset value setting, bit 0->[8] */
#define IMX290_SET_BLKLEVEL(x) IMX290_SET9(BLKLEVEL, x)
#define IMX290_VAL_BLKLEVEL_10BIT       0x03c
#define IMX290_VAL_BLKLEVEL_12BIT       0x0f0

#define IMX290_REG_PRESET300C           0x300c /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET300C         0x00   /*   Preset value */

#define IMX290_REG_PRESET300F           0x300f /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET300F         0x00   /*   Preset value */

#define IMX290_REG_PRESET3010           0x3010 /* [preset] Set to 21h */
  #define IMX290_VAL_PRESET3010         0x21   /*   Preset value */

#define IMX290_REG_PRESET3012           0x3012 /* [preset] Set to 64h */
  #define IMX290_VAL_PRESET3012         0x64   /*   Preset value */

#define IMX290_REG_PRESET3013           0x3013 /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET3013         0x00   /*   Preset value */

#define IMX290_REG_GAIN                 0x3014 /* Gain setting (0.0 dB to 63.0 dB / 0.3 dB step) */
#define IMX290_SET_GAIN(x)  { IMX290_REG_GAIN, (x), 0xff, 1}

#define IMX290_REG_PRESET3016           0x3016 /* [preset] Set to 09h */
  #define IMX290_VAL_PRESET3016         0x09   /*   Preset value */

#define IMX290_REG_VMAX_L               0x3018 /* [master] Vertical span (# lines, count from 1), bits [0:7] */
#define IMX290_REG_VMAX_M               0x3019 /* [master] Vertical span, bits [8:15] */
#define IMX290_REG_VMAX_H               0x301a /* [master] Vertical span, bits 0:1->[16:17] */
#define IMX290_SET_VMAX(x) IMX290_SET17(VMAX, x)
#define IMX290_VMAX_1080P               0x0465
#define IMX290_VMAX_720P                0x02ee

#define IMX290_REG_PRESET301B           0x301b /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET301B         0x00   /*   Preset value */

#define IMX290_REG_HMAX_L               0x301c /* [master] Horizontal span (# clocks, count from 1), bits [0:7] */
#define IMX290_REG_HMAX_H               0x301d /* [master] Horizontal span, bits [8:15] */
#define IMX290_SET_HMAX(x) IMX290_SET16(HMAX, x)
#define IMX290_HMAX_1080P_30FPS         0x1130
#define IMX290_HMAX_1080P_60FPS         0x0898
#define IMX290_HMAX_1080P_120FPS        0x044c
#define IMX290_HMAX_1080P_25FPS         0x14a0
#define IMX290_HMAX_1080P_50FPS         0x0a50
#define IMX290_HMAX_1080P_100FPS        0x0528
#define IMX290_HMAX_720P_30FPS          0x19c8
#define IMX290_HMAX_720P_60FPS          0x0ce4
#define IMX290_HMAX_720P_120FPS         0x0672
#define IMX290_HMAX_720P_25FPS          0x1ef0
#define IMX290_HMAX_720P_50FPS          0x0f78
#define IMX290_HMAX_720P_100FPS         0x07bc

#define IMX290_REG_ID_L                 0x301e /* Sensor ID is 0xb201 */
  #define IMX290_VAL_ID_L                 0xb2
#define IMX290_REG_ID_H                 0x301f
  #define IMX290_VAL_ID_H                 0x01

#define IMX290_REG_SHS1_L               0x3020 /* Storage time adjustment, in line units, bits [0:7] */
#define IMX290_REG_SHS1_M               0x3021 /* Storage time adjustment, bits [8:15] */
#define IMX290_REG_SHS1_H               0x3022 /* Storage time adjustment, bits 0:1->[16:17] */
#define IMX290_SET_SHS1(x) IMX290_SET17(SHS1, x)

#define IMX290_REG_SHS2_L               0x3024 /* [undoc] Storage time adjustment, in line units, bits [0:7] */
#define IMX290_REG_SHS2_M               0x3025 /* [undoc] Storage time adjustment, bits [8:15] */
#define IMX290_REG_SHS2_H               0x3026 /* [undoc] Storage time adjustment, bits 0:1->[16:17] */
#define IMX290_SET_SHS2(x) IMX290_SET17(SHS2, x)

#define IMX290_REG_SHS3_L               0x3028 /* [undoc] Storage time adjustment, in line units, bits [0:7] */
#define IMX290_REG_SHS3_M               0x3029 /* [undoc] Storage time adjustment, bits [8:15] */
#define IMX290_REG_SHS3_H               0x302a /* [undoc] Storage time adjustment, bits 0:1->[16:17] */
#define IMX290_SET_SHS3(x) IMX290_SET17(SHS3, x)

#define IMX290_REG_RHS1_L               0x3030 /* [undoc] Read(?) time adjustment, in line units, bits [0:7] */
#define IMX290_REG_RHS1_M               0x3031 /* [undoc] Read(?) time adjustment, bits [8:15] */
#define IMX290_REG_RHS1_H               0x3032 /* [undoc] Read(?) time adjustment, bits 0:1->[16:17] */
#define IMX290_SET_RHS1(x) IMX290_SET17(RHS1, x)

#define IMX290_REG_RHS2_L               0x3034 /* [undoc] Read(?) time adjustment, in line units, bits [0:7] */
#define IMX290_REG_RHS2_M               0x3035 /* [undoc] Read(?) time adjustment, bits [8:15] */
#define IMX290_REG_RHS2_H               0x3036 /* [undoc] Read(?) time adjustment, bits 0:1->[16:17] */
#define IMX290_SET_RHS2(x) IMX290_SET17(RHS2, x)

#define IMX290_REG_WINWV_OB             0x303a /* [crop] Vert crop size effective OB, 4-bit value */
#define IMX290_SET_WINWV_OB(x) { IMX290_REG_WINWV_OB, (x), 0xff, 1}

#define IMX290_REG_WINPV_L              0x303c /* [crop] Upper left corner, vertical position), bits [0:7] */
#define IMX290_REG_WINPV_H              0x303d /* [crop] Upper left corner vert, bits 0:2->[8:10] */
#define IMX290_SET_WINPV(x) IMX290_SET16(WINPV, x)

#define IMX290_REG_WINWV_L              0x303e /* [crop] Vertical cropping size, bits [0:7] */
#define IMX290_REG_WINWV_H              0x303f /* [crop] Vertical cropping size, bits 0:2->[8:10] */
#define IMX290_SET_WINWV(x) IMX290_SET16(WINWV, x)

#define IMX290_REG_WINPH_L              0x3040 /* [crop] Upper left corner, horiz position, mult of 4, bits [0:7] */
#define IMX290_REG_WINPH_H              0x3041 /* [crop] Upper left corner horiz, bits 0:2->[8:10] */
#define IMX290_SET_WINPH(x) IMX290_SET16(WINPH, x)

#define IMX290_REG_WINWH_L              0x3042 /* [crop] Horizontal cropping size, mult of 4, bits [0:7] */
#define IMX290_REG_WINWH_H              0x3043 /* [crop] Horizontal cropping size, bits 0:2->[8:10] */
#define IMX290_SET_WINWH(x) IMX290_SET16(WINWH, x)

#define IMX290_REG_DOL                  0x3045 /* [undoc] DOL */
  #define IMX290_VAL_DOL_LINEAR         0x01
  #define IMX290_VAL_DOL_FS_LIN         0x05
#define IMX290_SET_DOL_LINEAR { IMX290_REG_DOL, IMX290_VAL_DOL_LINEAR, 0xff, 1}
#define IMX290_SET_DOL_FS_LIN { IMX290_REG_DOL, IMX290_VAL_DOL_FS_LIN, 0xff, 1}

#define IMX290_REG_OPORT                0x3046 /* [lvds] Output interface selection */
  #define IMX290_BIT_ODBIT              1      /* [lvds] 0: 10bit, 1: 12bit. Fixed to 1 in MIPI */
  #define IMX290_MSK_OPORTSEL           0xf0   /* [lvds] Output interface selection */
    #define IMX290_VAL_PARSDR           0x00   /*   [lvds] Parallel CMOS SDR */
    #define IMX290_VAL_LVDS2CH          0xd0   /*   [lvds] LVDS 2 channels */
    #define IMX290_VAL_LVDS4CH          0xe0   /*   [lvds] LVDS 4 channels */
    #define IMX290_VAL_LVDS8CH          0xf0   /*   [lvds] LVDS 8 channels */
#define IMX290_SET_OPORT_MIPI_4LANES { IMX290_REG_OPORT, 0xe1, 0xff, 1}
#define IMX290_SET_OPORT_MIPI_2LANES { IMX290_REG_OPORT, 0xd1, 0xff, 1}

#define IMX290_REG_XVSLNG               0x3048 /* [master] XVS pulse width setting in master mode */
  #define IMX290_VAL_XVS1H              0x00   /*   [master] 1H XVS pulse length */
  #define IMX290_VAL_XVS2H              0x10   /*   [master] 2H XVS pulse length */
  #define IMX290_VAL_XVS4H              0x20   /*   [master] 4H XVS pulse length */
  #define IMX290_VAL_XVS8H              0x30   /*   [master] 8H XVS pulse length */

#define IMX290_REG_XHSLNG               0x3049 /* [master] XHS pulse width setting in master mode */
  #define IMX290_VAL_XHS0               0x00   /*   [master] XHS pulse length 0 */
  #define IMX290_VAL_XHS1               0x10   /*   [master] XHS pulse length 1 */
  #define IMX290_VAL_XHS2               0x20   /*   [master] XHS pulse length 2 */
  #define IMX290_VAL_XHS3               0x30   /*   [master] XHS pulse length 3 */

#define IMX290_REG_SOUTSEL              0x304b /* [master] Sync pulse settings */
  #define IMX290_MSK_XVSOUTSEL          0x3    /*   [master] XVS pin setting in master mode */
    #define IMX290_VAL_XVSOUT_HIGH      0x0    /*     [master] XVS pin fixed to High */
    #define IMX290_VAL_XVSOUT_VSYNC     0x2    /*     [master] XVS pin outputs VSYNC */
  #define IMX290_MSK_XHSOUTSEL          0xc    /*   [master] XHS pin setting in master mode */
    #define IMX290_VAL_XHSOUT_HIGH      0x0    /*     [master] XHS pin fixed to High */
    #define IMX290_VAL_XHSOUT_HSYNC     0x8    /*     [master] XHS pin outputs HSYNC */
#define IMX290_SOUTSEL_NONE { IMX290_REG_SOUTSEL, 0, 0xff, 1}

#define IMX290_REG_INCKSEL1             0x305c /* The value is set according to INCK */

#define IMX290_REG_INCKSEL2             0x305d /* The value is set according to INCK */

#define IMX290_REG_INCKSEL3             0x305e /* The value is set according to INCK */

#define IMX290_REG_INCKSEL4             0x305f /* The value is set according to INCK */

#define IMX290_REG_PRESET3070           0x3070 /* [preset] Set to 02h */
  #define IMX290_VAL_PRESET3070         0x02   /*   Preset value */

#define IMX290_REG_PRESET3071           0x3071 /* [preset] Set to 11h */
  #define IMX290_VAL_PRESET3071         0x11   /*   Preset value */

#define IMX290_REG_PRESET309B           0x309b /* [preset] Set to 10h */
  #define IMX290_VAL_PRESET309B         0x10   /*   Preset value */

#define IMX290_REG_PRESET30A2           0x30a2 /* [preset] Set to 02h */
  #define IMX290_VAL_PRESET30A2         0x02   /*   Preset value */

#define IMX290_REG_PRESET30A6           0x30a6 /* [preset] Set to 20h */
  #define IMX290_VAL_PRESET30A6         0x20   /*   Preset value */

#define IMX290_REG_PRESET30A8           0x30a8 /* [preset] Set to 20h */
  #define IMX290_VAL_PRESET30A8         0x20   /*   Preset value */

#define IMX290_REG_PRESET30AA           0x30aa /* [preset] Set to 20h */
  #define IMX290_VAL_PRESET30AA         0x20   /*   Preset value */

#define IMX290_REG_PRESET30AC           0x30ac /* [preset] Set to 20h */
  #define IMX290_VAL_PRESET30AC         0x20   /*   Preset value */

#define IMX290_REG_PRESET30B0           0x30b0 /* [preset] Set to 43h */
  #define IMX290_VAL_PRESET30B0         0x43   /*   Preset value */

// ########## BANK 1 (0x31xx) ################################################################################

#define IMX290_REG_PRESET310B           0x310b /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET310B         0x00   /*   Preset value */

#define IMX290_REG_PRESET3119           0x3119 /* [preset] Set to 9Eh */
  #define IMX290_VAL_PRESET3119         0x9e   /*   Preset value */

#define IMX290_REG_PRESET311C           0x311c /* [preset] Set to 1eh */
  #define IMX290_VAL_PRESET311C         0x1e   /*   Preset value */

#define IMX290_REG_PRESET311E           0x311e /* [preset] Set to 08h */
  #define IMX290_VAL_PRESET311E         0x08   /*   Preset value */

#define IMX290_REG_PRESET3128           0x3128 /* [preset] Set to 05h */
  #define IMX290_VAL_PRESET3128         0x05   /*   Preset value */

#define IMX290_REG_ADBIT1               0x3129 /* A/D number of bits */
  #define IMX290_VAL_ADBIT1_10BIT       0x1d   /*   10-bit */
  #define IMX290_VAL_ADBIT1_12BIT       0x00   /*   12-bit */

#define IMX290_REG_PRESET3134           0x3134 /* [preset] Set to 0fh */
  #define IMX290_VAL_PRESET3134         0x0f   /*   Preset value */

#define IMX290_REG_PRESET313B           0x313b /* [preset] Set to 50h */
  #define IMX290_VAL_PRESET313B         0x50   /*   Preset value */

#define IMX290_REG_PRESET313D           0x313d /* [preset] Set to 83h */
  #define IMX290_VAL_PRESET313D         0x83   /*   Preset value */

#define IMX290_REG_PRESET3150           0x3150 /* [preset] Set to 03h */
  #define IMX290_VAL_PRESET3150         0x03   /*   Preset value */

#define IMX290_REG_INCKSEL5             0x315e /* Value set according to INCK: 0x1a=37.125MHz, 0x1b=74.25MHz */

#define IMX290_REG_INCKSEL6             0x3164 /* Value set according to INCK: 0x1a=37.125MHz, 0x1b=74.25MHz */

#define IMX290_REG_ADBIT2               0x317c /* A/D number of bits */
  #define IMX290_VAL_ADBIT2_10BIT       0x12   /*   10-bit */
  #define IMX290_VAL_ADBIT2_12BIT       0x00   /*   12-bit */

#define IMX290_REG_PRESET317E           0x317e /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET317E         0x00   /*   Preset value */

#define IMX290_REG_PRESET317F           0x317f /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET317F         0x00   /*   Preset value */

#define IMX290_REG_ADBIT3               0x31ec /* A/D number of bits */
  #define IMX290_VAL_ADBIT3_10BIT       0x37   /*   10-bit */
  #define IMX290_VAL_ADBIT3_12BIT       0x0e   /*   12-bit */

// ########## BANK 2 (0x32xx) ################################################################################

#define IMX290_REG_PRESET32B8           0x32b8 /* [preset] Set to 50h */
  #define IMX290_VAL_PRESET32B8         0x50   /*   Preset value */

#define IMX290_REG_PRESET32B9           0x32b9 /* [preset] Set to 10h */
  #define IMX290_VAL_PRESET32B9         0x10   /*   Preset value */

#define IMX290_REG_PRESET32BA           0x32ba /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET32BA         0x00   /*   Preset value */

#define IMX290_REG_PRESET32BB           0x32bb /* [preset] Set to 04h */
  #define IMX290_VAL_PRESET32BB         0x04   /*   Preset value */

#define IMX290_REG_PRESET32C8           0x32c8 /* [preset] Set to 50h */
  #define IMX290_VAL_PRESET32C8         0x50   /*   Preset value */

#define IMX290_REG_PRESET32C9           0x32c9 /* [preset] Set to 10h */
  #define IMX290_VAL_PRESET32C9         0x10   /*   Preset value */

#define IMX290_REG_PRESET32CA           0x32ca /* [preset] Set to 00h */
  #define IMX290_VAL_PRESET32CA         0x00   /*   Preset value */

#define IMX290_REG_PRESET32CB           0x32cb /* [preset] Set to 04h */
  #define IMX290_VAL_PRESET32CB         0x04   /*   Preset value */

// ########## BANK 3 (0x33xx) ################################################################################

#define IMX290_REG_PRESET332C           0x332c /* [preset] Set to D3h */
  #define IMX290_VAL_PRESET332C         0xd3   /*   Preset value */

#define IMX290_REG_PRESET332D           0x332d /* [preset] Set to 10h */
  #define IMX290_VAL_PRESET332D         0x10   /*   Preset value */

#define IMX290_REG_PRESET332E           0x332e /* [preset] Set to 0Dh */
  #define IMX290_VAL_PRESET332E         0x0d   /*   Preset value */

#define IMX290_REG_PRESET3358           0x3358 /* [preset] Set to 06h */
  #define IMX290_VAL_PRESET3358         0x06   /*   Preset value */

#define IMX290_REG_PRESET3359           0x3359 /* [preset] Set to E1h */
  #define IMX290_VAL_PRESET3359         0xe1   /*   Preset value */

#define IMX290_REG_PRESET335A           0x335a /* [preset] Set to 11h */
  #define IMX290_VAL_PRESET335A         0x11   /*   Preset value */

#define IMX290_REG_PRESET3360           0x3360 /* [preset] Set to 1eh */
  #define IMX290_VAL_PRESET3360         0x1e   /*   Preset value */

#define IMX290_REG_PRESET3361           0x3361 /* [preset] Set to 61h */
  #define IMX290_VAL_PRESET3361         0x61   /*   Preset value */

#define IMX290_REG_PRESET3362           0x3362 /* [preset] Set to 10h */
  #define IMX290_VAL_PRESET3362         0x10   /*   Preset value */

#define IMX290_REG_PRESET33B0           0x33b0 /* [preset] Set to 08h */
  #define IMX290_VAL_PRESET33B0         0x08   /*   Preset value */

#define IMX290_REG_PRESET33B1           0x33b1 /* [preset] Set to 80h */
  #define IMX290_VAL_PRESET33B1         0x80   /*   Preset value */

#define IMX290_REG_PRESET33B2           0x33b2 /* [preset] Set to 1ah */
  #define IMX290_VAL_PRESET33B2         0x1a   /*   Preset value */

#define IMX290_REG_PRESET33B3           0x33b3 /* [preset] Set to 04h */
  #define IMX290_VAL_PRESET33B3         0x04   /*   Preset value */

// ########## BANK 4 (0x34xx) ################################################################################

#define IMX290_REG_REPETITION           0x3405 /* Refer to "Output signal Interface Control" */
#define IMX290_SET_REPETITION_4LANES_30FPS { IMX290_REG_REPETITION, 0x20, 0xff, 1}
#define IMX290_SET_REPETITION_4LANES_60FPS { IMX290_REG_REPETITION, 0x10, 0xff, 1}
#define IMX290_SET_REPETITION_4LANES_120FPS { IMX290_REG_REPETITION, 0x00, 0xff, 1}
#define IMX290_SET_REPETITION_2LANES_30FPS { IMX290_REG_REPETITION, 0x10, 0xff, 1}
#define IMX290_SET_REPETITION_2LANES_60FPS { IMX290_REG_REPETITION, 0x00, 0xff, 1}

#define IMX290_REG_PHY_LANES            0x3407 /* Number of physically connected lanes - 1 */

#define IMX290_REG_OPB_SIZE_V           0x3414 /* Vertical OB size setting. Refer to each operating setting. */
#define IMX290_SET_OPB_SIZE_V(x) { IMX290_REG_OPB_SIZE_V, (x), 0xff, 1}
#define IMX290_VAL_OPB_SIZE_V_1080P     0x0a
#define IMX290_VAL_OPB_SIZE_V_720P      0x04

#define IMX290_REG_Y_OUT_SIZE_L         0x3418 /* Vertical effective pixel size, bits [0:7] */
#define IMX290_REG_Y_OUT_SIZE_H         0x3419 /* Vertical effective pixel size, bits 0:4->[8:12] */
#define IMX290_SET_Y_OUT_SIZE(x) IMX290_SET12(Y_OUT_SIZE, x)
#define IMX290_VAL_Y_OUT_SIZE_1080P     0x0449
#define IMX290_VAL_Y_OUT_SIZE_720P      0x02d9

#define IMX290_REG_THSEXIT_L            0x342c /* Global timing setting, bits [0:7] */
#define IMX290_REG_THSEXIT_H            0x342d /* Global timing setting, bits [8:15] */

#define IMX290_REG_TCLKPRE_L            0x3430 /* Global timing setting, bits [0:7] */
#define IMX290_REG_TCLKPRE_H            0x3431 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TCLKPRE(x) IMX290_SET9(TCLKPRE, x)

#define IMX290_REG_CSI_DT_FMT_L         0x3441 /* RAW10: 0A0Ah / RAW12: 0C0Ch, bits [0:7] */
  #define IMX290_VAL_DT_FMT_RAW10_L       0x0a /*   RAW10: 0A0Ah */
  #define IMX290_VAL_DT_FMT_RAW12_L       0x0c /*   RAW12: 0C0Ch */
#define IMX290_REG_CSI_DT_FMT_H         0x3442 /* RAW10: 0A0Ah / RAW12: 0C0Ch, bits [8:15] */
  #define IMX290_VAL_DT_FMT_RAW10_H       0x0a /*   RAW10: 0A0Ah */
  #define IMX290_VAL_DT_FMT_RAW12_H       0x0c /*   RAW12: 0C0Ch */

#define IMX290_REG_CSI_LANE_MODE        0x3443 /* Number of MIPI lanes */
  #define IMX290_VAL_CSI_2LANES           0x01 /*   2 MIPI lanes */
  #define IMX290_VAL_CSI_4LANES           0x03 /*   4 MIPI lanes */

#define IMX290_REG_EXTCK_FREQ_L         0x3444 /* Master clock frequency, bits [0:7] */
  #define IMX290_VAL_FREQ37_L             0x20  /* Master clock: 0x2520=37.125 MHz, 0x4A40=74.25 MHz */
  #define IMX290_VAL_FREQ74_L             0x40  /* Master clock: 0x2520=37.125 MHz, 0x4A40=74.25 MHz */
#define IMX290_REG_EXTCK_FREQ_H         0x3445 /* Master clock frequency, bits [0:7] */
  #define IMX290_VAL_FREQ37_H             0x25  /* Master clock: 0x2520=37.125 MHz, 0x4A40=74.25 MHz */
  #define IMX290_VAL_FREQ74_H             0x4a  /* Master clock: 0x2520=37.125 MHz, 0x4A40=74.25 MHz */

#define IMX290_REG_TCLKPOST_L           0x3446 /* Global timing setting, bits [0:7] */
#define IMX290_REG_TCLKPOST_H           0x3447 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TCLKPOST(x) IMX290_SET9(TCLKPOST, x)
#define IMX290_TCLKPOST_1080P_2LANES_30FPS  0x57
#define IMX290_TCLKPOST_1080P_2LANES_60FPS  0x77
#define IMX290_TCLKPOST_1080P_4LANES_30FPS  0x47
#define IMX290_TCLKPOST_1080P_4LANES_60FPS  0x57
#define IMX290_TCLKPOST_1080P_4LANES_120FPS 0x77
#define IMX290_TCLKPOST_720P_2LANES_30FPS   0x4f
#define IMX290_TCLKPOST_720P_2LANES_60FPS   0x67
#define IMX290_TCLKPOST_720P_4LANES_30FPS   0x47
#define IMX290_TCLKPOST_720P_4LANES_60FPS   0x4f
#define IMX290_TCLKPOST_720P_4LANES_120FPS  0x67

#define IMX290_REG_THSZERO_L            0x3448 /* Global timing setting, bits [0:7] */
#define IMX290_REG_THSZERO_H            0x3449 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_THSZERO(x) IMX290_SET9(THSZERO, x)
#define IMX290_THSZERO_1080P_2LANES_30FPS  0x37
#define IMX290_THSZERO_1080P_2LANES_60FPS  0x67
#define IMX290_THSZERO_1080P_4LANES_30FPS  0x1f
#define IMX290_THSZERO_1080P_4LANES_60FPS  0x37
#define IMX290_THSZERO_1080P_4LANES_120FPS 0x67
#define IMX290_THSZERO_720P_2LANES_30FPS   0x2f
#define IMX290_THSZERO_720P_2LANES_60FPS   0x57
#define IMX290_THSZERO_720P_4LANES_30FPS   0x17
#define IMX290_THSZERO_720P_4LANES_60FPS   0x2f
#define IMX290_THSZERO_720P_4LANES_120FPS  0x57

#define IMX290_REG_THSPREPARE_L         0x344a /* Global timing setting, bits [0:7] */
#define IMX290_REG_THSPREPARE_H         0x344b /* Global timing setting, bit 0->[8] */
#define IMX290_SET_THSPREPARE(x) IMX290_SET9(THSPREPARE, x)
#define IMX290_THSPREPARE_1080P_2LANES_30FPS  0x1f
#define IMX290_THSPREPARE_1080P_2LANES_60FPS  0x47
#define IMX290_THSPREPARE_1080P_4LANES_30FPS  0x17
#define IMX290_THSPREPARE_1080P_4LANES_60FPS  0x1f
#define IMX290_THSPREPARE_1080P_4LANES_120FPS 0x47
#define IMX290_THSPREPARE_720P_2LANES_30FPS   0x17
#define IMX290_THSPREPARE_720P_2LANES_60FPS   0x2f
#define IMX290_THSPREPARE_720P_4LANES_30FPS   0x0f
#define IMX290_THSPREPARE_720P_4LANES_60FPS   0x17
#define IMX290_THSPREPARE_720P_4LANES_120FPS  0x2f

#define IMX290_REG_TCLKTRAIL_L          0x344c /* Global timing setting, bits [0:7] */
#define IMX290_REG_TCLKTRAIL_H          0x344d /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TCLKTRAIL(x) IMX290_SET9(TCLKTRAIL, x)
#define IMX290_TCLKTRAIL_1080P_2LANES_30FPS  0x1f
#define IMX290_TCLKTRAIL_1080P_2LANES_60FPS  0x37
#define IMX290_TCLKTRAIL_1080P_4LANES_30FPS  0x0f
#define IMX290_TCLKTRAIL_1080P_4LANES_60FPS  0x1f
#define IMX290_TCLKTRAIL_1080P_4LANES_120FPS 0x37
#define IMX290_TCLKTRAIL_720P_2LANES_30FPS   0x17
#define IMX290_TCLKTRAIL_720P_2LANES_60FPS   0x27
#define IMX290_TCLKTRAIL_720P_4LANES_30FPS   0x0f
#define IMX290_TCLKTRAIL_720P_4LANES_60FPS   0x17
#define IMX290_TCLKTRAIL_720P_4LANES_120FPS  0x27

#define IMX290_REG_THSTRAIL_L           0x344e /* Global timing setting, bits [0:7] */
#define IMX290_REG_THSTRAIL_H           0x344f /* Global timing setting, bit 0->[8] */
#define IMX290_SET_THSTRAIL(x) IMX290_SET9(THSTRAIL, x)
#define IMX290_THSTRAIL_1080P_2LANES_30FPS  0x1f
#define IMX290_THSTRAIL_1080P_2LANES_60FPS  0x37
#define IMX290_THSTRAIL_1080P_4LANES_30FPS  0x17
#define IMX290_THSTRAIL_1080P_4LANES_60FPS  0x1f
#define IMX290_THSTRAIL_1080P_4LANES_120FPS 0x3f
#define IMX290_THSTRAIL_720P_2LANES_30FPS   0x17
#define IMX290_THSTRAIL_720P_2LANES_60FPS   0x2f
#define IMX290_THSTRAIL_720P_4LANES_30FPS   0x0f
#define IMX290_THSTRAIL_720P_4LANES_60FPS   0x17
#define IMX290_THSTRAIL_720P_4LANES_120FPS  0x2f

#define IMX290_REG_TCLKZERO_L           0x3450 /* Global timing setting, bits [0:7] */
#define IMX290_REG_TCLKZERO_H           0x3451 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TCLKZERO(x) IMX290_SET9(TCLKZERO, x)
#define IMX290_TCLKZERO_1080P_2LANES_30FPS  0x77
#define IMX290_TCLKZERO_1080P_2LANES_60FPS  0xff
#define IMX290_TCLKZERO_1080P_4LANES_30FPS  0x47
#define IMX290_TCLKZERO_1080P_4LANES_60FPS  0x77
#define IMX290_TCLKZERO_1080P_4LANES_120FPS 0xff
#define IMX290_TCLKZERO_720P_2LANES_30FPS   0x57
#define IMX290_TCLKZERO_720P_2LANES_60FPS   0xbf
#define IMX290_TCLKZERO_720P_4LANES_30FPS   0x2b
#define IMX290_TCLKZERO_720P_4LANES_60FPS   0x57
#define IMX290_TCLKZERO_720P_4LANES_120FPS  0xbf

#define IMX290_REG_TCLKPREPARE_L        0x3452 /* Global timing setting, bits [0:7] */
#define IMX290_REG_TCLKPREPARE_H        0x3453 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TCLKPREPARE(x) IMX290_SET9(TCLKPREPARE, x)
#define IMX290_TCLKPREPARE_1080P_2LANES_30FPS  0x1f
#define IMX290_TCLKPREPARE_1080P_2LANES_60FPS  0x3f
#define IMX290_TCLKPREPARE_1080P_4LANES_30FPS  0x0f
#define IMX290_TCLKPREPARE_1080P_4LANES_60FPS  0x1f
#define IMX290_TCLKPREPARE_1080P_4LANES_120FPS 0x3f
#define IMX290_TCLKPREPARE_720P_2LANES_30FPS   0x17
#define IMX290_TCLKPREPARE_720P_2LANES_60FPS   0x2f
#define IMX290_TCLKPREPARE_720P_4LANES_30FPS   0x0b
#define IMX290_TCLKPREPARE_720P_4LANES_60FPS   0x17
#define IMX290_TCLKPREPARE_720P_4LANES_120FPS  0x2f

#define IMX290_REG_TLPX_L               0x3454 /* Global timing setting, bits [0:7] */
#define IMX290_REG_TLPX_H               0x3455 /* Global timing setting, bit 0->[8] */
#define IMX290_SET_TLPX(x) IMX290_SET9(TLPX, x)
#define IMX290_TLPX_1080P_2LANES_30FPS  0x17
#define IMX290_TLPX_1080P_2LANES_60FPS  0x37
#define IMX290_TLPX_1080P_4LANES_30FPS  0x0f
#define IMX290_TLPX_1080P_4LANES_60FPS  0x17
#define IMX290_TLPX_1080P_4LANES_120FPS 0x37
#define IMX290_TLPX_720P_2LANES_30FPS   0x17
#define IMX290_TLPX_720P_2LANES_60FPS   0x27
#define IMX290_TLPX_720P_4LANES_30FPS   0x0f
#define IMX290_TLPX_720P_4LANES_60FPS   0x17
#define IMX290_TLPX_720P_4LANES_120FPS  0x27

#define IMX290_REG_X_OUT_SIZE_L         0x3472 /* Horizontal effective pixel size, bits [0:7] */
#define IMX290_REG_X_OUT_SIZE_H         0x3473 /* Horizontal effective pixel size, bits 0:4->[8:12] */
#define IMX290_SET_X_OUT_SIZE(x) IMX290_SET12(X_OUT_SIZE, x)
#define IMX290_VAL_X_OUT_SIZE_1080P     0x079c
#define IMX290_VAL_X_OUT_SIZE_720P      0x051c

// ####################################################################################################
// General use settings:
// ####################################################################################################
#define IMX290_INCK_37125_1080P { IMX290_REG_INCKSEL1, 0x18, 0xff, 1}, { IMX290_REG_INCKSEL2, 0x03, 0xff, 1}, \
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1}, { IMX290_REG_INCKSEL4, 0x01, 0xff, 1}, \
    { IMX290_REG_INCKSEL5, 0x1A, 0xff, 1}, { IMX290_REG_INCKSEL6, 0x1A, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_L, IMX290_VAL_FREQ37_L, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_H, IMX290_VAL_FREQ37_H, 0xff, 1}

#define IMX290_INCK_37125_720P { IMX290_REG_INCKSEL1, 0x20, 0xff, 1}, { IMX290_REG_INCKSEL2, 0x0, 0xff, 1}, \
    { IMX290_REG_INCKSEL3, 0x20, 0xff, 1}, { IMX290_REG_INCKSEL4, 0x01, 0xff, 1}, \
    { IMX290_REG_INCKSEL5, 0x1A, 0xff, 1}, { IMX290_REG_INCKSEL6, 0x1A, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_L, IMX290_VAL_FREQ37_L, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_H, IMX290_VAL_FREQ37_H, 0xff, 1}

#define IMX290_INCK_74500_1080P { IMX290_REG_INCKSEL1, 0x0c, 0xff, 1}, { IMX290_REG_INCKSEL2, 0x03, 0xff, 1}, \
    { IMX290_REG_INCKSEL3, 0x10, 0xff, 1}, { IMX290_REG_INCKSEL4, 0x01, 0xff, 1}, \
    { IMX290_REG_INCKSEL5, 0x1b, 0xff, 1}, { IMX290_REG_INCKSEL6, 0x1b, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_L, IMX290_VAL_FREQ74_L, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_H, IMX290_VAL_FREQ74_H, 0xff, 1}

#define IMX290_INCK_74500_720P { IMX290_REG_INCKSEL1, 0x10, 0xff, 1}, { IMX290_REG_INCKSEL2, 0x00, 0xff, 1}, \
    { IMX290_REG_INCKSEL3, 0x10, 0xff, 1}, { IMX290_REG_INCKSEL4, 0x01, 0xff, 1}, \
    { IMX290_REG_INCKSEL5, 0x1b, 0xff, 1}, { IMX290_REG_INCKSEL6, 0x1b, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_L, IMX290_VAL_FREQ74_L, 0xff, 1}, \
    { IMX290_REG_EXTCK_FREQ_H, IMX290_VAL_FREQ74_H, 0xff, 1}

#define IMX290_SET_12BIT { IMX290_REG_ADBIT, IMX290_VAL_ADBIT_12BIT, 0xff, 1}, \
    { IMX290_REG_ADBIT1, IMX290_VAL_ADBIT1_12BIT, 0xff, 1},             \
    { IMX290_REG_ADBIT2, IMX290_VAL_ADBIT2_12BIT, 0xff, 1},             \
    { IMX290_REG_ADBIT3, IMX290_VAL_ADBIT3_12BIT, 0xff, 1},             \
    { IMX290_REG_CSI_DT_FMT_L, IMX290_VAL_DT_FMT_RAW12_L, 0xff, 1},     \
    { IMX290_REG_CSI_DT_FMT_H, IMX290_VAL_DT_FMT_RAW12_H, 0xff, 1},     \
    IMX290_SET_BLKLEVEL(IMX290_VAL_BLKLEVEL_12BIT)
      
#define IMX290_SET_10BIT { IMX290_REG_ADBIT, IMX290_VAL_ADBIT_10BIT, 0xff, 1}, \
    { IMX290_REG_ADBIT1, IMX290_VAL_ADBIT1_10BIT, 0xff, 1},             \
    { IMX290_REG_ADBIT2, IMX290_VAL_ADBIT2_10BIT, 0xff, 1},             \
    { IMX290_REG_ADBIT3, IMX290_VAL_ADBIT3_10BIT, 0xff, 1},             \
    { IMX290_REG_CSI_DT_FMT_L, IMX290_VAL_DT_FMT_RAW10_L, 0xff, 1},     \
    { IMX290_REG_CSI_DT_FMT_H, IMX290_VAL_DT_FMT_RAW10_H, 0xff, 1},     \
    IMX290_SET_BLKLEVEL(IMX290_VAL_BLKLEVEL_10BIT)

#define IMX290_SET_MIPI_4LANES IMX290_SET_OPORT_MIPI_4LANES, \
  { IMX290_REG_CSI_LANE_MODE, IMX290_VAL_CSI_4LANES, 0xff, 1}, { IMX290_REG_PHY_LANES, 0x03, 0xff, 1}
#define IMX290_SET_MIPI_2LANES IMX290_SET_OPORT_MIPI_2LANES, \
  { IMX290_REG_CSI_LANE_MODE, IMX290_VAL_CSI_2LANES, 0xff, 1}, { IMX290_REG_PHY_LANES, 0x01, 0xff, 1}

#define IMX290_SET_TIMINGS(x) IMX290_SET_TCLKPOST(IMX290_TCLKPOST_##x), IMX290_SET_THSZERO(IMX290_THSZERO_##x), \
  IMX290_SET_THSPREPARE(IMX290_THSPREPARE_##x), IMX290_SET_TCLKTRAIL(IMX290_TCLKTRAIL_##x), \
  IMX290_SET_THSTRAIL(IMX290_THSTRAIL_##x), IMX290_SET_TCLKZERO(IMX290_TCLKZERO_##x), \
  IMX290_SET_TCLKPREPARE(IMX290_TCLKPREPARE_##x), IMX290_SET_TLPX(IMX290_TLPX_##x)

#define IMX290_SOFT_RESET { IMX290_REG_RESET, IMX290_VAL_RESET_ON, 0xff, 1}, { IMX290_REG_DELAY, 300}


#define IMX290_SET_CROP(w, h) IMX290_SET_WIN_CROP, IMX290_SET_WINWV_OB(12), \
    IMX290_SET_WINPH((((1920 - (w))>>3)<<2)), IMX290_SET_WINPV((((1080 - (h))>>3)<<2)), \
    IMX290_SET_WINWH(((w) + 16)), IMX290_SET_WINWV(((h) + 16)), \
    IMX290_SET_VMAX(((h) + 40))

#endif // JEVOIS_IMX290_REGS
