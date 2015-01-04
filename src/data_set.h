#pragma once

#include "pgm.h"

void readImageList(const char *filename, PgmImage ***images);

void subSampleImage(PgmImage *image, PgmImage ***samples,
			int sample_width, int sample_heigth,
			int sample_step_x, int sample_step_y);
