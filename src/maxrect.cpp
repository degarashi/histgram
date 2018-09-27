#include "maxrect.hpp"

namespace dg {
	Rect GetMaxRect(const bool* ar, const lubee::SizeI size) {
		return GetMaxRect(ar, size, [](auto&&...){});
	}
}
