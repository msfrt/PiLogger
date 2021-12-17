#!/bin/bash

# copy service description to system-wide services directory
cp ./PiLogger.service /etc/systemd/system/

# enable the services to start up on boot
systemctl enable PiLogger

