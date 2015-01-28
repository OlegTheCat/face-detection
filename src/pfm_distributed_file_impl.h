#pragma once

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrixImpl *createPfmDistributedFileImpl(const char *storage_path,
							       int cols,
							       int rows);

