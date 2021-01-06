#!/bin/bash

# copy service description to system-wide services directory
cp ./socketcan-vcan0.service /etc/systemd/system/

# enable the services to start up on boot
systemctl enable socketcan-vcan0

