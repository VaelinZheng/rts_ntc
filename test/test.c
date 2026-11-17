#include "unity.h"
#include "rts_ntc.h"
#include <stddef.h>

#if CONFIG_RTS_NTC_IS_NTC_AT_UPPER
static uint16_t const adc_tab[] = {100U, 200U, 300U};
#else
static uint16_t const adc_tab[] = {300U, 200U, 100U};
#endif

void setUp(void)
{
}

void tearDown(void)
{
}

void test_init_and_conversion(void)
{
    rts_ntc_t ntc;
    int16_t temperature = 0;

    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_init(&ntc, adc_tab, 3U, -200));
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_adc_to_temp(&ntc, adc_tab[0], &temperature));
    TEST_ASSERT_EQUAL_INT16(-200, temperature);
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_adc_to_temp(&ntc, adc_tab[1], &temperature));
    TEST_ASSERT_EQUAL_INT16(-190, temperature);
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_adc_to_temp(&ntc, adc_tab[2], &temperature));
    TEST_ASSERT_EQUAL_INT16(-180, temperature);

#if CONFIG_RTS_NTC_IS_NTC_AT_UPPER
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_adc_to_temp(&ntc, 150U, &temperature));
#else
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_adc_to_temp(&ntc, 250U, &temperature));
#endif
    TEST_ASSERT_EQUAL_INT16(-195, temperature);
}

void test_invalid_arguments(void)
{
    rts_ntc_t ntc = {NULL, 0U, 0};
    int16_t temperature = 0;

    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_init(NULL, adc_tab, 3U, 0));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_init(&ntc, NULL, 3U, 0));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_init(&ntc, adc_tab, 0U, 0));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_adc_to_temp(NULL, adc_tab[0], &temperature));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_adc_to_temp(&ntc, adc_tab[0], &temperature));
    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_init(&ntc, adc_tab, 3U, 0));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_adc_to_temp(&ntc, adc_tab[0], NULL));
}

void test_out_of_range(void)
{
    rts_ntc_t ntc;
    int16_t temperature = 123;

    TEST_ASSERT_EQUAL(RTS_NTC_OK, rts_ntc_init(&ntc, adc_tab, 3U, -200));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_adc_to_temp(&ntc, 99U, &temperature));
    TEST_ASSERT_EQUAL(RTS_NTC_ERROR, rts_ntc_adc_to_temp(&ntc, 301U, &temperature));
    TEST_ASSERT_EQUAL_INT16(123, temperature);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_init_and_conversion);
    RUN_TEST(test_invalid_arguments);
    RUN_TEST(test_out_of_range);
    return UNITY_END();
}
