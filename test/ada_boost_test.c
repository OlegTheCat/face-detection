#include "ada_boost_test.h"

#include "minunit.h"
#include "ada_boost.h"

const char *testCreateAdaBoost() {
    AdaBoost *ab;

    ab = createAdaBoost();

    mu_assert("AdaBoost == NULL", ab != NULL);

    deleteAdaBoost(ab);
    return 0;
}

const char *testTrainAdaBoost() {
    return 0;
}

const char *testClassifyDataWithAdaBoost() {
    return 0;
}
