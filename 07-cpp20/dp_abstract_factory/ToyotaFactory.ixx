export module ToyotaFactory;

import IAbstractCarFactory;
import ToyotaSedan;
import ToyotaSuv;

export class ToyotaFactory :public IAbstractCarFactory {
public:
	// Í¨¹ý IAbstractCarFactory ¼Ì³Ð
	std::unique_ptr<ICar> makeSuv() override
	{
		return std::make_unique<ToyotaSuv>();
	}

	std::unique_ptr<ICar> makeSedan() override
	{
		return std::make_unique<ToyotaSedan>();
	}

};