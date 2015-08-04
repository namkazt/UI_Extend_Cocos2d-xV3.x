#ifndef __UI_BUTTON_EXTEND_H__
#define __UI_BUTTON_EXTEND_H__

#include "cocos2d.h"

class UIButtonExtend
{
	CC_SYNTHESIZE(cocos2d::Sprite*, objDisplay, ObjDisplay);
	CC_SYNTHESIZE(std::string, normalSpriteName, NormalSpriteName);
	CC_SYNTHESIZE(std::string, disableSpriteName, DisableSpriteName);
	CC_SYNTHESIZE(std::string, selectSpriteName, SelectSpriteName);
protected:
	cocos2d::Node* parent;
	cocos2d::EventListener* eventListener;
	int fixedPriority = -10;
	std::string name;
	bool m_isTouched = false;
	bool m_isEnable = true;
	bool m_isSelected = false;
public:
	UIButtonExtend();
	~UIButtonExtend();
	////////////////////////////////////////////////////
	/*
	@Note: create a normal button
	@param:
	std::string normalSpriteName: frame name of this button to display as normal
	std::string disableSpriteName: frame name of this button to display as disable
	std::string selectSpriteName: frame name of this button to display as selected
	*/
	static UIButtonExtend* create(std::string normalSpriteName, std::string selectSpriteName, std::string disableSpriteName);
	////////////////////////////////////////////////////
	void init();
	////////////////////////////////////////////////////
	typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> touchCallback;
	typedef std::function<bool(cocos2d::Touch* touch, cocos2d::Event* event)> touchCallbackWarped;
	/*
	@Note: set this callback to able to access when touch in button (finger on)
	*/
	touchCallback onTouchBegin = nullptr;
	touchCallback onTouchMove = nullptr;
	touchCallback onTouchEnd = nullptr;

	touchCallbackWarped onTouchBeginWarped = nullptr;
	touchCallbackWarped onTouchMoveWarped = nullptr;
	touchCallbackWarped onTouchEndWarped = nullptr;
	/*
	@Note: Event listener function
	*/
	void disableEventListener();
	void enableEventListener();
	void removeEventListener();
	void setupEventListener();
	////////////////////////////////////////////////////
	// add display to parent
	void setParent(cocos2d::Node* parent) { parent->addChild(this->objDisplay); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder) { parent->addChild(this->objDisplay, localZOrder); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder, int tag) { parent->addChild(this->objDisplay, localZOrder, tag); this->parent = parent; };
	void setParent(cocos2d::Node* parent, int localZOrder, std::string name) { parent->addChild(this->objDisplay, localZOrder, name); this->parent = parent; };
	////////////////////////////////////////////////////
	// utilities function
	void setEnable(bool isEnable);
	bool isEnable(){ return this->m_isEnable; };
	void setSelected(bool isSelected);
	bool isSelected(){ return this->m_isSelected; };
	//--- set fixed priority for order event listener
	void setFixedPriority(int newPriority){ this->fixedPriority = newPriority; cocos2d::Director::getInstance()->getEventDispatcher()->setPriority(this->eventListener, newPriority); };
	////////////////////////////////////////////////////
	void setName(std::string _name) { this->name = _name; };
	std::string getName(){ return this->name; };
	void setScale(float scale){ this->objDisplay->setScale(scale); };
	void setPosition(cocos2d::Vec2 pos) { this->objDisplay->setPosition(pos); };
};

#endif