export module Toyota;
import ICar;
export class Toyota:public ICar{
public:
	// Í¨¹ý ICar ¼Ì³Ð
	std::string info() const override
	{
		return "Toyota";
	}
};