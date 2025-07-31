export module ToyotaSedan;
import Toyota;
export class ToyotaSedan :public Toyota {
	// Í¨¹ý Toyota ¼Ì³Ð
	std::string info() const override
	{
		return "Toyota Sedan";
	}
};