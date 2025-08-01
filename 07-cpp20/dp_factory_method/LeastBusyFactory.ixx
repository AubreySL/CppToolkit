export module LeastBusyFactory;
import CarFactory;
import <memory>;
import <vector>;
import <stdexcept>;
export class LeastBusyFactory :public CarFactory {
public:
	explicit LeastBusyFactory(std::vector<std::unique_ptr<CarFactory>>&& factories);
protected:
	// Í¨¹ý CarFactory ¼Ì³Ð
	std::unique_ptr<ICar> createCar() override;
	
private:
	std::vector<std::unique_ptr<CarFactory>> m_factories;
};

LeastBusyFactory::LeastBusyFactory(std::vector<std::unique_ptr<CarFactory>>&& factories) :m_factories{ std::move(factories) }
{
	if (m_factories.empty())
	{
		throw std::runtime_error{ "No factories provided." };
	}
}

std::unique_ptr<ICar> LeastBusyFactory::createCar()
{

	CarFactory* bestSoFar{ m_factories[0].get() };
	for (auto& factory : m_factories) {
		if (factory->getNumberOfCarsProduced() < bestSoFar->getNumberOfCarsProduced()) {
			bestSoFar = factory.get();
		}
	}
	return bestSoFar->requestCar();
}
