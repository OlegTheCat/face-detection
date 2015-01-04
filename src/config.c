#include "config.h"

Config createDefaultConfig() {
    Config config;

    config.sub_sample_negatives = 1;
    config.sub_sample_step_x = 5;
    config.sub_sample_step_y = 5;

    return config;
}
