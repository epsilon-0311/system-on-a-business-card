#include <stdio.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <inttypes.h>


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define BTN_START_NODE	DT_ALIAS(btn_start)
#if !DT_NODE_HAS_STATUS(BTN_START_NODE, okay)
	#error "Unsupported board: btn_start devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_start = GPIO_DT_SPEC_GET_OR(BTN_START_NODE, gpios, {0});
static struct gpio_callback button_start_cb_data;

#define BTN_SELECT_NODE	DT_ALIAS(btn_select)
#if !DT_NODE_HAS_STATUS(BTN_SELECT_NODE, okay)
	#error "Unsupported board: btn_select devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_select = GPIO_DT_SPEC_GET_OR(BTN_SELECT_NODE, gpios, {0});
static struct gpio_callback button_select_cb_data;

#define BTN_UP_NODE	DT_ALIAS(btn_up)
#if !DT_NODE_HAS_STATUS(BTN_UP_NODE, okay)
	#error "Unsupported board: btn_up devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_up = GPIO_DT_SPEC_GET_OR(BTN_UP_NODE, gpios, {0});
static struct gpio_callback button_up_cb_data;

#define BTN_DOWN_NODE	DT_ALIAS(btn_down)
#if !DT_NODE_HAS_STATUS(BTN_DOWN_NODE, okay)
	#error "Unsupported board: btn_down devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_down = GPIO_DT_SPEC_GET_OR(BTN_DOWN_NODE, gpios, {0});
static struct gpio_callback button_down_cb_data;

#define BTN_LEFT_NODE	DT_ALIAS(btn_left)
#if !DT_NODE_HAS_STATUS(BTN_LEFT_NODE, okay)
	#error "Unsupported board: btn_left devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_left = GPIO_DT_SPEC_GET_OR(BTN_LEFT_NODE, gpios, {0});
static struct gpio_callback button_left_cb_data;

#define BTN_RIGHT_NODE	DT_ALIAS(btn_right)
#if !DT_NODE_HAS_STATUS(BTN_RIGHT_NODE, okay)
	#error "Unsupported board: btn_right devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_right = GPIO_DT_SPEC_GET_OR(BTN_RIGHT_NODE, gpios, {0});
static struct gpio_callback button_right_cb_data;

#define BTN_A_NODE	DT_ALIAS(btn_a)
#if !DT_NODE_HAS_STATUS(BTN_A_NODE, okay)
	#error "Unsupported board: btn_a devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_a = GPIO_DT_SPEC_GET_OR(BTN_A_NODE, gpios, {0});
static struct gpio_callback button_a_cb_data;

#define BTN_B_NODE	DT_ALIAS(btn_b)
#if !DT_NODE_HAS_STATUS(BTN_START_NODE, okay)
	#error "Unsupported board: btn_b devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_b = GPIO_DT_SPEC_GET_OR(BTN_B_NODE, gpios, {0});
static struct gpio_callback button_b_cb_data;

#define BTN_X_NODE	DT_ALIAS(btn_x)
#if !DT_NODE_HAS_STATUS(BTN_X_NODE, okay)
	#error "Unsupported board: btn_x devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_x = GPIO_DT_SPEC_GET_OR(BTN_X_NODE, gpios, {0});
static struct gpio_callback button_x_cb_data;

#define BTN_Y_NODE	DT_ALIAS(btn_y)
#if !DT_NODE_HAS_STATUS(BTN_Y_NODE, okay)
	#error "Unsupported board: btn_y devicetree alias is not defined"
#endif
static const struct gpio_dt_spec btn_y = GPIO_DT_SPEC_GET_OR(BTN_Y_NODE, gpios, {0});
static struct gpio_callback button_y_cb_data;

void btn_changed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    int start = gpio_pin_get(btn_start.port, btn_start.pin);
    int select = gpio_pin_get(btn_select.port, btn_select.pin);
    int up = gpio_pin_get(btn_up.port, btn_up.pin);
    int down = gpio_pin_get(btn_down.port, btn_down.pin);
    int left = gpio_pin_get(btn_left.port, btn_left.pin);
    int right = gpio_pin_get(btn_right.port, btn_right.pin);
    int a = gpio_pin_get(btn_a.port, btn_a.pin);
    int b = gpio_pin_get(btn_b.port, btn_b.pin);
    int x = gpio_pin_get(btn_x.port, btn_x.pin);
    int y = gpio_pin_get(btn_y.port, btn_y.pin);

	LOG_DBG("Intr: Start:%i Select:%i Up:%i Down:%i Left:%i Right:%i A:%i B:%i X:%i Y:%i\n", start, select, up, down, left, right, a, b, x, y);
}


int init_button(const struct gpio_dt_spec *btn, struct gpio_callback *btn_cb_data)
{
	int ret=device_is_ready(btn->port);
	if (!ret) {
		LOG_ERR("Error: btn device %s is not ready\n", btn->port->name);
		return ret;
	}

	ret = gpio_pin_configure_dt(btn, GPIO_INPUT);
	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure %s pin %d\n", ret, btn->port->name, btn->pin);
		return ret;
	}
	
	ret = gpio_pin_interrupt_configure_dt(btn, GPIO_INT_EDGE_BOTH);
	if (ret != 0) {
		LOG_ERR("Error %d: failed to configure interrupt on %s pin %d\n", ret, btn->port->name, btn_start.pin);
		return ret;
	}

	gpio_init_callback(btn_cb_data, btn_changed, BIT(btn->pin));
	gpio_add_callback(btn->port, btn_cb_data);

	return 0;
}

int init_buttons(void)
{
    int ret = init_button(&btn_start, &button_start_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_start initialisation error\n", btn_start.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_start at %s pin %d successfull\n", btn_start.port->name, btn_start.pin);


	ret = init_button(&btn_select, &button_select_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_select initialisation error\n", btn_select.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_select at %s pin %d successfull\n", btn_select.port->name, btn_select.pin);


	ret = init_button(&btn_up, &button_up_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_up initialisation error\n", btn_up.port->name);
		return ret; 
	}
	LOG_DBG("Set up btn_upt at %s pin %d successfull\n", btn_up.port->name, btn_up.pin);


	ret = init_button(&btn_down, &button_down_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_down initialisation error\n", btn_down.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_down at %s pin %d successfull\n", btn_down.port->name, btn_down.pin);


	ret = init_button(&btn_left, &button_left_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_left initialisation error\n", btn_left.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_left at %s pin %d successfull\n", btn_left.port->name, btn_left.pin);


	ret = init_button(&btn_right, &button_right_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_right initialisation error\n", btn_right.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_right at %s pin %d successfull\n", btn_right.port->name, btn_right.pin);


	ret = init_button(&btn_a, &button_a_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_a initialisation error\n", btn_a.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_a at %s pin %d successfull\n", btn_a.port->name, btn_a.pin);


	ret = init_button(&btn_b, &button_b_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_b initialisation error\n", btn_b.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_b at %s pin %d successfull\n", btn_b.port->name, btn_b.pin);

	ret = init_button(&btn_x, &button_x_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_x initialisation error\n", btn_x.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_x at %s pin %d successfull\n", btn_x.port->name, btn_x.pin);


	ret = init_button(&btn_y, &button_y_cb_data);
	if(ret != 0)
	{
		LOG_ERR("Error: btn device %s, btn_y initialisation error\n", btn_y.port->name);
		return ret;
	}
	LOG_DBG("Set up btn_y at %s pin %d successfull\n", btn_y.port->name, btn_y.pin);

    return 0;
}