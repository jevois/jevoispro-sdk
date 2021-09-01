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

#include <linux/delay.h>
#include "acamera_types.h"
#include "system_sensor.h"
#include "sensor_bus_config.h"
#include "acamera_command_api.h"
#include "acamera_sbus_api.h"
#include "acamera_sensor_api.h"
#include "system_timer.h"
#include "sensor_init.h"
#include "IMX290_regs.h"
#include "IMX290_seq.h"
#include "IMX290_config.h"
#include "acamera_math.h"
#include "system_am_mipi.h"
#include "system_am_adap.h"
#include "sensor_bsp_common.h"

#define NEED_CONFIG_BSP 1

#define AGAIN_MAX_DB 0x64
#define DGAIN_MAX_DB 0x6e

#define FS_LIN_1080P 1
#define FS_LIN_1080P_60FPS 0
static int sen_mode = 0;
static void start_streaming(void *ctx);
static void stop_streaming(void *ctx);

#define SENSOR_WRITE8(reg, val)                     \
  acamera_sbus_write_u8(&p_ctx->sbus, reg, val);
//  if (ret < 0) LOG(LOG_ERR, "Error WRITE8 %u to 0x%x (%s) in %s\n", val, reg, #reg, __func__);

#define SENSOR_WRITE16(reg, val)                                        \
  acamera_sbus_write_u8(&p_ctx->sbus, reg, (val) & 0xff);               \
  acamera_sbus_write_u8(&p_ctx->sbus, reg + 1, ((val) >> 8) & 0xff);

#define SENSOR_WRITE24(reg, val)                                        \
  acamera_sbus_write_u8(&p_ctx->sbus, reg, (val) & 0xff);               \
  acamera_sbus_write_u8(&p_ctx->sbus, reg + 1, ((val) >> 8) & 0xff);    \
  acamera_sbus_write_u8(&p_ctx->sbus, reg + 2, ((val) >> 16) & 0xff);

