/* Main_Node.h
    
*/

#pragma once

#include "base_node.h"
#include <map>

namespace hebi::firmware::protocol {

struct child_node_info {
    uint64_t t_last_update {};

    //Battery data info
    float voltage {};
    float current {};
    float capacity {};
    float capacity_max {};
};

class Main_Node : public Base_Node {
public:
    Main_Node();

    void update() override;

    child_node_info& getNodeFromID(node_id_t node_id);

protected:
    void recvd_data_battery_state(battery_state_msg msg) override;
    
    std::map<node_id_t, child_node_info> child_nodes_;

    uint16_t count_ {0};
    const uint8_t NODE_ID = 0; //Hardcoded node id to 0
};

};