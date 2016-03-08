#pragma once

#include <map>

#include "Item.h"

namespace itemstack{
	static std::map<Resource, unsigned char> createStackLimits(){
		std::map<Resource, unsigned char> r;

		r[PENICILLIN] = 5;
		r[FORCEPS] = 5;
		r[ALCOHOL] = 5;
		r[MORPHINE] = 5;
		r[SUTURE_KIT] = 5;
		r[SCALPEL] = 5;
		r[GAUZE] = 5;

		return r;
	}

	static const std::map<Resource, unsigned char> stackLimits = itemstack::createStackLimits();
}

static const unsigned char stackLimit(const Resource& r){
	if(itemstack::stackLimits.count(r) > 0){
		return itemstack::stackLimits.at(r);
	}
	return 1;
}

class ItemStack{
public:
	ItemStack();
	ItemStack(Resource resource, unsigned char amount);
	ItemStack(Item item, unsigned char amount);
	~ItemStack();

	Item item;

	unsigned char amount;
};