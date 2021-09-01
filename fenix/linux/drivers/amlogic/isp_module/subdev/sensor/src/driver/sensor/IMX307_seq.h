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

#if !defined(__IMX307_SENSOR_H__)
#define __IMX307_SENSOR_H__


/*-----------------------------------------------------------------------------
Initialization sequence - do not edit
-----------------------------------------------------------------------------*/

#include "sensor_init.h"

/* mclk:37.125MHz */

/*
 * bayer = BAYER_RGGB
 */
static acam_reg_t linear_1080p_30fps_223Mbps_4lane_12bits[] = {
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0xFFFF, 1},

    {0x3005, 0x01, 0xff, 1}, //0:10bit 1:12bit
    {0x3007, 0x00, 0xff, 1}, //full hd 1080p
    {0x3009, 0x02, 0xff, 1},
    {0x300a, 0xF0, 0xff, 1}, //black level
    {0x300b, 0x00, 0xff, 1},
    {0x300c, 0x00, 0xff, 1},
    {0x300f, 0x00, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3014, 0x00, 0xff, 1},//Gain
    {0x3018, 0x65, 0xff, 1}, /* VMAX[7:0] */
    {0x3019, 0x04, 0xff, 1}, /* VMAX[15:8] */
    {0x301a, 0x00, 0xff, 1}, /* VMAX[16] */
    {0x301b, 0x00, 0xff, 1},
    {0x301c, 0x30, 0xff, 1}, /* HMAX[7:0] */
    {0x301d, 0x11, 0xff, 1}, /* HMAX[15:8] */
    {0x3020, 0x01, 0xff, 1},//SHS1
    {0x3021, 0x00, 0xff, 1},//SHS1
    {0x3022, 0x00, 0xff, 1},//SHS1
    {0x3024, 0x00, 0xff, 1},//SHS2
    {0x3025, 0x00, 0xff, 1},//SHS2
    {0x3026, 0x00, 0xff, 1},//SHS2
    {0x3030, 0x00, 0xff, 1},//RHS1
    {0x3031, 0x00, 0xff, 1},//RHS1
    {0x3032, 0x00, 0xff, 1},//RHS1
    {0x3045, 0x01, 0xff, 1},//DOL
    {0x3046, 0x01, 0xff, 1},//LANE CHN
    {0x304b, 0x0a, 0xff, 1},

    {0x305c, 0x18, 0xff, 1},
    {0x305d, 0x03, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},
    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x00, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x00, 0xff, 1},
    {0x31ec, 0x0e, 0xff, 1},

    {0x3405, 0x20, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x0a, 0xff, 1},
    {0x3415, 0x01, 0xff, 1},
    {0x3418, 0x49, 0xff, 1},
    {0x3419, 0x04, 0xff, 1},
    {0x3441, 0x0c, 0xff, 1},
    {0x3442, 0x0c, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},
    {0x3446, 0x47, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x1f, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x17, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x0f, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x17, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x47, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x0f, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x0f, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},

    {0x3472, 0x9c, 0xff, 1},
    {0x3473, 0x07, 0xff, 1},
    {0x347b, 0x24, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0xFFFF, 1},
    {0x3049, 0x0A, 0xff, 1}, /* XVSOUTSEL XHSOUTSEL */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t linear_1080p_60fps_446Mbps_4lane_12bits[] = {
    //{0x3003, 0x01, 0xff, 1}, /* sw_reset */
    //{0xFFFF, 200},
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0x3002, 0x00, 0xff, 1}, /* XTMSTA */

    {0x3005, 0x01, 0xff, 1},
    {0x3007, 0x00, 0xff, 1},
    {0x3009, 0x01, 0xff, 1},
    {0x300a, 0xf0, 0xff, 1},
    {0x300b, 0x00, 0xff, 1},
    {0x300c, 0x00, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3018, 0x65, 0xff, 1},//VMAX
    {0x3019, 0x04, 0xff, 1},//VMAX

    {0x301c, 0x98, 0xff, 1},//* HMAX
    {0x301d, 0x08, 0xff, 1},//* HMAX

    {0x3046, 0x01, 0xff, 1},//Datasheet should modify, Tools should modify
    {0x304b, 0x0a, 0xff, 1},

    {0x305c, 0x18, 0xff, 1},
    {0x305d, 0x03, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},

    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x00, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x00, 0xff, 1},
    {0x31ec, 0x0e, 0xff, 1},

    {0x3405, 0x10, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x0a, 0xff, 1},
    {0x3415, 0x01, 0xff, 1},
    {0x3418, 0x49, 0xff, 1},
    {0x3419, 0x04, 0xff, 1},
    {0x3441, 0x0c, 0xff, 1},
    {0x3442, 0x0c, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},
    {0x3446, 0x57, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x37, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x1f, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x1f, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x1f, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x77, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x1f, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x17, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},

    {0x3472, 0x9c, 0xff, 1},
    {0x3473, 0x07, 0xff, 1},
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000}
};

