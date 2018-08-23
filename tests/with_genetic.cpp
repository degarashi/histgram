#include "lubee/tests/test.hpp"
#include "../genetic/gene_order.hpp"
#include "../genetic/environment.hpp"
#include "../genetic/pmx.hpp"
#include "../genetic/jgg.hpp"
#include "../genetic/bernoulli.hpp"
#include "../genetic/swap.hpp"
#include "../fit_cell.hpp"

namespace dg::test {
	using WithGenetic = lubee::test::Random;
	TEST_F(WithGenetic, DISABLED_Packing) {
		auto& mt = this->mt().refMt();
		const auto randI = [&mt=this->mt()](auto... arg){
			return mt.getUniform<size_t>({arg...});
		};

		using Gene = gene::order::path::VariableGene<int>;
		using PMX = gene::order::cross::PartiallyMapped;
		using Mutate = gene::order::Bernoulli<gene::order::mutate::Swap>;
		using Env_t = gene::Environment<std::mt19937, Gene, Fit_Cell, PMX, Mutate, gene::JustGenerationGap>;

		const size_t NRect = randI(1, 32),
					GeneLen = 4,
					Population = 128,
					NParent = 32,
					NChild = 64;
		constexpr double MutateP = 0.01;

		const SizeV toPlace{
			{2,2},
			{1,2},
			{3,1},
			{1,1}
		};
		const lubee::SizeI size{3,3};
		Fit_Cell fit(toPlace, CellBoard(size), 1.0, 1.0);
		Env_t env(mt, fit, {}, {MutateP, {}}, {NParent, NChild}, Population, GeneLen);
		for(int i=0 ; i<128 ; i++) {
			env.advance();
			auto& best = env.getBest();
			fit(best.gene);
			std::cout << best.score << std::endl;
		}
	}
}
