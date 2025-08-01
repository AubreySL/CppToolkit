export module ToyotaFactory;
import CarFactory;
import ICar;
import Toyota;
export class ToyotaFactory :public CarFactory {
protected:
	// Í¨¹ý CarFactory ¼Ì³Ð
	std::unique_ptr<ICar> createCar() override
	{
		return std::make_unique<Toyota>();
	}
};