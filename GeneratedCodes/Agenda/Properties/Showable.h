#ifndef __SHOWABLE_H__
#define __SHOWABLE_H__
class Showable
{

public:
	virtual ~Showable() {}

	virtual void showInfo() = 0;
};

#endif

