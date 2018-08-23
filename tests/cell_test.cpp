#include "../cell.hpp"
#include "lubee/tests/test.hpp"

namespace dg::test {
	namespace {
		template <class RD>
		lubee::PointI RandomPos(RD& rd, const lubee::SizeI s) {
			return {
				rd.template getUniform<int>({0, s.width-1}),
				rd.template getUniform<int>({0, s.height-1})
			};
		}
		template <class RD>
		lubee::SizeI RandomSize(RD& rd, const lubee::RangeI range) {
			return {
				rd.template getUniform<int>(range),
				rd.template getUniform<int>(range)
			};
		}
		template <class RD>
		lubee::RectI RandomRect(RD& rd, const lubee::SizeI rangePos, const lubee::RangeI rangeSize) {
			return {
				RandomPos(rd, rangePos),
				RandomSize(rd, rangeSize)
			};
		}

		CellBoard::PointV FindPlace(const NBoard& board, const lubee::SizeI rs) {
			CellBoard::PointV ret;
			uint32_t maxScore = 0;
			const auto size = board.getSize();
			for(int oy=0 ; oy<size.height-rs.height+1 ; oy++) {
				for(int ox=0 ; ox<size.width-rs.width+1 ; ox++) {
					uint32_t score = 0;
					bool valid = true;
					for(int i=0 ; i<rs.height ; i++) {
						for(int j=0 ; j<rs.width ; j++) {
							auto& cell = board.cellAt(ox+j, oy+i);
							if(cell.used) {
								valid = false;
								i=rs.height;
								break;
							}
							score += cell.score;
						}
					}
					if(valid) {
						if(maxScore < score) {
							maxScore = score;
							ret.clear();
							ret.emplace_back(lubee::PointI{ox,oy});
						} else if(maxScore == score) {
							ret.emplace_back(lubee::PointI{ox,oy});
						}
					}
				}
			}
			return ret;
		}
	}
	using Cell = lubee::test::Random;
	TEST_F(Cell, FindPlace) {
		using RectV = std::vector<lubee::RectI>;

		auto& rd = mt();
		// テストするエリアの広さ
		const auto size = RandomSize(rd, {1,32});

		// 初期エリアに適当に障害物を置く
		const size_t nObstacle = rd.getUniform<size_t>({1,32});
		RectV obstacle(nObstacle);
		for(size_t i=0 ; i<nObstacle ; i++) {
			obstacle[i] = RandomRect(rd, size, {1, std::max(size.width, size.height)});
		}
		CellBoard board(obstacle.data(), nObstacle, size);
		const lubee::SizeI sz{
			rd.getUniform<int>({1,size.width}),
			rd.getUniform<int>({1,size.height})
		};
		const auto cmp = [](const auto& s0, const auto& s1){
			auto* s0p = &s0.x;
			auto* s1p = &s1.x;
			return std::lexicographical_compare(s0p, s0p+2, s1p, s1p+2);
		};

		auto& nb = board.nboard();
		// nb.print(std::cout);
		auto res0 = CellBoard::FindPlace(nb, sz);
		std::sort(res0.begin(), res0.end(), cmp);
		auto res1 = FindPlace(nb, sz);
		std::sort(res1.begin(), res1.end(), cmp);
		ASSERT_EQ(res1, res0);
	}
	TEST_F(Cell, Regular) {
		using RectV = std::vector<lubee::RectI>;
		using SizeV = std::vector<lubee::SizeI>;

		auto& rd = mt();
		// テストするエリアの広さ
		const auto size = RandomSize(rd, {1,32});

		// 初期エリアに適当に障害物を置く
		const size_t nObstacle = rd.getUniform<size_t>({0,16});
		RectV obstacle(nObstacle);
		for(size_t i=0 ; i<nObstacle ; i++) {
			obstacle[i] = RandomRect(rd, size, {1, std::max(size.width, size.height)});
		}
		CellBoard board(obstacle.data(), nObstacle, size);
		const size_t sizeArea = board.getNEmptyCell();

		// 入力する矩形サイズを生成
		const size_t nInput = rd.getUniform<size_t>({0,16});
		size_t inputArea = 0;
		SizeV inputS,		// placeに成功したサイズ
			  inputF;		// 失敗したサイズ
		for(size_t i=0 ; i<nInput ; i++) {
			// 最低面積1のサイズ
			const auto sz = RandomSize(rd, {1, size.width});
			if(board.place(sz)) {
				inputS.emplace_back(sz);
				// 総面積を記録しておく
				inputArea += sz.area();
			} else {
				inputF.emplace_back(sz);
			}
		}

		// 入力したsizeがそのまま記録されているか
		const auto& placed = board.placedRect();
		ASSERT_EQ(inputS.size(), placed.size());
		const auto ns = inputS.size();
		for(size_t i=0 ; i<ns ; i++) {
			ASSERT_EQ(inputS[i], placed[i].size());
		}

		// placedRect同士が重なっていないか
		for(size_t i=0 ; i<ns ; i++) {
			for(size_t j=i+1 ; j<ns ; j++) {
				ASSERT_FALSE(placed[i].hit(placed[j]));
			}
		}
		// 入力した面積 <= 対象矩形領域
		ASSERT_LE(inputArea, sizeArea);

		// 残りマス数の分だけ1x1の矩形を入力したら必ず全てが埋まる
		if(inputArea < sizeArea) {
			const lubee::SizeI single(1,1);
			auto remain = sizeArea - inputArea;
			while(remain != 0) {
				--remain;
				ASSERT_TRUE(board.place(single));
			}
			ASSERT_FALSE(board.place(single));
		}
	}
}
