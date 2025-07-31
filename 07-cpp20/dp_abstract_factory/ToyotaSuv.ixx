export module ToyotaSuv;
import Toyota;
export class ToyotaSuv :public Toyota {
	// Í¨¹ý Toyota ¼Ì³Ð
	std::string info() const override
	{
		return "Toyota Suv";
	}
};