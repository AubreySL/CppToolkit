export module FordSuv;
import Ford;
export class FordSuv :public Ford {
	// ͨ�� Ford �̳�
	std::string info() const override
	{
		return "Ford Suv";
	}
};