#define SENSOR_READ8(reg)                                               \
  ret = acamera_sbus_read_u8(&p_ctx->sbus, reg);                        \
  if (ret < 0) LOG(LOG_ERR, "Error READ8 from 0x%x (%s) in %s\n", reg, #reg, __func__);

#define SENSOR_READ16(reg)                                              \
  ret = acamera_sbus_read_u8(&p_ctx->sbus, reg);                        \
  if (ret < 0) LOG(LOG_ERR, "Error READ16L from 0x%x (%s) in %s\n", reg, #reg, __func__); \
  ret |= acamera_sbus_read_u8(&p_ctx->sbus, reg + 1) << 8;              \
  if (ret < 0) LOG(LOG_ERR, "Error READ16H from 0x%x (%s) in %s\n", reg, #reg, __func__);

#define SENSOR_READ24(reg)                                              \
  ret = acamera_sbus_read_u8(&p_ctx->sbus, reg);                        \
  if (ret < 0) LOG(LOG_ERR, "Error READ24L from 0x%x (%s) in %s\n", reg, #reg, __func__); \
  ret |= acamera_sbus_read_u8(&p_ctx->sbus, reg + 1) << 8;              \
  if (ret < 0) LOG(LOG_ERR, "Error READ24M from 0x%x (%s) in %s\n", reg, #reg, __func__); \
  ret |= acamera_sbus_read_u8(&p_ctx->sbus, reg + 2) << 16;             \
  if (ret < 0) LOG(LOG_ERR, "Error READ24H from 0x%x (%s) in %s\n", reg, #reg, __func__);

// ####################################################################################################
static sensor_mode_t supported_modes[] =
{
 // --------------------------------------------------
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1920,
  .resolution.height = 1080,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_1080P_30FPS,
 },
 /*
 {
  // Mode that can be rescaled to XGA, VGA, QVGA, etc
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1440,
  .resolution.height = 1080,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_VGA1080_30FPS,
 },
 {
  // Mode that can be rescaled to SXGA, etc
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1350,
  .resolution.height = 1080,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_SXGA1080_30FPS,
 },
 {
  // Mode that can be rescaled to CIF, QCIF, etc
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1320,
  .resolution.height = 1080,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_CIF1080_30FPS,
 },
 */
 /*
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,
  .resolution.width = 1920,
  .resolution.height = 1080,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 891,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_1080P_60FPS,
 },
 */
 /*
 {
  // FIXME: Frame collision in ISP
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1280,
  .resolution.height = 720,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 149,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_720P_30FPS,
 },
 */
 /*
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 30 * 256,
  .resolution.width = 1280,
  .resolution.height = 720,
  .bits = 10,
  .exposures = 1,
  .lanes = 4,
  .bps = 149,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_720P_30FPS_10BITS,
 },
 */
 /*
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 1280,
  .resolution.height = 1024,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_SXGA_30FPS,
 },
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 1280,
  .resolution.height = 800,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_WXGA_30FPS,
 },
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 1024,
  .resolution.height = 768,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_XGA_30FPS,
 },
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 640,
  .resolution.height = 480,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_VGA_30FPS,
 },
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 352,
  .resolution.height = 288,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_CIF_30FPS,
 },
 {
  .wdr_mode = WDR_MODE_LINEAR,
  .fps = 60 * 256,  // collision bugfix
  .resolution.width = 320,
  .resolution.height = 240,
  .bits = 12,
  .exposures = 1,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = IMX290_SEQ_QVGA_30FPS,
 },
 */
 
 /*
 // --------------------------------------------------
 {
  .wdr_mode = WDR_MODE_FS_LIN,
  .fps = 25 * 256,
#if FS_LIN_1080P
  .resolution.width = 1920,
  .resolution.height = 1080,
#else
  .resolution.width = 1280,
  .resolution.height = 720,
#endif
  .bits = 10,
  .exposures = 3,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = 1,
 },
 // --------------------------------------------------
 {
  .wdr_mode = WDR_MODE_FS_LIN,
  .fps = 25 * 256,
#if FS_LIN_1080P
  .resolution.width = 1920,
  .resolution.height = 1080,
#else
  .resolution.width = 1280,
  .resolution.height = 720,
#endif
  .bits = 10,
  .exposures = 3,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = 2,
 },
 // --------------------------------------------------
 {
  .wdr_mode = WDR_MODE_FS_LIN,
  .fps = 25 * 256,
#if FS_LIN_1080P
  .resolution.width = 1920,
  .resolution.height = 1080,
#else
  .resolution.width = 1280,
  .resolution.height = 720,
#endif
  .bits = 10,
  .exposures = 3,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_NON,
  .num = 3,
 },
 // --------------------------------------------------
 {
  .wdr_mode = WDR_MODE_FS_LIN,
  .fps = 30 * 256,
#if FS_LIN_1080P
  .resolution.width = 1920,
  .resolution.height = 1080,
#else
  .resolution.width = 1280,
  .resolution.height = 720,
#endif
  .bits = 10,
  .exposures = 2,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_LINEINFO,
  .num = 4,
 },
 // --------------------------------------------------
#if FS_LIN_1080P_60FPS
 {
  .wdr_mode = WDR_MODE_FS_LIN,
  .fps = 60 * 256,
  .resolution.width = 1920,
  .resolution.height = 1080,
  .bits = 10,
  .exposures = 2,
  .lanes = 4,
  .bps = 446,
  .bayer = BAYER_RGGB,
  .dol_type = DOL_LINEINFO,
  .num = 5,
 },
#endif
 */
};

// ####################################################################################################
typedef struct _sensor_context_t
{
    uint8_t address; // Sensor address for direct write (not used currently)
    uint8_t seq_width;
    uint8_t streaming_flg;
    uint16_t again[4];
    uint8_t again_delay;
    uint16_t int_time_S;
    uint16_t int_time_M;
    uint16_t int_time_L;
    uint32_t shs1;
    uint32_t shs2;
    uint32_t shs3;
    uint32_t shs1_old;
    uint32_t shs2_old;
    uint32_t rhs1;
    uint32_t rhs2;
    uint32_t again_limit;
    uint8_t s_fps;
    uint32_t vmax;
    uint8_t int_cnt;
    uint8_t gain_cnt;
    uint32_t pixel_clock;
    uint16_t max_S;
    uint16_t max_M;
    uint16_t max_L;
    uint16_t frame;
    uint32_t wdr_mode;
    acamera_sbus_t sbus;
    sensor_param_t param;
	void *sbp;
} sensor_context_t;

// ####################################################################################################
#if SENSOR_BINARY_SEQUENCE
static const char p_sensor_data[] = SENSOR_IMX290_SEQUENCE_DEFAULT;
static const char p_isp_data[] = SENSOR_IMX290_ISP_CONTEXT_SEQ;
#else
static const acam_reg_t **p_sensor_data = imx290_seq_table;
static const acam_reg_t **p_isp_data = isp_seq_table;

#endif

// ####################################################################################################
static void sensor_hw_reset_enable(void) { system_reset_sensor(0); }
static void sensor_hw_reset_disable(void) { system_reset_sensor(3); }

// ####################################################################################################
static int32_t sensor_alloc_analog_gain(void *ctx, int32_t gain)
{
  sensor_context_t *p_ctx = ctx;
  uint16_t again = (gain * 20) >> LOG2_GAIN_SHIFT;
  
  if (again > p_ctx->again_limit) again = p_ctx->again_limit;
  
  if (p_ctx->again[0] != again)
  {
    p_ctx->gain_cnt = p_ctx->again_delay + 1;
    p_ctx->again[0] = again;
  }
  
  return (((int32_t)again) << LOG2_GAIN_SHIFT) / 20;
}

// ####################################################################################################
static int32_t sensor_alloc_digital_gain(void *ctx, int32_t gain)
{
  return 0;
}

// ####################################################################################################
static void sensor_alloc_integration_time(void *ctx, uint16_t *int_time_S, uint16_t *int_time_M, uint16_t *int_time_L)
{
  sensor_context_t *p_ctx = ctx;
  uint16_t tmp;
  
  switch (p_ctx->wdr_mode)
  {
    // --------------------------------------------------
  case WDR_MODE_LINEAR: // Normal mode
    if (*int_time_S > p_ctx->vmax - 2) *int_time_S = p_ctx->vmax - 2;
    if (*int_time_S < 1) *int_time_S = 1;
    tmp = p_ctx->vmax - *int_time_S - 1;
    if (p_ctx->int_time_S != tmp)
    {
      //LOG(LOG_INFO, "JEVOIS: changed integration time to %d (vmax=%d)\n", tmp, p_ctx->vmax);
      p_ctx->int_cnt = 2;
      p_ctx->int_time_S = tmp;
    }
    break;
    
    // --------------------------------------------------
  case WDR_MODE_FS_LIN: // DOL3 Frames
#ifdef FS_LIN_3DOL
    if (*int_time_S < 2) *int_time_S = 2;
    if (*int_time_S > p_ctx->max_S) *int_time_S = p_ctx->max_S;
    if (*int_time_L < 2) *int_time_L = 2;
    if (*int_time_L > p_ctx->max_L) *int_time_L = p_ctx->max_L;
    
    if (*int_time_M < 2) *int_time_M = 2;
    if (*int_time_M > p_ctx->max_M) *int_time_M = p_ctx->max_M;
    
    if (p_ctx->int_time_S != *int_time_S || p_ctx->int_time_M != *int_time_M || p_ctx->int_time_L != *int_time_L)
    {
      p_ctx->int_cnt = 3;
      
      p_ctx->int_time_S = *int_time_S;
      p_ctx->int_time_M = *int_time_M;
      p_ctx->int_time_L = *int_time_L;
      
      p_ctx->shs3 = p_ctx->frame - *int_time_L - 1;
      p_ctx->shs1 = p_ctx->rhs1 - *int_time_M - 1;
      p_ctx->shs2 = p_ctx->rhs2 - *int_time_S - 1;
    }
#else  //default DOL2 Frames
    if (*int_time_S < 2) *int_time_S = 2;
    if (*int_time_S > p_ctx->max_S) *int_time_S = p_ctx->max_S;
    if (*int_time_L < 2) *int_time_L = 2;
    if (*int_time_L > p_ctx->max_L) *int_time_L = p_ctx->max_L;
    
    if (p_ctx->int_time_S != *int_time_S || p_ctx->int_time_L != *int_time_L)
    {
      p_ctx->int_cnt = 2;
      
      p_ctx->int_time_S = *int_time_S;
      p_ctx->int_time_L = *int_time_L;
      
      p_ctx->shs2 = p_ctx->frame - *int_time_L - 1;
      p_ctx->shs1 = p_ctx->rhs1 - *int_time_S - 1;
    }
#endif
    break;
  }
}

//####################################################################################################
static int32_t sensor_ir_cut_set(void *ctx, int32_t ir_cut_state)
{
  sensor_context_t *t_ctx = ctx; sensor_bringup_t* sensor_bp = t_ctx->sbp;
  int ret;

  LOG(LOG_ERR, "ir_cut_state = %d", ir_cut_state);
  LOG(LOG_INFO, "entry ir cut");
  
  if (sensor_bp->ir_gname[0] <= 0 && sensor_bp->ir_gname[1] <= 0) { pr_err("get gpio id fail\n"); return 0; }
  
  if (ir_cut_state == 1)
  {
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[1], 1); if (ret < 0) pr_err("set ircut fail\n");
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[0], 0); if (ret < 0) pr_err("set ircut fail\n");
    mdelay(500);
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[0], 1); if (ret < 0) pr_err("set ircut fail\n");
  }
  else if (ir_cut_state == 0)
  {
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[1], 0); if (ret < 0) pr_err("set ircut fail\n");
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[0], 1); if (ret < 0) pr_err("set ircut fail\n");
    mdelay(500);
    ret = pwr_ir_cut_enable(sensor_bp, sensor_bp->ir_gname[1], 1); if (ret < 0) pr_err("set ircut fail\n");
  }
  else if (ir_cut_state == 2) return 0;
  else LOG(LOG_ERR, "sensor ir cut set failed");
  
  LOG(LOG_INFO, "exit ir cut");
  return 0;
}

