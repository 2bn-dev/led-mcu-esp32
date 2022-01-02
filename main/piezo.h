#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <unistd.h>

#define PIN_PIEZO 23
#define PIN_PIEZO_MASK (1ULL<<PIN_PIEZO)

#define PIEZO_FREQUENCY 2700 //hz

#define ONE_SECOND_IN_MS 1*1000
#define ONE_SECOND_IN_US ONE_SECOND_IN_MS*1000

#define PIEZO_CYCLE_TIME_US ONE_SECOND_IN_US/PIEZO_FREQUENCY
#define PIEZO_PULSE_TIME_US PIEZO_CYCLE_TIME_US/2 //50% duty cycle



void piezo_init();
void piezo_beep(uint32_t beep_ms);
