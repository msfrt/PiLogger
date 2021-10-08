#!/bin/bash

# copy service description to system-wide services directory
cp ./socketcan-can0.service /etc/systemd/system/
cp ./socketcan-can1.service /etc/systemd/system/
cp ./socketcan-vcan0.service /etc/systemd/system/

# enable the services to start up on boot
systemctl enable socketcan-can0
systemctl enable socketcan-can1
systemctl enable socketcan-vcan0

