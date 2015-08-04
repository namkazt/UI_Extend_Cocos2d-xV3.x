#include "UIScrollPanelExtend.h"

USING_NS_CC;

UIScrollPanelExtend::UIScrollPanelExtend()
{
}

UIScrollPanelExtend::~UIScrollPanelExtend()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->eventListener);
}

UIScrollPanelExtend* UIScrollPanelExtend::create(int borderW, int borderH, int displayWidth, int displayHeight)
{
	UIScrollPanelExtend* scrollPanel = new (std::nothrow) UIScrollPanelExtend();
	scrollPanel->init(borderW, borderH, displayWidth, displayHeight);
	return scrollPanel;
}

void UIScrollPanelExtend::init(int borderW, int borderH, int displayWidth, int displayHeight)
{
	m_displaySize = Size(displayWidth, displayHeight);
	m_borderSize = Size(borderW, borderH);
	//////////////////////////////////////////////////
	// init object display
	this->displayArea = Node::create();
	this->displayArea->setAnchorPoint({ 0, 0 });
	this->displayArea->setPositionY(displayHeight);
	///////////////////////////////////////////////////
	// init clipper
	this->displayContainer = ClippingNode::create();
	this->displayContainer->setContentSize(Size(borderW, borderH));
	this->displayContainer->setAnchorPoint({ 0, 0 });
	this->displayContainer->addChild(this->displayArea);

	///////////////////////////////////////////////////
	// init stencil
	this->displayStencil = DrawNode::create();
	Vec2 rectangle[4];
	rectangle[0] = Vec2(0, 0);
	rectangle[1] = Vec2(displayWidth, 0);
	rectangle[2] = Vec2(displayWidth, displayHeight);
	rectangle[3] = Vec2(0, displayHeight);
	Color4F white(1, 1, 1, 1);
	this->displayStencil->drawPolygon(rectangle, 4, white, 1, white);
	this->displayContainer->setStencil(this->displayStencil);

	///////////////////////////////////////////////////
	// init event listener
	this->setupEventListener();
}

void UIScrollPanelExtend::initScrollBar()
{
	if (!usingScrollBar) return;

	//---- check if do not need to create scroll bar
	if (m_scrollMode == UI_EXTEND_VERTICAL){
		if (accumulatedSize.height <= m_displaySize.height) {
			canDisplayScrollbar = false;
			finalBarHeight = 0;
			return;
		}
	}
	else if (m_scrollMode == UI_EXTEND_HORIZONTAL){
		if (accumulatedSize.width <= m_displaySize.width){
			canDisplayScrollbar = false;
			finalBarHeight = 0;
			return;
		}
	}
	else {
		return;
	}
	//----- clean
	if (m_scrollBackground != NULL) {
		m_scrollBackground->removeFromParent();
		m_scrollBackground = NULL;
		m_scrollBar->removeFromParent();
		m_scrollBar = NULL;
	}
	//----- create new scroll bar if need
	canDisplayScrollbar = true;
	//---- calculate bar size and position it must to be
	int barBackgroundWidth = scrollBarWidth;
	int barBackgroundHeight = m_displaySize.height;
	int barWidth = barBackgroundWidth;
	float barHeight = (accumulatedSize.height - barBackgroundHeight) / barBackgroundHeight;
	if (!this->isUsingFixedSpeed){
		barHeight > 6 ? this->scrollSpeed = 6 : this->scrollSpeed = barHeight;
		if (this->scrollSpeed < 1.0f) this->scrollSpeed = 1.0f;
	}
	barHeight = (barHeight * 100 / 6);
	barHeight = (100 - barHeight) * (m_displaySize.height / 3 * 2 - 24) / 100;
	if (barHeight > m_displaySize.height / 3 * 2) barHeight = m_displaySize.height / 3 * 2;
	if (barHeight < 24) barHeight = 24;
	finalBarHeight = (int)barHeight;
	//----- draw bar
	m_scrollBar = UIUtilitiesExtend::createSpriteWithColor(Size(barWidth, barHeight), barColor);
	m_scrollBar->setAnchorPoint({ 0, 0 });
	this->displayContainer->addChild(m_scrollBar,2);
	//------ draw bar background
	m_scrollBackground = UIUtilitiesExtend::createSpriteWithColor(Size(barBackgroundWidth, barBackgroundHeight), barBackgroundColor);
	m_scrollBackground->setAnchorPoint({ 0, 0 });
	this->displayContainer->addChild(m_scrollBackground, 1);
	//-----------------------------------------------------------------------------------
	//-- update bar position 
	m_scrollBackground->setPosition(m_displaySize.width - barBackgroundWidth, m_offset.y);
	//-- calculate positon of bar should be
	float roadBarToGo = m_displaySize.height - finalBarHeight;
	float realRoadToGo = accumulatedSize.height - m_displaySize.height;
	float realMove = this->displayArea->getPositionY() - m_displaySize.height;
	float barMove = (realMove * 100 / realRoadToGo) * roadBarToGo / 100;
	//-- TODO: need to update position here
	m_scrollBar->setPosition(m_displaySize.width - barBackgroundWidth, m_offset.y + (m_displaySize.height - barHeight - barMove));

	//--- fade out 
	if (m_scrollBackground->getNumberOfRunningActions() == 0 && this->isAutoHideScrollBar)
	{
		this->m_scrollBackground->setOpacity(0);
		this->m_scrollBar->setOpacity(0);
	}
}

