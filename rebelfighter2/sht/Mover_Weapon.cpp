#include "Mover.h"
#include "Hit.h"
#include "Input.h"
#include <assert.h>




//==============================================================
// 샷의 서브 루틴

// 보통 샷
void CMover::Shot() {

	// 약한 상태
	if (Valid.Berserk && Berserk.State!=BS_NORMAL) return;
	
	// 색 바꾸기
	if (Valid.ChangeColor) {
		New(ChangeColor.Shot[Color-1]);
	}

	// 집게
	if (Valid.Arm) {
		if (Arm.NumStock<=0) return;
		Arm.NumStock--;
		CMover* model=Weapon.Shot;
		CObject3D* obj3d=model->Obj3D;
		model->Obj3D=Arm.Stock[Arm.NumStock];
		New(model);
		model->Obj3D=obj3d;
	} else

	// 샷의 강화
	if (Valid.RFShot) {
		CMoverGroup* mg=GetGroup(MYSHIP_GROUP);
		CMover *mover;
		CHit hit=*RFShot.Hit;
		hit.SetPos(X, Y);
		if (mover=mg->FirstHit(&hit)) {
			do {
				if (mover!=this) {
					switch (RFShot.Mode) {
						case 0:
							New(RFShot.Shot);
							return;
						case 1:
							CMover model=*RFShot.Shot;
							model.Angle=
								atan2(X-mover->X, mover->Y-Y)/D3DX_PI/2+0.25f;
							New(&model);
							return;
					}
				}
			} while (mover=mg->NextHit(&hit));
			New(Weapon.Shot);
		}
	} else

	// 자유롭게 움직일 수 있는 조준
	if (Valid.MovableSightS && MovableSightS.SightMover) {
		CMover* sight=MovableSightS.SightMover;
		CMover model=*Weapon.Shot;
		model.Valid.Target=true;
		model.Target.Type=TG_MOVER;
		model.Target.Mover=sight;
		New(&model);
	} else

	// 합체
	if (Valid.Docking && Docking.Docked) {
		New(Docking.Shot);
	} else

	// 아이템에 의한 강화
	if (Valid.SAItem && SAItem.EffectMover) {
		New(SAItem.ShotModel);
	} else

	// 통상
	{
		New(Weapon.Shot);
		if (Valid.Captee && Captee.Doubled) {
			CMover* mover=New(Weapon.Shot);
			mover->X+=10;
		}
	}
}


// 에너지 샷
void CMover::BigShot(int power) {
	New(Weapon.BigShot);
}


// 최대 파워의 에너지 샷
void CMover::MaxShot() {
	New(Weapon.MaxShot);
}




//==============================================================
// 기본적인 샷 조작
void CMover::BasicShot(
	bool button,       // 지금의 버튼상태
	bool& prev_button  // 이전의 버튼상태
) {
	// 샷의 발사:
	// 전에 버튼이 눌려져 있지 않다가 이번에 처음 눌렸을 때만
	// 샷을 발사함.
	// 샷을 발사하는 구체적인 처리는 Shot 함수에서 수행하기로 함.
	if (!prev_button && button) Shot();
	
	// 지금의 버튼 상태를 저장해 둠
	prev_button=button;
}


//==============================================================
// 연사
void CMover::AutoShot(
	bool button  // 버튼 상태 (눌렸으면 true)
) {
	// 샷의 발사:
	// 버튼이 눌려지면 샷을 발사하기.
	// 발사에 관한 상세한 처리는 Shot 함수에서 수행하기로 함.
	if (button) Shot();
}


