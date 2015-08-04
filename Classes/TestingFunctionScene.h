#ifndef __TESING_FUNCTION_SCENE_H__
#define __TESING_FUNCTION_SCENE_H__

#include "cocos2d.h"
//--- extend UI lib
#include "UIScrollPanelExtend.h"
#include "UIButtonExtend.h"

class TestingFunctionScene : public cocos2d::Layer
{
protected:
	UIScrollPanelExtend* verScroll;
	UIScrollPanelExtend* hozScroll;
	UIScrollPanelExtend* bothScroll;

public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void update(float dt);

	// implement the "static create()" method manually
	CREATE_FUNC(TestingFunctionScene);
};

#endif
