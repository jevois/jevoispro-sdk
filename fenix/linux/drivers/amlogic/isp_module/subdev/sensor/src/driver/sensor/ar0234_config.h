// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// JeVois Smart Embedded Machine Vision Toolkit - Copyright (C) 2021 by Laurent Itti, the University of Southern
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

#ifndef __AR0234_CONFIG_H__
#define __AR0234_CONFIG_H__


#define FULL_EXTRA_HEIGHT 0
#define FULL_EXTRA_WIDTH 0
#define ISP_IMAGE_HEIGHT SENSOR_IMAGE_HEIGHT
#define ISP_IMAGE_WIDTH SENSOR_IMAGE_WIDTH
#define LOG2_SENSOR_AGAIN_MAXIMUM 0
#define LOG2_SENSOR_DGAIN_MAXIMUM 0
#define PREVIEW_EXTRA_HEIGHT 0
#define PREVIEW_EXTRA_WIDTH 0
#define RESOLUTION_CHANGE_ENABLED 0
#define SENSOR_AF_MOVE_DELAY 20
#define SENSOR_ANALOG_GAIN_APPLY_DELAY 1
#define SENSOR_BLACK_LEVEL_CORRECTION 0
#define SENSOR_BOARD_MASTER_CLOCK 24000
#define SENSOR_BUS i2c
#define SENSOR_CHIP_ID 0x0a56
#define SENSOR_DAY_LIGHT_INTEGRATION_TIME_LIMIT 300
#define SENSOR_DEV_ADDRESS 16 /* 0x20/2 */
#define SENSOR_DIGITAL_GAIN_APPLY_DELAY 1
#define SENSOR_EXP_NUMBER 1
#define SENSOR_IMAGE_HEIGHT 1200
#define SENSOR_IMAGE_HEIGHT_FULL 2448 /* fixme */
#define SENSOR_IMAGE_HEIGHT_PREVIEW 1200
#define SENSOR_IMAGE_WIDTH 1920
#define SENSOR_IMAGE_WIDTH_FULL 3264
#define SENSOR_IMAGE_WIDTH_PREVIEW 1920
#define SENSOR_INTEGRATION_TIME_APPLY_DELAY 1
#define SENSOR_MAX_INTEGRATION_TIME 4800
#define SENSOR_MAX_INTEGRATION_TIME_LIMIT SENSOR_MAX_INTEGRATION_TIME
#define SENSOR_MAX_INTEGRATION_TIME_NATIVE SENSOR_MAX_INTEGRATION_TIME
#define SENSOR_MAX_INTEGRATION_TIME_PREVIEW 4800
#define SENSOR_MIN_INTEGRATION_TIME 2
#define SENSOR_MIN_INTEGRATION_TIME_NATIVE SENSOR_MIN_INTEGRATION_TIME
#define SENSOR_OUTPUT_BITS 10
#define SENSOR_SEQUENCE_FULL_RES_HALF_FPS 2
#define SENSOR_SEQUENCE_FULL_RES_MAX_FPS 0
#define SENSOR_SEQUENCE_NAME default
#define SENSOR_SEQUENCE_PREVIEW_RES_HALF_FPS 3
#define SENSOR_SEQUENCE_PREVIEW_RES_MAX_FPS 1
#define SENSOR_TOTAL_HEIGHT 1310 /* fixme */
#define SENSOR_TOTAL_HEIGHT_PREVIEW 2586
#define SENSOR_TOTAL_WIDTH 2000
#define SENSOR_TOTAL_WIDTH_PREVIEW 3264
#define I2C_CLOCK_DIV 40
#define I2C_CONTROL_MASK 0

#endif
