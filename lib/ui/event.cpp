#include "event.h"
#include <Arduino.h>

// ซีรีส์สำหรับพล็อตกราฟ Chart
static lv_chart_series_t *ser_adc = NULL;

void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    int *pLED = (int *)lv_event_get_user_data(e);

    // ข้อ c: เมื่อกด onpump_bt ให้เปิด LED ขา 25
    if (obj == objects.onpump_bt && code == LV_EVENT_CLICKED)
    {
        if (pLED != NULL)
        {
            digitalWrite(*pLED, HIGH);
        }
    }
    // ข้อ d: เมื่อกด offpump_bt ให้ปิด LED ขา 25
    else if (obj == objects.offpump_bt && code == LV_EVENT_CLICKED)
    {
        if (pLED != NULL)
        {
            digitalWrite(*pLED, LOW);
        }
    }
    // ข้อ f: เมื่อกด adcpage_bt ให้เปลี่ยนไปยังหน้า ADC Page
    else if (obj == objects.adcpage_bt && code == LV_EVENT_CLICKED)
    {
        #if defined(EEZ_FOR_LVGL)
        eez_flow_push_screen(SCREEN_ID_ADC_PAGE, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0);
        #else
        loadScreen(SCREEN_ID_ADC_PAGE);
        #endif
    }
    // ข้อ g: เมื่อกด home_bt ให้เปลี่ยนกลับมายังหน้า Main
    else if (obj == objects.home_bt && code == LV_EVENT_CLICKED)
    {
        #if defined(EEZ_FOR_LVGL)
        eez_flow_push_screen(SCREEN_ID_MAIN, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0);
        #else
        loadScreen(SCREEN_ID_MAIN);
        #endif
    }
}

// ข้อ a, b: จัดการสัญญาณ PWM ที่ขา 26 (ความละเอียด 10 bit: 0 - 1023)
void pwmcontor(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        // ตรวจสอบว่าสวิตช์ on_off_pwm อยู่ที่สถานะ ON หรือไม่
        if (lv_obj_has_state(objects.on_off_pwm, LV_STATE_CHECKED))
        {
            char buf[32];
            // อ่านค่าเปอร์เซ็นต์ที่เลือกจาก Dropdown pwmpercent
            lv_dropdown_get_selected_str(objects.pwmpercent, buf, sizeof(buf));
            int pwmpercentchoose = String(buf).toInt();

            // คำนวณความกว้างพัลส์ 10-bit (0 - 1023)
            uint32_t pwm = (uint32_t)((pwmpercentchoose / 100.0f) * 1023);
            analogWrite(26, pwm);
        }
        else
        {
            // ข้อ b: สวิตช์เลื่อนไป OFF ให้หยุดสร้าง PWM
            analogWrite(26, 0);
        }
    }
}

// ข้อ e: อ่านสถานะ Pushbutton ขา 14 ทุก 0.1 วินาที (100 ms)
void get_sw_state(lv_timer_t *timer)
{
    int *pSW = (int *)timer->user_data;
    if (pSW == NULL || objects.spray_st == NULL) return;

    int state = digitalRead(*pSW);

    if (state == LOW) // เมื่อกดปุ่ม (ต่อแบบ Pull-up ลง GND)
    {
        lv_led_set_color(objects.spray_st, lv_palette_main(LV_PALETTE_GREEN));
        lv_led_on(objects.spray_st);
    }
    else // เมื่อปล่อยปุ่ม
    {
        lv_led_set_color(objects.spray_st, lv_palette_main(LV_PALETTE_RED));
        lv_led_on(objects.spray_st); // หรือ lv_led_off(objects.spray_st) หากต้องการให้ไฟดับ
    }
}
void update_adc_chart(lv_timer_t *timer)
{
    if (objects.adc33_chart == NULL || objects.adc33val == NULL) return;
    if (ser_adc == NULL)
    {
        lv_chart_set_type(objects.adc33_chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(objects.adc33_chart, 40); 
        lv_chart_set_range(objects.adc33_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 4095); 
        ser_adc = lv_chart_add_series(objects.adc33_chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
    }

    int adc_val = analogRead(33);
    lv_label_set_text_fmt(objects.adc33val, "%d", adc_val);
    lv_chart_set_next_value(objects.adc33_chart, ser_adc, adc_val);
    lv_chart_refresh(objects.adc33_chart);
}