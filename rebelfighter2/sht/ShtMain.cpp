#include "ShtMain.h"
#include <sstream>
using namespace std;


// 설명
static wchar_t common_inst[]=
	L"■메인 캐릭터의 이동/스테이지 선택■\n"
	L"방향 키/스틱 (디지털/아날로그)\n"
	L"\n"
	L"■스테이지 시작■\n"
	L"[Z]키/버튼 1\n"
	L"\n"
	L"■풀 스크린 모드로 전환■\n"
	L"[Alt]+[Enter]키\n"
	L"\n"
	L"■종료■\n"
	L"[ESC]키"
	;

static wchar_t book_pr[]=
	L"『게임 매니악스 슈팅 게임 알고리즘』\n"
	L"(원저:마츠우라 켄이치로/츠카사 유키, 소프트뱅크 크리에이티브 발간)\n"
	L"(번역:손정도, 한빛미디어 발간)\n"
	L"\n"
	L"■서적 정보 (Web)■\n"
	L"[F1]키"
	;

// 색
static const D3DCOLOR 
	name_color=D3DCOLOR_ARGB(255, 200, 200, 255),
	name_shade=D3DCOLOR_ARGB(128, 0, 128, 255),
	inst_color=D3DCOLOR_ARGB(255, 255, 255, 255),
	common_inst_color=D3DCOLOR_ARGB(255, 255, 200, 160),
	book_pr_color=D3DCOLOR_ARGB(255, 200, 255, 160);


//==============================================================
// ＜CShtGame＞스테이지 구축
void CShtGame::MakeStages() {
	CMover *crash[100];
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	CMoverGroup* mg=MoverGroupManager->GetGroup(MYSHIP_GROUP);
	CMoverGroup* wg=MoverGroupManager->GetGroup(WEAPON_GROUP);

	//==========================================================
	// 폭발
	crash[3]=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_MYSHIP])
		->SetScale(0.2f, 0.2f, 0.2f, 0.2f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	crash[0]=(new CMover(sg))
		->AddSpawn(crash[3], 0, 0, 1);

	crash[4]=(new CMover(sg))
		->SetObj3D(Obj3D[CRASH_ENEMY])
		->SetScale(0.5f, 0.5f, 0.1f, 0.1f)
		->SetAlpha(2, -0.04f)
		->SetNext(NULL, 50);
	crash[1]=(new CMover(sg))
		->AddSpawn(crash[4], 0, 0, 1);

	//==========================================================
	// 메인 캐릭터
	MyShipTopView=(new CMover(mg))
		->SetObj3D(Obj3D[MYSHIP])
		->SetMyShip(0, 37.5f, 90, 0.5f)
		->SetAutoRoll(0.1f, 0.01f)
		->SetDamage(1, 1, crash[0], NULL, false, false);
	MyShipSideView=(new CMover(mg))
		->SetObj3D(Obj3D[MYSHIP])
		->SetMyShip(0, 10, 37.5f, 0.5f)
		->SetAutoRoll(0.1f, 0.01f, false)
		->SetDamage(1, 1, crash[0], NULL, false, false);

	//==========================================================
	// 별
	// Obj3D를 사용하는 경우에는
	// ->SetStar(Obj3D[STAR0], -2, -2, 77, 102, 0, vy, -100, 100)
	// 등으로 해 줌.
	CMoverGroup* bgb=MoverGroupManager->GetGroup(BG_GROUP);
	float v;
	int i;
	for (i=0, v=0.2f; i<NUM_STARS; i++, v*=2) {
		StarTV[i]=(new CBgMover(bgb))
			->SetStar(0, v, 0.9f, 100, 0xffc0c0c0, 0xffffffff);
	}
	for (i=0, v=-0.2f; i<NUM_STARS; i++, v*=2) {
		StarSV[i]=(new CBgMover(bgb))
			->SetStar(v, 0, 0.9f, 100, 0xffc0c0c0, 0xffffffff);
	}

	//==========================================================
	// 스테이지 작성
	MakeStages_Bullet();
	MakeStages_MyShip();
	MakeStages_Weapon();
	MakeStages_Special();
	MakeStages_Enemy();
	MakeStages_Scroll();
}


//==============================================================
// ＜CShtGame＞스테이지 추가
void CShtGame::AddStage(
	CMover* stage, wstring name, wstring inst, VIEW_TYPE view_type
) {
	Stages.push_back(CStage(stage, name, inst, view_type));
}
void CShtGame::AddStage(CMover* stage, wstring name, VIEW_TYPE view_type) {
	AddStage(stage, name, L"", view_type);
}


