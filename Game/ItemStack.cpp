#include "ItemStack.h"

ItemStack::ItemStack(){
	amount = 0;
}

ItemStack::ItemStack(Resource resource, unsigned char amount){
	item = Item(resource);
	ItemStack::amount = amount;
}

ItemStack::ItemStack(Item item, unsigned char amount){
	ItemStack::item = item;
	ItemStack::amount = amount;
}

ItemStack::~ItemStack(){

}