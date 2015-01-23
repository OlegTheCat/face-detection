#pragma once

struct TrainEvaluator;
struct ClassifyEvaluator;

struct ImplConfig {
    struct TrainEvaluator *rds_train_evaluator;
    struct ClassifyEvaluator *rds_classify_evaluator;

    struct TrainEvaluator *ada_boost_train_evaluator;
    struct ClassifyEvaluator *ada_boost_classify_evaluator;
};

typedef struct ImplConfig ImplConfig;

ImplConfig *getDefaultImplConfig();
void setImplConfig(ImplConfig *impl_config);
ImplConfig *getCurrentImplConfig();
void deleteImplConfig(ImplConfig *ic);
