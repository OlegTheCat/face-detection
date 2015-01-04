#pragma once

struct Config {
    int sub_sample_negatives;
    int sub_sample_step_x;
    int sub_sample_step_y;
};

typedef struct Config Config;

Config createDefaultConfig();
