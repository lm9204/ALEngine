#pragma once

#include <cstdint>

namespace ale
{
const int32_t CHUNK_SIZE = 1024 * 1024;
const int32_t MAX_BLOCK_SIZE = 4096;
const int32_t BLOCK_SIZE_COUNT = 16;
const int32_t CHUNK_ARRAY_INCREMENT = 256;

struct Block
{
	Block *next;
};

// 특정 size의 블록 리스트가 보관된 공간
struct Chunk
{
	Block *blocks;
	int32_t blockSize;
};

class BlockAllocator
{
  public:
	BlockAllocator();
	~BlockAllocator();

	void *allocateBlock(int32_t size);
	void freeBlock(void *pointer, int32_t size);

  private:
	Chunk *m_chunks;	  // 전체 청크 메모리
	int32_t m_chunkCount; // 사용 중인 청크 수
	int32_t m_chunkSpace; // 전체 청크 공간

	Block *m_availableBlocks[BLOCK_SIZE_COUNT];

	static int32_t s_blockSizes[BLOCK_SIZE_COUNT];
	static uint8_t s_blockSizeLookup[MAX_BLOCK_SIZE + 1];
	static bool s_blockSizeLookupInitialized;
};
} // namespace ale
