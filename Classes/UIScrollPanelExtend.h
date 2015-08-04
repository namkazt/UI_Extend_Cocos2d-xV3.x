#ifndef __UI_SCROLL_PANEL_EXTEND_H__
#define __UI_SCROLL_PANEL_EXTEND_H__

#include "UIButtonExtend.h"
#include "UIUtilitiesExtend.h"

class UIScrollPanelExtend
{
public:
	enum UI_EXTEND_SCROLL_MODE {
		UI_EXTEND_HORIZONTAL,
		UI_EXTEND_VERTICAL,
		UI_EXTEND_BOTH
	};

protected:
	//////////////////////////////////////////////////////
	// scroll bar
	bool isAutoHideScrollBar = false;
	float autoHideScrollTime = 0.5f;
	bool usingScrollBar = false;
	bool canDisplayScrollbar = false;
	bool isUsingFixedSpeed = false;
	int finalBarHeight = 0;
	int scrollBarWidth = 8;
	cocos2d::Color4B barColor{0,0,0,255};
	cocos2d::Color4B barBackgroundColor{54,54,54,255};
	cocos2d::Sprite* m_scrollBackground = NULL;
	cocos2d::Sprite* m_scrollBar = NULL;
	float scrollSpeed = 1.0f;
	void initScrollBar();
	void updateSCrollBarPosition();
	/////////////////////////////////////////////////////
	cocos2d::Node* displayArea = NULL;
	cocos2d::ClippingNode* displayContainer = NULL;
	cocos2d::DrawNode* displayStencil = NULL;
	cocos2d::Node* parent = NULL;
	////////////////////////////////////////////////////
	cocos2d::EventListener* eventListener;
	int fixedPriority = -5;
	////////////////////////////////////////////////////
	bool m_isTouched = false;
	bool m_isSelected = false;
	bool m_isEnable = true;
	bool m_isScrolling = false;
	////////////////////////////////////////////////////
	UI_EXTEND_SCROLL_MODE m_scrollMode = UI_EXTEND_HORIZONTAL;
	cocos2d::Size m_displaySize;
	cocos2d::Size m_borderSize;;
	cocos2d::Vec2 m_position;
	cocos2d::Vec2 m_offset;
	void updateOffsetAndPos();
	cocos2d::Size accumulatedSize;
	cocos2d::Size getAccumulatedSize();
	void setScrolling(bool scrolling) { this->m_isScrolling = scrolling; };
	bool checkHookUpdateScroll(cocos2d::Vec2 location);
	void hookOnUIExtended(UIButtonExtend* button);
public:
	UIScrollPanelExtend();
	~UIScrollPanelExtend();
	////////////////////////////////////////////////////
	/*
	@Note: create a normal button
	@param:
	std::string normalSpriteName: frame name of this button to display as normal
	std::string disableSpriteName: frame name of this button to display as disable
	*/
	static UIScrollPanelExtend* create(int borderW, int borderH, int displayWidth, int displayHeight);
	////////////////////////////////////////////////////
	void init(int borderW, int borderH, int displayWidth, int displayHeight);
	void update(float dt);
	void setOffset(cocos2d::Vec2 offset);
	void setPosition(cocos2d::Vec2 position);
	void setScrollMode(UI_EXTEND_SCROLL_MODE mode){ this->m_scrollMode = mode; };
	////////////////////////////////////////////////////
	typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> touchCallback;
	/*
	@Note: set this callback to able to access when touch in button (finger on)
	*/
	touchCallback onTouchBegin = nullptr;
	touchCallback onTouchMove = nullptr;
	touchCallback onTouchEnd = nullptr;
	/*
	@Note: function Event listener
	*/
	void disableEventListener();
	void enableEventListener();
	void removeEventListener();
	void setupEventListener();
	////////////////////////////////////////////////////
	// add display to parent
	void setParent(cocos2d::Node* parent) { parent->addChild(this->displayContainer); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder) { parent->addChild(this->displayContainer, localZOrder); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder, int tag) { parent->addChild(this->displayContainer, localZOrder, tag); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder, std::string name) { parent->addChild(this->displayContainer, localZOrder, name); this->parent = parent; };

	////////////////////////////////////////////////////
	// add child 
	void addChild(cocos2d::Node* child) { this->displayArea->addChild(child); this->getAccumulatedSize(); };
	void addChild(cocos2d::Node* child, int localZOrder) { this->displayArea->addChild(child, localZOrder); this->getAccumulatedSize(); };
	void addChild(cocos2d::Node* child, int localZOrder, int tag){ this->displayArea->addChild(child, localZOrder, tag); this->getAccumulatedSize(); };
	void addChild(cocos2d::Node* child, int localZOrder, std::string name) { this->displayArea->addChild(child, localZOrder, name); this->getAccumulatedSize(); };
	void addChild(UIButtonExtend* child) { this->displayArea->addChild(child->getObjDisplay()); this->hookOnUIExtended(child); this->getAccumulatedSize(); };
	void addChild(UIButtonExtend* child, int localZOrder) { this->displayArea->addChild(child->getObjDisplay(), localZOrder); this->hookOnUIExtended(child); this->getAccumulatedSize(); };
	void addChild(UIButtonExtend* child, int localZOrder, int tag){ this->displayArea->addChild(child->getObjDisplay(), localZOrder, tag); this->hookOnUIExtended(child); this->getAccumulatedSize(); };
	void addChild(UIButtonExtend* child, int localZOrder, std::string name) { this->displayArea->addChild(child->getObjDisplay(), localZOrder, name); this->hookOnUIExtended(child); this->getAccumulatedSize(); };
	////////////////////////////////////////////////////
	void setEnable(bool _enable) { this->m_isEnable = _enable; };
	bool isEnable() { return this->m_isEnable; };
	bool isScrolling() { return this->m_isScrolling; };
	void setFixedPriority(int newPriority);
	void setUseScrollBar(bool _enable) { this->usingScrollBar = _enable; };
	void setAutoHideScrollBar(bool _enable) { this->isAutoHideScrollBar = _enable; if(!_enable){ this->m_scrollBackground->setOpacity(255); this->m_scrollBar->setOpacity(255); } };
	void setAutoHideScrollBar(bool _enable, float _time) { this->setAutoHideScrollBar(_enable); this->autoHideScrollTime = _time; };
	float getScrollSpeed(){ return this->scrollSpeed; };
	void setScrollSpeed(float speed){ this->scrollSpeed = speed; this->isUsingFixedSpeed = true; };
	void setScrollBarWidth(int w){ this->scrollBarWidth = w; };
	void setScrollBarColor(cocos2d::Color4B _barColor, cocos2d::Color4B _barBackgroundColor){ this->barColor = _barColor; this->barBackgroundColor = _barBackgroundColor; };
	////////////////////////////////////////////////////
	void removeAllChildren();
	void removeAllChildrenWithCleanup(bool b);
};

#endif