// ####################################################################################################
static void sensor_update(void *ctx)
{
  sensor_context_t *p_ctx = ctx;
  
  if (p_ctx->int_cnt || p_ctx->gain_cnt)
  {
    // ---------- Start Changes -------------
    SENSOR_WRITE8(IMX290_REG_REGHOLD, IMX290_VAL_REGHOLD_ON);
    
    // ---------- Analog Gain -------------
    if (p_ctx->gain_cnt)
    {
      p_ctx->gain_cnt--;
      SENSOR_WRITE8(IMX290_REG_GAIN, p_ctx->again[p_ctx->again_delay]);
    }
    
    // -------- Integration Time ----------
    if (p_ctx->int_cnt)
    {
      p_ctx->int_cnt--;
      switch (p_ctx->wdr_mode)
      {
      case WDR_MODE_LINEAR:
        SENSOR_WRITE24(IMX290_REG_SHS1_L, p_ctx->int_time_S);
        //LOG(LOG_INFO, "JEVOIS: set integration time to %d\n", p_ctx->int_time_S);
        break;
        
      case WDR_MODE_FS_LIN:
        p_ctx->shs2_old = p_ctx->shs2;
        p_ctx->shs1_old = p_ctx->shs1;
#ifdef FS_LIN_3DOL
        SENSOR_WRITE24(IMX290_REG_SHS3_L, p_ctx->shs3);
#endif
        SENSOR_WRITE24(IMX290_REG_SHS1_L, p_ctx->shs1_old);
        SENSOR_WRITE24(IMX290_REG_SHS2_L, p_ctx->shs2_old);
        break;
      }
    }
    
    // ---------- End Changes -------------
    SENSOR_WRITE8(IMX290_REG_REGHOLD, IMX290_VAL_REGHOLD_OFF);
  }
  p_ctx->shs1_old = p_ctx->shs1;
  p_ctx->shs2_old = p_ctx->shs2;
  p_ctx->again[3] = p_ctx->again[2];
  p_ctx->again[2] = p_ctx->again[1];
  p_ctx->again[1] = p_ctx->again[0];
}