//==============================================================
// ＜CShtGame＞스테이지 시작
void CShtGame::StartStage() {
	if (Stages.empty()) return;
	CStage& stage=Stages[SelectedStage];

	CMover* mover;
	MoverGroupManager->DeleteAll();

	// 메인 캐릭터
	CMoverGroup* mg=MoverGroupManager->GetGroup(MYSHIP_GROUP);
	mg->SetInput(Input);

	// 스테이지
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	if (mover=sg->New()) mover->Init(stage.Mover);

	// 별
	CBgMover **star=(stage.ViewType==VIEW_TOP?StarTV:StarSV);
	for (int i=0; i<NUM_STARS; i++) star[i]->New();

	// 스테이지 이름
	Text->Clear();
	DrawShadedText(stage.Name, 0, 0, 
		name_color, name_shade, ALIGN_LEFT, ALIGN_TOP);
}


//==============================================================
// ＜CShtGame＞스테이지 선택
#define DEADZONE 3
void CShtGame::SelectStage() {
	if (Stages.empty()) return;

	// 스테이지 선택
	int size=Stages.size();
	SelectedStage=(SelectedStage%size+size)%size;
	CStage& stage=Stages[SelectedStage];
	
	CMover* mover;
	MoverGroupManager->DeleteAll();

	// 메인 캐릭터
	CMoverGroup* mg=MoverGroupManager->GetGroup(MYSHIP_GROUP);
	mg->SetInput(NULL);

	// 스테이지
	CMoverGroup* sg=MoverGroupManager->GetGroup(STAGE_GROUP);
	if (mover=sg->New()) mover->Init(stage.Mover);
	
	// 별
	CBgMover **star=(stage.ViewType==VIEW_TOP?StarTV:StarSV);
	for (int i=0; i<NUM_STARS; i++) star[i]->New();

	// 뷰 설정
	VIEW_TYPE vt=stage.ViewType;
	CObject3D::SetViewType(vt);
	MoverGroupManager->SetScreenHit(vt==VIEW_TOP?
		CHit(0, 75, 0, 100) : CHit(0, 100, 0, 75));
	MoverGroupManager->SetDeadzoneHit(vt==VIEW_TOP?
		CHit(0-DEADZONE, 75+DEADZONE, 0-DEADZONE, 100+DEADZONE) : 
		CHit(0-DEADZONE, 100+DEADZONE, 0-DEADZONE, 75+DEADZONE));

	// 스테이지 이름과 설명을 표시
	Text->Clear();
	int w2=Text->GetWidth()/2, y=0;
	wostringstream wos;
	wos<<L"Stage#"<<SelectedStage+1<<ends;
	DrawShadedText(wos.str(), w2, y, name_color, name_shade, ALIGN_CENTER, ALIGN_TOP);
	y+=20;
	DrawShadedText(stage.Name, w2, y, name_color, name_shade, ALIGN_CENTER, ALIGN_TOP);
	y+=40;
	y=DrawLines(stage.Inst, y, 20, inst_color);
	y+=20;
	y=DrawLines(common_inst, y, 20, common_inst_color);
	y+=20;
	y=DrawLines(book_pr, y, 20, book_pr_color);
}


//==============================================================
// ＜CShtGame＞생성자
CShtGame::CShtGame()
:	CGame(L"게임 매니악스 슈팅 게임 알고리즘 데모 프로그램 Ver.1.01", true),
	SelectedStage(0), GameMode(GM_SELECT)
{
	// 도움말
	HelpURL=L"http://www.hanb.co.kr/book/look.html?isbn=978-89-6848-053-9";

	// 텍스트，폰트
	Text=new CText(512, 512);
	LargeFont=CreateFont(20, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
//		DEFAULT_PITCH, L"ＭＳゴシック");
		DEFAULT_PITCH, L"MS Gothic");
	SmallFont=CreateFont(16, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
//		DEFAULT_PITCH, L"ＭＳゴシック");
		DEFAULT_PITCH, L"MS Gothic");

	// 이동물체
	MoverGroupManager=new CMoverGroupManager(Graphics->GetDevice());

	// 3D모델 읽어들이기
	CObject3D::Init(Graphics->GetDevice());
	for (int i=0; i<OBJ3D_COUNT; i++) Obj3D.push_back(new CObject3D());
	MakeModels();

	// 스테이지 생성과 선택
	MakeStages();
	SelectStage();
}


