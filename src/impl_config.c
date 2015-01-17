#include "impl_config.h"

#include <stdlib.h>

#include "rds_ig_train_evaluator.h"
#include "rds_single_threaded_classify_evaluator.h"

ImplConfig *global_impl_config;

ImplConfig *getDefaultImplConfig() {
    ImplConfig *ic;

    ic = malloc(sizeof(ImplConfig));

    ic->rds_train_evaluator = createRdsIgTrainEvaluator();
    ic->rds_classify_evaluator = createRdsSingleThreadedClassifyEvaluator();
    ic->ada_boost_train_evaluator = NULL;
    ic->ada_boost_classify_evaluator = NULL;

    return ic;
}

void setImplConfig(ImplConfig *impl_config) {
    global_impl_config = impl_config;
}

const ImplConfig *getCurrentImplConfig() {
    return global_impl_config;
}

void deleteImplConfig(ImplConfig *ic) {
    if (ic != NULL) {
	if (ic->rds_train_evaluator != NULL) {
	    free(ic->rds_train_evaluator);
	}

	if (ic->rds_classify_evaluator != NULL) {
	    free(ic->rds_classify_evaluator);
	}

	if (ic->ada_boost_train_evaluator != NULL) {
	    free(ic->ada_boost_train_evaluator);
	}

	if (ic->ada_boost_classify_evaluator != NULL) {
	    free(ic->ada_boost_classify_evaluator);
	}

	free(ic);
    }
}