void UIScrollPanelExtend::updateSCrollBarPosition()
{
	if (canDisplayScrollbar && usingScrollBar){
		//-- calculate positon of bar should be
		float roadBarToGo = m_displaySize.height - finalBarHeight;
		float realRoadToGo = accumulatedSize.height - m_displaySize.height;
		float realMove = this->displayArea->getPositionY() - m_displaySize.height;
		float barMove = (realMove * 100 / realRoadToGo) * roadBarToGo / 100;
		m_scrollBar->setPositionY(m_offset.y + (m_displaySize.height - finalBarHeight - barMove));
	}
}

void UIScrollPanelExtend::setOffset(cocos2d::Vec2 offset)
{
	this->m_offset = offset;
	this->updateOffsetAndPos();
}

void UIScrollPanelExtend::setPosition(cocos2d::Vec2 pos)
{
	this->m_position = pos;
	this->updateOffsetAndPos();
}

void UIScrollPanelExtend::updateOffsetAndPos()
{
	this->displayArea->setPosition(this->m_offset + Vec2(0, m_displaySize.height));
	this->displayStencil->setPosition(this->m_offset);
	this->displayContainer->setPosition(this->m_position);
}

void UIScrollPanelExtend::setupEventListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		Vec2 locationInNode = touch->getLocation();
		Rect rect = Rect(m_position.x, m_position.y, m_displaySize.width, m_displaySize.height);
		if (rect.containsPoint(locationInNode))
		{
			if (this->isAutoHideScrollBar){
				/////////////////////////////////////////////////////
				// auto hide scrollbar
				if (this->m_scrollBackground->getNumberOfRunningActions() == 0){
					FadeTo* fadeIn = FadeTo::create(this->autoHideScrollTime, 255);
					fadeIn->setTag(1);
					this->m_scrollBackground->runAction(fadeIn);
					this->m_scrollBar->runAction(fadeIn->clone());
				}
				else{
					// tag == 1 is fade in | tag == 2 is fade out
					if (this->m_scrollBackground->getActionByTag(2) != NULL){
						this->m_scrollBackground->getActionByTag(2)->stop();
						FadeTo* fadeIn = FadeTo::create(this->autoHideScrollTime, 255);
						fadeIn->setTag(1);
						this->m_scrollBackground->runAction(fadeIn);
						this->m_scrollBar->runAction(fadeIn->clone());
					}
				}
			}

			this->m_isTouched = true;
			/////////////////////////////////////////////////////
			// callback when touch begin
			if (this->onTouchBegin != nullptr && this->m_isEnable){
				this->onTouchBegin(touch, event);
			}
			return true;
		}
		return false;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		Vec2 locationInNode = touch->getLocation();
		Rect rect = Rect(m_position.x, m_position.y, m_displaySize.width, m_displaySize.height);
		if (rect.containsPoint(locationInNode))
		{
			if (!m_isScrolling) m_isScrolling = true;
			checkHookUpdateScroll(touch->getDelta());
			/////////////////////////////////////////////////////
			// callback when touch move
			if (this->onTouchMove != nullptr && this->m_isEnable){
				this->onTouchMove(touch, event);
			}
		}
		else{
			m_isScrolling = false;
		}
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		if (m_isScrolling){
			m_isScrolling = false;
		}

		if (this->isAutoHideScrollBar){
			/////////////////////////////////////////////////////
			// auto hide scrollbar
			if (this->m_scrollBackground->getNumberOfRunningActions() == 0){
				FadeTo* fadeOut = FadeTo::create(this->autoHideScrollTime, 0);
				fadeOut->setTag(2);
				this->m_scrollBackground->runAction(fadeOut);
				this->m_scrollBar->runAction(fadeOut->clone());
			}
			else{
				// tag == 1 is fade in | tag == 2 is fade out
				if (this->m_scrollBackground->getActionByTag(1) != NULL){
					this->m_scrollBackground->getActionByTag(1)->stop();
					FadeTo* fadeOut = FadeTo::create(this->autoHideScrollTime, 0);
					fadeOut->setTag(2);
					this->m_scrollBackground->runAction(fadeOut);
					this->m_scrollBar->runAction(fadeOut->clone());
				}
			}
		}

		if (this->m_isTouched){
			this->m_isTouched = false;
			////////////////////////////////////////////////////
			// callback when touch end
			if (this->onTouchEnd != nullptr && this->m_isEnable){
				this->onTouchEnd(touch, event);
			}
		}
	};

	this->eventListener = listener;

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(this->eventListener, fixedPriority);
}

