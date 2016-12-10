/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#define pr_fmt(fmt) "%s:%d "fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/wakelock.h>
#include "msm_led_flash.h"

static struct wake_lock torch_wakelock;
static uint8_t torch_enable;
static struct led_trigger *torch_trigger;
//Begin modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.
static struct led_trigger *flash_trigger;
//End modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.

static void msm_led_torch_brightness_set(struct led_classdev *led_cdev,
				enum led_brightness value)
{
/*Begin modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.
	if (!torch_trigger) {
		pr_err("No torch trigger found, can't set brightness\n");
		return;
	}

	led_trigger_event(torch_trigger, value);
*/
	pr_err("value:%d, torch_enable:%u\n", value, torch_enable);
    if (torch_trigger)
        led_trigger_event(torch_trigger, value);
    else if (flash_trigger)
        led_trigger_event(flash_trigger, (value < LED_HALF) ? value : LED_HALF);

	if (value && !torch_enable) {
		wake_lock(&torch_wakelock);
		torch_enable = 1;
	} else if (!value) {
		wake_unlock(&torch_wakelock);
		torch_enable = 0;
	}
//End modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.
};

static struct led_classdev msm_torch_led = {
	.name			= "torch-light",
	.brightness_set	= msm_led_torch_brightness_set,
	.brightness		= LED_OFF,
};

int32_t msm_led_torch_create_classdev(struct platform_device *pdev,
				void *data)
{
	int rc;
	struct msm_led_flash_ctrl_t *fctrl =
		(struct msm_led_flash_ctrl_t *)data;
/*Begin modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.
	if (!fctrl || !fctrl->torch_trigger) {
		pr_err("Invalid fctrl or torch trigger\n");
		return -EINVAL;
	}

	torch_trigger = fctrl->torch_trigger;
	msm_led_torch_brightness_set(&msm_torch_led, LED_OFF);
*/
    if (!fctrl || (!fctrl->torch_trigger && !fctrl->flash_trigger[0]))
    {
        pr_err("Invalid fctrl or led flash\n");
        return -EINVAL;
    }
    if (fctrl->torch_trigger)
    {
        torch_trigger = fctrl->torch_trigger;
        msm_led_torch_brightness_set(&msm_torch_led, LED_OFF);
    } else
        flash_trigger = fctrl->flash_trigger[0];

	wake_lock_init(&torch_wakelock, WAKE_LOCK_SUSPEND, "torch_wakelock");
	torch_enable = 0;
//End modify by weicai.long@tcl.com for use LED flash as torch, 2013/11/06.
	rc = led_classdev_register(&pdev->dev, &msm_torch_led);
	if (rc) {
		pr_err("Failed to register led dev. rc = %d\n", rc);
		return rc;
	}

	return 0;
};
