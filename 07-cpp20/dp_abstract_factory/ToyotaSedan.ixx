export module ToyotaSedan;
import Toyota;
export class ToyotaSedan :public Toyota {
	// ͨ�� Toyota �̳�
	std::string info() const override
	{
		return "Toyota Sedan";
	}
};