//==============================================================
// ＜CShtGame＞파괴자
CShtGame::~CShtGame() {
	for (int i=0; i<(int)Obj3D.size(); i++) delete Obj3D[i];
	CObject3D::Uninit();
	delete MoverGroupManager;
	delete Text;
}


//==============================================================
// 이동
void CShtGame::Move() {
	static bool 
		prev_stick=false, prev_select=false,
		pausing=false, prev_pause=false;

	// 입력상태 취득
	Input->UpdateState();		
	const CInputState* is=Input->GetState(0);

	// 게임 진행
	if (!pausing) {
		MoverGroupManager->Check();
		MoverGroupManager->Move();
		
		// 모드 전환
		switch (GameMode) {
			case GM_GAME:
				if (!prev_select && is->Button[0]) {
					GameMode=GM_SELECT;
					SelectStage();
				}
				break;
			case GM_SELECT:
				if (!prev_stick && is->Left) {
					SelectedStage--;
					SelectStage();
				} else
				if (!prev_stick && is->Right) {
					SelectedStage++;
					SelectStage();
				} else
				if (!prev_stick && is->Up) {
					SelectedStage-=10;
					SelectStage();
				} else
				if (!prev_stick && is->Down) {
					SelectedStage+=10;
					SelectStage();
				} else
				if (!prev_select && is->Button[0]) {
					GameMode=GM_GAME;
					StartStage();
				}
				break;
		}
		prev_stick=is->Up||is->Down||is->Left||is->Right;
		prev_select=is->Button[0];
	}

	// 포즈(일시정지)
	if (!prev_pause && is->Button[3]) pausing=!pausing;
	prev_pause=is->Button[3];
}


//==============================================================
// 화면 표시

// 여러 줄로 된 텍스트 표시
int CShtGame::DrawLines(wstring lines, int y, int vy, D3DCOLOR color) {
	int w2=Text->GetWidth()/2;
	wstring s=lines, t;
	while (!s.empty()) {
		size_t i=s.find(L"\n");
		if (i!=wstring::npos) {
			t=s.substr(0, i);
			s=s.substr(i+1);
		} else {
			t=s;
			s.clear();
		}
		if (!t.empty()) {
			Text->DrawText(t, w2, y, color, SmallFont, ALIGN_CENTER, ALIGN_CENTER);
			y+=vy;
		} else {
			y+=vy/2;
		}
	}
	return y;
}

// 그림자를 가진 텍스트 표시
void CShtGame::DrawShadedText(
	wstring text, int x, int y, D3DCOLOR color, D3DCOLOR shade_color,
	ALIGN_TYPE ax, ALIGN_TYPE ay
) {
	Text->DrawText(text, x+2, y+2, shade_color, LargeFont, ax, ay);
	Text->DrawText(text, x+3, y+2, shade_color, LargeFont, ax, ay);
	Text->DrawText(text, x+1, y  , color      , LargeFont, ax, ay);
	Text->DrawText(text, x  , y  , color      , LargeFont, ax, ay);
}


void CShtGame::SetDevice(LPDIRECT3DDEVICE9* dxdevice) {
	Graphics->SetDevice(dxdevice);
}

// 게임화면 표시
void CShtGame::Draw() {

	if (!Graphics->BeginScene()) return;

	// 화면 클리어
	CImage::SetDeviceState();
	Graphics->Clear(D3DCOLOR_XRGB(0, 0, 64));

	// 이동물체 표시
	CObject3D::SetDeviceState();
	Graphics->Clear();
	MoverGroupManager->Draw();

	// 문자 표시
	CImage::SetDeviceState();
	if (Stages.size()>0) {
		CStage& stage=Stages[SelectedStage];
		int w=Graphics->GetWidth(), h=Graphics->GetHeight(),
			tw=Text->GetWidth(), th=Text->GetHeight();

		// 선택화면 : 스테이지 이름，설명，공통설명 표시
		if (GameMode==GM_SELECT) {
			Text->Draw((w-tw)/2, (h-th)/2);
		} else
		
		// 게임화면 : 스테이지 이름만 표시
		{
			Text->Draw(stage.ViewType==VIEW_TOP?(w-h*3/4)/2:0, 0, 
				D3DCOLOR_ARGB(128, 255, 255, 255));
		}
	}

	// 플립
	Graphics->EndScene();
	Graphics->Present();
}



//
////==============================================================
//// 메인 루틴
//INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, INT) {
//	CShtGame* game=new CShtGame();
//	game->Run();
//	delete game;
//	return 0;
//}
//
