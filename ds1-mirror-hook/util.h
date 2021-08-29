#pragma once

#include <limits>

template<typename T>
void swapBits(T& n, unsigned int bit1, unsigned int bit2)
{
	bool b1 = n & bit1;
	bool b2 = n & bit2;
	if (b1 != b2)
	{
		n ^= bit1;
		n ^= bit2;
	}
}

// avoid integer overflow when negating min value
template<typename T>
void safeNegate(T& n)
{
	if (n == std::numeric_limits<T>::min())
	{
		n++;
	}
	n *= -1;
	if (n == std::numeric_limits<T>::min() + 1)
	{
		n--;
	}
}