// ####################################################################################################
static uint16_t sensor_get_id(void *ctx)
{
  sensor_context_t *p_ctx = ctx; int ret;
  SENSOR_READ16(IMX290_REG_ID_L);
  LOG(LOG_CRIT, "JEVOIS: camera sensor ID is 0x%x", ret);
  
  if (ret != SENSOR_CHIP_ID)
  {
    LOG(LOG_ERR, "%s: Wrong sensor ID 0x%x - IMX290 should be 0x%x\n", __func__, ret, SENSOR_CHIP_ID);
    return 0xFF;
  }
  return 0;
}
// ####################################################################################################
void sensor_dump(void *ctx)
{
  sensor_context_t *p_ctx = ctx; sensor_param_t *param = &p_ctx->param;
  int ret;

  int bit=-1, win=-1, frsel=-1, cg=-1, blk=-1, gain=-1, hmax=-1, vmax=-1, shs1=-1, shs2=-1, shs3=-1, rhs1=-1, rhs2=-1,
    winwv_ob=-1, winpv=-1, winwv=-1, winph=-1, winwh=-1, oport=-1, xvs=-1, xhs=-1, sout=-1, inck1=-1, inck2=-1,
    inck3=-1, inck4=-1, inck5=-1, inck6=-1, repet=-1, opbv=-1, yout=-1, thsex=-1, tclkpre=-1, dtfmt=-1,
    tclkpost=-1, thsz=-1, thsprep=-1, tclktr=-1, thstr=-1, tclkz=-1, tclkprep=-1, tlpx=-1, xout=-1;

  if (sensor_get_id(ctx) != 0) return;

  SENSOR_READ8(IMX290_REG_ADBIT);
  if (ret == IMX290_VAL_ADBIT_10BIT) bit = 10;
  else if (ret == IMX290_VAL_ADBIT_12BIT) bit = 12;

  SENSOR_READ8(IMX290_REG_WIN);
  if ((ret & IMX290_MSK_WINMODE) == IMX290_VAL_WIN_1080P) win = 1080;
  else if ((ret & IMX290_MSK_WINMODE) == IMX290_VAL_WIN_720P) win = 720;
  else if ((ret & IMX290_MSK_WINMODE) == IMX290_VAL_WIN_CROP) win = 480; // 480 for crop

  SENSOR_READ8(IMX290_REG_FRAME);
  if ((ret & IMX290_MSK_FRSEL) == IMX290_VAL_FRSEL_30FPS) frsel=30;
  else if ((ret & IMX290_MSK_FRSEL) == IMX290_VAL_FRSEL_60FPS) frsel=60;
  else if ((ret & IMX290_MSK_FRSEL) == IMX290_VAL_FRSEL_120FPS) frsel=120;
  if ((ret & IMX290_MSK_FDG_SEL) == IMX290_VAL_LCG) cg = 0;
  else if ((ret & IMX290_MSK_FDG_SEL) == IMX290_VAL_HCG) cg = 1;

  SENSOR_READ16(IMX290_REG_BLKLEVEL_L); blk = ret;
  SENSOR_READ8(IMX290_REG_GAIN); gain = ret;
  SENSOR_READ16(IMX290_REG_HMAX_L); hmax = ret;
  SENSOR_READ24(IMX290_REG_VMAX_L); vmax = ret;
  SENSOR_READ24(IMX290_REG_SHS1_L); shs1 = ret;
  SENSOR_READ24(IMX290_REG_SHS2_L); shs2 = ret;
  SENSOR_READ24(IMX290_REG_SHS3_L); shs3 = ret;
  SENSOR_READ24(IMX290_REG_RHS1_L); rhs1 = ret;
  SENSOR_READ24(IMX290_REG_RHS2_L); rhs2 = ret;

  SENSOR_READ8(IMX290_REG_WINWV_OB); winwv_ob = ret;
  SENSOR_READ16(IMX290_REG_WINPV_L); winpv = ret;
  SENSOR_READ16(IMX290_REG_WINWV_L); winwv = ret;
  SENSOR_READ16(IMX290_REG_WINPH_L); winph = ret;
  SENSOR_READ16(IMX290_REG_WINWH_L); winwh = ret;

  SENSOR_READ8(IMX290_REG_OPORT); oport = ret;
  SENSOR_READ8(IMX290_REG_XVSLNG); xvs = ret;
  SENSOR_READ8(IMX290_REG_XHSLNG); xhs = ret;
  SENSOR_READ8(IMX290_REG_SOUTSEL); sout = ret;

  SENSOR_READ8(IMX290_REG_INCKSEL1); inck1 = ret;
  SENSOR_READ8(IMX290_REG_INCKSEL2); inck2 = ret;
  SENSOR_READ8(IMX290_REG_INCKSEL3); inck3 = ret;
  SENSOR_READ8(IMX290_REG_INCKSEL4); inck4 = ret;
  SENSOR_READ8(IMX290_REG_INCKSEL5); inck5 = ret;
  SENSOR_READ8(IMX290_REG_INCKSEL6); inck6 = ret;

  SENSOR_READ8(IMX290_REG_REPETITION); repet = ret;
  SENSOR_READ8(IMX290_REG_OPB_SIZE_V); opbv = ret;

  SENSOR_READ16(IMX290_REG_Y_OUT_SIZE_L); yout = ret;

  SENSOR_READ16(IMX290_REG_THSEXIT_L); thsex = ret;
  SENSOR_READ16(IMX290_REG_TCLKPRE_L); tclkpre = ret;
  SENSOR_READ16(IMX290_REG_CSI_DT_FMT_L); dtfmt = ret;
  SENSOR_READ16(IMX290_REG_TCLKPOST_L); tclkpost = ret;
  SENSOR_READ16(IMX290_REG_THSZERO_L); thsz = ret;
  SENSOR_READ16(IMX290_REG_THSPREPARE_L); thsprep = ret;
  SENSOR_READ16(IMX290_REG_TCLKTRAIL_L); tclktr = ret;
  SENSOR_READ16(IMX290_REG_THSTRAIL_L); thstr = ret;
  SENSOR_READ16(IMX290_REG_TCLKZERO_L); tclkz = ret;
  SENSOR_READ16(IMX290_REG_TCLKPREPARE_L); tclkprep = ret;
  SENSOR_READ16(IMX290_REG_TLPX_L); tlpx = ret;

  SENSOR_READ16(IMX290_REG_X_OUT_SIZE_L); xout = ret;

  printk("bit=%d win=%d frsel=%d cg=%d blk=%d gain=%d\n", bit, win, frsel, cg, blk, gain);
  printk("hmax=%d (0x%x) vmax=%d (0x%x) shs1=%d shs2=%d shs3=%d rhs1=%d rhs2=%d\n",
         hmax, hmax, vmax, vmax, shs1, shs2, shs3, rhs1, rhs2);
  printk("winwv_ob=%d winpv=%d winwv=%d winph=%d winwh=%d\n", winwv_ob, winpv, winwv, winph, winwh);
  printk("oport=0x%x xvslng=0x%x xhslng=0x%x soutsel=0x%x\n", oport, xvs, xhs, sout);
  printk("inck1=0x%x inck2=0x%x inck3=0x%x inck4=0x%x inck5=0x%x inck6=0x%x\n",
      inck1, inck2, inck3, inck4, inck5, inck6);
  printk("repet=0x%x opbv=0x%x yout=%d xout=%d dtfmt=0x%x\n", repet, opbv, yout, xout, dtfmt);
  printk("thsex=0x%x tclkpre=0x%x tclkpost=0x%x thsz=0x%x thsprep=0x%x\n",
      thsex, tclkpre, tclkpost, thsz, thsprep);
  printk("tclktr=0x%x thstr=0x%x tclkz=0x%x tclkprep=0x%x tlpx=0x%x\n",
      tclktr, thstr, tclkz, tclkprep, tlpx);
  if (param)
  {
    printk("EXTRA PARAMS: total=%dx%d active=%dx%d\n",
        param->total.width, param->total.height, param->active.width, param->active.height);
  }
}

