#include "lubee/src/tests/test.hpp"
#include "../genetic/src/path/gene.hpp"
#include "../genetic/src/environment.hpp"
#include "../genetic/src/path/cross/pmx.hpp"
#include "../genetic/src/generation/jgg.hpp"
#include "../genetic/src/bernoulli.hpp"
#include "../genetic/src/mutate/swap.hpp"
#include "../fit_cell.hpp"

namespace dg::test {
	using WithGenetic = lubee::test::Random;
	TEST_F(WithGenetic, DISABLED_Packing) {
		auto& mt = this->mt().refMt();
		const auto randI = [&mt=this->mt()](auto... arg){
			return mt.getUniform<size_t>({arg...});
		};

		using Gene = gene::path::VariableGene<int>;
		using PMX = gene::path::cross::PartiallyMapped;
		using Mutate = gene::Bernoulli<gene::mutate::Swap>;
		using Pool_t = gene::Pool<Gene, Fit_Cell>;
		using Env_t = gene::Environment<std::mt19937, Gene, Pool_t, PMX, Mutate, gene::JustGenerationGap>;

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
		Env_t env(
			mt,
			Pool_t{mt, fit, gene::NoClip{}, Population, GeneLen},
			PMX{},
			Mutate{MutateP, {}},
			gene::JustGenerationGap{NParent, NChild}
		);
		for(int i=0 ; i<128 ; i++) {
			env.advance();
			auto& best = env.getBest();
			fit(best.gene);
			std::cout << best.score << std::endl;
		}
	}
}
