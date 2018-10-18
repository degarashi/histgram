#include "../maxrect.hpp"
#include "lubee/src/tests/test.hpp"

namespace dg::test {
	namespace {
		// 総当たりによる素直で確実な実装
		Rect SimpleMaxRect(const bool* cell, const lubee::SizeI size) {
			const auto check = [cell, size](const size_t x, const size_t y, const size_t w, const size_t h) {
				const size_t lenX = std::min<size_t>(size.width, x+w),
						   lenY = std::min<size_t>(size.height, y+h);
				for(size_t i=y ; i<lenY ; i++) {
					for(size_t j=x ; j<lenX ; j++) {
						assert(i < size_t(size.height) &&
								j < size_t(size.width));
						if(cell[i*size.width + j])
							return false;
					}
				}
				return true;
			};
			int maxArea = 0;
			Rect rect = Rect::Zero();
			for(int i=0 ; i<size.height ; i++) {
				for(int j=0 ; j<size.width ; j++) {
					for(int h=1 ; h<=size.height-i ; h++) {
						for(int w=1 ; w<=size.width-j ; w++) {
							if(check(j,i,w,h)) {
								if(maxArea < w*h) {
									maxArea = w*h;
									rect = {j, i, lubee::SizeI{w, h}};
								}
							}
						}
					}
				}
			}
			return rect;
		}
	}
	using MaxRect = lubee::test::Random;
	TEST_F(MaxRect, Regular) {
		auto& rd = mt();
		const lubee::SizeI size = {
			rd.getUniform<int>({0, 32}),
			rd.getUniform<int>({0, 32})
		};
		std::unique_ptr<bool>	ar(new bool[size.area()]);
		auto *const ar_p = ar.get();
		for(int i=0 ; i<size.height ; i++) {
			for(int j=0 ; j<size.width; j++) {
				ar_p[i*size.width + j] = static_cast<bool>(rd.getUniform<int>({0, 1}));
			}
		}

		const auto res0 = GetMaxRect(ar_p, size);
		const auto res1 = SimpleMaxRect(ar_p, size);
		const auto chk = [size](const lubee::RectI& r){
			r.checkValidness();
			assert(r.x0 + r.width() <= size.width);
			assert(r.y0 + r.height() <= size.height);
		};
		chk(res0);
		chk(res1);
		if(res0 == res1) {}
		else
			ASSERT_EQ(res1.size().area(), res0.size().area());
	}
}
