/*
 * The Clear BSD License
 * Copyright (c) 2017, NXP
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

#ifndef __SRTM_DEFS_H__
#define __SRTM_DEFS_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*!
 * @addtogroup srtm
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Defines SRTM major version */
#define SRTM_VERSION_MAJOR (0x01U)
/*! @brief Defines SRTM minor version */
#define SRTM_VERSION_MINOR (0x00U)
/*! @brief Defines SRTM bugfix version */
#define SRTM_VERSION_BUGFIX (0x00U)

/*! @brief SRTM version definition */
#define SRTM_MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/* IAR ARM build tools */
#if defined(__ICCARM__)

#include <intrinsics.h>

#ifndef SRTM_PACKED_BEGIN
#define SRTM_PACKED_BEGIN __packed
#endif

#ifndef SRTM_PACKED_END
#define SRTM_PACKED_END
#endif

#ifndef SRTM_ANON_DEC_BEGIN
#define SRTM_ANON_DEC_BEGIN \
  _Pragma("language=extended")
#endif

#ifndef SRTM_ANON_DEC_END
#define SRTM_ANON_DEC_END   \
  _Pragma("language=default")
#endif

/* GNUC */
#elif defined(__GNUC__)

#ifndef SRTM_PACKED_BEGIN
#define SRTM_PACKED_BEGIN
#endif

#ifndef SRTM_PACKED_END
#define SRTM_PACKED_END __attribute__((__packed__))
#endif

/* anonymous unions are enabled by default */
#ifndef SRTM_ANON_DEC_BEGIN
#define SRTM_ANON_DEC_BEGIN
#endif

#ifndef SRTM_ANON_DEC_END
#define SRTM_ANON_DEC_END
#endif

/* ARMCC */
#elif defined(__CC_ARM)

#ifndef SRTM_PACKED_BEGIN
#define SRTM_PACKED_BEGIN _Pragma("pack(1U)")
#endif

#ifndef SRTM_PACKED_END
#define SRTM_PACKED_END _Pragma("pack()")
#endif

#ifndef SRTM_ANON_DEC_BEGIN
#define SRTM_ANON_DEC_BEGIN \
  _Pragma("push")           \
  _Pragma("anon_unions")
#endif

#ifndef SRTM_ANON_DEC_END
#define SRTM_ANON_DEC_END   \
  _Pragma("pop")
#endif

#else
/* There is no default definition here to avoid wrong structures packing in case of not supported compiler */
#error Please implement the structure packing macros for your compiler here!
#endif

/*! @brief Defines SRTM debug message function. If user want to debug SRTM, he should define 
    SRTM_DEBUG_MESSAGE_FUNC to proper printf function, as well as define
    SRTM_DEBUG_VERBOSE_LEVEL to intended verbose level. */
#ifdef SRTM_DEBUG_MESSAGE_FUNC
  extern int SRTM_DEBUG_MESSAGE_FUNC(const char *fmt_s, ...);

  #ifndef SRTM_DEBUG_VERBOSE_LEVEL
  #define SRTM_DEBUG_VERBOSE_LEVEL SRTM_DEBUG_VERBOSE_WARN
  #endif
  
  #define SRTM_DEBUG_MESSAGE(verbose, ...)             \
      do {                                             \
          if ((verbose) <= SRTM_DEBUG_VERBOSE_LEVEL) { \
              SRTM_DEBUG_MESSAGE_FUNC(__VA_ARGS__);    \
          }                                            \
      } while(0)
#else
  #define SRTM_DEBUG_MESSAGE(verbose, format, ...)
#endif
      
/*! @brief SRTM debug message verbose definition */
#define SRTM_DEBUG_VERBOSE_NONE  (0U)
#define SRTM_DEBUG_VERBOSE_ERROR (1U)
#define SRTM_DEBUG_VERBOSE_WARN  (2U)
#define SRTM_DEBUG_VERBOSE_INFO  (3U)
#define SRTM_DEBUG_VERBOSE_DEBUG (4U)

/**
* @brief Timeout definition: infinite wait that never timeout
*/
#define SRTM_WAIT_FOR_EVER       (0xFFFFFFFFU)

/**
* @brief Timeout definition: no wait that return immediately
*/
#define SRTM_NO_WAIT             (0x0U)

/*! @brief SRTM error code */
typedef enum _srtm_status
{
    SRTM_Status_Success = 0x00U,  /*!< Success */
    SRTM_Status_Error,            /*!< Failed */

    SRTM_Status_InvalidParameter, /*!< Invalid parameter */
    SRTM_Status_InvalidMessage,   /*!< Invalid message */
    SRTM_Status_InvalidState,     /*!< Operate in invalid state */
    SRTM_Status_OutOfMemory,      /*!< Memory allocation failed */
    SRTM_Status_Timeout,          /*!< Timeout when waiting for an event */
    SRTM_Status_ListAddFailed,    /*!< Cannot add to list as node already in another list */
    SRTM_Status_ListRemoveFailed, /*!< Cannot remove from list as node not in list */

    SRTM_Status_TransferTimeout,  /*!< Transfer timeout */
    SRTM_Status_TransferNotAvail, /*!< Transfer failed due to peer core not ready */
    SRTM_Status_TransferFailed,   /*!< Transfer failed due to communication failure */

    SRTM_Status_ServiceNotFound,  /*!< Cannot find service for a request/notification */
    SRTM_Status_ServiceVerMismatch, /*!< Service version cannot support the request/notification */
} srtm_status_t;

/**
* @brief SRTM message is a pointer to the SRTM message instance
*/
typedef struct _srtm_message *srtm_message_t;

/**
* @brief SRTM request is a pointer to the SRTM request message
*/
typedef srtm_message_t srtm_request_t;

/**
* @brief SRTM response is a pointer to the SRTM response message
*/
typedef srtm_message_t srtm_response_t;

/**
* @brief SRTM notification is a pointer to the SRTM notification message
*/
typedef srtm_message_t srtm_notification_t;

/**
* @brief SRTM procedure is a pointer to the SRTM local procedure message
*/
typedef srtm_message_t srtm_procedure_t;

/**
* @brief SRTM rawdata is a pointer to the SRTM raw data message
*/
typedef srtm_message_t srtm_rawdata_t;

/**
* @brief SRTM dispatcher is a pointer to the SRTM dispatcher instance
*/
typedef struct _srtm_dispatcher *srtm_dispatcher_t;

/**
* @brief SRTM peer core handle is a pointer to the SRTM peer core instance
*/
typedef struct _srtm_peercore *srtm_peercore_t;

/**
* @brief SRTM channel handle is a pointer to the SRTM channel instance
*/
typedef struct _srtm_channel *srtm_channel_t;

/**
* @brief SRTM service handle is a pointer to the SRTM service instance
*/
typedef struct _srtm_service *srtm_service_t;

/**
* @brief SRTM version fields
*/
typedef struct _srtm_version
{
    uint8_t major;  /*!< Major */
    uint8_t minor;  /*!< Minor */
    uint8_t bugfix; /*!< Bug fix */
} srtm_version_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Get SRTM version.
 *
 * @return SRTM version.
 */
static inline uint32_t SRTM_GetVersion(void)
{
    return SRTM_MAKE_VERSION(SRTM_VERSION_MAJOR, SRTM_VERSION_MINOR, SRTM_VERSION_BUGFIX);
}

/*! @} */

#endif /* __SRTM_DEFS_H__ */