/*
 * Copyright (c) 2018-2022, Xiaojin.Zheng
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date             Author                  Notes
 * 2022-06-11       Xiaojin.Zheng           Frist add
*/

#include "rts_ntc.h"
#if (RTS_NTC_ENABLE)
#include "stdint.h"
#include "stdio.h"

static uint16_t const *_adc_tab=0;
static uint16_t _tab_item_num=0;
static int16_t _offset_tempX10=0;

/*
 * adc_tab : 温度对应adc数值数组，从大排到小;数组下标表示温度
 * tab_item_num : adc_tab数组的长度
 * offset_tempX10 : 开始的温度
 */
void rts_ntc_init(uint16_t const *adc_tab,uint16_t tab_item_num,int16_t offset_tempX10)
{
    _adc_tab=adc_tab;
    _tab_item_num=tab_item_num;
    _offset_tempX10=offset_tempX10;
}

/*
 * adc_val : adc value
 * tempX10 : output temp x 10
 * return : RTS_NTC_OK or RTS_NTC_ERROR
 */
rts_ntc_error_t rts_ntc_adc_to_temp(uint16_t adc_val,int16_t *tempX10)
{
    uint16_t i;
    int16_t temp=0;

    if((adc_val>_adc_tab[0]) || (adc_val<_adc_tab[_tab_item_num-1]))
    {
        return RTS_NTC_ERROR;
    }

    for(i=0;i<_tab_item_num;i++)
    {
        if(adc_val>_adc_tab[i])
        {
            temp=i-1; //温度整数部分
            temp *=10;
            temp += ((_adc_tab[i-1]-adc_val)*10)/(_adc_tab[i-1]-_adc_tab[i]);//小数部分

            *tempX10=temp+_offset_tempX10;
            return RTS_NTC_OK;
        }
        else if(adc_val==_adc_tab[i])
        {
            temp=i; //温度整数部分

            *tempX10=temp*10+_offset_tempX10;
            return RTS_NTC_OK;
        }
    }

    return RTS_NTC_ERROR;
}

#endif
