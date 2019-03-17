#pragma once

#include <array>
#include <cppkafka/cppkafka.h>

#include <eosio/chain_plugin/chain_plugin.hpp>

#include "types.hpp"
#include "actions.hpp"

namespace kafka {

using namespace std;
using namespace cppkafka;
using namespace eosio;

class kafka {
public:
    void set_config(Configuration config);
    void set_topic(const string& topic);
    void set_partition(int partition);
    void set_poll_interval(unsigned interval);
    void start();
    void stop();

    void push_block(const chain::block_state_ptr& block_state, bool irreversible, bool produce);
    std::pair<uint32_t, uint32_t> push_transaction(const chain::transaction_receipt& transaction_receipt, const BlockPtr& block, uint16_t block_seq);
    void push_transaction_trace(const chain::transaction_trace_ptr& transaction_trace);
    void push_action(const chain::action_trace& action_trace, uint64_t parent_seq);

private:
    bool is_token(name account);

    Configuration config_;
    string topic_;

    int partition_{-1};

    unsigned poll_interval_ = 0;
    unsigned poll_counter_ = 0;

    std::unique_ptr<Producer> producer_;

    std::unordered_map<transaction_id_type, chain::transaction_trace_ptr> cached_traces_;
    std::unordered_map<transaction_id_type, vector<ActionPtr>> cached_actions_;

    int producer_stats_counter_ = 0;
    std::unique_ptr<ProducerSchedule> producer_schedule_;

    std::unordered_set<name> cached_tokens_;

    std::unordered_map<uint64_t, ram_deal> cached_ram_deals_;

    std::unordered_map<uint64_t, claimed_rewards> cached_claimed_rewards_;
};

}
