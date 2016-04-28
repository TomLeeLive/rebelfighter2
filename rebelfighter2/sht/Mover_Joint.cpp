#include "Mover.h"
#include "Hit.h"
#include "Input.h"
#include <assert.h>




//==============================================================
// 생성자

CJointMover::CJointMover(CMoverGroup* group)
:	CMover(group)
{}


CJointMoverGroup::CJointMoverGroup(
	CMoverGroupManager* manager, LPDIRECT3DDEVICE9 device, 
	int num_all_movers
)
:	CMoverGroup(manager, device, num_all_movers)
{
	int i;
	for (i=0; i<NumAllMovers; i++) delete AllMovers[i];
	for (i=0; i<NumAllMovers; i++) AllMovers[i]=new CJointMover(this);
	DeleteAll();
}


CJointMoverGroup::~CJointMoverGroup() {
}




//==============================================================
// 초기화

void CJointMover::Init(CMover* model_joint, CMover* parent) {
	CMover::Init(model_joint, parent);

	assert(model_joint->Group==GetGroup(JOINT_GROUP));
	CJointMover* model=(CJointMover*)model_joint;
	Joint=model->Joint;

	// 촉수
	if (Valid.Tentacle) {
		JOINT& jo=Joint;
		int n=jo.NumPart-1;
		CMover* head=jo.Mover[0]=New(jo.HeadModel);
		CMover* root=jo.Mover[n]=New(jo.RootModel);
		float hx=head->X, hy=head->Y, rx=root->X, ry=root->Y;
		for (int i=1; i<n; i++) {
			CMover* body=jo.Mover[i]=New(jo.BodyModel);
			body->X=(hx*(n-i)+rx*i)/n;
			body->Y=(hy*(n-i)+ry*i)/n;
		}
	}

	// 다관절
	if (Valid.Joint) {
		JOINT& jo=Joint;
		int i, n=jo.NumPart;
		CMover* head=jo.Mover[0]=New(jo.HeadModel);
		CMover* root=jo.Mover[n-1]=New(jo.RootModel);
		for (i=1; i<n-1; i++) jo.Mover[i]=New(jo.BodyModel);
		for (i=0; i<n; i++) jo.Rad[i]=0;
		float px, py;
		px=jo.Dist*cos(jo.CRad);
		py=jo.Dist*sin(jo.CRad);
		for (i=n-2; i>=0; i--) {
			jo.Mover[i]->X=jo.Mover[i+1]->X+px;
			jo.Mover[i]->Y=jo.Mover[i+1]->Y+py;
		}
		jo.Target=GetGroup(MYSHIP_GROUP)->GetRandomUsedMover();
	}
}




//==============================================================
// 이동(후)

