/*
 * Copyright 2015 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MPP_HAL_H__
#define __MPP_HAL_H__

#include "hal_task.h"
#include "mpp_buf_slot.h"

typedef enum MppHalType_e {
    HAL_MODE_LIBVPU,
    HAL_MODE_V4L2,
    HAL_MODE_BUTT,
} HalWorkMode;


typedef enum MppHalHardType_e {
    HAL_VDPU1,          /**< VDPU1 for 3188/3288/3368/312x */
    HAL_VDPU2,          /**< VDPU2 for 3228/3399/1108 */
    HAL_VEPU,           //!< vpu combined encoder
    HAL_RKVDEC,         //!< rock-chip h264 h265 vp9 combined decoder
    HAL_RKVENC,         //!< rock-chip h264 h265 combined encoder
    HAL_VEPU1,		/* VEPU1 for 3188/3288/3368/312x */
    HAL_DEVICE_BUTT,
    HAL_INVALID = 0xffff, /* Used for initialize variable safely */
} HalDeviceId;


typedef void*   MppHalCtx;


typedef struct MppHalCfg_t {
    // input
    MppCtxType      type;
    MppCodingType   coding;
    HalWorkMode     work_mode;
    HalDeviceId     device_id;
    MppBufSlots     frame_slots;
    MppBufSlots     packet_slots;

    // output
    HalTaskGroup    tasks;
    RK_S32          task_count;
    RK_U32          fast_mode;
    IOInterruptCB   hal_int_cb;
} MppHalCfg;

typedef struct MppHalApi_t {
    char            *name;
    MppCtxType      type;
    MppCodingType   coding;
    RK_U32          ctx_size;
    RK_U32          flag;

    MPP_RET (*init)(void *ctx, MppHalCfg *cfg);
    MPP_RET (*deinit)(void *ctx);

    // task preprocess function
    MPP_RET (*reg_gen)(void *ctx, HalTaskInfo *syn);

    // hw operation function
    MPP_RET (*start)(void *ctx, HalTaskInfo *task);
    MPP_RET (*wait)(void *ctx, HalTaskInfo *task);

    MPP_RET (*reset)(void *ctx);
    MPP_RET (*flush)(void *ctx);
    MPP_RET (*control)(void *ctx, RK_S32 cmd, void *param);
} MppHalApi;

typedef void* MppHal;

#ifdef __cplusplus
extern "C" {
#endif

MPP_RET mpp_hal_init(MppHal *ctx, MppHalCfg *cfg);
MPP_RET mpp_hal_deinit(MppHal ctx);

MPP_RET mpp_hal_reg_gen(MppHal ctx, HalTaskInfo *task);
MPP_RET mpp_hal_hw_start(MppHal ctx, HalTaskInfo *task);
MPP_RET mpp_hal_hw_wait(MppHal ctx, HalTaskInfo *task);

MPP_RET mpp_hal_reset(MppHal ctx);
MPP_RET mpp_hal_flush(MppHal ctx);
MPP_RET mpp_hal_control(MppHal ctx, RK_S32 cmd, void *param);

HalDeviceId mpp_hal_get_vpu_version(RK_U8 is_encoder);
#ifdef __cplusplus
}
#endif

#endif /*__MPP_HAL_H__*/

