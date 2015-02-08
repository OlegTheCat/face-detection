#pragma once

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrixImpl *createPfmFileImpl(const char *storage_path, int rows, int cols);
struct PersistentFloatMatrixImpl *createBufferedPfmFileImpl(const char *storage_path,
							    int rows,
							    int cols,
							    int preload_cols_num);

