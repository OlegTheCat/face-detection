#pragma once

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrixImpl *createPfmFileImpl(const char *storage_path, int rows, int cols);

