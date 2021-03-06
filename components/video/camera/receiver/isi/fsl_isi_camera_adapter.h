/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_ISI_CAMERA_ADAPTER_H_
#define _FSL_ISI_CAMERA_ADAPTER_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef ISI_CAMERA_QUEUE_SIZE
#define ISI_CAMERA_QUEUE_SIZE 4U
#endif

#if (ISI_CAMERA_QUEUE_SIZE < 3)
#error The ISI camera queue size should not be less than 3
#endif

#define ISI_MAX_ACTIVE_BUF 2

/*
 * One empty room reserved to distinguish whether the queue is full or empty.
 */
#define ISI_CAMERA_ACTUAL_QUEUE_SIZE (ISI_CAMERA_QUEUE_SIZE + 1)

/*! @brief The private data used by the ISI camera receiver. */
typedef struct _isi_private_data
{
    video_ringbuf_t fullRingBuf;                         /*!< The ring buffer to save full frame buffers. */
    video_ringbuf_t emptyRingBuf;                        /*!< The ring buffer to save empty frame buffers. */
    volatile uint8_t outputBufIdx;                       /*!< Current active output buffer index. */
    volatile bool isTransferStarted;                     /*!< Transfer started using ISI_ADAPTER_Start. */
    void *fullRingBufMem[ISI_CAMERA_ACTUAL_QUEUE_SIZE];  /*!< Memory for ring buffer. */
    void *emptyRingBufMem[ISI_CAMERA_ACTUAL_QUEUE_SIZE]; /*!< Memory for ring buffer. */
    uint32_t activeBuf[ISI_MAX_ACTIVE_BUF];              /*!< Active frame buffers. */
    uint32_t activeBufSave[ISI_MAX_ACTIVE_BUF];          /*!< Save the active buffer address when transfer stoped. */
    uint8_t activeBufSaveCnt;                            /*!< Valid buffers count saved in activeBufSave. */
    uint32_t dropFrame;                                  /*!< Buffer to save the droped frame. */
    camera_receiver_callback_t callback;                 /*!< Save the callback. */
    void *userData;                                      /*!< Parameter for the callback. */
} isi_private_data_t;

/*!
 * @brief The resources used by the ISI camera receiver.
 */
typedef struct _isi_resource
{
    ISI_Type *isiBase;    /*!< ISI peripheral register base. */
    uint8_t isiInputPort; /*!< ISI input port. */
} isi_resource_t;

/*!
 * @brief The resources used by the ISI camera receiver.
 */
typedef struct
{
    uint8_t outputBytesPerPixel;            /*!< Output byte per pixel. */
    video_pixel_format_t outputPixelFormat; /*!< Output frame pixel format. */
    uint32_t outputFrameResolution;         /*!< Output frame resolution. */
} isi_ext_config_t;

/*! @brief ISI camera receiver operations structure. */
extern const camera_receiver_operations_t isi_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief ISI camera adapter IRQ handler.
 *
 * Application should install this handler to the ISI IRQ handler.
 *
 * @param handle Camera receiver handle.
 */
void ISI_ADAPTER_IRQHandler(camera_receiver_handle_t *handle);
#if defined(__cplusplus)
}
#endif

#endif /* _FSL_ISI_CAMERA_ADAPTER_H_ */
