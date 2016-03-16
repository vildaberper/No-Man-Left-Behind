#pragma once

#include "Cursor.h"
#include "Manager.h"

class CursorSet{
public:
	CursorSet(Manager* manager);
	~CursorSet();

	void main();
	void box();
	void hand();
	void truck();

	void hide();
private:
	Cursor* main_;
	Cursor* box_;
	Cursor* hand_;
	Cursor* truck_;
};