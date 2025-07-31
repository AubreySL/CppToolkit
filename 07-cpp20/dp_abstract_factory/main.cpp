//import <iostream>;
//import <format>;
#include <iostream>
#include <format>
import IAbstractCarFactory;
import FordFactory;
import ToyotaFactory;

using namespace std;

void createSomeCards(IAbstractCarFactory& carFactory) {
	auto sedan{ carFactory.makeSedan() };
	auto suv{ carFactory.makeSuv() };
	cout << format("sedan: {}\n", sedan->info());
	cout << format("suv: {}\n", (*suv).info());
}

int main() {
	FordFactory fordFactory;
	ToyotaFactory toyotaFactory;
	createSomeCards(fordFactory);
	createSomeCards(toyotaFactory);
}