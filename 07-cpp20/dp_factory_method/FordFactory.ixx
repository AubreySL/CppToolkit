export module FordFactory;
import CarFactory;
import ICar;
import Ford;
export class FordFactory :public CarFactory {
protected:
	// Í¨¹ý CarFactory ¼Ì³Ð
	std::unique_ptr<ICar> createCar() override
	{
		return std::make_unique<Ford>();
	}
};