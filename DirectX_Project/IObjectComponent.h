#pragma once

#include <string>

class IObjectComponent {
private:
	std::string name;
protected:
	IObjectComponent(std::string n) :
		name(n) {}
public:	
	virtual ~IObjectComponent() {}
	std::string GetName() {
		return name;
	}
};