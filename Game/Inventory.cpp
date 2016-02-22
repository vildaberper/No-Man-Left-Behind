#include "Inventory.h"

Inventory::Inventory(const unsigned char size){
	Inventory::size = size;
	content = new ItemStack[size];
}
Inventory::~Inventory(){
	delete[] content;
}

std::vector<ItemStack> Inventory::setSize(unsigned char size){
	ItemStack* old = content;
	content = new ItemStack[size];
	std::vector<ItemStack> left;

	for(int i = 0; i < size; i++){
		if(i < Inventory::size){
			content[i] = old[i];
		}
		else{
			left.push_back(ItemStack(old[i].item, old[i].amount));
		}
	}
	delete[] old;

	return left;
}
unsigned char Inventory::getSize() const{
	return size;
}

ItemStack& Inventory::put(ItemStack& is, unsigned char slot){
	if(slot >= size){
		return is;
	}

	if(content[slot].item.type != is.item.type){
		if(content[slot].amount == 0){
			content[slot].item.type = is.item.type;
		}
		else{
			return is;
		}
	}

	int total = content[slot].amount + is.amount;
	int left = total - stackLimit(is.item.type);

	left = left > 0 ? left : 0;
	content[slot].amount = total - left;
	is.amount = left;

	return is;
}
ItemStack& Inventory::put(ItemStack& is){
	for(size_t i = 0; i < size && is.amount > 0; i++){
		put(is, unsigned char(i));
	}

	return is;
}

ItemStack& Inventory::swap(ItemStack& is, unsigned char slot){
	if(slot >= size)
		return ItemStack();
	if(is.amount > stackLimit(is.item.type)){
		return is;
	}

	ItemStack r = content[slot];
	content[slot] = is;

	return r;
}
ItemStack& Inventory::take(unsigned char slot){
	return swap(ItemStack(), slot);
}

bool Inventory::take(ItemStack is){
	if(!has(is)){
		return false;
	}

	for(size_t i = 0; i < size && is.amount > 0; i++){
		if(content[i].item.type == is.item.type){
			if(content[i].amount >= is.amount){
				content[i].amount -= is.amount;
				is.amount = 0;
			}
			else{
				is.amount -= content[i].amount;
				content[i].amount = 0;
			}
		}
	}

	return true;
}

bool Inventory::has(ItemStack is) const{
	int total = 0;

	for(size_t i = 0; i < size && total < is.amount; i++){
		if(content[i].item.type == is.item.type){
			total += content[i].amount;
		}
	}

	return total >= is.amount;
}

void Inventory::clear(){
	for(size_t i = 0; i < size; i++){
		content[i].amount = 0;
	}
}