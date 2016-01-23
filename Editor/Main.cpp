#include "Editor.h"

int main(){
	Editor* editor = new Editor();

	editor->run();

	delete editor;

	return 0;
}