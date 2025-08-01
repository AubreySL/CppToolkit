import ToyotaFactory;
import FordFactory;
import LeastBusyFactory;
import <iostream>;
import <vector>;

using namespace std;
void callFn();

int main()
{
	ToyotaFactory myFactory;
	auto myCar{ myFactory.requestCar() };
	cout << myCar->info() << endl;
	cout << endl;
	callFn();
}

void callFn() {
	vector<unique_ptr<CarFactory>> factories;

	factories.push_back(make_unique<FordFactory>());
	factories.push_back(make_unique<FordFactory>());
	factories.push_back(make_unique<FordFactory>());
	factories.push_back(make_unique<ToyotaFactory>());

	factories[0]->requestCar();
	factories[0]->requestCar();
	factories[1]->requestCar();
	factories[3]->requestCar();

	LeastBusyFactory leastBusyFactory{ move(factories) };
	for (size_t i{ 0 },length{ 10 }; i < length; i++)
	{
		auto theCar{ leastBusyFactory.requestCar() };
		cout << theCar->info() << endl;
	}
}

