export module Toyota;
import ICar;
export class Toyota:public ICar{
public:
	// ͨ�� ICar �̳�
	std::string info() const override
	{
		return "Toyota";
	}
};