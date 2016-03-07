#pragma once

#include <map>
#include <vector>

#include "ItemStack.h"

class Inventory {
public:
	Inventory();
	Inventory(const unsigned char& size);
	~Inventory();

	virtual std::vector<ItemStack> setSize(const unsigned char& size);
	unsigned char getSize() const;

	virtual ItemStack& put(ItemStack& is, unsigned char slot);
	virtual ItemStack& put(ItemStack& is);

	virtual bool swap(ItemStack& is, unsigned char slot);
	virtual ItemStack take(unsigned char slot);
	ItemStack& at(unsigned char slot);

	virtual bool take(ItemStack is);

	bool has(ItemStack is) const;

	virtual void clear();

	ItemStack* content;
private:
	unsigned char size;
	bool firstSet = true;
};