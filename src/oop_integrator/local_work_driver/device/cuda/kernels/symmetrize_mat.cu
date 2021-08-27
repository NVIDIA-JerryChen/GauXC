#include "symmetrize_mat.hpp"
#include "device_specific/cuda_device_constants.hpp"

namespace GauXC {

__global__ void symmetrize_matrix_device( size_t N, double* A, size_t LDA ) {

  constexpr uint32_t block_size = cuda::warp_size;

  __shared__ double buffer[block_size][block_size+1];  // Pad shared memory to resolve shared memory

  const size_t num_blocks = ((N + block_size - 1) / block_size);

  for (int i = blockIdx.x; i < num_blocks; i += gridDim.x) {
    // TODO This could be load balanced if need be
    const int i_coord = i * block_size;
    for (int j = i; j < num_blocks; j++) {
      const int j_coord = j * block_size;

      // Read in block to buffer
      // TODO These could be vector reads/writes if this becomes significant
      if (i_coord + threadIdx.y < N && j_coord + threadIdx.x < N) {
        buffer[threadIdx.y][threadIdx.x] = A[(i_coord + threadIdx.y) * LDA + j_coord + threadIdx.x];
      }
      __syncthreads();

      // Write buffer
      if (j_coord + threadIdx.y < N && i_coord + threadIdx.x < N) {
        if ((j_coord != i_coord || threadIdx.x < threadIdx.y)) { // handles the diagonal block
          A[(j_coord + threadIdx.y) * LDA + i_coord + threadIdx.x] = buffer[threadIdx.x][threadIdx.y];
        }
      }
      __syncthreads();
    }
  }
}



void symmetrize_matrix( int32_t N, double* A, size_t LDA, cudaStream_t stream ) {
  const size_t num_blocks = ((N + cuda::warp_size - 1) / cuda::warp_size);
  // Warp size must equal max_warps_per_thread_block must equal 32
  dim3 threads(cuda::warp_size, cuda::max_warps_per_thread_block), blocks(num_blocks);
  symmetrize_matrix_device<<<blocks, threads, 0, stream>>>(N, A, LDA);
}

}