// ####################################################################################################
static void sensor_set_iface(sensor_mode_t *mode)
{
  am_mipi_info_t mipi_info;
  struct am_adap_info info;
  
  if (mode == NULL) { LOG(LOG_ERR, "Error input param\n"); return; }
  
  memset(&mipi_info, 0, sizeof(mipi_info));
  memset(&info, 0, sizeof(struct am_adap_info));
  mipi_info.fte1_flag = get_fte1_flag();
  mipi_info.lanes = mode->lanes;
  mipi_info.ui_val = 1000 / mode->bps;
  
  if ((1000 % mode->bps) != 0) mipi_info.ui_val += 1;
  
  am_mipi_init(&mipi_info);
  
  switch (mode->bits)
  {
  case 10: info.fmt = AM_RAW10; break;
  case 12: info.fmt = AM_RAW12; break;
  default: info.fmt = AM_RAW10; break;
  }
  
  info.img.width = mode->resolution.width;
  info.img.height = mode->resolution.height;
  info.path = PATH0;
  if (mode->wdr_mode == WDR_MODE_FS_LIN)
  {
    info.mode = DOL_MODE;
    info.type = mode->dol_type;
    if (info.type == DOL_LINEINFO) { info.offset.long_offset = 0xa; info.offset.short_offset = 0x1d; }
  }
  else info.mode = DIR_MODE;

  am_adap_set_info(&info);
  am_adap_init();
  am_adap_start(0);
}

