#pragma once

struct PersistentFloatMatrix;
struct DataSet;
enum Label;

typedef void (*TrainFunc)(void *evaluator_data,
			  void *classifier,
			  struct DataSet *ds);

typedef void (*ClassifyFunc)(void *evaluator_data,
			     const void *classifier,
			     struct PersistentFloatMatrix *pfm,
			     enum Label *labels);

struct TrainEvaluator {
    void *evaluator_data;
    TrainFunc train_func;
};

typedef struct TrainEvaluator TrainEvaluator;

struct ClassifyEvaluator {
    void *evaluator_data;
    ClassifyFunc classify_func;
};

typedef struct ClassifyEvaluator ClassifyEvaluator;
