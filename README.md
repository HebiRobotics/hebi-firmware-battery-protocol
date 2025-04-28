# hebi-firmware-battery-protocol
This repo contains a basic CAN protocol for interfacing with HEBI products. The "nodes" folder includes examples of how to parse the messages.

## Message Structure
Every message follows the CAN 2.0B format (29bit EID, 8 bytes data). The EID is split into the following format:

```
struct {
    uint32_t node_id:8;     //Logical UID for this node
    uint32_t msg_type:12;   //Defined with "MessageType" enum
    uint32_t index_crc:8;   //Byte index or CRC depending on message type
    uint32_t rsvd:1;        //TODO: Assign these
    uint32_t invalid:3;     //Pad to 32 bits
};
```

This protocol assumes the presence of a primary node at a fixed `node_id = 0x00`. Additionally, `node_id = 0xFF` is reserved for nodes that have not been assigned a valid address. Other than the invalid node address of `0xFF`, no two nodes should ever have the same address. Certain message types (such as the `CTRL_SET_NODE_ID` message) are only allowed to be sent from the primary node. A secondary node should always send responses with its own id populated in the `node_id` field. 

Possible values for "`msg_type`" are defined in `base_msg.h` with the `MessageType` enum. 

The `index_crc` portion of the EID is primarily used to split larger messages into several smaller parts. With some message types it may be used as a CRC. 

## Project Info
Please refer to the [Common Firmware](https://github.com/HebiRobotics/hebi-firmware-battery-common) repo for more general information on this project.