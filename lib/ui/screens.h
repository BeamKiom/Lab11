#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_ADC_PAGE = 2,
    _SCREEN_ID_LAST = 2
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *adc_page;
    lv_obj_t *pwmpercent;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *spray_st;
    lv_obj_t *obj2;
    lv_obj_t *on_off_pwm;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *onpump_bt;
    lv_obj_t *offpump_bt;
    lv_obj_t *adcpage_bt;
    lv_obj_t *adc33_chart;
    lv_obj_t *home_bt;
    lv_obj_t *adc33val;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void create_screen_adc_page();
void tick_screen_adc_page();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/