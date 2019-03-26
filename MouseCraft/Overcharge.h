#pragma once
#include "Contraption.h"

class Mouse;

class Overcharge :
	public Contraption
{
public:
	Overcharge();
	~Overcharge();

	bool use(Mouse* m) override;
	void show() override;
};

