# PiLogger <!-- omit in toc -->

- [SocketCAN](#socketcan)
  - [Installing](#installing)
  - [Useful commands](#useful-commands)
      - [Bring up a virtual CAN interface](#bring-up-a-virtual-can-interface)
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