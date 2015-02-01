#include "ab_distributed_train_evaluator.h"

#include <mpi.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ada_boost.h"
#include "ab_st_train_evaluator.h"
#include "evaluators.h"
#include "mpi_utils.h"

int rdsDataPackSize(int examples_num) {
    int int_size;
    int float_size;
    int int_arr_size;

    MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &int_size);
    MPI_Pack_size(1, MPI_FLOAT, MPI_COMM_WORLD, &float_size);
    MPI_Pack_size(examples_num, MPI_INT, MPI_COMM_WORLD, &int_arr_size);

    return int_size + float_size * 3 + int_arr_size;
}

void packRdsData(Rds *rds,
		 Label *res_labels,
		 int examples_num,
		 char *out_buf,
		 int buf_size,
		 int *position) {
    MPI_Pack(&rds->feature_idx, 1, MPI_INT, out_buf, buf_size, position, MPI_COMM_WORLD);
    MPI_Pack(&rds->left_val, 1, MPI_FLOAT, out_buf, buf_size, position, MPI_COMM_WORLD);
    MPI_Pack(&rds->right_val, 1, MPI_FLOAT, out_buf, buf_size, position, MPI_COMM_WORLD);
    MPI_Pack(&rds->threshold, 1, MPI_FLOAT, out_buf, buf_size, position, MPI_COMM_WORLD);
    MPI_Pack((int *)res_labels, examples_num, MPI_INT, out_buf, buf_size, position, MPI_COMM_WORLD);
}

void unpackRdsData(Rds *rds,
		   Label *res_labels,
		   int examples_num,
		   char *in_buf,
		   int in_size,
		   int *position) {
    MPI_Unpack(in_buf, in_size, position, &rds->feature_idx, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(in_buf, in_size, position, &rds->left_val, 1, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(in_buf, in_size, position, &rds->right_val, 1, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(in_buf, in_size, position, &rds->threshold, 1, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Unpack(in_buf, in_size, position, res_labels, examples_num, MPI_INT, MPI_COMM_WORLD);
}

void trainWeakDistributed(AdaBoost *ab, DataSet *ds, float *weights,
			  int comm_rank, int comm_size) {
    Wrds wrds;
    Rds *stumps, best_stump;
    float min_wg_error, beta, alpha;
    Label *res_labels;
    int i, feature_idx, position, pack_size;
    Range col_range;
    char *send_buf;
    char *recv_buf;
    Rds *proc_best_stumps;

    col_range = getIdxRangeForProcess(getFeaturesNum(ds), comm_rank, comm_size);
    stumps = malloc(sizeof(Rds) * rangeSize(&col_range));
    /* proc_printf(">>> range size = %d\n\n", rangeSize(&col_range)); */

    for (i = 0, feature_idx = col_range.from; feature_idx < col_range.to; i++, feature_idx++) {
	stumps[i] = createRds(feature_idx);
	trainRds(&stumps[i], ds);
    }

    res_labels = malloc(sizeof(float) * getExamplesNum(ds));

    getBestStump(stumps, rangeSize(&col_range), ds, weights, res_labels, &best_stump, &min_wg_error);

    classifyDataWithRds(&best_stump, ds->data, res_labels);

    pack_size = rdsDataPackSize(getExamplesNum(ds));
    send_buf = malloc(pack_size);
    recv_buf = malloc(pack_size * comm_size);

    position = 0;
    packRdsData(&best_stump, res_labels, getExamplesNum(ds), send_buf, pack_size, &position);

    MPI_Allgather(send_buf, position, MPI_PACKED,
		  recv_buf, position, MPI_PACKED, MPI_COMM_WORLD);


    proc_best_stumps = malloc(sizeof(Rds) * comm_size);

    position = 0;
    for (i = 0; i < comm_size; i++) {
	unpackRdsData(proc_best_stumps,
		      res_labels,
		      getExamplesNum(ds),
		      recv_buf,
		      pack_size * comm_size,
		      &position);
    }

    getBestStump(proc_best_stumps, comm_size, ds, weights, res_labels, &best_stump, &min_wg_error);

    beta = min_wg_error / (1 - min_wg_error);
    alpha = logf(1 / beta);

    classifyDataWithRds(&best_stump, ds->data, res_labels);

    updateWeigths(res_labels,
		  ds->labels,
		  beta,
		  weights,
		  getExamplesNum(ds));

    wrds.wg = alpha;
    wrds.stump = best_stump;

    addToArrayList(&ab->weighted_stumps, &wrds);

    ab->threshold += 0.5 * alpha;
    free(res_labels);
    free(stumps);
}

void trainAdaBoostDistributed(void *data,
			      void *classifier,
			      DataSet *ds) {
    (void)data;

    float *weights;
    int t;
    AdaBoost *ab;
    int comm_rank, comm_size;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    ab = (AdaBoost *)classifier;

    weights = malloc(sizeof(float) * getExamplesNum(ds));
    initWeights(weights, ds);
    for (t = 0; t < ab->max_stumps; t++) {
	normalizeWeigths(weights, getExamplesNum(ds));
	trainWeakDistributed(ab, ds, weights, comm_rank, comm_size);
    }

    free(weights);
}


TrainEvaluator *createAbDistributedTrainEvaluator() {
    TrainEvaluator *evaluator;

    evaluator = malloc(sizeof(TrainEvaluator));
    evaluator->evaluator_data = NULL;
    evaluator->train_func = trainAdaBoostDistributed;

    return evaluator;
}
