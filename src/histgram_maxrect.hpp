#pragma once
#include "lubee/src/size.hpp"
#include <vector>
#include <cassert>

namespace dg {
	// 下辺がX軸上にあると仮定した矩形
	struct HRect {
		int				x;
		lubee::SizeI	size;

		bool operator == (const HRect & r) const noexcept;
		void print(std::ostream& os) const;
		static HRect Zero() noexcept;
	};
	template <class H, class GetHeight, class GetWidth, class NtfRect>
	HRect GetMaxRect(const H* hist, const size_t n, const GetHeight& getHeight, const GetWidth& getWidth, NtfRect&& ntf) {
		if(n == 0)
			return HRect::Zero();

		int maxA = 0;
		HRect res = {};
		const auto checkRectSize = [&maxA, &res, &ntf](const auto& it, const int curPos){
			assert(curPos > it.beginPos);
			const lubee::SizeI sz{
				curPos-it.beginPos,
				it.height
			};
			ntf(it.beginPos, sz);
			const auto a = sz.area();
			if(maxA < a) {
				maxA = a;
				res.x = it.beginPos;
				res.size = sz;
			}
		};

		struct Item {
			int		height;
			int		beginPos;
		};
		using Stack = std::vector<Item>;
		Stack stack;
		const auto N = n;
		int pos = 0;
		for(size_t i=0 ; i<N ; pos+=getWidth(hist[i++])) {
			const auto h = getHeight(hist[i]);
			if(stack.empty() || stack.back().height < h)
				stack.emplace_back(Item{h, pos});
			else {
				if(stack.back().height == h)
					continue;
				int lastPos;
				while(!stack.empty() && stack.back().height > h) {
					checkRectSize(stack.back(), pos);
					lastPos = stack.back().beginPos;
					stack.pop_back();
				}
				if(h > 0) {
					stack.emplace_back(Item{h, lastPos});
				}
			}
		}
		while(!stack.empty()) {
			checkRectSize(stack.back(), pos);
			stack.pop_back();
		}
		return res;
	}
	template <class CB>
	HRect GetMaxRect(const int* height, const size_t n, const CB& cb) {
		return GetMaxRect(
			height,
			n,
			[](auto h){ return h; },
			[](auto){ return 1; },
			cb
		);
	}
	template <class CB>
	HRect GetMaxRect(const lubee::SizeI* hist, const size_t n, const CB& cb) {
		return GetMaxRect(
			hist,
			n,
			[](const auto& h){ return h.height; },
			[](const auto& h){ return h.width; },
			cb
		);
	}
	HRect GetMaxRect(const int* height, size_t n);
	HRect GetMaxRect(const lubee::SizeI* hist, size_t n);
}
