#include "histgram_maxrect.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace dg {
	// ----------------- HRect -----------------
	bool HRect::operator == (const HRect& r) const noexcept {
		return x == r.x &&
			size == r.size;
	}
	void HRect::print(std::ostream& os) const {
		os << "{x=" << x << ", " << size << "}";
	}
	HRect HRect::Zero() noexcept {
		return {
			.x = 0,
			.size = {}
		};
	}

	HRect GetMaxRect(const int* height, const size_t n) {
		return GetMaxRect(height, n, [](auto&&...){});
	}
	HRect GetMaxRect(const lubee::SizeI* hist, const size_t n) {
		return GetMaxRect(hist, n, [](auto&&...){});
	}
}