void CJointMover::PostMove() {
	CMover::PostMove();

	// 촉수
	if (Valid.Tentacle) {
		JOINT& jo=Joint;
		CMover *head=jo.Mover[0], *root=jo.Mover[jo.NumPart-1];

		// 선두의 이동 범위를 제한하기:
		// 선두와 뿌리의 거리가 제한치를 넘으면
		// 이동 가능 범위 안으로 되돌림.
		float dx=head->X-root->X, dy=head->Y-root->Y;
		float d=sqrt(dx*dx+dy*dy);
		if (d>jo.MaxDist) {
			head->X=dx*jo.MaxDist/d+root->X;
			head->Y=dy*jo.MaxDist/d+root->Y;
		}

		// 중간 부분의 좌표를 계산하기:
		// 인접한 두 부분의 좌표의 평균을 취함.
		// 계산은 선두에서 뿌리 쪽을 향해서 실시함.
		// 필요에 따라 계산을 여러 번 반복해 줌.
		for (int l=0; l<jo.NumLoop; l++) {
			for (int i=1, n=jo.NumPart-1; i<n; i++) {
				jo.Mover[i]->X=(jo.Mover[i-1]->X+jo.Mover[i+1]->X)/2;
				jo.Mover[i]->Y=(jo.Mover[i-1]->Y+jo.Mover[i+1]->Y)/2;
			}
		}
	}

	// 다관절
	if (Valid.Joint) {
		JOINT& jo=Joint;
		int i, n=jo.NumPart, n1=n-1;
		float c, s, vrad=jo.VRad, 
			x0=jo.Mover[0]->X, y0=jo.Mover[0]->Y;
		
		// 전반부 처리:
		// 선두에서 뿌리 쪽을 향해 관절의 각도를 구함.
		c=(float)cos(vrad);
		s=(float)sin(vrad);
		for (i=1; i<jo.NumPart; i++) {
			float dxt, dyt;       // 관절에서 목표물로 향하는 벡터
			float dxn, dyn;       // 관절에서 선두를 향하는 벡터
			float dxr=0, dyr=0;   // 시계 방향 벡터
			float dxl=0, dyl=0;   // 반 시계 방향 벡터
			float dpn, dpr, dpl;  // 내적((회전하지 않음, 시계 방향 회전, 반 시계 방향 회전)
			
			// 관절에서 목표물로 향하는 벡터를 계산
			dxt=jo.Target->X-jo.Mover[i]->X;
			dyt=jo.Target->Y-jo.Mover[i]->Y;

			// 관절에서 선두를 향하는 벡터와 내적을 계산
			// (회전하지 않을 때의 벡터)
			dxn=x0-jo.Mover[i]->X;
			dyn=y0-jo.Mover[i]->Y;
			dpn=dxt*dxn+dyt*dyn;
			
			// 시계 방향으로 회전하는 벡터의 계산:
			// 회전 각도의 한계치를 넘었을 때는 회전하지 않음
			if (jo.Rad[i]+vrad<=jo.LRad) {
				dxr=c*dxn-s*dyn;
				dyr=s*dxn+c*dyn;
				dpr=dxt*dxr+dyt*dyr;
			} else dpr=dpn;
			
			// 반 시계 방향으로 회전하는 벡터의 계산:
			// 회전 각도의 한계치를 넘었을 때는 회전하지 않음
			if (jo.Rad[i]-vrad>=-jo.LRad) {
				dxl= c*dxn+s*dyn;
				dyl=-s*dxn+c*dyn;
				dpl=dxt*dxl+dyt*dyl;
			} else dpl=dpn;
			
			// 회전 방향 선택:
			// 내적을 비교하여 3종류 중에서 선택.
			// 선두를 회전시킨 후, 새로운 선두의 위치를 구하기.
			if (dpr>dpn && dpr>dpl) {
				jo.Rad[i]+=vrad;
				x0=jo.Mover[i]->X+dxr;
				y0=jo.Mover[i]->Y+dyr;
			}
			if (dpl>dpn && dpl>dpr) {
				jo.Rad[i]-=vrad;
				x0=jo.Mover[i]->X+dxl;
				y0=jo.Mover[i]->Y+dyl;
			}
		}
		
		// 후반부 처리:
		// 뿌리에서 선두 쪽을 향해 관절의 위치를 구하기
		float px, py, qx, qy;
		px=jo.Dist*cos(jo.CRad);
		py=jo.Dist*sin(jo.CRad);
		for (i=n-2; i>=0; i--) {
			c=(float)cos(jo.Rad[i+1]);
			s=(float)sin(jo.Rad[i+1]);
			qx=c*px-s*py;
			qy=s*px+c*py;
			jo.Mover[i]->X=jo.Mover[i+1]->X+qx;
			jo.Mover[i]->Y=jo.Mover[i+1]->Y+qy;
			px=qx;
			py=qy;
		}
	}
}




//==============================================================
// 설정

// 촉수
CJointMover* CJointMover::SetTentacle(
	CMover* head, CMover* body, CMover* root, 
	int num_part, int num_loop, float max_dist
) {
	Valid.Tentacle=true;
	Joint.HeadModel=head;
	Joint.BodyModel=body;
	Joint.RootModel=root;
	Joint.NumPart=num_part;
	Joint.NumLoop=num_loop;
	Joint.MaxDist=max_dist;
	return this;
}

// 다관절
CJointMover* CJointMover::SetJoint(
	CMover* head, CMover* body, CMover* root, 
	int num_part, float center_angle, float vangle, float limit_angle, float dist
) {
	Valid.Joint=true;
	Joint.HeadModel=head;
	Joint.BodyModel=body;
	Joint.RootModel=root;
	Joint.NumPart=num_part;
	Joint.CRad=D3DX_PI*2*center_angle;
	Joint.VRad=D3DX_PI*2*vangle;
	Joint.LRad=D3DX_PI*2*limit_angle;
	Joint.Dist=dist;
	return this;
}

