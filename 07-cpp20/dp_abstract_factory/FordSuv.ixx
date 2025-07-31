export module FordSuv;
import Ford;
export class FordSuv :public Ford {
	// Í¨¹ý Ford ¼Ì³Ð
	std::string info() const override
	{
		return "Ford Suv";
	}
};