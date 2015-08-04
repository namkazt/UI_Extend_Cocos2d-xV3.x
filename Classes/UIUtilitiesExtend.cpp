#include "UIUtilitiesExtend.h"

USING_NS_CC;

//----------------------------------------------------------
//-- function to create a sprite with rect texture without load image
Sprite* UIUtilitiesExtend::createSpriteWithColor(Size size, Color4B color)
{
	GLubyte *buffer = (GLubyte *)malloc(sizeof(GLubyte) * 4);
	buffer[0] = color.r;
	buffer[1] = color.g;
	buffer[2] = color.b;
	buffer[3] = color.a;
	Size textSz = Size(size.width, size.height);
	auto tex = new Texture2D();
	tex->initWithData(buffer, sizeof(GLubyte) * 4, Texture2D::PixelFormat::RGBA8888, 1, 1, textSz);
	auto sprite = Sprite::create();
	sprite->setTexture(tex);
	sprite->setTextureRect(Rect(0, 0, textSz.width, textSz.height));
	tex->release();
	free(buffer);

	return sprite;
}