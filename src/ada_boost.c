#include "ada_boost.h"

AdaBoost *createAdaBoost() {
    AdaBoost *ab;

    ab = malloc(sizeof(AdaBoost));

    ab->weighted_stumps = createArrayList(sizeof(Wrds));
    ab->threshold = 0;

    return ab;
}
void trainAdaBoost(AdaBoost *ab, struct DataSet *ds) {
    (void)ab;
    (void)ds;
}

void classifyDataWithAdaBoost(const AdaBoost *ab,
			      struct PersistentFloatMatrix *data,
			      Label *labels) {
    (void)ab;
    (void)data;
    (void)labels;
}
void deleteAdaBoost(AdaBoost *ab) {
    if (ab != NULL) {
	deleteArrayList(&ab->weighted_stumps);
	free(ab);
    }
}
