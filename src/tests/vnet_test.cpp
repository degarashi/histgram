#include "../vnet.hpp"
#include "lubee/src/tests/test.hpp"

namespace dg::test {
	using VNet = lubee::test::Random;
	TEST_F(VNet, Regular) {
		auto& rd = mt();
		const auto n = rd.getUniform<size_t>({1, 128});
		std::vector<lubee::RectI> rect(n);
		for(auto& r : rect) {
			constexpr lubee::RangeI Range = {-8192, 8192};
			{
				const int pos[2] = {
					rd.getUniform<int>(Range),
					rd.getUniform<int>(Range)
				};
				r.x0 = std::min(pos[0], pos[1]);
				r.x1 = std::max(pos[0], pos[1]);
			}
			{
				const int pos[2] = {
					rd.getUniform<int>(Range),
					rd.getUniform<int>(Range)
				};
				r.y0 = std::min(pos[0], pos[1]);
				r.y1 = std::max(pos[0], pos[1]);
			}
		}
		auto itr = rect.begin();
		::dg::VNet vnet0(*itr++);
		while(itr != rect.end()) {
			vnet0.takeIn(*itr++);
		}
		std::shuffle(rect.begin(), rect.end(), mt().refMt());

		itr = rect.begin();
		::dg::VNet vnet1(*itr++);
		while(itr != rect.end()) {
			vnet1.takeIn(*itr++);
		}

		ASSERT_EQ(vnet1, vnet0);
		ASSERT_EQ(vnet1.getLength(), vnet0.getLength());
	}
}
