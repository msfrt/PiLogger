#!/bin/bash

sudo systemctl stop shutdown_button
sudo systemctl disable shutdown_button
sudo rm /etc/systemd/system/shutdown_button.service









