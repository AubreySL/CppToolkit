export module IAbstractCarFactory;
import ICar;
import <memory>;
export class IAbstractCarFactory {
public:
	virtual ~IAbstractCarFactory() = default;
	virtual std::unique_ptr<ICar> makeSuv() = 0;
	virtual std::unique_ptr<ICar> makeSedan() = 0;
};