#include "Hit.h"


// 접촉 판정 영역이 겹쳐져 있다면 true를 반환
bool CHit::IsHit(float x, float y, CHit& op, float ox, float oy) {
	float
		l=x+L, t=y+T, 
		r=x+R, b=y+B,
		opl=ox+op.L, opt=oy+op.T, 
		opr=ox+op.R, opb=oy+op.B;
	if (l==r || t==b || opl==opr || opt==opb) return false;
	return !(r<=opl || opr<=l || b<=opt || opb<=t);
}


// 접촉 판정 영역이 완전히 내부에 있다면 true를 반환
bool CHit::IsInside(float x, float y, CHit& op, float ox, float oy) {
	float
		l=x+L, t=y+T, 
		r=x+R, b=y+B,
		opl=ox+op.L, opt=oy+op.T, 
		opr=ox+op.R, opb=oy+op.B;
	return (opl<=l && r<=opr && opt<=t && b<=opb);
}



