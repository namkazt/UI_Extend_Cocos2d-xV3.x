# UI Extend For Cocos2dx v3.x

This is just my work from some game when we using almost just button and scroll box. so we do not need to using build-in UI. so i create my own ui extend lib. it's pretty easy to use and extend if need:
### UIButtonExtend Class
It is just a simple button with 3 state : normal, selected, disable but i changed to like javascript using:
```c++
UIButtonExtend* button = UIButtonExtend::create("normal", "selected", "disable");
button->setPosition(Vec2( x, y ));
```
To set parent : 
```c++
button->setParent(Node* xxx);
```

Callback : 
```c++
typedef std::function<void(cocos2d::Touch* touch, cocos2d::Event* event)> touchCallback;
typedef std::function<bool(cocos2d::Touch* touch, cocos2d::Event* event)> touchCallbackWarped;
//----------------------------------------------------
//-- this event dispacth when touch begin, move, end
touchCallback onTouchBegin = nullptr;
touchCallback onTouchMove = nullptr;
touchCallback onTouchEnd = nullptr;
//----------------------------------------------------
//-- this is condition to dispatch above event 
//-- e.g: onTouchBeginWarped is must return true to make sure onTouchBegin run
//-- this is optinal callback if you want to hook to above event to control it
touchCallbackWarped onTouchBeginWarped = nullptr;
touchCallbackWarped onTouchMoveWarped = nullptr;
touchCallbackWarped onTouchEndWarped = nullptr;

//////////////////////////////////////////////////////////
// USING 
//////////////////////////////////////////////////////////
button->onTouchEnd = [=](Touch* touch, Event* event){
	log("Click on item vertical: %i", i + 1);
};
```

### UIScrollPanelExtend Class (W.I.P)
So it a scrollable panel so we can put into it sprite, any Node* base class, or UIButtonExtend ( i knew what happen if you use scroll build-in cocos with ui button, it can not scroll if you touch in button inside scroll panel. ). Anyway this Scroll Panel have more features.

> Warning: Currently only Vertical working.

```c++
//--- Create your scroll panel
verScroll = UIScrollPanelExtend::create(223, 617, 209, 598);

//--- set scroll mode : UI_EXTEND_VERTICAL, UI_EXTEND_HORIZONTAL, UI_EXTEND_BOTH
verScroll->setScrollMode(UIScrollPanelExtend::UI_EXTEND_VERTICAL);

//--- set true if you want use scroll bar ( current not allow change side to display )
verScroll->setUseScrollBar(true);

//--- set true if you want use auto hide scroll bar feature with delay time
verScroll->setAutoHideScrollBar(true, 0.5f);

//--- set fixed scroll speed ( set it if you do not want it auto calculate )
verScroll->setScrollSpeed(2.5f); //nếu không set fix speed thì hệ thống sẽ tự tính speed

//--- (customize) set bar width 
verScroll->setScrollBarWidth(10);

//--- (customize) set bar color, and bar background color ( all is Color4B)
verScroll->setScrollBarColor({ 67, 209, 136, 255 }, {0, 99, 48, 255});

//-------------------------------------------------------------------
// set offset function
//     Outside is border part, Inside is scrollable part and is display part too
//     Offset is space diffirent between 
//     ----*---------------------------
//     |~~~*-----------------------   |
//     |   |                      |   |
//     |   |    Scroll part       |   |
//     |   |                      |   |
//     |   |                      |   |
//     |    -----------------------   |
//     --------------------------------
verScroll->setOffset({ 14 / 2, 19 / 2 });

verScroll->setPosition(verBG->getPosition());
//---------------------------------
//--- set parent to this scroll
verScroll->setParent(this);
```


