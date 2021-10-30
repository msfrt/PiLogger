#!/bin/bash

sudo apt-get install -y systemd
sudo apt-get isntall -y python3
sudo apt install -y python3-lgpio

sudo cp ./shutdown_button.service /etc/systemd/system/

sudo systemctl enable shutdown_button
sudo systemctl start shutdown_button









