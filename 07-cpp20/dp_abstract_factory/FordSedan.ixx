export module FordSedan;

import Ford;
export class FordSedan :public Ford {
	// Í¨¹ý Ford ¼Ì³Ð
	std::string info() const override
	{
		return "Ford Sedan";
	}
};