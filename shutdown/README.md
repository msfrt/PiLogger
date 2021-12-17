# Pi Safe Shutdown Button

This installs a safe shutdown for a system using Ubuntu 21.04+

## Requirements:

* python3
* lgpio (`sudo apt install python3-lgpio`)
* systemd (`sudo apt-get install -y systemd`)

## Installation

Simply cd into the directory of this readme, then run `sudo ./install.sh`. This will
install the service and start it.

## Usage

Connect GPIO3 to ground (presumably through a button) on a raspberry pi to enact the shutdown. Most Pis from the factory use the same GPIO pin behavior to wake the Pi after it's been shut down. This is great - a multi purpose button! On and off!

If you want to modify the service for any reason, it's called `shutdown_button`. To view the status of the button, simply run `sudo systemctl status shutdown_button`.
