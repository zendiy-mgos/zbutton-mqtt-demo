load("api_config.js")
load("api_events.js")
load("api_zbutton_gpio.js")
load("api_zbutton_mqtt.js")

function onBtnEvent(ev, evdata, ud) {
  let btn = ZenThing.getFromHandle(evdata);
  if (ev === ZenButton.EV_ON_CLICK) {
    print("Button", btn.id, "CLICKED");
  } else if (ev === ZenButton.EV_ON_DBLCLICK) {
    print("Button", btn.id, "DOUBLE-CLICKED");
  } else if (ev === ZenButton.EV_ON_PRESS) {
    print("Button", btn.id, "PRESSED", btn.getPressCounter());
  } else if (ev === ZenButton.EV_ON_PRESS_END) {
    print("Button", btn.id, "RELESED after", btn.getPressDuration(), "(ms)");
  }
}

/* Create button using defualt configuration. */   
let btn1 = ZenButton.create(Cfg.get('app.btn1.id'));

let success = false;
if (btn1) {
  let gpioCfg = {activeHigh: Cfg.get('app.btn1.gpio.active_high')};
  if (btn1.GPIO.attach(Cfg.get('app.btn1.gpio.pin'), gpioCfg)) {
    if (btn1.MQTT.attach(Cfg.get('app.mqtt.event_topic'))) {
      Event.addGroupHandler(ZenButton.EV_ON_ANY, onBtnEvent, null);
      success = true;
    }
  }
}

if (!success) {
  if (btn1) {
    btn1.GPIO.detach();
    btn1.close();
  }
  print('Error initializing the firmware');
}
