#include "ShtMain.h"


//==============================================================
// ＜CShtGame＞스테이지 구축(특수 공격)
void CShtGame::MakeStages_Special() {
	CMover *stage;
	CMoverGroup* mg=MoverGroupManager->GetGroup(MYSHIP_GROUP);
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	CMoverGroup* bg=MoverGroupManager->GetGroup(BULLET_GROUP);
	CMoverGroup* eg=MoverGroupManager->GetGroup(ENEMY_GROUP);
	CMoverGroup* wg=MoverGroupManager->GetGroup(WEAPON_GROUP);
	CMoverGroup* gg=MoverGroupManager->GetGroup(GROUND_GROUP);
	CMoverGroup* ig=MoverGroupManager->GetGroup(ITEM_GROUP);
	CMoverGroup* ag=MoverGroupManager->GetGroup(WATER_GROUP);
	CMoverGroup* rg=MoverGroupManager->GetGroup(BARRIER_GROUP);
	CMover *myship;

	CMover* shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* max_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* enemy_crash=(new CMover(sg))
		->AddSpawn((new CMover(sg))
			->SetObj3D(Obj3D[CRASH_ENEMY])
			->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
			->SetAlpha(2, -0.04f)
			->SetNext(NULL, 50)
			, 0, 0, 1);
	CMover* enemy=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetPos(37.5f, 0)
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, enemy_crash, NULL, false, false);
	CMover* bullet=(new CMover(bg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.6f, 0)
		->SetYaw(0.05f);

	//==========================================================
	// 폭탄
	CMover* bomb_bullet=(new CMover(eg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.6f, 0)
		->SetYaw(0.05f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* bomb_enemy=(new CMover(enemy))
		->AddSpawn(bomb_bullet, 0, 50, -1);
	CMover* bomb_crash=(new CMover(wg))
		->SetObj3D(Obj3D[CRASH_MISSILE])
		->SetScale(8, 8, 0.1f, 0.1f)
		->SetAlpha(2, -0.02f)
		->SetNext(NULL, 200);
	CMover* bomb=(new CMover(wg))
		->SetObj3D(Obj3D[MISSILE_SMALL])
		->SetDirected(1.0f, 0.75f)
		->SetDamage(0, 1, NULL, NULL, false, false)
		->AddSpawn(bomb_crash, 40, 0, 1)
		->SetNext(NULL, 40);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_BASIC_SHOT, bomb);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(bomb_enemy, 0, 100, -1);
	AddStage(stage, L"폭탄",
		L"폭발할 때까지 잠시 시간이 걸리는 폭탄. \n메인 캐릭터는 무적상태가 되지 않음.\n\n"
		L"■폭탄■\n[X]키/버튼2");

	//==========================================================
	// 근접 공격
	CMover* punch=(new CMover(wg))
		->SetObj3D(Obj3D[PUNCH])
		->SetAlpha(0.2f, 0.03f);
	myship=(new CMover(MyShipTopView))
		->SetPunch(punch, 1, 60);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(bomb_enemy, 0, 100, -1);
	AddStage(stage, L"근접공격",
		L"버튼을 누르면 일정 시간동안 근접공격을 함.\n\n"
		L"■근접공격■\n[X]키/버튼2");

	//==========================================================
	// 유도 폭파
	CMover* ie_crash=(new CMover(wg))
		->SetObj3D(Obj3D[CRASH_ENEMY])
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	CMover* ie_enemy=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY])
		->SetAiming(0.3f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, ie_crash, NULL, false, false);
	CMover* ie_enemy0=(new CMover(ie_enemy))->SetPos(37.5f, 0);
	CMover* ie_enemy1=(new CMover(ie_enemy))->SetPos(0, 20);
	CMover* ie_enemy2=(new CMover(ie_enemy))->SetPos(75, 20);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_BASIC_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(ie_enemy0, 0, 60, -1)
		->AddSpawn(ie_enemy1, 20, 60, -1)
		->AddSpawn(ie_enemy2, 40, 60, -1);
	AddStage(stage, L"유도 폭파",
		L"파괴된 적기의 후폭풍이 다른 적기에게 피해를 줌.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 아이템에 의한 특수공격
	CMover* sa_item0=(new CMover(ig))
		->SetObj3D(Obj3D[ITEM_YELLOW])
		->SetPos(37.5f, 0)
		->SetDirected(0.3f, 0.25f)
		->SetRoll(0.01f)
		->SetItemEffect_SA(true);
	CMover* sa_effect=(new CMover(sg))
		->SetObj3D(Obj3D[BERSERK])
		->SetRoll(0.01f);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_AUTO_SHOT, shot)
		->SetSAItem(max_shot, sa_effect, 300, 60);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(sa_item0, 0, 150, -1);
	AddStage(stage, L"아이템에 의한 특수공격",
		L"아이템을 얻으면 일정시간동안 강력한 샷을 쏠 수 있게됨.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 무적상태
	CMover* inv_bullet=(new CMover(eg))
		->SetObj3D(Obj3D[BULLET_SMALL_BLUE])
		->SetAiming(0.6f, 0)
		->SetYaw(0.05f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* inv_enemy=(new CMover(enemy))
		->AddSpawn(inv_bullet, 0, 50, -1);
	myship=(new CMover(MyShipTopView))
		->SetDamage(1, 1, NULL, NULL, true, false);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(inv_enemy, 0, 60, -1);
	AddStage(stage, L"무적상태",
		L"적기나 탄에 맞아도 끄떡없는 무적상태.");

	//==========================================================
	// 버서크
	CMover* bs_item=(new CMover(ig))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetDirected(3.0f, 0.75f);
	CMover* bs_myship2=(new CMover(mg))
		->SetObj3D(Obj3D[MYSHIP2])
		->SetPos(37.5f, 60)
		->AddSpawn(bs_item, 0, 5, -1);
	CMover* bs_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0)
		->SetAutoYaw()
		->SetRand(1, 1)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* bs_effect=(new CMover(sg))
		->SetObj3D(Obj3D[BERSERK])
		->SetRoll(0.04f)
		->AddSpawn(bs_shot, 0, 1, -1)
		->AddSpawn(bs_shot, 0, 1, -1);
	myship=(new CMover(MyShipTopView))
		->SetBerserk(bs_effect, 10, 300, 60, 200)
		->SetWeapon_Shot(WP_BASIC_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(bs_myship2, 0, 0, 1);
	AddStage(stage, L"버서크",
		L"아군 캐릭터의 샷을 연달아 맞으면 버서크 상태가 됨.\n"
		L"버서크 상태에서는 모든 방향으로 샷을 난사함.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 아군을 집어서 던지기
	CMover* tfs_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_SMALL])
		->SetDirected(3.0f, 0.75f)
		->SetAutoYaw();
	CHit* tfs_hit=new CHit(-6, 6, -6, 6);
	CMover* tfs_myship2=(new CMover(mg))
		->SetMyShip(1, 37.5f, 60, 0.5f)
		->SetObj3D(Obj3D[MYSHIP2])
		->AddSpawn(tfs_shot, 0, 5, -1)
		->SetThrow(1, tfs_hit, -1.0f, -2.0f, 0.1f, 300);
	myship=(new CMover(MyShipTopView))
		->SetThrow(1, tfs_hit, -1.0f, -2.0f, 0.1f, 300);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(tfs_myship2, 0, 0, 1);
	AddStage(stage, L"아군을 집어서 던지기",
		L"아군 캐릭터 근처에서 버튼을 누르면 아군을 집어서 던질 수 있게됨.\n\n"
		L"■던지기■\n[X]키/버튼2");

	//==========================================================
	// 적기를 집어서 던지기
	CMover* te_enemy=(new CMover(enemy));
	CMover* te_crash=(new CMover(wg))
		->SetObj3D(Obj3D[CRASH_ENEMY])
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	CMover* te_shot=(new CMover(wg))
		->SetObj3D(Obj3D[ENEMY])
		->SetDirected(2.0f, 0.75f)
		->SetRoll(0.05f)
		->SetDamage(1, 1, te_crash, NULL, false, false);
	myship=(new CMover(MyShipTopView))
		->SetArm(2, Obj3D[BARRIER], Obj3D[BULLET_SMALL_BLUE], 2.0f, 20, 10, 5)
		->SetWeapon_Shot(WP_BASIC_SHOT, te_shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(te_enemy, 0, 60, -1);
	AddStage(stage, L"적기를 집어서 던지기",
		L"적기를 집게로 집어서 다른 버튼을 눌러 던지기.\n\n"
		L"■던지기■\n[X]키/버튼2\n\n"
		L"■집게■\n[C]키/버튼3");

	//==========================================================
	// 적기를 사로잡아서 아군으로 만들기
	CMover* ce_enemy=(new CMover(enemy))
		->AddSpawn(bullet, 0, 50, -1);
	CMover* ce_shot=(new CMover(rg))
		->SetObj3D(Obj3D[ENEMY])
		->SetRoll(0.02f)
		->AddSpawn(shot, 0, 5, -1)
		->SetDamage(1, 10, enemy_crash, NULL, false, false);
	myship=(new CMover(MyShipTopView))
		->SetArm(2, Obj3D[BARRIER], Obj3D[BULLET_SMALL_BLUE], 2.0f, 20, 10, 5)
		->SetWeapon_Shot(WP_BASIC_SHOT, ce_shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(ce_enemy, 0, 60, -1);
	AddStage(stage, L"적기를 사로잡아서 아군으로 만들기",
		L"적기를 집게로 사로잡은 후 다른 버튼으로 공격시키기.\n\n"
		L"■사로잡은 적기 공격■\n[X]키/버튼2\n\n"
		L"■집게■\n[C]키/버튼3");
	
	//==========================================================
	// 적기에 사로잡힌 메인 캐릭터를 되찾아서 파워 업 하기
	CMover* cp_beam=(new CMover(bg))
		->SetObj3D(Obj3D[BARRIER])
		->SetScale(0.5f, 0.1f, 0.1f, 0.05f)
		->SetDirected(1.0f, 0.25f)
		->SetAlpha(1, -0.02f)
		->SetNext(NULL, 30);
	CMover* cp_enemy=(new CMover(eg))
		->SetObj3D(Obj3D[ENEMY_REV])
		->SetCaptor(cp_beam);
	CMover* cp_enemy_crash=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_ENEMY])
		->SetScale(0.2f, 0.2f, 0.2f, 0.2f)
		->SetAlpha(2, -0.04f)
		->AddSpawn(cp_enemy, 50, 0, 1)
		->SetNext(NULL, 50);
	cp_enemy
		->SetDamage(1, 1, cp_enemy_crash, NULL, false, false);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_BASIC_SHOT, shot)
		->SetCaptee();
	CMover* cp_myship_crash=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_MYSHIP])
		->SetScale(0.2f, 0.2f, 0.2f, 0.2f)
		->SetAlpha(2, -0.04f)
		->AddSpawn(myship, 50, 0, 1)
		->SetNext(NULL, 50);
	myship
		->SetDamage(1, 1, cp_myship_crash, NULL, false, false);
	stage=(new CMover(sg))
		->SetPos(37.5f, 30)
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(cp_enemy, 0, 0, 1);
	AddStage(stage, L"적기에 사로잡힌 메인 캐릭터를 되찾아서 파워업하기",
		L"적기의 빔을 맞으면 메인 캐릭터를 빼앗김.\n"
		L"빼앗긴 상태에서 적기를 파괴하면 메인 캐릭터가 파워업하여 2중 공격이 됨.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 아군에 접근하여 샷을 강화시키기
	CMover* rf1_myship=(new CMover(mg))
		->SetMyShip(1, 37.5f, 60, 0.5f)
		->SetObj3D(Obj3D[MYSHIP2]);
	CMover* rf1_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0.75f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false);
	CHit* rf1_hit=new CHit(-4, 4, -4, 4);
	myship=(new CMover(MyShipTopView))
		->SetRFShot(0, rf1_shot, rf1_hit)
		->SetWeapon_Shot(WP_AUTO_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(rf1_myship, 0, 0, 1);
	AddStage(stage, L"아군에 접근하여 샷을 강화시키기",
		L"아군 캐릭터에 접근하면 강력한 샷을 쏠 수 있음.\n\n"
		L"■샷■\n[X]키/버튼2");
	
	//==========================================================
	// 아군 방향으로 강한 샷을 발사하기
	myship=(new CMover(MyShipTopView))
		->SetRFShot(1, rf1_shot, rf1_hit)
		->SetWeapon_Shot(WP_AUTO_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(rf1_myship, 0, 0, 1);
	AddStage(stage, L"아군 방향으로 강한 샷을 발사하기",
		L"아군 캐릭터에 접근하면 강력한 샷을 쏠 수 있음.\n"
		L"아군 캐릭터와의 위치관계에 따라 샷의 방향이 바뀜.\n\n"
		L"■샷■\n[X]키/버튼2");
	
	//==========================================================
	// 샷을 아군에 맞춰서 강화시키기
	CMover* rf3_myship=(new CMover(mg))
		->SetMyShip(1, 37.5f, 60, 0.5f)
		->SetRFShot(2, rf1_shot, rf1_hit)
		->SetObj3D(Obj3D[MYSHIP2]);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_AUTO_SHOT, shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(rf3_myship, 0, 0, 1);
	AddStage(stage, L"샷을 아군에 맞춰서 강화시키기",
		L"아군 캐릭터에게 샷을 쏘아 맞추면 강력한 샷으로 바뀜.\n\n"
		L"■샷■\n[X]키/버튼2");

	//==========================================================
	// 메인 캐릭터의 색을 바꾸어 탄을 흡수하기
	CMover** sc_effect=new CMover*[2];
	sc_effect[0]=sc_effect[1]=(new CMover(sg))
		->SetObj3D(Obj3D[BERSERK])
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(1, -0.08f)
		->SetNext(NULL, 10);
	CMover* sc_crash1=(new CMover(sg))
		->SetObj3D(Obj3D[BULLET_SMALL_ORANGE])
		->SetScale(1, 1, 0.01f, 0.01f)
		->SetAlpha(1, -0.1f)
		->SetNext(NULL, 10);
	CMover* sc_crash2=(new CMover(sg))
		->SetObj3D(Obj3D[BULLET_SMALL_GREEN])
		->SetScale(1, 1, 0.01f, 0.01f)
		->SetAlpha(1, -0.1f)
		->SetNext(NULL, 10);
	CMover* sc_bullet1=(new CMover(bullet))
		->SetObj3D(Obj3D[BULLET_SMALL_ORANGE])
		->SetColor(1)
		->SetPos(20, 0)
		->SetRand(0.2f, 0.2f)
		->SetDamage(1, 1, sc_crash1, NULL, false, false);
	CMover* sc_bullet2=(new CMover(bullet))
		->SetObj3D(Obj3D[BULLET_SMALL_GREEN])
		->SetColor(2)
		->SetPos(55, 0)
		->SetRand(0.2f, 0.2f)
		->SetDamage(1, 1, sc_crash2, NULL, false, false);
	CMover** sc_shot=new CMover*[2];
	sc_shot[0]=(new CMover(shot))
		->SetColor(1)
		->SetObj3D(Obj3D[SHOT_SMALL_ORANGE]);
	sc_shot[1]=(new CMover(shot))
		->SetColor(2)
		->SetObj3D(Obj3D[SHOT_SMALL_GREEN]);
	CHit* sc_hit=new CHit(-4, 4, -4, 4);
	CObject3D** sc_obj3d=new CObject3D*[2];
	sc_obj3d[0]=Obj3D[MYSHIP];
	sc_obj3d[1]=Obj3D[MYSHIP2];
	myship=(new CMover(MyShipTopView))
		->SetColor(1)
		->SetChangeColor(2, 2, sc_obj3d, sc_shot, sc_effect, sc_hit)
		->SetWeapon_Shot(WP_AUTO_SHOT, sc_shot[0]);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(sc_bullet1, 0, 1, -1)
		->AddSpawn(sc_bullet2, 0, 1, -1);
	AddStage(stage, L"메인 캐릭터의 색을 바꾸어 탄을 흡수하기", 
		L"메인 캐릭터와 같은 색의 탄을 흡수하는 특수공격.\n"
		L"샷도 메인 캐릭터와 같은 색깔이 됨.\n\n"
		L"■샷■\n"
		L"[X]키/버튼2\n\n"
		L"■색깔 전환■\n"
		L"[C]키/버튼3");

	//==========================================================
	// 적기의 탄을 튕겨내기
	CMover* rf_enemy0=(new CMover(enemy))
		->SetPos(20, 0)
		->SetDamage(1, 10, enemy_crash, NULL, false, false)
		->AddSpawn(bullet, 0, 10, -1);
	CMover* rf_enemy1=(new CMover(enemy))
		->SetPos(55, 0)
		->SetDamage(1, 10, enemy_crash, NULL, false, false)
		->AddSpawn(bullet, 0, 10, -1);
	CMover* rf_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetAiming(3.0f, 0)
		->SetAutoYaw()
		->SetTarget(TG_RANDOM_ENEMY, 0, shot)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* reflect=(new CMover(sg))
		->SetObj3D(Obj3D[BERSERK]);
	myship=(new CMover(MyShipTopView))
		->SetReflect(1, reflect, rf_shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(rf_enemy0, 0, 50, -1)
		->AddSpawn(rf_enemy1, 25, 50, -1);
	AddStage(stage, L"적기의 탄을 튕겨내기",
		L"보호막에 맞은 탄을 적기에게 튕겨내는 특수공격.\n\n"
		L"■보호막■\n"
		L"[X]키/버튼2");

	//==========================================================
	// 레이저끼리 부딪히기
	CMover* lc_crash=(new CMover(wg))
		->SetObj3D(Obj3D[CRASH_MISSILE])
		->SetScale(12, 12, 0.2f, 0.2f)
		->SetAlpha(1, -0.05f)
		->SetNext(NULL, 20);
	CMover* lc_crash_small=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_MISSILE])
		->SetScale(4, 4, 0.2f, 0.2f)
		->SetAlpha(1, -0.05f)
		->SetNext(NULL, 20);
	CMover* lc_shot0=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0.75f)
		->SetAutoYaw()
		->SetDamage(1, 1, lc_crash_small, NULL, false, false)
		->SetCollision(20, lc_crash);
	CMover* lc_shot1=(new CMover(eg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetPos(37.5f, 0)
		->SetDirected(3.0f, 0.25f)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false);
	myship=(new CMover(MyShipTopView))
		->SetWeapon_Shot(WP_AUTO_SHOT, lc_shot0);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(lc_shot1, 0, 1, -1);
	AddStage(stage, L"레이저끼리 부딪히기",
		L"레이저끼리 충돌시키면 강력한 공격이 발생함.\n\n"
		L"■레이저■\n[X]키/버튼2");

	//==========================================================
	// 적기의 탄에 메인 캐릭터를 스치기
	CMover* gb_bullet=(new CMover(bullet))
		->SetPos(37.5f, 0)
		->SetRand(0.2f, 0.2f);
	CMover* gb_effect=(new CMover(sg))
		->SetObj3D(Obj3D[BULLET_SMALL_PURPLE])
		->SetScale(1.4f, 1.4f, -0.1f, -0.1f)
		->SetAlpha(1, -0.1f)
		->SetNext(NULL, 10);
	CMover* gb_field=(new CMover(wg))
		->SetObj3D(Obj3D[BERSERK])
		->SetFollowParent()
		->SetGraze(gb_effect)
		->SetDamage(1, 1, NULL, NULL, true, false);
	myship=(new CMover(MyShipTopView))
		->AddSpawn(gb_field, 0, 0, 1);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(gb_bullet, 0, 1, -1);
	AddStage(stage, L"적기의 탄에 메인 캐릭터를 스치기",
		L"적기가 쏜 탄이 메인 캐릭터 근처를 지나가면 '스침'판정");

	//==========================================================
	// 적기나 탄의 움직임을 느리게 하기
	CMover* sl_bullet=(new CMover(bullet))
		->SetRand(0.1f, 0.1f);
	CMover* sl_enemy=(new CMover(enemy))
		->SetRand(0.1f, 0.1f)
		->AddSpawn(sl_bullet, 0, 8, -1);
	myship=(new CMover(MyShipTopView))
		->SetSlow(1, -1)
		->SetQuick(2, 1);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(sl_enemy, 0, 50, -1);
	AddStage(stage, L"적기나 탄의 움직임을 느리게 하기", 
		L"버튼조작으로 탄이나 적기의 움직임을 변화시키기.\n\n"
		L"■슬로우■\n"
		L"[X]키/버튼2\n\n"
		L"■퀵■\n"
		L"[C]키/버튼3"
		);

	//==========================================================
	// 자유롭게 움직일 수 있는 조준
	CMover* ms1_sight=(new CMover(sg))
		->SetObj3D(Obj3D[SIGHT_BOMB_READY]);
	CMover* ms1_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_LARGE])
		->SetDirected(3.0f, 0.75f)
		->SetDamage(1, 1, NULL, NULL, false, false);
	CMover* ms1_attack=(new CMover(sg))
		->SetPos(37.5f, 50)
		->SetObj3D(Obj3D[SIGHT_BOMB_TARGET])
		->AddSpawn(ms1_shot, 0, 1, -1)
		->SetNext(NULL, 60);
	CMover* ms1_enemy=(new CMover(enemy))
		->SetRand(0.1f, 0.1f);
	myship=(new CMover(MyShipTopView))
		->SetMovableSightB(1, 0, -10, 1.0f, ms1_sight, ms1_attack);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(ms1_enemy, 0, 50, -1);
	AddStage(stage, L"자유롭게 움직일 수 있는 조준",
		L"버튼을 꾹 누르고 있으면 조준을 움직일 수 있음.\n"
		L"버튼을 떼면 조준의 위치에 공격을 가함.\n\n"
		L"■조준이동■\n[X]키/버튼2");

	//==========================================================
	// 메인 캐릭터와 조준을 동시에 움직이기
	CMover* ms2_sight=(new CMover(sg))
		->SetObj3D(Obj3D[SIGHT_BOMB_READY]);
	CMover* ms2_shot=(new CMover(wg))
		->SetObj3D(Obj3D[SHOT_MEDIUM])
		->SetAiming(3.0f, 0)
		->SetAutoYaw()
		->SetDamage(1, 1, NULL, NULL, false, false);
	myship=(new CMover(MyShipTopView))
		->SetMovableSightS(0, -20, 1.0f, ms2_sight)
		->SetWeapon_Shot(WP_AUTO_SHOT, ms2_shot);
	stage=(new CMover(sg))
		->AddSpawn(myship, 0, 0, 1)
		->AddSpawn(ms1_enemy, 0, 50, -1);
	AddStage(stage, L"메인 캐릭터와 조준을 동시에 움직이기.",
		L"메인 캐릭터와 조준을 서로 다른 키로 조작하기.\n"
		L"샷은 조준을 향해 발사됨.\n\n"
		L"■조준이동■\n[E][D][S][F]키\n\n"
		L"■샷■\n[X]키/버튼2"
		);
}



