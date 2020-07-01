#include "mgos.h"
#include "mgos_zbutton_gpio.h"
#include "mgos_zbutton_mqtt.h"

void mg_btn_on_event_cb(int ev, void *ev_data, void *ud) {
  struct mgos_zbutton *handle = (struct mgos_zbutton *)ev_data;
  if (handle) {
    if (ev == MGOS_EV_ZBUTTON_ON_CLICK) {
      LOG(LL_INFO, ("Button '%s' CLICKED", handle->id));
    } else if (ev == MGOS_EV_ZBUTTON_ON_DBLCLICK) {
      LOG(LL_INFO, ("Button '%s' DOUBLE-CLICKED", handle->id));
    } else if (ev == MGOS_EV_ZBUTTON_ON_PRESS) {
      LOG(LL_INFO, ("Button '%s' PRESSED (#%d)", handle->id,
        mgos_zbutton_press_counter_get(handle)));
    }
  }
  (void) ud;
}

enum mgos_app_init_result mgos_app_init(void) {
  struct mgos_zbutton_cfg cfg = MGOS_ZBUTTON_CFG;
  struct mgos_zbutton *btn1 = mgos_zbutton_create(mgos_sys_config_get_app_btn1_id(), &cfg);
  if (btn1) {
    struct mgos_zbutton_gpio_cfg gpio_cfg = MGOS_ZBUTTON_GPIO_CFG;
    gpio_cfg.active_high = mgos_sys_config_get_app_btn1_gpio_active_high();

    if (mgos_zbutton_gpio_attach(btn1, mgos_sys_config_get_app_btn1_gpio_pin(), &gpio_cfg)) {
      if (mgos_zbutton_mqtt_attach(btn1, mgos_sys_config_get_app_mqtt_event_topic(), NULL)) {
        mgos_event_add_handler(MGOS_EV_ZBUTTON_ON_CLICK, mg_btn_on_event_cb, NULL);
        mgos_event_add_handler(MGOS_EV_ZBUTTON_ON_DBLCLICK, mg_btn_on_event_cb, NULL);
        mgos_event_add_handler(MGOS_EV_ZBUTTON_ON_PRESS, mg_btn_on_event_cb, NULL); 
        return MGOS_APP_INIT_SUCCESS;
      }
      mgos_zbutton_gpio_detach(btn1);
    }
    mgos_zbutton_close(btn1);
  }

  return MGOS_APP_INIT_ERROR;
}