export module CarFactory;
import ICar;
import <memory>;

export class CarFactory {
public:
	virtual ~CarFactory() = default;

	std::unique_ptr<ICar> requestCar()
	{
		++m_numberOfCarsProduced;
		return createCar();
	}
	size_t getNumberOfCarsProduced() const { return m_numberOfCarsProduced; }
protected:
	virtual std::unique_ptr<ICar> createCar() = 0;
private:
	size_t m_numberOfCarsProduced{ 0 };
};