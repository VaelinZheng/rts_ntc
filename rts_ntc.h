/*
 * Copyright (c) 2018-2026, vaelin.zheng@gmail.com
 *
 * SPDX-License-Identifier: MIT
*/

#ifndef _RTS_NTC_H_
#define _RTS_NTC_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(KCONFIG_HEADER_FILE)
    #if defined(__C51__) || defined(__ARMCC_VERSION)
        #define _RTS_MACRO_TO_STR_IMPL(s) #s
        #define _RTS_MACRO_TO_STR(s) _RTS_MACRO_TO_STR_IMPL(s)
        #include _RTS_MACRO_TO_STR(KCONFIG_HEADER_FILE)
        #undef _RTS_MACRO_TO_STR
        #undef _RTS_MACRO_TO_STR_IMPL
    #else
        #include KCONFIG_HEADER_FILE
    #endif
#endif

#if (CONFIG_RTS_NTC_ENABLE)
#include "stdint.h"

#define RTS_NTC_VERSION_MAJOR  (0)
#define RTS_NTC_VERSION_MINOR  (2)
#define RTS_NTC_VERSION_PATCH  (1)
#define RTS_NTC_VERSION        ((RTS_NTC_VERSION_MAJOR << 16) | (RTS_NTC_VERSION_MINOR << 8) | (RTS_NTC_VERSION_PATCH))

typedef enum
{
    RTS_NTC_OK = 0,
    RTS_NTC_ERROR,
    RTS_NTC_ADC_BELOW_MIN, // ADC value is below the lookup table range.
    RTS_NTC_ADC_ABOVE_MAX, // ADC value is above the lookup table range.
} rts_ntc_error_t;

typedef struct
{
    uint16_t const *adc_tab; // ADC lookup table indexed by increasing temperature.
    uint16_t tab_item_num;   // Number of entries in adc_tab.
    int16_t offset_tempX10;  // Temperature at index zero, in tenths of a degree Celsius.
} rts_ntc_t;

rts_ntc_error_t rts_ntc_init(rts_ntc_t *self, uint16_t const *adc_tab, uint16_t tab_len, int16_t offset_tempX10);
rts_ntc_error_t rts_ntc_adc_to_temp(const rts_ntc_t *self, uint16_t adc_val, int16_t *tempX10);

#endif
#ifdef __cplusplus
}
#endif
#endif
