#include "../nboard.hpp"
#include "lubee/src/tests/test.hpp"

namespace dg::test {
	using NBoard = lubee::test::Random;
	TEST_F(NBoard, CalcScore) {
		auto& rd = mt();
		// 独自に塗り潰した配列と、placeObstacle()で塗った配列では同じ結果になる
		const lubee::SizeI size = {
			rd.getUniform<int>({1,64}),
			rd.getUniform<int>({1,64})
		};
		::dg::NBoard nbManual(size),
					nbPlace(size);

		size_t n = rd.getUniform<size_t>({0, 64});
		while(n != 0) {
			--n;

			lubee::RectI obs;
			// 本当は{0, size.width-1}だが、はみだしもOKなのでそのテスト
			obs.x0 = rd.getUniform<int>({-size.width, size.width});
			obs.y0 = rd.getUniform<int>({-size.height, size.height});
			// 本当は{0, size.width-obs.x0}だが、はみだしもOKなのでそのテスト
			obs.setSize({
				rd.getUniform<int>({1, size.width}),
				rd.getUniform<int>({1, size.height})
			});
			nbPlace.placeObstacle(obs);

			const auto obsC = obs.clip(lubee::RectI{{0,0}, size});
			for(int i=obsC.y0 ; i<obsC.y1 ; i++) {
				for(int j=obsC.x0 ; j<obsC.x1 ; j++) {
					nbManual.cellAt(j, i).used = true;
				}
			}
		}
		nbManual.refresh();

		ASSERT_EQ(nbManual, nbPlace);
	}
}
