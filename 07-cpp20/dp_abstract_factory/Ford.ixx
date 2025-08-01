export module Ford;
import ICar;
export class Ford : public ICar {
public:
	// Í¨¹ý ICar ¼Ì³Ð
	std::string info() const override
	{
		return "Ford";
	}
};