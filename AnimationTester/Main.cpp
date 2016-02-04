#include "AnimationTester.h"

int main(){
	AnimationTester* animationTester = new AnimationTester();

	animationTester->run();

	delete animationTester;

	return 0;
}