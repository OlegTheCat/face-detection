#include "ada_boost.h"

#include "data_set.h"
#include "persistent_float_matrix.h"
#include "impl_config.h"
#include "evaluators.h"

AdaBoost *createAdaBoost(int max_features) {
    return createAdaBoostWithEvaluators(max_features, NULL, NULL);
}

AdaBoost *createAdaBoostWithEvaluators(int max_features,
				       TrainEvaluator *te,
				       ClassifyEvaluator *ce) {
    AdaBoost *ab;

    ab = malloc(sizeof(AdaBoost));

    ab->weighted_stumps = createArrayList(sizeof(Wrds));
    ab->threshold = 0;
    ab->max_stumps = max_features;

    if (te == NULL) {
	ab->train_evaluator =
	    getCurrentImplConfig()->ada_boost_train_evaluator;
    } else {
	ab->train_evaluator = te;
    }

    if (ce == NULL) {
	ab->classify_evaluator =
	    getCurrentImplConfig()->ada_boost_classify_evaluator;
    } else {
	ab->classify_evaluator = ce;
    }

    return ab;

}


void trainAdaBoost(AdaBoost *ab, DataSet *ds) {
    ab->
	train_evaluator->
	train_func(ab->
		   train_evaluator->
		   evaluator_data, ab, ds);
}

void classifyDataWithAdaBoost(const AdaBoost *ab,
			      Pfm *data,
			      Label *labels) {
    ab->
	classify_evaluator->
	classify_func(ab->
		      classify_evaluator->
		      evaluator_data,
		      ab, data, labels);
}

void deleteAdaBoost(AdaBoost *ab) {
    if (ab != NULL) {
	deleteArrayList(&ab->weighted_stumps);
	free(ab);
    }
}