bool UIScrollPanelExtend::checkHookUpdateScroll(cocos2d::Vec2 location)
{
	this->getAccumulatedSize();

	if (m_isScrolling){
		switch (m_scrollMode)
		{
		case UIScrollPanelExtend::UI_EXTEND_HORIZONTAL:
		{
			int newX = this->displayArea->getPositionX() + location.x;
			if (newX > accumulatedSize.width - this->m_displaySize.width) newX = accumulatedSize.width - this->m_displaySize.width;
			if (newX < 0) newX = 0;
			this->displayArea->setPositionX(newX);

			this->updateSCrollBarPosition();
			break;
		}
		case UIScrollPanelExtend::UI_EXTEND_VERTICAL:{
			int newY = this->displayArea->getPositionY() + (location.y * scrollSpeed);
			if (newY > accumulatedSize.height + m_offset.y) newY = accumulatedSize.height + m_offset.y;
			if (newY < this->m_displaySize.height) newY = this->m_displaySize.height;
			this->displayArea->setPositionY(newY);

			this->updateSCrollBarPosition();
			break;
		}
		case UIScrollPanelExtend::UI_EXTEND_BOTH:{
			int newX = this->displayArea->getPositionX() + location.x;
			if (newX > accumulatedSize.width - this->m_displaySize.width) newX = accumulatedSize.width - this->m_displaySize.width;
			if (newX < 0) newX = 0;
			int newY = this->displayArea->getPositionY() + location.y;
			if (newY > accumulatedSize.height) newY = accumulatedSize.height;
			if (newY < this->m_displaySize.height) newY = this->m_displaySize.height;
			this->displayArea->setPosition(newX, newY);
			break;
		}
		}

		return true;
	}
	else{
		return false;
	}
}

