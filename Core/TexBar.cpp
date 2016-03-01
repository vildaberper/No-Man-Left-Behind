#include "TexBar.h"

TexBar::TexBar(){

}

TexBar::~TexBar(){

}

bool TexBar::isValid(){
	return left != NULL && middle != NULL && right != NULL;
}