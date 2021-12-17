# dave yonkers, 2021

import time
import lgpio as sbc
import os

chip = 0  # GPIO chip
gpio = 3  # button is connected to GPIO
debounce_micros = 100000

# open the gpio chip.
# gets gpio chip's handle upon success
handle = sbc.gpiochip_open(chip)  
if handle < 0:
    print(f"Couldn't open gpio chip {chip} ({sbc.error_text(handle)})")
    exit()


# register the gpio for alerts (edge detection)
err = sbc.gpio_claim_alert(handle, gpio, sbc.FALLING_EDGE)
if err < 0:
    print("GPIO in use {}:{} ({})".format(
        chip, gpio, sbc.error_text(err)))
    exit()


# set the debounce time in microseconds so that the button is only pressed
# once when it's actually pressed once
sbc.gpio_set_debounce_micros(handle, gpio, debounce_micros)

# callback function called when edge is detected
def shutdown_callback(chip, gpio, level, timestamp):
    os.system('systemctl poweroff')  # doesn't require sudo


# register the callback function and attach it to the gpio pin
cb1 = sbc.callback(0, gpio, sbc.FALLING_EDGE, shutdown_callback)
print(cb1)

# sleep to avoid busy-waiting, but still keep the program running
try:
    while True:
        time.sleep(10)
except KeyboardInterrupt:
    print(cb1.tally())
    cb1.cancel()
