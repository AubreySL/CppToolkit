export module FordFactory;
import CarFactory;
import ICar;
import Ford;
export class FordFactory :public CarFactory {
protected:
	// ͨ�� CarFactory �̳�
	std::unique_ptr<ICar> createCar() override
	{
		return std::make_unique<Ford>();
	}
};