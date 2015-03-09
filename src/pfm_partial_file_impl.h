#pragma once

struct PersistentFloatMatrixImpl;

#define PFMI_OUT_OF_COL_RANGE -1

struct PersistentFloatMatrixImpl *createPfmPartialFileImpl(const char *storage_path,
							   int start_col,
							   int end_col,
							   int rows);
