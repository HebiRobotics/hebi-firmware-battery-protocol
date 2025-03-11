#include "parser.h"

using namespace hebi::firmware::protocol;

bool CAN_Parser::tryParseMsg(uint32_t EID, uint8_t len, uint8_t data[8]){
    MessageType msg_type = msg_type_from_eid(EID);
    uint8_t node_id = node_id_from_eid(EID);

    switch(msg_type){
        case MessageType::DATA_BATTERY_STATE:
            if(len != battery_state_msg::MSG_LEN_BYTES) return false;

            auto parsed = battery_state_msg(node_id, data);
            recvd_data_battery_state(parsed); //Trigger event

            return true; //Successful
        default:
            //Invalid Message!
            return false;
    }
}