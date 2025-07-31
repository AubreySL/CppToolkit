export module ICar;

import <string>;
export class ICar
{
public:
	virtual ~ICar() = default;
	virtual std::string info() const = 0;
};