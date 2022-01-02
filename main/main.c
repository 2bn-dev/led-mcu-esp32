#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "nvs_flash.h"
//#include "esp_netif.h"
//#include "esp_eth.h"
//#include "protocol_examples_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "wifi.h"
#include "webserver.h"
#include "piezo.h"


#define PIN_PIEZO 23
#define PIN_PIEZO_MASK (1ULL<<PIN_PIEZO)

static const char *TAG = "main-esp32";

const uart_port_t uart_num = UART_NUM_1;


static void uart_init(){
	uart_config_t uart_config = {
	    .baud_rate = 115200,
	    .data_bits = UART_DATA_8_BITS,
	    .parity = UART_PARITY_DISABLE,
	    .stop_bits = UART_STOP_BITS_1,
	    .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
	    .rx_flow_ctrl_thresh = 122,
	};
	// Configure UART parameters
	ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
}

void app_main(void){
	uart_init();
	piezo_init();
	piezo_beep(50);
	usleep(50000);
	piezo_beep(50);

	//Initialize NVS
	ESP_ERROR_CHECK(nvs_flash_init());
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();
	webserver_init();
}
