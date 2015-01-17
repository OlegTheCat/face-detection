#pragma once

struct PersistentFloatMatrix;
struct ClassifyEvaluator;
enum Label;

void classifyDataWithRdsSingleThreaded(void *data,
				       const void *classifier,
				       struct PersistentFloatMatrix *pfm,
				       enum Label *labels);

struct ClassifyEvaluator *createRdsSingleThreadedClassifyEvaluator();
