export module FordSedan;

import Ford;
export class FordSedan :public Ford {
	// ͨ�� Ford �̳�
	std::string info() const override
	{
		return "Ford Sedan";
	}
};