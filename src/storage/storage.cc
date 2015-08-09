/*!
 * Copyright (c) 2015 by Contributors
 */
#include <mshadow/tensor.h>
#include <mxnet/storage.h>
namespace mxnet {

// class NaiveStorageManager : public StorageManager {
//  public:
//   virtual Handle Alloc(size_t size, Context ctx);
//   virtual void Free(Handle handle);
// };

StorageManager::Handle StorageManager::Alloc(size_t size, Context ctx) {
  Handle hd;
  hd.ctx = ctx;
  if (ctx.dev_mask == cpu::kDevMask) {
    hd.dptr = calloc(size, sizeof(real_t));
  } else {
#if MXNET_USE_CUDA
    cudaMalloc(&hd.dptr, size);
#endif
  }
  return hd;
}

void StorageManager::Free(StorageManager::Handle handle) {
  if (handle.ctx.dev_mask == cpu::kDevMask) {
    free(handle.dptr);
    handle.dptr = NULL;
    // cudaFreeHost(handle.dptr);
  } else {
#if MXNET_USE_CUDA
    cudaFree(handle.dptr);
#endif
  }
}

StorageManager* StorageManager::Get() {
  static StorageManager inst;
  return &inst;
}

}  // namespace mxnet