cocos2d::Size UIScrollPanelExtend::getAccumulatedSize()
{
	float minX = 0;
	float minY = 0;
	float maxX = 0;
	float maxY = 0;

	for (int i = 0; i < this->displayArea->getChildrenCount(); i++)
	{
		Vec2 anchorPoint = this->displayArea->getChildren().at(i)->getAnchorPoint();
		Point pos = this->displayArea->getChildren().at(i)->getPosition();
		Size size = this->displayArea->getChildren().at(i)->getContentSize();
		if (pos.x - size.width * anchorPoint.x < minX) minX = pos.x - size.width * anchorPoint.x;
		if (pos.y - size.height * anchorPoint.y < minY) minY = pos.y - size.height * anchorPoint.y;
		if (pos.x - (size.width * (anchorPoint.x - 1)) > maxX) maxX = pos.x + size.width / 2;
		if (pos.y - (size.height * (anchorPoint.y - 1)) > maxY) maxY = pos.y + size.height / 2;
	}

	if (accumulatedSize.width != maxX - minX || accumulatedSize.height != maxY - minY)
	{
		accumulatedSize = Size(maxX - minX, maxY - minY);
		this->initScrollBar();
	}
	return Size(maxX - minX, maxY - minY);
}

void UIScrollPanelExtend::disableEventListener()
{
	this->eventListener->setEnabled(false);
}

void UIScrollPanelExtend::enableEventListener()
{
	this->eventListener->setEnabled(true);
}

void UIScrollPanelExtend::removeEventListener()
{
	this->eventListener->setEnabled(false);
	Director::getInstance()->getEventDispatcher()->removeEventListener(this->eventListener);
	this->eventListener = NULL;
}

void UIScrollPanelExtend::setFixedPriority(int newPriority)
{
	Director::getInstance()->getEventDispatcher()->setPriority(this->eventListener, newPriority);
	this->fixedPriority = newPriority;
}

void UIScrollPanelExtend::hookOnUIExtended(UIButtonExtend* button)
{
	button->onTouchBeginWarped = [=](Touch* touch, Event* event){
		if (this->isAutoHideScrollBar){
			/////////////////////////////////////////////////////
			// auto hide scrollbar
			if (this->m_scrollBackground->getNumberOfRunningActions() == 0){
				FadeTo* fadeIn = FadeTo::create(this->autoHideScrollTime, 255);
				fadeIn->setTag(1);
				this->m_scrollBackground->runAction(fadeIn);
				this->m_scrollBar->runAction(fadeIn->clone());
			}
			else{
				// tag == 1 is fade in | tag == 2 is fade out
				if (this->m_scrollBackground->getActionByTag(2) != NULL){
					this->m_scrollBackground->getActionByTag(2)->stop();
					FadeTo* fadeIn = FadeTo::create(this->autoHideScrollTime, 255);
					fadeIn->setTag(1);
					this->m_scrollBackground->runAction(fadeIn);
					this->m_scrollBar->runAction(fadeIn->clone());
				}
			}
		}
		return true;
	};
	button->onTouchMove = [=](Touch* touch, Event* event) {
		this->setScrolling(true);
		this->checkHookUpdateScroll(touch->getDelta());
	};
	button->onTouchEndWarped = [=](Touch* touch, Event* event) {
		if (!this->isScrolling()){
			return true;
		}
		this->setScrolling(false);

		if (this->isAutoHideScrollBar){
			/////////////////////////////////////////////////////
			// auto hide scrollbar
			if (this->m_scrollBackground->getNumberOfRunningActions() == 0){
				FadeTo* fadeOut = FadeTo::create(this->autoHideScrollTime, 0);
				fadeOut->setTag(2);
				this->m_scrollBackground->runAction(fadeOut);
				this->m_scrollBar->runAction(fadeOut->clone());
			}
			else{
				// tag == 1 is fade in | tag == 2 is fade out
				if (this->m_scrollBackground->getActionByTag(1) != NULL){
					this->m_scrollBackground->getActionByTag(1)->stop();
					FadeTo* fadeOut = FadeTo::create(this->autoHideScrollTime, 0);
					fadeOut->setTag(2);
					this->m_scrollBackground->runAction(fadeOut);
					this->m_scrollBar->runAction(fadeOut->clone());
				}
			}
		}

		return false;
	};
}

void UIScrollPanelExtend::removeAllChildren()
{
	if (this->displayArea != NULL && this->displayArea != nullptr)
	{
		this->displayArea->removeAllChildren();
	}
}

void UIScrollPanelExtend::removeAllChildrenWithCleanup(bool b)
{
	if (this->displayArea != NULL && this->displayArea != nullptr)
	{
		this->displayArea->removeAllChildrenWithCleanup(b);
	}
}
