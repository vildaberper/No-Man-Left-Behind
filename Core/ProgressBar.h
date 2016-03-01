#pragma once

#include "SFMLI.h"
#include "TexBar.h"
#include "Vector.h"

class ProgressBar{
public:
	ProgressBar();
	~ProgressBar();

	Vector position;
	Vector size;

	TexBar* background = NULL;
	TexBar* progressbar = NULL;

	float progress = 0.0f;
};