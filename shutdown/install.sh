#!/bin/bash

sudo apt-get install -y systemd
sudo apt-get install -y python3
sudo apt install -y python3-lgpio

sudo cp ./shutdown_button.service /etc/systemd/system/
sudo cp ./shutdown.py /etc/

sudo systemctl enable shutdown_button
sudo systemctl start shutdown_button









