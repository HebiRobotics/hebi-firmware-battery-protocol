#include "parser.h"

using namespace hebi::firmware::protocol;

Message_Parser::Message_Parser(){

}

void Message_Parser::update(){
    bool has_data = true;
    while(has_data){
        auto msg_opt = rx_buffer_.take();
        if(msg_opt.has_value()){
            tryParseMsg(msg_opt.value());
        } else {
            has_data = false;
        }

    }
}

bool Message_Parser::tryParseMsg(base_msg &msg){
    auto msg_type = msg_type_from_eid(msg.EID.raw);
    auto node_id = node_id_from_eid(msg.EID.raw);
    
    switch(msg_type){
        case MessageType::DATA_BATTERY_STATE: {
            if(msg.len != battery_state_msg::MSG_LEN_BYTES) return false;

            auto parsed = battery_state_msg(node_id, msg.data8);
            recvd_data_battery_state(parsed); //Trigger event

            return true; //Successful
        }
        default:
            //Invalid Message!
            return false;
    }
}