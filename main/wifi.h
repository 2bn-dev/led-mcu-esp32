#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define WIFI_MAXIMUM_RETRY 10

#define WIFI_SSID "ELEPHANTS!"
#define WIFI_PASSWORD "elephantselephantselephants"


void wifi_init_sta(void);
