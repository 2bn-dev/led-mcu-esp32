#include "piezo.h"

static const char *TAG = "piezo";


void piezo_init(){
	ESP_LOGI(TAG, "piezo_init()");
	gpio_config_t piezo_conf = {};
	piezo_conf.intr_type = GPIO_INTR_DISABLE;
	piezo_conf.mode = GPIO_MODE_DISABLE;
	piezo_conf.pin_bit_mask = PIN_PIEZO_MASK;
	piezo_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	piezo_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	gpio_config(&piezo_conf);

	gpio_set_pull_mode(PIN_PIEZO, GPIO_PULLDOWN_ONLY);
	gpio_set_drive_capability(PIN_PIEZO, GPIO_DRIVE_CAP_3);
}

void piezo_beep(uint32_t beep_ms){
	ESP_LOGI(TAG, "piezo_beep(%d)", beep_ms);
	bool piezo_en = false;
	uint32_t start_time = esp_log_timestamp();


	for(uint64_t x = 0;; x++){
		if(piezo_en){
			gpio_pulldown_en(PIN_PIEZO);
		}else{
			gpio_pulldown_dis(PIN_PIEZO);
		}

		piezo_en = !piezo_en;
		usleep(PIEZO_PULSE_TIME_US);
		if(esp_log_timestamp()-start_time > beep_ms){
			break;
		}
	}
	gpio_pulldown_dis(PIN_PIEZO);
}
