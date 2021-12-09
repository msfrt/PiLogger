# PiLogger <!-- omit in toc -->

- [Hardware setup](#hardware-setup)
  - [Installing Ubuntu](#installing-ubuntu)
    - [Updating network information after installation](#updating-network-information-after-installation)
    - [Updating username and hostname after installation](#updating-username-and-hostname-after-installation)
  - [SSH keygen and key swap](#ssh-keygen-and-key-swap)
  - [i2c Setup](#i2c-setup)
    - [Sixfab Power API](#sixfab-power-api)
  - [Install Telegraf](#install-telegraf)
  - [CAN Hat setup](#can-hat-setup)
- [SocketCAN](#socketcan)
  - [Installing](#installing)
  - [Useful commands](#useful-commands)
      - [Bring up a virtual CAN interface](#bring-up-a-virtual-can-interface)
      - [Bring up a hardware CAN interface](#bring-up-a-hardware-can-interface)
      - [Automatically add virtual/physical CAN interface upon system startup](#automatically-add-virtualphysical-can-interface-upon-system-startup)
      - [Sending and viewing CAN messages](#sending-and-viewing-can-messages)
- [InfluxDB](#influxdb)
  - [Installation](#installation)
    - [Creating an InfluxDB service](#creating-an-influxdb-service)


# Hardware setup

## Installing Ubuntu

There are many ways to succeed at installing Ubuntu Server. This application was built on Ubuntu Server 21.04, so check yourself with earlier versions. I don't need to explain how to do this. You can read [this tutorial](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview) instead. Just note that the WiFi section is a bit outdated. In the YAML file, I believe that you will need to escape the stuff in quotations. For example, instead of wifi name `"Formula House"`, you would put `\"Formula House\"`.

### Updating network information after installation

I updated the WiFi information after reading [his article](https://linuxconfig.org/ubuntu-20-04-connect-to-wifi-from-command-line). Please note that this time, the quotations are *not* escaped. This is **actual yaml**.

### Updating username and hostname after installation

Again, so easy. Google really is amazing. [This post](https://askubuntu.com/questions/34074/how-do-i-change-my-username) showed me how to change the username from ubuntu to whatever I wanted, and [this article](https://www.cyberciti.biz/faq/ubuntu-change-hostname-command/) told me how to change the hostname. Please note, you must create a temporary user when changing the username (which is explained in the post I linked).

## SSH keygen and key swap

To log into the pi without the use of a password, you can exchange ssh keys. Do to so, make sure that you can an ssh key created by running `ssh-keygen`. You can hit enter to use all of the default options. This will create an ssh key. You should do this on both the server and your local PC (I think - it doesn't hurt).

Then, from your local Mac or Linux machine (not connected to the pi), you need to copy your local key to the server. To do this, you can type the command `ssh-copy-id <user>@<server>`. You should now be able to ssh without using a password.

On Windows, you must do this **in Windows PowerShell** instead to copy the ID: `type $env:USERPROFILE\.ssh\id_rsa.pub | ssh {IP-ADDRESS-OR-FQDN} "cat >> .ssh/authorized_keys"`. Of course, replace `{IP-ADDRESS-OR-FQDN}` as appropriate.

## i2c Setup

To interface with the Sixfab Power HAT, i2c is used. The tools used to communicate with it are diabled by default in Ubuntu. To enable it, run the following commands:
1. `sudo apt update`
2. `sudo apt upgrade -y`
3. `sudo apt install -y i2c-tools python3-pip`

Then, add the line `dtparam=i2c_arm=on` in the file `/boot/config.txt` (you may need to create the file if it is not there)

By default, you must access i2c using sudo, otherwise your prompts will fail. I would reccomend adding yourself to the i2c user group as explained in [this wonderful post](https://askubuntu.com/a/1273900).

1. `sudo groupadd i2c` (group may exist already)
2. `sudo chown :i2c /dev/i2c-1` (or `i2c-0`)
3. `sudo chmod g+rw /dev/i2c-1`
4. `sudo usermod -aG i2c *INSERT YOUR USERNAME*`

Log out, then log back in. You should be able to run `i2cdetect -y 1` without sudo.

### Sixfab Power API

Install the api that interfaces (*sometimes, only if you're lucky*) with the battery UPS. `pip3 install sixfab-power`. The GitHub repository for the API is located [here](https://github.com/sixfab/sixfab-power-python-api). The API is essentially a Python wrapper for their i2c communication protocol.

## Install Telegraf

Telegraf is a program from the makers of InfluxDB that writes usage metrics to an influx instance. This is very useful to have on the raspberry pi. The installation instructs are on there website, [here](https://docs.influxdata.com/telegraf/v1.20/introduction/installation/), but the steps to install from the package manager are below:

1. `wget -qO- https://repos.influxdata.com/influxdb.key | sudo tee /etc/apt/trusted.gpg.d/influxdb.asc >/dev/null
source /etc/os-release`
1. `echo "deb https://repos.influxdata.com/${ID} ${VERSION_CODENAME} stable" | sudo tee /etc/apt/sources.list.d/influxdb.list`
1. `sudo apt-get update && sudo apt-get install telegraf`

After doing that, make sure that Telegraf runs during startup by enabling the service with `sudo systemctl enable telegraf`

What's nice about telegraf is that everything about how it operates is defined within a single configuration file located in the `/etc/telegraf` folder. You can edit the `telegraf.conf` as you please, but for a Raspberry Pi, I'd suggest copying the one in the `telegraf` folder of this repository. Review and edit the information in the influxdb configuration section, then copy it to `/etc/telegraf/telegraf.conf`. Make sure you have a valid token from influx that telegraf can use to write to the bucket specified in the config file. Then, after everything is in the config file, run `systemctl reload telegraf`.

## CAN Hat setup

To setup the [PiCAN2 Duo](https://copperhilltech.com/pican2-duo-can-bus-board-for-raspberry-pi/) CAN hat on Ubuntu Server, you can follow the instructions on [CopperHill's website](https://copperhilltech.com/blog/pican2-pican3-and-picanm-driver-installation-for-raspberry-pi/) with the one exception that instead of adding the overlays in `/boot/config.txt` you should pop the SD card out, load it into another computer, and edit the `usercfg.txt` file instead.

# SocketCAN

## Installing

```bash
sudo apt-get install can-utils
```

## Useful commands


#### Bring up a virtual CAN interface

```bash
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```
Verify that the interface is up:

```bash
ifconfig
```

#### Bring up a hardware CAN interface

```bash
sudo /sbin/ip link set can0 up type can bitrate 1000000
sudo /sbin/ip link set can1 up type can bitrate 1000000
```

#### Automatically add virtual/physical CAN interface upon system startup

The commands required to bring up a SocketCAN interface have been made into service descriptions, located in the `services` folder. To install the services on a fresh Ubuntu installation, simply run `sudo install-can-services.sh` from the `services` directory in this repository. This will copy the service files to the system directory and enable them to start upon boot.

To reload an interface, simply run `sudo systemctl reload socketcan-vcan0.service`. To manually stop the CAN interface, you can run `sudo systemctl stop socketcan-vcan0.service`. Please note that once you stop the service, you can't bring up the interface again with `sudo systemctl start socketcan-vcan0.service`, since the start script creates the interface files that were already exist from system boot. If you accidentally stop the interface, you can manually bring it back up with `ifconfig vcan0 up`.

#### Sending and viewing CAN messages

Sending and viewing is easy. To send a message, use the format `cansend <interface> <ID>#<DATA>`. Note: The ID and data fields are in hexedecimal.

```bash
cansend vcan0 123#DEADBEEF
```

To view messages on the bus, use `candump <interface>`.

```bash
candump vcan0
```

If looking for a particular message on a busy bus, you can combine `candump` with `grep` to filter out any unwanted messages.

```bash
candump vcan0 | grep 123
```

# InfluxDB

An Influx installation is only required for the server. If you are simply setting up a vehicle Pi, you can skip Influx installation.

[InfluxDB v2.0](https://influxdata.com) is the timeseries database used to store all data. It is vital that this runs, otherwise nothing will work

## Installation

Installation instructions are pretty straightforward [on their website](https://docs.influxdata.com/influxdb/v2.0/get-started/). The only addendum here is that we'd like for the database deamon to start up on boot, so we'll need to create a service for it after influx is installed

### Creating an InfluxDB service

These instructions are adapted from [this site](http://blog.lemminger.eu/run-influxdb-2-0-as-a-service/).

1. Create an influxdb user: `sudo useradd -rs /bin/false influxdb`
1. Create a user folder: `sudo mkdir /home/influxdb; chown influxdb /home/influxdb`
1. Install the services in the services directory of this repository by running the script `sudo ./install-services.sh`

The influxdb2 service will now start on boot, and all influx files will be stored in `/home/influxdb`. If you'd like to start it immediately after installing, run `sudo systemctl start influxdb2.service`


