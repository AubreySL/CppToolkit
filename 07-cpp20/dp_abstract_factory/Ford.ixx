export module Ford;
import ICar;
export class Ford : public ICar {
public:
	// ͨ�� ICar �̳�
	std::string info() const override
	{
		return "Ford";
	}
};