#include "TestingFunctionScene.h"

USING_NS_CC;

Scene* TestingFunctionScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TestingFunctionScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool TestingFunctionScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	//------------------------------------------------------------------------
	auto verBG = Sprite::create("test_scene/ver_bg.png");
	verBG->setAnchorPoint({ 0, 0 });
	verBG->setPosition(12, 10);
	this->addChild(verBG);

	auto hozBG = Sprite::create("test_scene/hoz_bg.png");
	hozBG->setAnchorPoint({ 0, 0 });
	hozBG->setPosition(257, 416);
	this->addChild(hozBG);

	auto bothBG = Sprite::create("test_scene/both_bg.png");
	bothBG->setAnchorPoint({ 0, 0 });
	bothBG->setPosition(257, 10);
	this->addChild(bothBG);

	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("test_scene/ver_item.png", Rect(0, 0, 186, 87)), "ver_item");
	SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("test_scene/hoz_item.png", Rect(0, 0, 128, 171)), "hoz_item");
	//------------------------------------------------------------------------
	//-- testing scroll box extend
	verScroll = UIScrollPanelExtend::create(223, 617, 209, 598);
	verScroll->setScrollMode(UIScrollPanelExtend::UI_EXTEND_VERTICAL);
	verScroll->setUseScrollBar(true);
	verScroll->setAutoHideScrollBar(true, 0.5f);
	verScroll->setScrollSpeed(2.5f); //nếu không set fix speed thì hệ thống sẽ tự tính speed
	verScroll->setScrollBarWidth(10);
	verScroll->setScrollBarColor({ 67, 209, 136, 255 }, {0, 99, 48, 255});
	verScroll->setOffset({ 14 / 2, 19 / 2 });
	verScroll->setPosition(verBG->getPosition());
	verScroll->setParent(this);
	//-- test add items
	for (int i = 0; i < 50; i++)
	{
		UIButtonExtend* button = UIButtonExtend::create("ver_item", "ver_item", "ver_item");
		button->setPosition(Vec2( 3, -95 * i ));
		verScroll->addChild(button);
		button->onTouchEnd = [=](Touch* touch, Event* event){
			log("Click on item vertical: %i", i + 1);
		};
	}


	scheduleUpdate();
	return true;
}

void TestingFunctionScene::update(float dt)
{
}