// ####################################################################################################
static void sensor_set_mode(void *ctx, uint8_t mode)
{
  sensor_context_t *p_ctx = ctx; sensor_param_t *param = &p_ctx->param; acamera_sbus_ptr_t p_sbus = &p_ctx->sbus;
  uint8_t setting_num = 0; int ret;
  sen_mode = mode;
  
  sensor_hw_reset_enable();
  system_timer_usleep(10000);
  sensor_hw_reset_disable();
  system_timer_usleep(50000);
  
  setting_num = param->modes_table[mode].num;

  // Check sensor ID one more time to make surr I2C is ok:
  if (sensor_get_id(ctx) != 0) return;

  LOG(LOG_INFO, "JEVOIS: sensor_set_mode %d", mode);
  
  sensor_load_sequence(p_sbus, p_ctx->seq_width, p_sensor_data, setting_num);

  switch (param->modes_table[mode].wdr_mode)
  {
  case WDR_MODE_LINEAR:
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode load seq %d WDR_MODE_LINEAR", setting_num);
    p_ctx->s_fps = 30;//////param->modes_table[mode].fps >> 8; // JEVOIS WAS 30
    p_ctx->again_delay = 0;
    param->integration_time_apply_delay = 2;
    param->isp_exposure_channel_delay = 0;
    break;
    
  case WDR_MODE_FS_LIN:
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode load seq %d WDR_MODE_FS_LIN", setting_num);
    p_ctx->again_delay = 0;
    param->integration_time_apply_delay = 2;
    param->isp_exposure_channel_delay = 0;
    p_ctx->s_fps = 50;
    break;
    
  default: LOG(LOG_ERR, "Invalid wdr_mode 0x%x\n", param->modes_table[mode].wdr_mode); return;
  }

  /*
  if (param->modes_table[mode].fps == 25 * 256)
  {
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode set 25fps");
    SENSOR_WRITE16(IMX290_REG_VMAX_L, 0x0546);  // JEVOIS FIXME what about 720p
    p_ctx->s_fps = 25;
    p_ctx->vmax = 1350;
  }
  else if ((param->modes_table[mode].exposures == 2) && (param->modes_table[mode].fps == 30 * 256))
  {
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode set 30fps");
    p_ctx->s_fps = 30;
    p_ctx->vmax = 1220;
  }
  else if ((param->modes_table[mode].exposures == 2) && (param->modes_table[mode].fps == 60 * 256))
  {
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode set 60fps");
    p_ctx->s_fps = 60;
    p_ctx->vmax = 1220;
  }
  else
  {
    LOG(LOG_INFO, "JEVOIS: sensor_set_mode set no fps");
    //p_ctx->vmax = ((uint32_t)acamera_sbus_read_u8(p_sbus,0x8219)<<8)|acamera_sbus_read_u8(p_sbus,0x8218);
    p_ctx->vmax = 1125;
  }
  */
  
  SENSOR_READ8(IMX290_REG_WIN); ret &= IMX290_MSK_WINMODE;
  switch (ret)
  {
  case IMX290_VAL_WIN_1080P: // HD 1080p
    param->active.width = 1920;
    param->active.height = 1080;
    p_ctx->max_L = 2236;
    //p_ctx->max_M = 486 + 30;
    p_ctx->max_S = 198;
    p_ctx->rhs1 = 201;
    //p_ctx->rhs2 = 560;
    // p_ctx->vmax=1125;
    break;

  case IMX290_VAL_WIN_720P:
    ///JEVOIS #if FS_LIN_1080P
    ///JEVOIS param->active.width = 1920;
    ///JEVOIS param->active.height = 1080;
    ///JEVOIS #else
    // HD 720p
    param->active.width = 1280;
    param->active.height = 720;
    ///JEVOIS #endif
    /***************************
    p_ctx->max_L = 2502;
    p_ctx->max_M = 420;
    p_ctx->max_S = 60;
    p_ctx->rhs1 = 427;
    p_ctx->rhs2 = 494;
    */

    //p_ctx->vmax = IMX290_VMAX_720P;////JEVOIS should set vmax according to resolution
    
    break;
    
  case IMX290_VAL_WIN_CROP:
    param->active.width = param->modes_table[mode].resolution.width;
    param->active.height = param->modes_table[mode].resolution.height;
    
  default:
    LOG(LOG_CRIT,"WRONG IMAGE SIZE CONFIG");
    break;
  }
  
  /***************** FIXME
  if (param->modes_table[mode].exposures == 1)
  {
    SENSOR_WRITE24(IMX290_REG_RHS1_L, p_ctx->rhs1);
    //SENSOR_WRITE24(IMX290_REG_RHS2_L, p_ctx->rhs2);
  }
  */
  SENSOR_READ16(IMX290_REG_HMAX_L);
  param->total.width = (uint16_t)(ret);
  param->pixels_per_line = param->total.width;
  param->lines_per_second = p_ctx->pixel_clock / param->total.width;
  SENSOR_READ24(IMX290_REG_VMAX_L);
  param->total.height = (uint16_t)(ret);
  p_ctx->vmax = ret;
  param->integration_time_min = SENSOR_MIN_INTEGRATION_TIME;
  if (param->modes_table[mode].wdr_mode == WDR_MODE_LINEAR)
  {
    param->integration_time_limit = p_ctx->vmax - 2;
    param->integration_time_max = p_ctx->vmax - 2;
  }
  else
  {
    param->integration_time_limit = p_ctx->max_S;
    param->integration_time_max = p_ctx->max_S;
    if (param->modes_table[mode].exposures == 2)
    {
      param->integration_time_long_max = (p_ctx->vmax << 1) - 256;
      param->lines_per_second = param->lines_per_second >> 1;
      p_ctx->frame = p_ctx->vmax << 1;
    }
    else
    {
      param->integration_time_long_max = (p_ctx->vmax << 2) - 256;
      param->lines_per_second = param->lines_per_second >> 2;
      p_ctx->frame = p_ctx->vmax << 2;
    }
  }
  param->sensor_exp_number = param->modes_table[mode].exposures;
  param->mode = mode;
  p_ctx->wdr_mode = param->modes_table[mode].wdr_mode;
  param->bayer = param->modes_table[mode].bayer;
  
  sensor_set_iface(&param->modes_table[mode]);
  
  sensor_dump(ctx);
}

