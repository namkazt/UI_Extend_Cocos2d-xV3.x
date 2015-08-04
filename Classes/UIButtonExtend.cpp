#include "UIButtonExtend.h"

USING_NS_CC;

UIButtonExtend::UIButtonExtend()
{
}

UIButtonExtend::~UIButtonExtend()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->eventListener);
}

UIButtonExtend* UIButtonExtend::create(std::string normalSpriteName, std::string selectSpriteName, std::string disableSpriteName)
{
	UIButtonExtend* button = new (std::nothrow) UIButtonExtend();
	button->setNormalSpriteName(normalSpriteName);
	button->setDisableSpriteName(disableSpriteName);
	button->setSelectSpriteName(selectSpriteName);
	button->init();
	return button;
}

void UIButtonExtend::init()
{
	this->objDisplay = Sprite::createWithSpriteFrameName(normalSpriteName);
	this->objDisplay->setAnchorPoint({ 0, 1 });
	///////////////////////////////////////////////////
	// init event listener
	this->setupEventListener();
}

void UIButtonExtend::setupEventListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		Vec2 locationInNode = this->getObjDisplay()->convertToNodeSpace(touch->getLocation());
		Size s = this->getObjDisplay()->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			this->setSelected(true);
			this->m_isTouched = true;
			if (this->onTouchBeginWarped != nullptr)
			{
				if (this->onTouchBeginWarped(touch, event))
				{
					/////////////////////////////////////////////////////
					// callback when touch begin
					if (this->onTouchBegin != nullptr && this->m_isEnable){
						this->onTouchBegin(touch, event);
					}
					return true;
				}
				return false;
			}
			else{
				/////////////////////////////////////////////////////
				// callback when touch begin
				if (this->onTouchBegin != nullptr && this->m_isEnable){
					this->onTouchBegin(touch, event);
				}
				return true;
			}
		}
		return false;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		if (this->onTouchMoveWarped != nullptr)
		{
			if (this->onTouchMoveWarped(touch, event))
			{
				/////////////////////////////////////////////////////
				// callback when touch move
				if (this->onTouchMove != nullptr && this->m_isEnable){
					this->onTouchMove(touch, event);
				}
			}
		}
		else{
			/////////////////////////////////////////////////////
			// callback when touch move
			if (this->onTouchMove != nullptr && this->m_isEnable){
				this->onTouchMove(touch, event);
			}
		}
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		if (this->m_isTouched){
			this->setSelected(false);
			this->m_isTouched = false;

			if (this->onTouchEndWarped != nullptr)
			{
				if (this->onTouchEndWarped(touch, event))
				{
					////////////////////////////////////////////////////
					// callback when touch end
					if (this->onTouchEnd != nullptr && this->m_isEnable){
						this->onTouchEnd(touch, event);
					}
				}
			}
			else{
				////////////////////////////////////////////////////
				// callback when touch end
				if (this->onTouchEnd != nullptr && this->m_isEnable){
					this->onTouchEnd(touch, event);
				}
			}
		}
	};

	this->eventListener = listener;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->eventListener, fixedPriority);
	
}

void UIButtonExtend::disableEventListener()
{
	this->eventListener->setEnabled(false);
}

void UIButtonExtend::enableEventListener()
{
	this->eventListener->setEnabled(true);
}


void UIButtonExtend::removeEventListener()
{
	this->eventListener->setEnabled(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->eventListener);
	this->eventListener = NULL;
}


void UIButtonExtend::setEnable(bool enable)
{
	if (enable && enable != this->m_isEnable){
		this->objDisplay->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(this->normalSpriteName));
	}
	if (!enable && enable != this->m_isEnable){
		this->objDisplay->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(this->disableSpriteName));
	}
	this->m_isEnable = enable;
}

void UIButtonExtend::setSelected(bool selected)
{
	if (!this->m_isEnable) return;

	if (selected && selected != this->m_isSelected){
		this->objDisplay->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(this->selectSpriteName));
	}
	if (!selected && selected != this->m_isSelected){
		this->objDisplay->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(this->normalSpriteName));
	}
	this->m_isSelected = selected;
}