//==============================================================
// 에너지샷
void CMover::PowerShot(
	bool button,    // 버튼 상태 (눌렸으면 true)
	int min_power,  // 에너지 샷이 발사될 최소한의 파워
	int max_power,  // 최대 파워
	int& power      // 모은 에너지
) {
	// 버튼을 누르고 있을 때:
	// 에너지를 모음
	if (button && power<max_power) power++;
	
	// 버튼을 누르고 있지 않을 때:
	// 모은 에너지가 0보다 크다면 샷을 발사하고 에너지를 0으로 되돌림.
	if (!button && power>0) {
		
		// 최대 파워일 때:
		// 특별한 샷을 발사.
		// 구체적인 처리는 MaxShot 함수에서 수행하기로 함.
		if (power==max_power) MaxShot(); else
		
		// 최소 파워 이상, 최대 파워 미만일 경우:
		// 모아둔 에너지에 해당하는 공격력의 에너지 샷을 발사함.
		// 구체적인 처리는 BigShot 함수에서 수행하기로 함.
		if (min_power<=power) BigShot(power);
		
		// 에너지를 0으로 되돌림.
		power=0;
	}
}


//==============================================================
// 세미 오토 연사
void CMover::SemiAutoShot(
	bool button,    // 버튼 상태 (눌렸으면 true)
	int min_power,  // 에너지의 최소치
	int max_power,  // 에너지의 최대치
	int max_gauge,  // 연사 게이지의 최대치
	int& power,     // 모은 에너지
	int& gauge      // 연사 게이지
) {
	// 버튼을 누르고 있을 때:
	if (button) {
		
		// 에너지가 부족할 경우:
		// 보통 샷을 발사.
		if (power<min_power) Shot();
	
		// 파워를 늘리고, 연사 게이지를 최대로 함.
		if (power<max_power) power++;
		gauge=max_gauge;
	}
	
	// 버튼을 떼고 있을 때:
	else {
		
		// 에너지가 충분할 경우:
		// 에너지 샷을 발사:
		// 모아둔 에너지가 최대치에 도달했을 때는 특별한 샷을 발사.
		if (min_power<=power) {
			if (power==max_power) MaxShot(); 
				else BigShot(power);
			gauge=0;
		}
		
		// 연사 게이지가 0보다 클 경우:
		// 보통 샷을 발사하고, 연사 게이지를 1 감소시킴.
		if (gauge>0) {
			Shot();
			gauge--;
		}

		// 에너지를 0으로 함.
		power=0;
	}
}


//==============================================================
// 버튼을 뗀 상태일 때 에너지를 모으는 에너지 탄
void CMover::PowerShotReleased(
	bool button,    // 버튼 상태 (눌렸으면 true)
	int min_power,  // 에너지 샷이 발사될 최소한의 파워
	int max_power,  // 최대 파워
	int& power      // 모은 에너지
) {
	// 버튼을 누르고 있지 않을 때:
	// 에너지를 모음
	if (!button && power<max_power) power++;
	
	// 버튼을 누르고 있을 때:
	// 모은 에너지가 0보다 크다면 샷을 발사하고 에너지를 0으로 되돌림.
	if (button && power>0) {
		
		// 최대 파워일 때:
		// 특별한 샷을 발사.
		// 구체적인 처리는 MaxShot 함수에서 수행하기로 함.
		if (power==max_power) MaxShot(); else
		
		// 최소 파워 이상, 최대 파워 미만일 경우:
		// 모아둔 에너지에 해당하는 공격력의 에너지 샷을 발사함.
		// 구체적인 처리는 BigShot 함수에서 수행하기로 함.
		if (min_power<=power) BigShot(power);
		
		// 에너지를 0으로 되돌림.
		power=0;
	}
}