// ####################################################################################################
static const sensor_param_t *sensor_get_parameters(void *ctx)
{
  sensor_context_t *p_ctx = ctx;
  return (const sensor_param_t *)&p_ctx->param;
}

// ####################################################################################################
static void sensor_disable_isp(void *ctx)
{ }

// ####################################################################################################
static uint32_t read_register(void *ctx, uint32_t address)
{
  sensor_context_t *p_ctx = ctx; int ret;
  SENSOR_READ8(address);
  return ret;
}

// ####################################################################################################
static void write_register(void *ctx, uint32_t address, uint32_t data)
{
  sensor_context_t *p_ctx = ctx;
  SENSOR_WRITE8(address, data);
}

// ####################################################################################################
static void stop_streaming(void *ctx)
{
  sensor_context_t *p_ctx = ctx;
  p_ctx->streaming_flg = 0;
  
  SENSOR_WRITE8(IMX290_REG_STANDBY, IMX290_VAL_STANDBY_ON);
  
  reset_sensor_bus_counter();
  am_adap_deinit();
  am_mipi_deinit();
}

// ####################################################################################################
static void start_streaming(void *ctx)
{
  sensor_context_t *p_ctx = ctx;
  sensor_param_t *param = &p_ctx->param;
  sensor_set_iface(&param->modes_table[sen_mode]);
  p_ctx->streaming_flg = 1;
  SENSOR_WRITE8(IMX290_REG_STANDBY, IMX290_VAL_STANDBY_OFF);
  
#ifdef ACAMERA_FPGA_SENSOR_START_AUTO_CALIBRATION_FSM_0123_DEFAULT
  // trigger auto calibration process for the imx290 sensor
  acamera_fpga_sensor_start_auto_calibration_fsm_0123_write(0, 0);
  acamera_fpga_sensor_start_auto_calibration_fsm_0123_write(0, 1);
  
  acamera_fpga_sensor_start_auto_calibration_fsm_4567_write(0, 0);
  acamera_fpga_sensor_start_auto_calibration_fsm_4567_write(0, 1);
#endif
}

// ####################################################################################################
static void sensor_test_pattern(void *ctx, uint8_t mode)
{
  sensor_context_t *p_ctx = ctx;
  acamera_sbus_ptr_t p_sbus = &p_ctx->sbus;
  sensor_load_sequence(p_sbus, p_ctx->seq_width, p_sensor_data, IMX290_SEQ_TEST_PATTERN);
}

// ####################################################################################################
void sensor_deinit_imx290(void *ctx)
{
  sensor_context_t *t_ctx = ctx;
  reset_sensor_bus_counter();
  am_adap_deinit();
  am_mipi_deinit();
  acamera_sbus_deinit(&t_ctx->sbus, sbus_i2c);
  if (t_ctx != NULL && t_ctx->sbp != NULL) clk_am_disable(t_ctx->sbp);
}

