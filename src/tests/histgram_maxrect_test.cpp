#include "../histgram_maxrect.hpp"
#include "lubee/src/tests/test.hpp"

namespace dg::test {
	namespace {
		// 総当たりによる素直で確実な実装
		template <class H, class GetHeight, class GetWidth>
		HRect SimpleMaxRect(const H& hist, const GetHeight& getHeight, const GetWidth& getWidth) {
			if(hist.empty()) {
				return HRect::Zero();
			}

			HRect res;
			int maxArea = 0;
			const auto n = hist.size();
			int pos = 0;
			for(size_t i=0 ; i<n ; pos+=getWidth(hist[i++])) {
				const int baseHeight = getHeight(hist[i]);
				for(int j=1 ; j<=baseHeight ; j++) {
					size_t k = i+1;
					int w = getWidth(hist[i]);
					while(k < n) {
						if(getHeight(hist[k]) < j)
							break;
						w += getWidth(hist[k++]);
					}
					const int a = w * j;
					if(a > maxArea) {
						maxArea = a;
						res.x = i;
						res.size = {w, j};
					}
				}
			}
			return res;
		}
	}
	using HistgramMaxRect = lubee::test::Random;
	TEST_F(HistgramMaxRect, FixedWidth) {
		auto& rd = mt();
		std::vector<int> height;
		const int N = rd.getUniform<int>({0, 1024});
		for(int i=0 ; i<N ; i++)
			height.emplace_back(rd.getUniform<int>({0, 256}));

		const auto res0 = GetMaxRect(height.data(), height.size());
		const auto res1 = SimpleMaxRect(
							height,
							[](const auto h){ return h; },
							[](auto){ return 1; }
						);
		if(res0 == res1) {}
		else
			ASSERT_EQ(res1.size.area(), res0.size.area());
	}
	TEST_F(HistgramMaxRect, VariableWidth) {
		auto& rd = mt();
		std::vector<lubee::SizeI> hist;
		const int N = rd.getUniform<int>({0, 1024});
		for(int i=0 ; i<N ; i++) {
			hist.emplace_back(lubee::SizeI{
				rd.getUniform<int>({1, 16}),
				rd.getUniform<int>({0, 256})
			});
		}

		const auto res0 = GetMaxRect(hist.data(), hist.size());
		const auto res1 = SimpleMaxRect(
							hist,
							[](const auto& h){ return h.height; },
							[](const auto& h){ return h.width; }
						);
		if(res0 == res1) {}
		else
			ASSERT_EQ(res1.size.area(), res0.size.area());
	}
}
