/*
 * Copyright (c) 2018-2026, vaelin.zheng@gmail.com
 *
 * SPDX-License-Identifier: MIT
*/

#include "rts_ntc.h"
#include <stddef.h>
#if (CONFIG_RTS_NTC_ENABLE)

/**
 * @brief Initialize an NTC conversion instance.
 *
 * @param self         Instance to initialize.
 * @param adc_tab      ADC lookup table indexed by increasing temperature.
 * @param tab_len      Number of entries in the lookup table; must be nonzero.
 * @param offset_tempX10 Temperature at table index zero, in tenths of a degree Celsius.
 * @return RTS_NTC_OK on success; otherwise RTS_NTC_ERROR.
 */
rts_ntc_error_t rts_ntc_init(rts_ntc_t *self, uint16_t const *adc_tab, uint16_t tab_len, int16_t offset_tempX10)
{
    if ((self == NULL) || (adc_tab == NULL) || (tab_len == 0U))
    {
        return RTS_NTC_ERROR;
    }

    self->adc_tab = adc_tab;
    self->tab_item_num = tab_len;
    self->offset_tempX10 = offset_tempX10;

    return RTS_NTC_OK;
}
#if (CONFIG_RTS_NTC_IS_NTC_AT_UPPER)
/**
 * @brief Convert an ADC reading when the NTC thermistor is connected to the
 *        upper side of the voltage divider.
 *
 * @param self     Initialized NTC conversion instance.
 * @param adc_val  ADC reading within the lookup table range.
 * @param tempX10  Receives the temperature in tenths of a degree Celsius.
 * @return RTS_NTC_OK on success; otherwise RTS_NTC_ERROR.
 */
rts_ntc_error_t rts_ntc_adc_to_temp(const rts_ntc_t *self, uint16_t adc_val, int16_t *tempX10)
{
    uint16_t i;
    int16_t temp=0;

    if ((self == NULL) || (self->adc_tab == NULL) || (tempX10 == NULL) || (self->tab_item_num == 0U))
    {
        return RTS_NTC_ERROR;
    }

    if((adc_val > self->adc_tab[self->tab_item_num - 1U]) || (adc_val < self->adc_tab[0]))
    {
        return RTS_NTC_ERROR;
    }

    for(i=0; i<self->tab_item_num; i++)
    {
        if(adc_val < self->adc_tab[i])
        {
            temp = (int16_t)i - 1; // Lower lookup-table index for this interval.
            temp *= 10;
            temp += ((int16_t)adc_val - (int16_t)self->adc_tab[i - 1U]) * 10 / ((int16_t)self->adc_tab[i] - (int16_t)self->adc_tab[i - 1U]); // Interpolate tenths of a degree.

            *tempX10 = temp + self->offset_tempX10;
            return RTS_NTC_OK;
        }
        else if(adc_val == self->adc_tab[i])
        {
            temp = (int16_t)i; // Exact lookup-table index.

            *tempX10 = temp * 10 + self->offset_tempX10;
            return RTS_NTC_OK;
        }
    }

    return RTS_NTC_ERROR;
}
#else
/**
 * @brief Convert an ADC reading when the NTC thermistor is connected to the
 *        lower side of the voltage divider.
 *
 * @param self     Initialized NTC conversion instance.
 * @param adc_val  ADC reading within the lookup table range.
 * @param tempX10  Receives the temperature in tenths of a degree Celsius.
 * @return RTS_NTC_OK on success; otherwise RTS_NTC_ERROR.
 */
rts_ntc_error_t rts_ntc_adc_to_temp(const rts_ntc_t *self, uint16_t adc_val, int16_t *tempX10)
{
    uint16_t i;
    int16_t temp=0;

    if ((self == NULL) || (self->adc_tab == NULL) || (tempX10 == NULL) || (self->tab_item_num == 0U))
    {
        return RTS_NTC_ERROR;
    }

    if((adc_val > self->adc_tab[0]) || (adc_val < self->adc_tab[self->tab_item_num - 1U]))
    {
        return RTS_NTC_ERROR;
    }

    for(i=0; i<self->tab_item_num; i++)
    {
        if(adc_val > self->adc_tab[i])
        {
            temp = (int16_t)i - 1; // Lower lookup-table index for this interval.
            temp *= 10;
            temp += ((int16_t)self->adc_tab[i - 1U] - (int16_t)adc_val) * 10 / ((int16_t)self->adc_tab[i - 1U] - (int16_t)self->adc_tab[i]); // Interpolate tenths of a degree.

            *tempX10 = temp + self->offset_tempX10;
            return RTS_NTC_OK;
        }
        else if(adc_val == self->adc_tab[i])
        {
            temp = (int16_t)i; // Exact lookup-table index.

            *tempX10 = temp * 10 + self->offset_tempX10;
            return RTS_NTC_OK;
        }
    }

    return RTS_NTC_ERROR;
}
#endif
#endif
