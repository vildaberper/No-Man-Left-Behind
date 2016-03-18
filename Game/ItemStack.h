#pragma once

#include <map>

#include "Item.h"
#include "GameConstants.h"

static const unsigned char stackLimit(const Resource& r){
	switch(r){
	case PENICILLIN:
		return unsigned char(gc::stackSizePennicillin);
		break;
	case FORCEPS:
		return unsigned char(gc::stackSizeForceps);
		break;
	case ALCOHOL:
		return unsigned char(gc::stackSizeAlcohol);
		break;
	case MORPHINE:
		return unsigned char(gc::stackSizeMorphine);
		break;
	case SUTURE_KIT:
		return unsigned char(gc::stackSizeSutureKit);
		break;
	case SCALPEL:
		return unsigned char(gc::stackSizeScalpel);
		break;
	case GAUZE:
		return unsigned char(gc::stackSizeGauze);
		break;
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