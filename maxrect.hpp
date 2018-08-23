#pragma once
#include "rect.hpp"
#include "histgram_maxrect.hpp"

namespace dg {
	template <class CB>
	Rect GetMaxRect(const bool* ar, const lubee::SizeI size, const CB& cb) {
		Rect ret = Rect::Zero();
		if(size.area() > 0) {
			// 上方向に幾つ空きマスが連続しているかのヒストグラムを作成
			std::vector<int>	hist(size.area());
			for(int j=0 ; j<size.width ; j++) {
				hist[j] = ar[j] ? 0 : 1;
			}
			for(int i=1 ; i<size.height ; i++) {
				for(int j=0 ; j<size.width ; j++) {
					hist[i*size.width + j] = ar[i*size.width + j] ?
						0 :
						hist[(i-1)*size.width + j]+1;
				}
			}
			int maxA = 0;
			for(int i=0 ; i<size.height ; i++) {
				const auto hr = GetMaxRect(
									hist.data() + i*size.width,
									size.width,
									[&cb, &i](const int x, const lubee::SizeI s){
										cb(lubee::RectI{
											lubee::PointI{
												x,
												i+1 - s.height
											},
											s
										});
									}
								);
				const lubee::RectI rect{
					lubee::PointI{
						hr.x,
						i+1 - hr.size.height
					},
					hr.size
				};
				const auto a = rect.area();
				if(maxA < a) {
					maxA = a;
					static_cast<lubee::RectI&>(ret) = rect;
				}
			}
		}
		return ret;
	}
	// 障害物に重ならない最大長方形を求める
	Rect GetMaxRect(const bool* ar, lubee::SizeI size);
}
