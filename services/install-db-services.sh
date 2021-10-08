#!/bin/bash

# copy service description to system-wide services directory
cp ./influxdb2.service /etc/systemd/system/

# enable the services to start up on boot
systemctl enable influxdb2

