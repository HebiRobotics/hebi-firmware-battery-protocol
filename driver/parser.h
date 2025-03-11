/* parser.h
    Common message parsing functions
*/

#include "all_msg.h"


namespace hebi::firmware::protocol {

class CAN_Parser {
public:
    inline uint8_t node_id_from_eid(uint32_t EID){
        return ( EID & 0xFF );
    }

    inline MessageType msg_type_from_eid(uint32_t EID){
        return static_cast<MessageType>((EID & (0xFFF << 8)) >> 8);
    }
    
    bool tryParseMsg(uint32_t EID, uint8_t len, uint8_t data[8]);
protected:
    virtual void recvd_data_battery_state(battery_state_msg msg) {}
};

};