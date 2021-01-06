# PiLogger <!-- omit in toc -->

- [SocketCAN](#socketcan)
  - [Installing](#installing)
  - [Useful commands](#useful-commands)
      - [Bring up a virtual CAN interface](#bring-up-a-virtual-can-interface)
      - [Automatically add virtual/physical CAN interface upon system startup](#automatically-add-virtualphysical-can-interface-upon-system-startup)
      - [Sending and viewing CAN messages](#sending-and-viewing-can-messages)

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


#### Automatically add virtual/physical CAN interface upon system startup

The commands required to bring up a SocketCAN interface have been made into service descriptions, located in the `services` folder. To install the services on a fresh Ubuntu installation, simply run `sudo install-services.sh` from the `services` directory. This will copy the service files to the system directory and enable them to start upon boot.

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