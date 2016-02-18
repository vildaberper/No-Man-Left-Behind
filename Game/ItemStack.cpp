#include "ItemStack.h"

ItemStack::ItemStack(){
	amount = 0;
}

ItemStack::ItemStack(Item item, unsigned char amount){
	ItemStack::item = item;
	ItemStack::amount = amount;
}

ItemStack::~ItemStack(){

}