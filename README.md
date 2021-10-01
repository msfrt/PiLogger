# PiLogger <!-- omit in toc -->

- [](#)
- [SocketCAN](#socketcan)
  - [Installing](#installing)
  - [Useful commands](#useful-commands)
      - [Bring up a virtual CAN interface](#bring-up-a-virtual-can-interface)
      - [Sending and viewing CAN messages](#sending-and-viewing-can-messages)


# Hardware setup

## Installing Ubuntu

There are many ways to succeed at installing Ubuntu Server. This application was built on Ubuntu Server 20.04, so check yourself with earlier versions. I don't need to explain how to do this. You can read [this tutorial](https://ubuntu.com/tutorials/how-to-install-ubuntu-on-your-raspberry-pi#1-overview) instead. Just note that the WiFi section is a bit outdated. In the YAML file, I believe that you will need to escape the stuff in quotations. For example, instead of wifi name `"Formula House"`, you would put `\"Formula House\"`.

### Updating network information after installation

I updated the WiFi information after reading [his article](https://linuxconfig.org/ubuntu-20-04-connect-to-wifi-from-command-line). Please note that this time, the quotations are *not* escaped. This is **actual yaml**.

### Updating username and hostname after installation

Again, so easy. Google really is amazing. [This post](https://askubuntu.com/questions/34074/how-do-i-change-my-username) showed me how to change the username from ubuntu to whatever I wanted, and [this article](https://www.cyberciti.biz/faq/ubuntu-change-hostname-command/) told me how to change the hostname. Please note, you must create a temporary user when changing the username (which is explained in the post I linked).

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