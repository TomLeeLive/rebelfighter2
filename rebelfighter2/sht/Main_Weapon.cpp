#include "ShtMain.h"


//==============================================================
// ＜CShtGame＞스테이지 구축(무기)
void CShtGame::MakeStages_Weapon() {
	CMover *stage, *enemy[100];
	CMoverGroup* mg=MoverGroupManager->GetGroup(MYSHIP_GROUP);
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	CMoverGroup* bg=MoverGroupManager->GetGroup(BULLET_GROUP);
	CMoverGroup* eg=MoverGroupManager->GetGroup(ENEMY_GROUP);
	CMoverGroup* wg=MoverGroupManager->GetGroup(WEAPON_GROUP);
	CMoverGroup* gg=MoverGroupManager->GetGroup(GROUND_GROUP);
	CMover *myship;

	CMover* shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* big_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* max_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* laser=(new CMover(wg))
		->SetObj3D(Obj3D[LASER_THICK_BLUE])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* lock_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetAiming(3.0f, 0)
		->SetTarget(
			TG_LOCKED_ENEMY, 0, 
			(new CMover(wg))
				->SetObj3D(Obj3D[SHOT_MEDIUM])
				->SetDirected(3.0f, 0.75f)
				->SetDamage(1, 1, NULL, NULL, false, false)
				->SetLock(0))
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false);

	CMover* enemy_crash=(new CMover(sg))
		->AddSpawn((new CMover(sg))
			->SetObj3D(Obj3D[CRASH_ENEMY])
			->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
			->SetAlpha(2, -0.04f)
			->SetNext(NULL, 50)
			, 0, 0, 1);

	//==========================================================
	// 기본적인 샷 조작
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_BASIC_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"기본적인 샷 조작", 
		L"버튼을 한 번 누를 때마다 한 발씩 발사되는 샷.\n\n"
		L"■샷■\n[X]키/버튼２");

	//==========================================================
	// 연사
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_AUTO_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"연사",
		L"버튼을 꾹 누르고 있으면 연속적으로 발사되는 샷.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 에너지 샷
	myship=(new CMover(MyShipTopView))
		->SetWeapon_PowerShot(
			WP_POWER_SHOT, shot, big_shot, max_shot, 
			Obj3D[POWER_EFFECT], Obj3D[MAX_EFFECT],
			30, 120, 0);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"에너지 샷",
		L"버튼을 꾹 눌러서 에너지를 모아 발사하는 샷.\n"
		L"에너지는 2단계로 모아짐.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 세미 오토 연사
	myship=(new CMover(MyShipTopView))
		->SetWeapon_PowerShot(
			WP_SEMI_AUTO_SHOT, shot, big_shot, max_shot, 
			Obj3D[POWER_EFFECT], Obj3D[MAX_EFFECT],
			15, 120, 15);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"세미 오토 연사", 
		L"버튼을 적절히 연타하면 보통 샷이 발사되고\n"
		L"꾹 누르고 있으면 에너지 샷이 발사됨.\n\n"
		L"■샷/에너지 샷■\n[X]키/버튼2");

	//==========================================================
	// 버튼을 떼어서 에너지를 모으는 에너지 샷
	myship=(new CMover(MyShipTopView))
		->SetWeapon_PowerShot(
			WP_POWER_SHOT_RELEASED, shot, big_shot, max_shot, 
			Obj3D[POWER_EFFECT], Obj3D[MAX_EFFECT],
			30, 120, 0);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"버튼을 떼어서 에너지를 모으는 에너지 샷",
		L"버튼을 떼고 있으면 에너지가 모이고,\n"
		L"버튼을 누르면 샷이 발사됨.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 연사와 레이저를 같이 사용하기
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Laser(WP_SHOT_AND_LASER, shot, laser, 15, 15);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"연사와 레이저를 같이 사용하기",
		L"버튼을 적절히 연타하면 보통 샷이 발사되고 \n"
		L"꾹 누르고 있으면 에너지 샷이 발사됨.\n\n"
		L"■샷/레이저■\n[X]키/버튼2");

	//==========================================================
	// 록 샷
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Laser(WP_LOCK_SHOT, shot, lock_shot, 15, 15);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 100, enemy_crash, NULL, false, false);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawn(enemy[0], 0, 60, -1);
	AddStage(stage, L"록 샷",
		L"버튼을 적절히 연타하면 보통 샷이 발사되고\n"
		L"꾹 누르고 있으면 적기를 추적하는 록 샷이 발사됨.\n\n"
		L"■샷/록 샷■\n[X]키/버튼2");

	//==========================================================
	// 커맨드 샷
	COMMAND_LIST* cmd_list=new COMMAND_LIST();
	cmd_list->NumCommand=2;
	COMMAND& cmd0=cmd_list->Command[0];
	cmd0.Length=3;
	cmd0.Limit=30;
	cmd0.Input[0]=CMD_DOWN;
	cmd0.Input[1]=CMD_DOWN|CMD_RIGHT;
	cmd0.Input[2]=CMD_RIGHT|CMD_BUTTON1;
	cmd0.Model=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetDirected(3.0f, 0.0f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false)
		->AddSpawn((new CMover(wg))
			->SetObj3D(Obj3D[SHOT_MEDIUM])
			->SetDirected(3.0f, -0.1f)
			->SetAutoYaw()
			->SetDamage(1, 1, NULL, NULL, false, false)
			, 0, 0, 1)
		->AddSpawn((new CMover(wg))
			->SetObj3D(Obj3D[SHOT_MEDIUM])
			->SetDirected(3.0f, 0.1f)
			->SetAutoYaw()
			->SetDamage(1, 1, NULL, NULL, false, false)
			, 0, 0, 1);
	COMMAND& cmd1=cmd_list->Command[1];
	cmd1.Length=3;
	cmd1.Limit=30;
	cmd1.Input[0]=CMD_RIGHT;
	cmd1.Input[1]=CMD_DOWN;
	cmd1.Input[2]=CMD_DOWN|CMD_RIGHT|CMD_BUTTON1;
	cmd1.Model=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetDirected(3.0f, 0.75f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false)
		->AddSpawn((new CMover(wg))
			->SetObj3D(Obj3D[SHOT_MEDIUM])
			->SetDirected(3.0f, 0.65f)
			->SetAutoYaw()
			->SetDamage(1, 1, NULL, NULL, false, false)
			, 0, 0, 1)
		->AddSpawn((new CMover(wg))
			->SetObj3D(Obj3D[SHOT_MEDIUM])
			->SetDirected(3.0f, 0.85f)
			->SetAutoYaw()
			->SetDamage(1, 1, NULL, NULL, false, false)
			, 0, 0, 1);
	myship=(new CMover(MyShipSideView))
		->SetWeapon_Command(WP_COMMAND_SHOT, cmd_list);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1);
	AddStage(stage, L"커맨드 샷", 
		L"스틱과 버튼 조작을 조합하여 특별한 샷을 발사하기.\n\n"
		L"■커맨드 샷1■\n하，우하，우，[X]키/버튼2\n\n"
		L"■커맨드 샷2■\n우，하，우하，[X]키/버튼2"
		,VIEW_SIDE);

	//==========================================================
	// 조준을 사용한 폭격
	myship=(new CMover(MyShipTopView))
		->SetSightedBomb(
			0, -30, 
			Obj3D[SIGHT_BOMB_READY], 
			Obj3D[SIGHT_BOMB_LOCK], 
			Obj3D[SIGHT_BOMB_TARGET], 
			Obj3D[SIGHT_BOMB_BUSY],
			(new CMover(wg))
				->SetObj3D(Obj3D[SIGHT_BOMB_BOMB])
				->SetDirected(1.0f, 0.75f)
				->SetDamage(1, 1, NULL, NULL, false, true)
				->AddSpawn(
					(new CMover(wg))
						->SetObj3D(Obj3D[SIGHT_BOMB_BOMB])			
						->SetNext(NULL, 0)
						->SetDamage(1, 1, NULL, NULL, false, false),
					30, 0, 1)
				->SetNext(NULL, 30)
			);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, enemy_crash, NULL, false, false);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawn(enemy[0], 0, 60, -1);
	AddStage(stage, L"조준을 사용한 폭격",
		L"조준의 위치에 폭탄을 떨어뜨리기.\n\n"
		L"■폭격■\n[X]키/버튼2");

	//==========================================================
	// 록 온 레이저
	CMover* lock_on_laser=(new CMover(wg))
		->SetObj3D(Obj3D[LASER_THIN_BLUE])
		->SetDirected(1.2f, 0.75f)
		->SetHoming(0.02f)
		->SetAccel(-0.001f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false, true)
		->SetLaser(40, -0.02f, true);
	myship=(new CMover(MyShipTopView))
		->SetLockonLaser(
			4, 0, -30, 
			Obj3D[SIGHT_BOMB_READY], 
			Obj3D[SIGHT_BOMB_LOCK], 
			Obj3D[SIGHT_BOMB_TARGET], 
			Obj3D[SIGHT_BOMB_BUSY],
			lock_on_laser);
	enemy[0]=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, enemy_crash, NULL, false, false);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->SetPos(37.5f, 0)
		->AddSpawn(enemy[0], 0, 60, -1);
	AddStage(stage, L"록 온 레이저",
		L"조준으로 고정시킨 적기에 유도 레이저를 발사하기.\n\n"
		L"■레이저■\n[X]키/버튼2");

	//==========================================================
	// 지형에 따라 날아가는 미사일
	CMover* missile_crash=(new CMover(wg))
		->SetObj3D(Obj3D[CRASH_MISSILE])
		->SetDirected(0.3f, 0.5f)
		->SetNotInherit(IH_YAW)
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	CMover* missile=(new CMover(wg))
		->SetObj3D(Obj3D[MISSILE_SMALL])
		->SetAutoYaw()
		->SetDirected(0.5f, 0.1f)
		->SetGroundHit(GR_HIT, GR_ALONG)
		->SetLaser(40, -0.025f, false, Obj3D[MISSILE_TAIL_SMALL], true, true)
		->SetDamage(1, 1, missile_crash, NULL, false, false);
	myship=(new CMover(MyShipSideView))
		->SetWeapon_Shot(WP_BASIC_SHOT, missile);
	CMover* gnd_y=(new CMover(gg))
		->SetObj3D(Obj3D[GROUND_YELLOW])
		->SetDirected(0.3f, 0.5f);
	CMover* gnd0=(new CMover(sg))
		->SetPos(105, 55)
		->AddSpawn(gnd_y, 0, 170, -1);
	CMover* gnd1=(new CMover(sg))
		->SetPos(105, 63)
		->AddSpawn(gnd_y, 0, 100, -1);
	CMover* gnd2=(new CMover(sg))
		->SetPos(105, 63)
		->AddSpawn(gnd_y, 26, 100, -1);
	CMover* gnd3=(new CMover(sg))
		->SetPos(105, 71)
		->AddSpawn(gnd_y, 0, 25, -1);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(gnd0, 0, 0, 1)
		->AddSpawn(gnd1, 0, 0, 1)
		->AddSpawn(gnd2, 0, 0, 1)
		->AddSpawn(gnd3, 0, 0, 1);
	AddStage(stage, L"지형에 따라 날아가는 미사일", 
		L"지형의 기복에 맞추어 상하로 움직이며 날아가는 미사일.\n\n"
		L"■미사일■\n[X]키/버튼2"
		,VIEW_SIDE);

	//==========================================================
	// 지형에 반사하는 샷
	CMover* reflect_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetAutoYaw()
		->SetDirected(0.8f, 0.15f)
		->SetGroundHit(GR_HIT, GR_REFLECT);
	myship=(new CMover(MyShipSideView))
		->SetWeapon_Shot(WP_BASIC_SHOT, reflect_shot);
	CMover* gnd4=(new CMover(sg))
		->SetPos(105, 12)
		->AddSpawn(gnd_y, 50, 100, -1);
	CMover* gnd5=(new CMover(sg))
		->SetPos(105, 12)
		->AddSpawn(gnd_y, 76, 100, -1);
	CMover* gnd6=(new CMover(sg))
		->SetPos(105, 4)
		->AddSpawn(gnd_y, 50, 25, -1);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(gnd1, 0, 0, 1)
		->AddSpawn(gnd2, 0, 0, 1)
		->AddSpawn(gnd3, 0, 0, 1)
		->AddSpawn(gnd4, 0, 0, 1)
		->AddSpawn(gnd5, 0, 0, 1)
		->AddSpawn(gnd6, 0, 0, 1);
	AddStage(stage, L"지형에 반사하는 샷", 
		L"지형에 부딪히면 상하 이동방향이 반전되는 샷.\n\n"
		L"■샷■\n[X]키/버튼2"
		,VIEW_SIDE);

	//==========================================================
	// 지형에 반사하는 레이저
	CMover* reflect_laser=(new CMover(wg))
		->SetObj3D(Obj3D[LASER_THIN_PURPLE])
		->SetAutoYaw()
		->SetDirected(0.8f, 0.15f)
		->SetGroundHit(GR_HIT, GR_REFLECT)
		->SetLaser(40, -0.02f, true);
	myship=(new CMover(MyShipSideView))
		->SetWeapon_Shot(WP_BASIC_SHOT, reflect_laser);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(gnd1, 0, 0, 1)
		->AddSpawn(gnd2, 0, 0, 1)
		->AddSpawn(gnd3, 0, 0, 1)
		->AddSpawn(gnd4, 0, 0, 1)
		->AddSpawn(gnd5, 0, 0, 1)
		->AddSpawn(gnd6, 0, 0, 1);
	AddStage(stage, L"지형에 반사하는 레이저", 
		L"지형에 부딪히면 상하 이동방향이 반전되는 레이저\n\n"
		L"■레이저■\n[X]키/버튼2"		
		,VIEW_SIDE);

	//==========================================================
	// 방향 전환에 의한 전방위공격
	CMover* turn_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetDirected(3.0f, 0.75f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* myship_crash=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_MYSHIP])
		->SetScale(0.2f, 0.2f, 0.2f, 0.2f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	myship=(new CMover(mg))
		->SetObj3D(Obj3D[MYSHIP])
		->SetMyShip(0, 37.5f, 90, 0.5f)
		->SetWeapon_Shot(WP_AUTO_SHOT, turn_shot)
		->SetDamage(1, 1, myship_crash, NULL, false, false)
		->SetTurnMarker(2, 30, 0.75f, Obj3D[SIGHT_BOMB_READY]);
	enemy[0]=(new CMover(eg))
		->SetPos(37.5f, 0)
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, enemy_crash, NULL, false, false);
	enemy[1]=(new CMover(enemy[0]))
		->SetPos(37.5f, 100);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(enemy[0], 0, 60, -1)
		->AddSpawn(enemy[1], 0, 60, -1);
	AddStage(stage, L"방향 전환에 의한 전방위공격",
		L"방향전환 버튼을 누르고 있는 동안 메인 캐릭터의 방향전환이 가능\n\n"
		L"■샷■\n[X]키/버튼2\n\n"
		L"■방향전환■\n[C]키/버튼3");
}



