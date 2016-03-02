#include "Cursor.h"

Cursor::Cursor(Manager* manager, const std::string& name){
	setAnimationType(STATES);
	applyAnimation(manager, name);
	viewRelative = true;
	scaleRelative = false;
	cb.offset = Vector(0.156f, 0.14f);
}

Cursor::~Cursor(){

}