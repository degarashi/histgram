#pragma once
#include "rect.hpp"

namespace dg {
	// 仮想配線長クラス
	class VNet {
		public:
			using VLength = uint32_t;
			enum Corner {
				LT,
				RT,
				LB,
				RB,
				N_Corner
			};
		private:
			Rect	_rect[N_Corner];

		public:
			VNet(const lubee::RectI& first);
			// 各仮想配線長矩形が引数のrectを含むように拡大させる
			void takeIn(const lubee::RectI& r) noexcept;
			// (縦辺 + 横辺)
			VLength getLength() const noexcept;
			bool operator == (const VNet& vn) const noexcept;
	};
}
