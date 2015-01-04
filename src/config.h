#pragma once

struct Config {
    int sub_sample_negatives;
    int sub_sample_step_x;
    int sub_sample_step_y;
    const char *pos_list_path;
    const char *neg_list_path;
};

typedef struct Config Config;

Config createDefaultConfig();
