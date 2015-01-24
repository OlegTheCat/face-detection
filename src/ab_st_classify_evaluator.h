#pragma once

struct PersistentFloatMatrix;
struct ClassifyEvaluator;
enum Label;

void classifyDataWithAbSingleThreaded(void *data,
				      const void *classifier,
				      struct PersistentFloatMatrix *pfm,
				      enum Label *labels);

struct ClassifyEvaluator *createAbStClassifyEvaluator();