// ####################################################################################################
void sensor_init_imx290(void **ctx, sensor_control_t *ctrl, void* sbp)
{
  // Local sensor data structure
  static sensor_context_t s_ctx; int ret;
  sensor_bringup_t* sensor_bp = (sensor_bringup_t*) sbp;
  
  // JEVOIS: turn on power
#if NEED_CONFIG_BSP
  pr_info("JEVOIS: sensor power on\n");
  ret = pwr_am_enable(sensor_bp, "power-enable", 1);
  if (ret < 0) pr_err("set power fail\n");
  system_timer_usleep(5000); // power up settle delay
#endif
  
  pr_info("JEVOIS: enable clock\n");
  ret = clk_am_enable(sensor_bp, "gen_clk");
  if (ret < 0) pr_err("set mclk fail\n");
  system_timer_usleep(25000); // clock settle delay
  
#if NEED_CONFIG_BSP
  pr_info("JEVOIS: sensor reset enable\n");
  ret = reset_am_enable(sensor_bp, "reset", 1);
  if (ret < 0) pr_info("set reset fail\n");
  system_timer_usleep(50000); // reset delay
#endif
  
  *ctx = &s_ctx;
  s_ctx.sbp = sbp;
  
  s_ctx.sbus.mask = SBUS_MASK_SAMPLE_8BITS | SBUS_MASK_ADDR_16BITS | SBUS_MASK_ADDR_SWAP_BYTES;
  s_ctx.sbus.control = I2C_CONTROL_MASK;
  s_ctx.sbus.bus = 1;
  s_ctx.sbus.device = SENSOR_DEV_ADDRESS;
  acamera_sbus_init(&s_ctx.sbus, sbus_i2c);
  
  // Initial local parameters
  s_ctx.address = SENSOR_DEV_ADDRESS;
  s_ctx.seq_width = 1;
  s_ctx.streaming_flg = 0;
  s_ctx.again[0] = 0;
  s_ctx.again[1] = 0;
  s_ctx.again[2] = 0;
  s_ctx.again[3] = 0;
  s_ctx.again_limit = AGAIN_MAX_DB + DGAIN_MAX_DB;
  s_ctx.pixel_clock = 148500000;
  
  s_ctx.param.again_accuracy = 1 << LOG2_GAIN_SHIFT;
  s_ctx.param.sensor_exp_number = 1;
  s_ctx.param.again_log2_max = ((AGAIN_MAX_DB + DGAIN_MAX_DB) << LOG2_GAIN_SHIFT) / 20;
  s_ctx.param.dgain_log2_max = 0;
  s_ctx.param.integration_time_apply_delay = 2;
  s_ctx.param.isp_exposure_channel_delay = 0;
  s_ctx.param.modes_table = supported_modes;
  s_ctx.param.modes_num = array_size(supported_modes);
  s_ctx.param.sensor_ctx = &s_ctx;
  s_ctx.param.isp_context_seq.sequence = p_isp_data;
  s_ctx.param.isp_context_seq.seq_num= SENSOR_IMX290_CONTEXT_SEQ;
  s_ctx.param.isp_context_seq.seq_table_max = array_size(isp_seq_table);
  
  ctrl->alloc_analog_gain = sensor_alloc_analog_gain;
  ctrl->alloc_digital_gain = sensor_alloc_digital_gain;
  ctrl->alloc_integration_time = sensor_alloc_integration_time;
  ctrl->ir_cut_set= sensor_ir_cut_set;
  ctrl->sensor_update = sensor_update;
  ctrl->set_mode = sensor_set_mode;
  ctrl->get_id = sensor_get_id;
  ctrl->get_parameters = sensor_get_parameters;
  ctrl->disable_sensor_isp = sensor_disable_isp;
  ctrl->read_sensor_register = read_register;
  ctrl->write_sensor_register = write_register;
  ctrl->start_streaming = start_streaming;
  ctrl->stop_streaming = stop_streaming;
  ctrl->sensor_test_pattern = sensor_test_pattern;
  
  // Reset sensor during initialization
  pr_info("JEVOIS: sensor HW reset start\n");
  sensor_hw_reset_enable();
  system_timer_usleep(1000); // reset at least 1 ms
  sensor_hw_reset_disable();
  system_timer_usleep(50000); // wait for chip to come back up
  pr_info("JEVOIS: sensor HW reset done\n");
  
  // JEVOIS: do a software reset
  //  pr_info("JEVOIS: sensor software reset start...\n");
  //acamera_sbus_write_u8(&s_ctx.sbus, IMX290_REG_RESET, IMX290_VAL_RESET_ON); // Will be cleared after reset
  //system_timer_usleep(50000); // reset at least 1 ms
  //pr_info("JEVOIS: sensor software reset done.\n");
  
  // JEVOIS: get sensor id:
  sensor_get_id(&s_ctx);
}
