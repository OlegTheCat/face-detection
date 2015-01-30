#pragma once

#include "pfm_partial_file_impl.h"

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrixImpl *createPfmDistributedFileImpl(const char *storage_path,
							       int cols,
							       int rows);

