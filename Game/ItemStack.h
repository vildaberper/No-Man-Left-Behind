#pragma once

#include <map>

#include "Item.h"

namespace itemstack{
	static std::map<Resource, unsigned char> createStackLimits(){
		std::map<Resource, unsigned char> r;

		r[PENICILLIN] = 16;
		r[FORCEPS] = 16;
		r[ALCOHOL] = 16;
		r[MORPHINE] = 16;
		r[SUTURE_KIT] = 16;
		r[SCALPEL] = 16;
		r[GAUZE] = 16;

		return r;
	}

	static const std::map<Resource, unsigned char> stackLimits = itemstack::createStackLimits();
}

static const unsigned char stackLimit(const Resource& r){
	if(itemstack::stackLimits.count(r) > 0){
		return itemstack::stackLimits.at(r);
	}
	return 16;
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