#include "ShtMain.h"


//==============================================================
// ＜CShtGame＞스테이지 구축(탄)
void CShtGame::MakeStages_Bullet() {
	CMover *stage, *enemy[100], *bullet[100], *crash[100];	
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	CMoverGroup* bg=MoverGroupManager->GetGroup(BULLET_GROUP);
	CMoverGroup* eg=MoverGroupManager->GetGroup(ENEMY_GROUP);

	//==========================================================
	// 조준탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.6f, 0)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 20, -1);
	stage=(new CMover(sg))
		->SetPos(37.5f, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 1, 60, -1);
	AddStage(stage, L"조준탄", L"메인 캐릭터를 향해 날아오는 탄");

	//==========================================================
	// 조준탄 2
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.5f, 0)
		->SetYaw(0.05f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetAiming(0.7f, 0)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 40, -1)
		->AddSpawn(bullet[1], 20, 40, -1);
	stage=(new CMover(sg))
		->SetPos(37.5f, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 1, 80, -1);
	AddStage(stage, L"조준탄2", L"속도가 다른 2종류의 조준탄");
	
	//==========================================================
	// 조준탄(DDA)
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAimingDDA(0.6f, 0)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAimingDDA(0.3f, 0)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 20, -1);
	stage=(new CMover(sg))
		->SetPos(37.5f, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 1, 60, -1);
	AddStage(stage, L"조준탄(DDA)", L"DDA를 사용한 조준탄");
	
	//==========================================================
	// 조준탄(고정 소수)
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAimingFPN(0.6f, 0)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAimingFPN(0.3f, 0)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 20, -1);
	stage=(new CMover(sg))
		->SetPos(37.5f, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 1, 60, -1);
	AddStage(stage, L"조준탄(고정소수)", L"고정소수를 이용한 조준탄");
	
	//==========================================================
	// 방향탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected(0.3f, 0.4f)
		->SetYaw(0.05f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetDirected(0.3f, 0.9f)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetDirected(0.3f, 0.15f)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 15, -1)
		->AddSpawn(bullet[1], 0, 15, -1);
	stage=(new CMover(sg))
		->SetPos(-2, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 80, -1);
	AddStage(stage, L"방향탄", L"지정한 방향으로 날아가는 탄");

	//==========================================================
	// 테이블을 사용한 방향탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected16_3(0.4f, 0.4f)
		->SetYaw(0.05f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetDirected16_3(0.4f, 0.9f)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetDirected16_3(0.4f, 0.15f)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 15, -1)
		->AddSpawn(bullet[1], 0, 15, -1);
	stage=(new CMover(sg))
		->SetPos(-2, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 80, -1);
	AddStage(stage, L"테이블을 사용한 방향탄", L"16방향, 속도 3의 테이블을 사용한 방향탄\n");

	//==========================================================
	// 방향탄(DDA)
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirectedDDA(0.3f, 0.4f)
		->SetYaw(0.05f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetDirectedDDA(0.3f, 0.9f)
		->SetYaw(0.05f);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetDirectedDDA(0.3f, 0.15f)
		->SetAutoYaw()
		->AddSpawn(bullet[0], 0, 15, -1)
		->AddSpawn(bullet[1], 0, 15, -1);
	stage=(new CMover(sg))
		->SetPos(-2, -2)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 80, -1);
	AddStage(stage, L"방향탄(DDA)", L"DDA를 사용한 방향탄");

	//==========================================================
	// n-way탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_BLUE])
		->SetDirected(0.3f, 0.25f)
		->SetAutoYaw();
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_PURPLE])
		->SetDirected(0.3f, 0.25f)
		->SetAutoYaw();
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawnNWay(bullet[0], 0, 40, -1, 32, 0.01f)
		->AddSpawnNWay(bullet[1], 20, 40, -1, 33, 0.01f);
	AddStage(stage, L"n-way탄", L"부채꼴 모양으로 펼쳐지는 탄");

	//==========================================================
	// 원형탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_BLUE])
		->SetAiming(0.3f, 0)
		->SetAutoYaw();
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_PURPLE])
		->SetAiming(0.3f, 0)
		->SetAutoYaw();
	stage=(new CMover(sg))
		->SetPos(37.5f, 50)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawnCircle(bullet[0], 0, 40, -1, 80, false)
		->AddSpawnCircle(bullet[1], 20, 40, -1, 80, true);
	AddStage(stage, L"원형탄", L"원형으로 펼쳐지는 탄");

	//==========================================================
	// 분열탄
	bullet[2]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.5f, 0)
		->SetYaw(0.01f);
	bullet[3]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetAiming(0.6f, 0)
		->SetYaw(0.05f);
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_MEDIUM_BLUE])
		->SetAiming(0.3f, 0)
		->SetYaw(0.01f)
		->AddSpawnNWay(bullet[2], 60, 0, 1, 7, 0.02f)
		->SetNext(NULL, 62);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_MEDIUM_PURPLE])
		->SetAiming(0.4f, 0)
		->SetYaw(0.05f)
		->AddSpawnNWay(bullet[3], 60, 0, 1, 8, 0.02f)
		->SetNext(NULL, 62);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 0, 40, -1)
		->AddSpawn(bullet[1], 20, 40, -1);
	AddStage(stage, L"분열탄", L"분열하는 탄");

	//==========================================================
	// 간이 유도탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetSimpleHoming(0.6f)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetSimpleHoming(1.2f)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 1, 30, -1)
		->AddSpawn(bullet[1], 15, 30, -1);
	AddStage(stage, L"간이 유도탄", L"단순한 유도 알고리즘을 사용한 탄");

	//==========================================================
	// 유도탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetHoming(0.6f, 0.003f)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetHoming(1.2f, 0.003f)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 1, 30, -1)
		->AddSpawn(bullet[1], 15, 30, -1);
	AddStage(stage, L"유도탄", L"조금 피하기 쉽게 만든 유도탄");

	//==========================================================
	// 유도 레이저
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_BLUE])
		->SetHoming(0.6f, 0.003f)
		->SetAutoYaw()
		->SetLaser(40, -0.02f, true);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_PURPLE])
		->SetHoming(1.2f, 0.003f)
		->SetAutoYaw()
		->SetLaser(80, -0.01f, true);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 1, 60, -1)
		->AddSpawn(bullet[1], 30, 60, -1);
	AddStage(stage, L"유도 레이저", L"메인 캐릭터를 좇아오는 레이저");

	//==========================================================
	// 유도 레이저 2
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_BLUE])
		->SetDirected(0.6f, -0.25f)
		->SetAutoYaw()
		->SetLaser(40, -0.02f, true);
	bullet[0]=(new CMover(bullet[1]))
		->SetDirected(0.6f, -0.2f)
		->SetHoming(0.01f)
		->AddSpawn(bullet[1], 60, 0, 1)
		->SetNext(NULL, 60);
	bullet[3]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_PURPLE])
		->SetDirected(0.6f, -0.25f)
		->SetAutoYaw()
		->SetLaser(40, -0.02f, true);
	bullet[2]=(new CMover(bullet[3]))
		->SetDirected(0.6f, 0.7f)
		->SetHoming(0.01f)
		->AddSpawn(bullet[3], 60, 0, 1)
		->SetNext(NULL, 60);
	enemy[0]=(new CMover(eg))
		->SetPos(0, 20)
		->AddSpawn(bullet[0], 0, 100, -1);
	enemy[1]=(new CMover(eg))
		->SetPos(75, 20)
		->AddSpawn(bullet[2], 50, 100, -1);
	enemy[2]=(new CMover(eg))
		->SetPos(0, 60)
		->AddSpawn(bullet[0], 0, 100, -1);
	enemy[3]=(new CMover(eg))
		->SetPos(75, 60)
		->AddSpawn(bullet[2], 50, 100, -1);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 1, 0, 1)
		->AddSpawn(enemy[1], 1, 0, 1)
		->AddSpawn(enemy[2], 1, 0, 1)
		->AddSpawn(enemy[3], 1, 0, 1);
	AddStage(stage, L"유도 레이저2", L"도중에 추적을 그만두고 직진하는 레이저");

	//==========================================================
	// 유도 미사일
	crash[0]=(new CMover(bg))
		->SetObj3D(Obj3D[CRASH_MISSILE])
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[MISSILE_SMALL])
		->SetHoming(0.8f, 0.001f)
		->SetAutoYaw()
		->SetAutoRoll(0.1f, 0.01f)
		->AddSpawn(crash[0], 100, 0, 1)
		->SetNext(NULL, 100)
		->SetLaser(40, -0.025f, false, Obj3D[MISSILE_TAIL_SMALL], true, true)
		->SetDamage(1, 1, crash[0], NULL, false, false);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 1, 30, -1);
	AddStage(stage, L"유도 미사일", L"메인 캐릭터를 좇아오는 미사일");

	//==========================================================
	// 가속 n-way탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected(0.01f, 0.25f)
		->SetYaw(0.05f)
		->SetAccel(0.005f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetDirected(0.01f, 0.25f)
		->SetYaw(0.05f)
		->SetAccel(0.005f);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawnNWay(bullet[0], 0, 40, -1, 32, 0.01f)
		->AddSpawnNWay(bullet[1], 20, 40, -1, 33, 0.01f);
	AddStage(stage, L"가속 n-way탄", L"점점 속도가 빨라지는 n-way탄");

	//==========================================================
	// 가속 유도 레이저
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_BLUE])
		->SetHoming(0.2f, 0.003f)
		->SetAutoYaw()
		->SetAccel(0.01f)
		->SetLaser(40, -0.02f, true);
	bullet[1]=(new CMover(bg))->SetAccel(-0.01f);
	bullet[0]->SetNext(bullet[1], 60);
	bullet[1]->SetNext(bullet[0], 60);
	bullet[2]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_PURPLE])
		->SetHoming(0.4f, 0.003f)
		->SetAutoYaw()
		->SetAccel(0.02f)
		->SetLaser(40, -0.02f, true);
	bullet[3]=(new CMover(bg))->SetAccel(-0.02f);
	bullet[2]->SetNext(bullet[3], 40);
	bullet[3]->SetNext(bullet[2], 40);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 1, 60, -1)
		->AddSpawn(bullet[2], 30, 60, -1);
	AddStage(stage, L"가속 유도 레이저", L"가속과 감속을 반복하는 유도 레이저");

	//==========================================================
	// 낙하탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected(1.2f, 0.7f)
		->SetYaw(-0.01f)
		->SetAccelXY(0, 0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetDirected(0.6f, 0.5f)
		->SetYaw(-0.01f)
		->SetAccelXY(0, 0.01f);
	enemy[0]=(new CMover(eg))
		->SetPos(100, 75)
		->AddSpawn(bullet[0], 0, 10, -1);
	enemy[1]=(new CMover(eg))
		->SetPos(100, 25)
		->AddSpawn(bullet[1], 5, 10, -1);
	stage=(new CMover(sg))
		->AddSpawn(MyShipSideView, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 0, 1)
		->AddSpawn(enemy[1], 0, 0, 1);
	AddStage(stage, L"낙하탄", L"중력에 끌려 낙하하는 탄", VIEW_SIDE);

	//==========================================================
	// 낙하 미사일
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[MISSILE_SMALL])
		->SetAiming(1.2f, 0)
		->SetAutoYaw()
		->SetAutoRoll(0.1f, 0.01f, true, false)
		->SetAccelXY(0, 0.01f)
		->AddSpawn(crash[0], 80, 0, 1)
		->SetNext(NULL, 80)
		->SetLaser(40, -0.025f, false, Obj3D[MISSILE_TAIL_SMALL], true, true)
		->SetDamage(1, 1, crash[0], NULL, false, false);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY_CANNON])
		->SetDirected(0.4f, 0.5f)
		->SetYaw(0.01f)
		->AddSpawn(bullet[0], 0, 10, -1);
	enemy[1]=(new CMover(eg))->SetAccel(-0.1f);
	enemy[2]=(new CMover(eg))->SetAccel(0);
	enemy[3]=(new CMover(eg))->SetAccel(0.1f);
	enemy[0]->SetNext(enemy[1], 180);
	enemy[1]->SetNext(enemy[2], 7);
	enemy[2]->SetNext(enemy[3], 180);
	enemy[3]->SetNext(enemy[0], 7);
	stage=(new CMover(sg))
		->AddSpawn(MyShipSideView, 0, 0, 1)
		->SetPos(90, 70)
		->AddSpawn(enemy[0], 0, 0, 1);
	AddStage(stage, L"낙하 미사일", L"중력에 끌려 낙하하는 미사일", VIEW_SIDE);

	//==========================================================
	// 회전탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_BLUE])
		->SetLooping(0, 0.005f, 0, 15, 0, 0)
		->SetAutoYaw();
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_PURPLE])
		->SetLooping(0, -0.0025f, 0, 30, 0, 0)
		->SetAutoYaw();
	stage=(new CMover(sg))
		->SetPos(37.5f, 50)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawnCircle(bullet[0], 0, 0, 1, 10, false)
		->AddSpawnCircle(bullet[1], 0, 0, 1, 20, false);
	AddStage(stage, L"회전탄", L"정해진 점 주위를 회전하는 탄");

	//==========================================================
	// 회전탄 2
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetLooping(0, 0.001f, 0, 0, 0.2f, 0)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetLooping(0, -0.001f, 0, 0, 0.3f, 0)
		->SetYaw(-0.01f);
	stage=(new CMover(sg))
		->SetPos(37.5f, 50)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawnCircle(bullet[0], 0, 40, -1, 10, false)
		->AddSpawnCircle(bullet[1], 20, 40, -1, 20, false);
	AddStage(stage, L"회전탄2", L"반경이 점점 길어지는 회전탄");

	//==========================================================
	// 조준탄 + 회전탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetLooping(0, 0.001f, 0, 0, 0.1f, 0)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetLooping(0, -0.001f, 0, 0, 0.2f, 0)
		->SetYaw(-0.01f);
	bullet[2]=(new CMover(bg))
		->SetAiming(0.6f, 0)
		->AddSpawnCircle(bullet[0], 0, 0, 1, 8, false)
		->AddSpawnCircle(bullet[1], 0, 0, 1, 16, false);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[2], 0, 60, -1);
	AddStage(stage, L"조준탄+회전탄", L"중심 부분을 조준탄처럼 움직이도록 한 회전탄");

	//==========================================================
	// 회오리탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_BLUE])
		->SetDirected(0.2f, 0)
		->SetAutoYaw();
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[NEEDLE_SMALL_PURPLE])
		->SetDirected(0.4f, 0)
		->SetAutoYaw();
	stage=(new CMover(sg))
		->SetPos(37.5f, 50)
		->SetYaw(0.01f)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 0, 1, -1)
		->AddSpawn(bullet[1], 1, 1, -1);
	AddStage(stage, L"회오리탄", L"회오리 모양으로 넓어지는 탄");

	//==========================================================
	// 정지하는 유도탄
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetHoming(0.6f, 0.003f)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))->SetAccel(-0.05f);
	bullet[2]=(new CMover(bg))->SetYaw(0);
	bullet[3]=(new CMover(bg))->SetAccel(0.05f);
	bullet[0]->SetNext(bullet[1], 60);
	bullet[1]->SetNext(bullet[2], 11);
	bullet[2]->SetNext(bullet[3], 30);
	bullet[3]->SetNext(bullet[0], 11);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 0, 60, -1);
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetHoming(0.6f, 0.003f)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))->SetAccel(-0.05f);
	bullet[2]=(new CMover(bg))->SetYaw(0);
	bullet[3]=(new CMover(bg))->SetAccel(0.05f);
	bullet[0]->SetNext(bullet[1], 40);
	bullet[1]->SetNext(bullet[2], 11);
	bullet[2]->SetNext(bullet[3], 10);
	bullet[3]->SetNext(bullet[0], 11);
	stage->AddSpawn(bullet[0], 30, 60, -1);
	AddStage(stage, L"정지하는 유도탄", L"정지와 이동을 반복하는 유도탄");

	//==========================================================
	// 도망가는 유도 레이저
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_BLUE])
		->SetHoming(0.6f, 0.003f)
		->SetAutoYaw()
		->SetLaser(40, -0.02f, true);
	bullet[1]=(new CMover(bg))
		->SetHoming(0.03f)
		->SetAccel(-0.05f);
	bullet[2]=(new CMover(bg))
		->SetHoming(0.03f)
		->SetAccel(0.05f);
	bullet[0]->SetNext(bullet[1], 90);
	bullet[1]->SetNext(bullet[2], 30);
	bullet[2]->SetNext(bullet[0], 30);
	stage=(new CMover(sg))
		->SetPos(37.5f, 0)
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(bullet[0], 0, 60, -1);
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THIN_PURPLE])
		->SetHoming(0.8f, 0.003f)
		->SetAutoYaw()
		->SetLaser(40, -0.02f, true);
	bullet[1]=(new CMover(bg))
		->SetHoming(0.03f)
		->SetAccel(-0.08f);
	bullet[2]=(new CMover(bg))
		->SetHoming(0.03f)
		->SetAccel(0.08f);
	bullet[0]->SetNext(bullet[1], 40);
	bullet[1]->SetNext(bullet[2], 20);
	bullet[2]->SetNext(bullet[0], 20);
	stage->AddSpawn(bullet[0], 30, 60, -1);
	AddStage(stage, L"도망가는 유도 레이저", L"추적과 도망을 반복하는 유도 레이저");

	//==========================================================
	// 직진하는 빔
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[LASER_THICK_BLUE])
		->SetDirected(2.0f, 0.25f);
	enemy[0]=(new CMover(eg))
		->SetPos(37.5f, 0)
		->AddSpawn(bullet[0], 0, 1, -1)
		->SetNext(NULL, 25);
	enemy[1]=(new CMover(enemy[0]))
		->SetPos(15, 0);
	enemy[2]=(new CMover(enemy[0]))
		->SetPos(60, 0);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 70, -1)
		->AddSpawn(enemy[1], 40, 80, -1)
		->AddSpawn(enemy[2], 40, 90, -1);
	AddStage(stage, L"직진하는 빔", L"정해진 방향으로 똑바로 직진하는 빔");

	//==========================================================
	// 안전지대
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected(0.3f, 0.25f)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawnNWay(bullet[0], 0, 40, -1, 19, 0.02f);
	AddStage(stage, L"안전지대", L"안전지대가 존재하는 단순한 n-way탄의 예");

	//==========================================================
	// 안전지대 2
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetDirected(0.3f, 0.25f)
		->SetYaw(0.01f);
	bullet[1]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetAiming(0.8f, 0)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawnNWay(bullet[0], 0, 40, -1, 19, 0.02f)
		->AddSpawn(bullet[1], 15, 30, -1);
	AddStage(stage, L"안전지대2", L"조준탄을 더하여 안전지대를 없앤 예");

	//==========================================================
	// 난수
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.8f, 0)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawnNWay(bullet[0], 0, 10, -1, 5, 0.02f);
	AddStage(stage, L"난수", L"난수를 사용하지 않는 단순한 n-way탄");

	//==========================================================
	// 난수 2
	bullet[0]=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetAiming(0.8f, 0)
		->SetRand(0.2f, 0.05f)
		->SetYaw(0.01f);
	stage=(new CMover(sg))
		->AddSpawn(MyShipTopView, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawnNWay(bullet[0], 0, 10, -1, 5, 0.02f);
	AddStage(stage, L"난수2", L"방향과 속도에 난수를 더한 n-way탄");
}

