export module ToyotaSuv;
import Toyota;
export class ToyotaSuv :public Toyota {
	// ͨ�� Toyota �̳�
	std::string info() const override
	{
		return "Toyota Suv";
	}
};