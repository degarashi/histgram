#include "rect.hpp"
#include <iostream>

namespace dg {
	Rect Rect::Zero() noexcept {
		return {{}, {}};
	}
	void Rect::takeIn(const lubee::PointI p) noexcept {
		x0 = std::min(x0, p.x);
		y0 = std::min(y0, p.y);
		x1 = std::max(x1, p.x);
		y1 = std::max(y1, p.y);
	}
}
