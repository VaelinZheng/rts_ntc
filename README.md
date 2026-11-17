# rts_ntc

[![Build and Test](https://github.com/VaelinZheng/rts_ntc/actions/workflows/build-test.yml/badge.svg)](https://github.com/VaelinZheng/rts_ntc/actions/workflows/build-test.yml)

rts_ntc converts an ADC reading from an NTC thermistor voltage divider into a
temperature. It uses a caller-provided lookup table and linear interpolation
between adjacent entries.

## Configuration

The component is controlled by these Kconfig options:

| Option | Meaning | Default |
| --- | --- | --- |
| `CONFIG_RTS_NTC_ENABLE` | Enable the rts_ntc API and implementation. | `y` |
| `CONFIG_RTS_NTC_IS_NTC_AT_UPPER` | Set to `y` when the NTC is connected to VCC (the upper side of the divider); set to `n` when connected to GND (the lower side). | `n` |

The configuration macros must be available when compiling the component. They
can be supplied as compiler definitions or through a generated header selected
with `KCONFIG_HEADER_FILE`.

## Lookup table

The table is indexed by increasing temperature. Entry `i` represents:

```text
temperature = offset_tempX10 + i * 10
```

Temperatures are in tenths of a degree Celsius. For example, an offset of
`-200` represents `-20.0 °C` at index zero, and each next entry represents a
temperature 1.0 °C higher.

The ADC values must be strictly monotonic and match the selected divider
configuration:

- NTC connected to VCC (`CONFIG_RTS_NTC_IS_NTC_AT_UPPER=y`): ADC values increase
  with temperature.
- NTC connected to GND (`CONFIG_RTS_NTC_IS_NTC_AT_UPPER=n`): ADC values
  decrease with temperature.

The table is not copied by `rts_ntc_init`; it must remain valid for as long as
the initialized `rts_ntc_t` instance is used.

## Real ADC table example

```c
#include <stdio.h>
#include "rts_ntc.h"

/* Configure CONFIG_RTS_NTC_ENABLE=1 and CONFIG_RTS_NTC_IS_NTC_AT_UPPER=0. */
static uint16_t const ntc_adc_tab[] = {
    4072U, // -55 °C
    4070U,
    4068U,
    4066U,
    4064U,
    4061U, // -50 °C
    4059U,
    4056U,
    4053U,
    4050U,
    4047U,
    4044U,
    4040U,
    4037U,
    4033U,
    4028U, // -40 °C
    4024U,
    4019U,
    4014U,
    4009U,
    4004U,
    3998U,
    3992U,
    3986U,
    3979U,
    3972U, // -30 °C
    3965U,
    3957U,
    3949U,
    3940U,
    3932U,
    3922U,
    3913U,
    3902U,
    3892U,
    3880U, // -20 °C
    3869U,
    3857U,
    3844U,
    3831U,
    3817U,
    3803U,
    3788U,
    3772U,
    3756U,
    3739U, // -10 °C
    3722U,
    3703U,
    3685U,
    3665U,
    3645U,
    3624U,
    3603U,
    3580U,
    3557U,
    3534U, // 0 °C
    3509U,
    3484U,
    3458U,
    3431U,
    3404U,
    3375U,
    3346U,
    3317U,
    3286U,
    3257U, // 10 °C
    3223U,
    3191U,
    3158U,
    3124U,
    3089U,
    3054U,
    3018U,
    2982U,
    2945U,
    2907U, // 20 °C
    2869U,
    2831U,
    2792U,
    2752U,
    2713U,
    2672U,
    2632U,
    2591U,
    2550U,
    2509U, // 30 °C
    2467U,
    2426U,
    2384U,
    2342U,
    2300U,
    2259U,
    2217U,
    2175U,
    2134U,
    2092U, // 40 °C
    2051U,
    2010U,
    1969U,
    1928U,
    1888U,
    1848U,
    1808U,
    1769U,
    1730U,
    1687U, // 50 °C
    1654U,
    1616U,
    1579U,
    1543U,
    1507U,
    1471U,
    1437U,
    1402U,
    1368U,
    1335U, // 60 °C
    1303U,
    1270U,
    1239U,
    1208U,
    1178U,
    1148U,
    1119U,
    1091U,
    1063U,
    1036U, // 70 °C
    1009U,
    983U,
    958U,
    933U,
    909U,
    885U,
    862U,
    839U,
    817U,
    796U, // 80 °C
    775U,
    754U,
    734U,
    715U,
    697U,
    678U,
    660U,
    643U,
    625U,
    609U, // 90 °C
    593U,
    577U,
    562U,
    547U,
    533U,
    519U,
    505U,
    492U,
    479U,
    467U, // 100 °C
    454U,
    443U,
    431U,
    420U,
    409U,
    398U,
    388U,
    378U,
    368U,
    359U, // 110 °C
    350U,
    341U,
    332U,
    324U,
    316U,
    308U,
    300U,
    292U,
    285U,
    278U, // 120 °C
    271U,
    264U,
    257U,
    251U,
    245U // 125 °C
};

int main(void)
{
    rts_ntc_t ntc;
    int16_t temperatureX10;
    uint16_t const adc_val = 4071U;

    if (rts_ntc_init(&ntc, ntc_adc_tab,
                     (uint16_t)(sizeof(ntc_adc_tab) / sizeof(ntc_adc_tab[0])),
                     -550) != RTS_NTC_OK)
    {
        return 1;
    }

    if (rts_ntc_adc_to_temp(&ntc, adc_val, &temperatureX10) != RTS_NTC_OK)
    {
        return 1;
    }

    int16_t fractionX10 = temperatureX10 % 10;
    if (fractionX10 < 0)
    {
        fractionX10 = -fractionX10;
    }
    printf("Temperature: %d.%d °C\n", temperatureX10 / 10, fractionX10);
    /* For ADC 4071, temperatureX10 is -545 (-54.5 °C). */
    return 0;
}
```

This table is for an NTC thermistor connected to GND and contains one entry per
degree from -55 °C to 125 °C. With an ADC reading of 4071, linear interpolation
between 4072 and 4070 produces -54.5 °C.

`rts_ntc_init` and `rts_ntc_adc_to_temp` return `RTS_NTC_OK` on success and
`RTS_NTC_ERROR` for invalid arguments or an ADC reading outside the table
range. The conversion result is written to `temperatureX10` in tenths of a
degree Celsius.
