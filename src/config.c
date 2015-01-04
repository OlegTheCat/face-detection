#include "config.h"

#include "constants.h"

Config createDefaultConfig() {
    Config config;

    config.sub_sample_negatives = 1;
    config.sub_sample_step_x = 5;
    config.sub_sample_step_y = 5;
    config.pos_list_path = POS_LIST;
    config.neg_list_path = NEG_LIST;

    return config;
}