static acam_reg_t linear_720p_30fps_149Mbps_4lane_12bits[] = {
    //{0x3003, 0x01, 0xff, 1}, /* sw_reset */
    //{0xFFFF, 200},
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0x3002, 0x00, 0xff, 1}, /* XTMSTA */

    {0x3005, 0x01, 0xff, 1},
    {0x3007, 0x10, 0xff, 1},
    {0x3009, 0x02, 0xff, 1},
    {0x300a, 0xf0, 0xff, 1},
    {0x300b, 0x00, 0xff, 1},
    {0x300c, 0x00, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3018, 0xee, 0xff, 1},//VMAX
    {0x3019, 0x02, 0xff, 1},//VMAX

    {0x301c, 0xc8, 0xff, 1},//* HMAX
    {0x301d, 0x19, 0xff, 1},//* HMAX

    {0x3046, 0x01, 0xff, 1},//Datasheet should modify, Tools should modify
    {0x304b, 0x0a, 0xff, 1},

    {0x305c, 0x20, 0xff, 1},
    {0x305d, 0x00, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},

    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x00, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x00, 0xff, 1},
    {0x31ec, 0x0e, 0xff, 1},

    {0x3405, 0x20, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x04, 0xff, 1},
    {0x3415, 0x01, 0xff, 1},
    {0x3418, 0xd9, 0xff, 1},
    {0x3419, 0x02, 0xff, 1},
    {0x3441, 0x0c, 0xff, 1},
    {0x3442, 0x0c, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},

    {0x3446, 0x47, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x17, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x0f, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x0f, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x0f, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x2b, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x0b, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x0f, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},

    {0x3472, 0x1c, 0xff, 1},
    {0x3473, 0x05, 0xff, 1},
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t linear_720p_60fps_297Mbps_4lane_12bits[] = {
    //{0x3003, 0x01, 0xff, 1}, /* sw_reset */
    //{0xFFFF, 200},
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0x3002, 0x00, 0xff, 1}, /* XTMSTA */

    {0x3005, 0x01, 0xff, 1},
    {0x3007, 0x10, 0xff, 1},
    {0x3009, 0x01, 0xff, 1},
    {0x300a, 0xf0, 0xff, 1},
    {0x300b, 0x00, 0xff, 1},
    {0x300c, 0x00, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3018, 0xee, 0xff, 1},//VMAX
    {0x3019, 0x02, 0xff, 1},//VMAX

    {0x301c, 0xe4, 0xff, 1},//* HMAX
    {0x301d, 0x0c, 0xff, 1},//* HMAX

    {0x3046, 0x01, 0xff, 1},//Datasheet should modify, Tools should modify
    {0x304b, 0x0a, 0xff, 1},

    {0x305c, 0x20, 0xff, 1},
    {0x305d, 0x00, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},

    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x00, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x00, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x00, 0xff, 1},
    {0x31ec, 0x0e, 0xff, 1},

    {0x3405, 0x10, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x04, 0xff, 1},
    {0x3415, 0x01, 0xff, 1},
    {0x3418, 0xd9, 0xff, 1},
    {0x3419, 0x02, 0xff, 1},
    {0x3441, 0x0c, 0xff, 1},
    {0x3442, 0x0c, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},

    {0x3446, 0x4f, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x2f, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x17, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x17, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x17, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x57, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x17, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x17, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},
    {0x3472, 0x1c, 0xff, 1},
    {0x3473, 0x05, 0xff, 1},
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t dol_1080p_30fps_4lane_10bits[] = {
    //{0x3003, 0x01, 0xff, 1}, /* sw_reset */
    //{0xFFFF, 200},
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* XTMSTA */

    {0x3005, 0x00, 0xff, 1},
    {0x3007, 0x00, 0xff, 1},
    {0x3009, 0x01, 0xff, 1},
    {0x300a, 0x3c, 0xff, 1},
    {0x300c, 0x11, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3018, 0xC4, 0xff, 1},//VMAX change from 0465 to 04C4
    {0x3019, 0x04, 0xff, 1},//VMAX

    {0x301c, 0xEC, 0xff, 1},//* HMAX */ change from 0898 to 07EC
    {0x301d, 0x07, 0xff, 1},//* HMAX */
    {0x3020, 0x3c, 0xff, 1},//SHS1
    {0x3021, 0x00, 0xff, 1},//SHS1
    {0x3022, 0x00, 0xff, 1},//SHS1
    {0x3024, 0xcb, 0xff, 1},//SHS2
    {0x3025, 0x00, 0xff, 1},//SHS2
    {0x3026, 0x00, 0xff, 1},//SHS2
    {0x3030, 0xc9, 0xff, 1},//RHS1
    {0x3031, 0x00, 0xff, 1},//RHS1
    {0x3032, 0x00, 0xff, 1},//RHS1

    {0x3045, 0x05, 0xff, 1},//DOL
    {0x3046, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    {0x304b, 0x0a, 0xff, 1},
    {0x3418, 0x68, 0xff, 1},//Y_out size, tools should modify from 089c to 0968
    {0x3419, 0x09, 0xff, 1},//Y_out size

    {0x305c, 0x18, 0xff, 1},
    {0x305d, 0x03, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},

    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x11, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x12, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    {0x3405, 0x10, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x0a, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},
    {0x3441, 0x0a, 0xff, 1},
    {0x3442, 0x0a, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},

    {0x3446, 0x57, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x37, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x1f, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x1f, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x1f, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x77, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x1f, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x17, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},
    {0x3472, 0xa0, 0xff, 1},
    {0x3473, 0x07, 0xff, 1},
    {0x347b, 0x23, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t dol_720p_30fps_4lane_10bits[] = {
    //{0x3003, 0x01, 0xff, 1}, /* sw_reset */
    //{0xFFFF, 200},
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* XTMSTA */

    {0x3005, 0x00, 0xff, 1},
    {0x3007, 0x10, 0xff, 1},
    {0x3009, 0x01, 0xff, 1},
    {0x300a, 0x3c, 0xff, 1},
    {0x300c, 0x11, 0xff, 1},
    {0x3011, 0x0a, 0xff, 1},
    {0x3018, 0xee, 0xff, 1},//VMAX
    {0x3019, 0x02, 0xff, 1},//VMAX

    {0x301c, 0xe4, 0xff, 1},//* HMAX
    {0x301d, 0x0c, 0xff, 1},//* HMAX
    {0x3020, 0x02, 0xff, 1},//SHS1
    {0x3021, 0x00, 0xff, 1},//SHS1
    {0x3022, 0x00, 0xff, 1},//SHS1
    {0x3024, 0x7b, 0xff, 1},//SHS2
    {0x3025, 0x05, 0xff, 1},//SHS2
    {0x3026, 0x00, 0xff, 1},//SHS2
    {0x3030, 0x09, 0xff, 1},//RHS1
    {0x3031, 0x00, 0xff, 1},//RHS1
    {0x3032, 0x00, 0xff, 1},//RHS1

    {0x3045, 0x05, 0xff, 1},//DOL
    {0x3046, 0x00, 0xff, 1},//Datasheet should modify, Tools should modify
    {0x304b, 0x0a, 0xff, 1},

    {0x305c, 0x20, 0xff, 1},
    {0x305d, 0x00, 0xff, 1},
    {0x305e, 0x20, 0xff, 1},
    {0x305f, 0x01, 0xff, 1},

    {0x309e, 0x4a, 0xff, 1},
    {0x309f, 0x4a, 0xff, 1},

    {0x3106, 0x11, 0xff, 1}, //Need double confirm, H company 11h, 8/3th version
    {0x311c, 0x0e, 0xff, 1},
    {0x3128, 0x04, 0xff, 1},
    {0x3129, 0x1d, 0xff, 1},

    {0x313b, 0x41, 0xff, 1},
    {0x315e, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x3164, 0x1a, 0xff, 1},// 1A:37.125MHz 1B:74.25MHz
    {0x317c, 0x12, 0xff, 1},
    {0x31ec, 0x37, 0xff, 1},

    {0x3405, 0x10, 0xff, 1},
    {0x3407, 0x03, 0xff, 1},
    {0x3414, 0x04, 0xff, 1},
    {0x3415, 0x00, 0xff, 1},
    {0x3418, 0xba, 0xff, 1},
    {0x3419, 0x05, 0xff, 1},
    {0x3441, 0x0a, 0xff, 1},
    {0x3442, 0x0a, 0xff, 1},
    {0x3443, 0x03, 0xff, 1},
    {0x3444, 0x20, 0xff, 1},//mclk :37.125M
    {0x3445, 0x25, 0xff, 1},

    {0x3446, 0x4f, 0xff, 1},//global timming
    {0x3447, 0x00, 0xff, 1},
    {0x3448, 0x2f, 0xff, 1},
    {0x3449, 0x00, 0xff, 1},
    {0x344a, 0x17, 0xff, 1},
    {0x344b, 0x00, 0xff, 1},
    {0x344c, 0x17, 0xff, 1},
    {0x344d, 0x00, 0xff, 1},
    {0x344e, 0x17, 0xff, 1},
    {0x344f, 0x00, 0xff, 1},
    {0x3450, 0x57, 0xff, 1},
    {0x3451, 0x00, 0xff, 1},
    {0x3452, 0x17, 0xff, 1},
    {0x3453, 0x00, 0xff, 1},
    {0x3454, 0x17, 0xff, 1},
    {0x3455, 0x00, 0xff, 1},

    {0x3472, 0x20, 0xff, 1},
    {0x3473, 0x05, 0xff, 1},
    {0x347b, 0x23, 0xff, 1},//add
    {0x3480, 0x49, 0xff, 1},

    {0xFFFF, 1},
    {0x3002, 0x00, 0xff, 1}, /* master mode start */
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t imx307_test_pattern[] = {
    {0x3000, 0x01, 0xff, 1}, /* standby */
    {0x300a, 0x00, 0xff, 1},
    {0x300e, 0x00, 0xff, 1},
    {0x300f, 0x00, 0xff, 1},
    {0x308c, 0x21, 0xff, 1},
    {0x0000, 0x0000, 0x0000, 0x0000},
};

static acam_reg_t settings_context_imx307[] = {
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

static const acam_reg_t *imx307_seq_table[] = {
    linear_1080p_30fps_223Mbps_4lane_12bits,
    linear_1080p_60fps_446Mbps_4lane_12bits,
    linear_720p_30fps_149Mbps_4lane_12bits,
    linear_720p_60fps_297Mbps_4lane_12bits,
    dol_1080p_30fps_4lane_10bits,
    dol_720p_30fps_4lane_10bits,
    imx307_test_pattern,
};

static const acam_reg_t *isp_seq_table[] = {
    settings_context_imx307,
};

#define SENSOR_IMX307_SEQUENCE_DEFAULT_LINEAR_60FPS_DIFF    0
#define SENSOR_IMX307_SEQUENCE_DEFAULT_WDR_1080P_2DOL    4
#define SENSOR_IMX307_SEQUENCE_DEFAULT_LINEAR    0
#define SENSOR_IMX307_SEQUENCE_DEFAULT_WDR_1080P    4
#define SENSOR_IMX307_SEQUENCE_DEFAULT_WDR_720P    5
//#define SENSOR_IMX307_SEQUENCE_DEFAULT_WDR_1080P_DIFF    5
#define SENSOR_IMX307_SEQUENCE_DEFAULT_TEST_PATTERN    6

#define SENSOR_IMX307_CONTEXT_SEQ 0
#endif /* __IMX307_SENSOR_H__ */
