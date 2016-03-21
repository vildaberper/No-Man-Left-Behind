#include "CursorSet.h"

CursorSet::CursorSet(Manager* manager){
	main_ = new Cursor(manager, "cursorMain");
	box_ = new Cursor(manager, "cursorBox");
	box_->cb.offset = Vector(0.5f, 0.5f);
	hand_ = new Cursor(manager, "cursorHand");
	truck_ = new Cursor(manager, "cursorTruck");
	truck_->cb.offset = Vector(0.5f, 0.5f);
}

CursorSet::~CursorSet(){
	gi::cursor = nullptr;
	delete main_;
	delete box_;
	delete hand_;
}

void CursorSet::main(){
	gi::cursor = main_;
	gi::showCursor = true;
}
void CursorSet::box(){
	gi::cursor = box_;
	gi::showCursor = true;
}
void CursorSet::hand(){
	gi::cursor = hand_;
	gi::showCursor = true;
}
void CursorSet::truck(){
	gi::cursor = truck_;
	gi::showCursor = true;
}

void CursorSet::hide(){
	gi::showCursor = false;
}