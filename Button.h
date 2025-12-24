#pragma once
#include<functional>
#include<graphics.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")


struct Button {
	int x, y, width, height;
	const char* text;
	std::function<void()> onClick;

	Button(int x,int y,int w,int h,const char* txt,std::function<void()> action)
		:x(x),y(y),width(w),height(h),text(txt),onClick(action){}

	bool isHover() const;//获取点击
	void draw() const;//绘制

};