#pragma once
#include "lubee/src/rect.hpp"

namespace dg {
	struct Rect : lubee::RectI {
		using lubee::RectI::Rect;

		static Rect Zero() noexcept;
		// 点Pを含むように矩形を拡大させる
		void takeIn(lubee::PointI p) noexcept;
	};
}
