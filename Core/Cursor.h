#pragma once

#include "Animatable.h"

class Cursor : public Animatable{
public:
	Cursor(Manager* manager, const std::string& name);
	virtual ~Cursor();
};