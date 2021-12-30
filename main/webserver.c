
#include "webserver.h"


static const char *TAG = "webserver";



static esp_err_t hello_get_handler(httpd_req_t *req){
	char*  buf;
	size_t buf_len;

	/*
	 * Read URL query string length and allocate memory for length + 1,
	 * extra byte for null termination 
	 *
	 */

	buf_len = httpd_req_get_url_query_len(req) + 1;

	if (buf_len > 1) {
		buf = malloc(buf_len);
		if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
			ESP_LOGI(TAG, "Found URL query => %s", buf);
		}

		free(buf);
	}

	const char* resp_str = (const char*) req->user_ctx;
	httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

	/*
	bool piezo_en = false;

	for(uint x = 0; x < 2700/5; x++){
		if(piezo_en){
			gpio_pulldown_en(PIN_PIEZO);
		}else{
			gpio_pulldown_dis(PIN_PIEZO);
		}

		piezo_en = !piezo_en;
		usleep(1000000/2700);
	}
	gpio_pulldown_dis(PIN_PIEZO);
	*/

	return ESP_OK;
}

static const httpd_uri_t hello = {
	.uri       = "/hello",
	.method    = HTTP_GET,
	.handler   = hello_get_handler,
};

static httpd_handle_t start_webserver(void){
	httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable = true;

	// Start the httpd server
	ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
	if (httpd_start(&server, &config) == ESP_OK) {
		// Set URI handlers
		ESP_LOGI(TAG, "Registering URI handlers");
		httpd_register_uri_handler(server, &hello);
		return server;
	}

	ESP_LOGI(TAG, "Error starting server!");

	return NULL;
}

static void stop_webserver(httpd_handle_t server){
	// Stop the httpd server
	httpd_stop(server);
}

static void disconnect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
	httpd_handle_t* server = (httpd_handle_t*) arg;
	if (*server){
		ESP_LOGI(TAG, "Stopping webserver");
		stop_webserver(*server);
		*server = NULL;
	}
}

static void connect_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
	httpd_handle_t* server = (httpd_handle_t*) arg;
	if (*server == NULL) {
		ESP_LOGI(TAG, "Starting webserver");
		*server = start_webserver();
	}
}


void webserver_init(void){
	static httpd_handle_t server = NULL;


	/* 
	 * This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
	 * Read "Establishing Wi-Fi or Ethernet Connection" section in
	 * examples/protocols/README.md for more information about this function.
	 *
	 */

	/* 
	 * Register event handlers to stop the server when Wi-Fi or Ethernet is disconnected,
	 * and re-start it upon connection.
	 *
	 */

	ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
	ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));

	/* Start the server for the first time */
	server = start_webserver();
}
