#!/bin/bash

# this file adds the appropriate files for your system to automatically
# bring up the CAN interfaces

# the can.conf files loads the can and can_raw kernal modules on start
sudo cp can.conf /etc/modules-load.d/

# the network interfaces will be managed by system-networkd
sudo systemctl start systemd-networkd
sudo systemctl enable systemd-networkd

# place network config files in the proper folder
sudo cp 80-can0.network /etc/systemd/network/
sudo cp 80-can1.network /etc/systemd/network/

# finally reboot the network manager
sudo systemctl restart systemd-networkd


