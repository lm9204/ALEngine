#pragma once

#include <cstring>
#include <stdint.h>

namespace ale
{
// Non-owning raw buffer class
struct SBuffer
{
	uint8_t *data = nullptr;
	uint64_t _size = 0;

	SBuffer() = default;

	SBuffer(uint64_t size)
	{
		allocate(size);
	}

	SBuffer(const SBuffer &) = default;

	static SBuffer copy(SBuffer other)
	{
		SBuffer result(other._size);
		memcpy(result.data, other.data, other._size);
		return result;
	}

	void allocate(uint64_t size)
	{
		release();

		data = new uint8_t[size];
		_size = size;
	}

	void release()
	{
		delete[] data;
		data = nullptr;
		_size = 0;
	}

	template <typename T> T *as()
	{
		return (T *)data;
	}

	operator bool() const
	{
		return (bool)data;
	}
};

struct ScopedBuffer
{
	ScopedBuffer(SBuffer buffer) : m_Buffer(buffer)
	{
	}

	ScopedBuffer(uint64_t size) : m_Buffer(size)
	{
	}

	~ScopedBuffer()
	{
		m_Buffer.release();
	}

	uint8_t *data()
	{
		return m_Buffer.data;
	}
	uint64_t size()
	{
		return m_Buffer._size;
	}

	template <typename T> T *as()
	{
		return m_Buffer.as<T>();
	}

	operator bool() const
	{
		return m_Buffer;
	}

  private:
	SBuffer m_Buffer;
};
} // namespace ale