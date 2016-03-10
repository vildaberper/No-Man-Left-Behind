#include "TexBar.h"

TexBar::TexBar(){

}

TexBar::TexBar(sf::Texture* left, sf::Texture* middle, sf::Texture* right){
	TexBar::left = left;
	TexBar::middle = middle;
	TexBar::right = right;
}

TexBar::~TexBar(){

}

bool TexBar::isValid(){
	return (left != NULL && middle != NULL && right != NULL) || middle != NULL;
}