#!/bin/bash

# make a folder in the etc directory to store the configuration file
sudo mkdir /etc/PiLogger

# disable the logger if it exists
sudo systemctl stop PiLogger

# move the config file to the etc directory and move the binary file to the bin directory
sudo cp src/build/logger.conf /etc/PiLogger/
sudo cp src/build/logger /bin/

# copy service description to system-wide services directory
cp PiLogger.service /etc/systemd/system/

# enable the services to start up on boot
sudo systemctl enable PiLogger
sudo systemctl start PiLogger


