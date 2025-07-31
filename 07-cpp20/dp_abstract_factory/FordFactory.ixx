export module FordFactory;
import IAbstractCarFactory;
import FordSuv;
import FordSedan;

export class FordFactory :public IAbstractCarFactory {
public:
	// ͨ�� IAbstractCarFactory �̳�
	std::unique_ptr<ICar> makeSuv() override
	{
		return std::make_unique<FordSuv>();
	}

	std::unique_ptr<ICar> makeSedan() override
	{
		return std::make_unique<FordSedan>();
	}
};