//==============================================================
// 커맨드 샷
void CMover::CommandShot(const CInputState* is) {
	int& index=Weapon.Cmd.HistIndex;
	int* hist=Weapon.Cmd.Hist;
	COMMAND_LIST* list=Weapon.Cmd.List;

	// 입력 내역을 기록하기
	hist[index]=
		(is->Up?CMD_UP:0)|
		(is->Down?CMD_DOWN:0)|
		(is->Left?CMD_LEFT:0)|
		(is->Right?CMD_RIGHT:0)|
		(is->Button[0]?CMD_BUTTON0:0)|
		(is->Button[1]?CMD_BUTTON1:0)|
		(is->Button[2]?CMD_BUTTON2:0)|
		(is->Button[3]?CMD_BUTTON3:0);

	// 커맨드가 입력되었는지 판정하기
	int c, j, i;
	for (c=0; c<list->NumCommand; c++) {
		COMMAND& cmd=list->Command[c];
		
		// 커맨드 입력 확인
		for (i=0, j=cmd.Length-1; j>=0; j--) {
			for (; i<cmd.Limit; i++) {
				if (hist[(index-i+NUM_CMD_HIST)%NUM_CMD_HIST]==
					cmd.Input[j]) break;
			}
			if (i==cmd.Limit) break;
		}
		
		// 커맨드가 입력된 것이 확인되었음:
		// 커맨드 샷을 발사하고 입력 내역을 지움.
		if (j==-1) {
			CMover* mover=Group->GetManager()->GetGroup(WEAPON_GROUP)->New();
			if (mover) mover->Init(cmd.Model, this);
			for (i=0; i<NUM_CMD_HIST; i++) hist[i]=CMD_NONE;
		}
	}
	
	// 기록 위치를 갱신하기
	index=(index+1)%NUM_CMD_HIST;
}


//==============================================================
// 연사와 레이저를 함께 사용하기
void CMover::ShotAndLaser(
	bool button,    // 버튼 상태 (눌렸으면 true)
	int min_power,  // 에너지 샷이 발사될 최소한의 파워
	int max_gauge,  // 연사 게이지의 최대치
	int& power,     // 모은 에너지
	int& gauge      // 연사 게이지
) {
	// 버튼을 누르고 있을 때:
	if (button) {
		
		// 에너지가 부족할 경우:
		// 에너지를 늘려주고 연사 게이지를 최대값으로 하여
		// 샷을 발사함. 구체적인 처리는 Shot 함수에서 수행하기로 함.
		if (power<min_power) {
			power++;
			gauge=max_gauge;
			Shot();
		}
	
		// 에너지가 충분한 경우:
		// 연사 게이지를 0으로 하고 레이저를 발사.
		else {
			gauge=0;
			CMover* mover=Group->GetManager()->GetGroup(WEAPON_GROUP)->New();
			if (mover) mover->Init(Weapon.Laser, this);
		}
	}
	
	// 버튼을 누르고 있지 않을 때:
	else {
		
		// 연사 게이지가 0보다 클 경우:
		// 보통 샷을 쏘고 연사 게이지를 1 감소시킴.
		if (gauge>0) {
			Shot();
			gauge--;
		}

		// 에너지를 0으로 함.
		power=0;
	}
}


//==============================================================
// 록 샷
void CMover::LockShot(
	bool button,     // 버튼 상태 (눌렸으면 true)
	int min_power,   // 에너지의 최소치
	int max_gauge,   // 연사 게이지의 최대치
	int& power,      // 모은 에너지
	int& gauge       // 연사 게이지
) {
	// 버튼을 누르고 있을 때:
	if (button) {
		
		// 에너지가 부족할 경우:
		// 에너지를 늘리고 연사 게이지를 최대값으로 한 후 
		// 샷을 발사. 구체적인 처리는 Shot 함수에서 수행하기로 함.
		if (power<min_power) {
			power++;
			gauge=max_gauge;
			Shot();
		}
	
		// 에너지가 충분할 경우:
		// 연사 게이지를 0으로 한 후 록 샷을 발사.
		else {
			gauge=0;
			
			// 록 샷을 발사:
			// 적기를 추적할 때는 조준탄을 쏨.
			// ("메인 캐릭터를 향하는 탄"과 같음)
			// 적기를 추적하지 않을 때는 메인 캐릭터의 정면을 향해 발사.
			// ("임의 방향으로 날아가는 탄"과 같음)
			CMover* mover=Group->GetManager()->GetGroup(WEAPON_GROUP)->New();
			mover->Init(Weapon.Laser, this);
		}
	}
	
	// 버튼을 누르고 있지 않을 때:
	else {
		
		// 연사 게이지가 0보다 클 경우:
		// 보통 샷을 쏜 후, 연사 게이지를 1 감소시킴.
		if (gauge>0) {
			Shot();
			gauge--;
		}

		// 에너지를 0으로 함.
		power=0;

		// 록을 해제함
		ClearLockedEnemy();
	}
}




