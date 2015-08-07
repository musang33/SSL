// VisualTool.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "VisualTool.h"

#include "..\Core\Entity.h"
#include "..\Core\StateNPC.h"
#include "..\Core\StatePlayer.h"
#include "..\Core\LuaManager.h"
#include "..\Core\TimerWorker.h"
#include "..\Core\MessageManager.h"
#include "..\Core\EntityManager.h"
#include "..\Core\ThreadEventManager.h"
#include "..\Core\Event.h"
#include "..\Core\EntityFactory.h"

#include "..\Core\ActionMove.h"
#include "..\Core\ActionAI.h"
#include "..\Core\ActionState.h"

#include "..\Core\FactoryNpcImpl.h"
#include "..\Core\FactoryPlayerImpl.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass( HINSTANCE hInstance );
BOOL				InitInstance( HINSTANCE, int );
BOOL				InitEntities();
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK	About( HWND, UINT, WPARAM, LPARAM );
void				DrawEntity();
void				DrawCell();
//==============================================================

const static INT TIMER_ENTITY = 10000;

//==============================================================

HWND hWnd;

const static INT mostLeftX = 8;
const static INT mostLeftY = 8;

const static INT xCellCount = 15;
const static INT yCellCount = 15;
const static INT cellSize = 25;
const static INT spaceBetweenEdgeAndEntity = 2;

//=============================================================

class World
{
public:
	World() {};
	~World() {};
};

//==============================================================

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_VISUALTOOL, szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );
		
	// Thread initialize
	SSL::ThreadEventManager::GetInstance()->StartThreadWorker( 5 );

	// Factory initialize
	SSL::EntityFactory::GetInstance()->AddFactoryImpl( new SSL::FactoryNpcImpl );
	SSL::EntityFactory::GetInstance()->AddFactoryImpl( new SSL::FactoryPlayerImpl );
	
	// User와 NPC 설정
	if ( !InitEntities() )
	{
		return FALSE;
	}

	// 응용 프로그램 초기화를 수행합니다.
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}	

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_VISUALTOOL ) );

	// 기본 메시지 루프입니다.
	while ( GetMessage( &msg, NULL, 0, 0 ) )
	{
		if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return ( int ) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_VISUALTOOL ) );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH ) ( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCE( IDC_VISUALTOOL );
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassEx( &wcex );
}

VOID CALLBACK MyTimerProc(
	HWND hwnd,        // handle to window for timer messages 
	UINT message,     // WM_TIMER message 
	UINT idTimer,     // timer identifier 
	DWORD dwTime )     // current system time 
{			
	const SSL::EntityManager::ENTITY_MAP& entityMap = SSL::EntityManager::GetInstance()->GetEntityMap();
	for ( const auto &it : entityMap )
	{
		SSL::ActionState* as = SSL::GetEntityAction( it.second );

		if ( false == as->IsDead() )
		{			
			auto sp = std::make_shared<SSL::ST_ENTITY_UPDATE>( it.second->ID() );
			SSL::ThreadEventManager::GetInstance()->PushEvent( sp );
		}
	}
	
	DrawCell();
	DrawEntity();
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL );

	if ( !hWnd )
	{
		return FALSE;
	}

	SetTimer( hWnd, TIMER_ENTITY, 1000, ( TIMERPROC ) MyTimerProc );

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return TRUE;
}

BOOL InitEntities()
{
	SSL::LuaManager::GetInstance()->init( "..\\.\\lua_script\\script_list.lua" );

	for ( int i = 0; i < 10; i++ )
	{		
		SSL::Entity* npc = SSL::EntityFactory::GetInstance()->CreateEntity( SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE );		
		SSL::EntityManager::GetInstance()->RegisterEntity( npc );
		SSL::ActionMove* amNpc = SSL::GetEntityAction( npc );
		amNpc->SetCurLocation( rand() % xCellCount , rand() & xCellCount );
	}	
		
	SSL::Entity* player = SSL::EntityFactory::GetInstance()->CreateEntity( SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE );	
	SSL::EntityManager::GetInstance()->RegisterEntity( player );

	SSL::ActionMove* am = SSL::GetEntityAction( player );
	am->SetCurLocation( 7, 10 );

	return TRUE;
}

void DrawBitmap( int x, int y, UINT32 bitMapId )
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	HDC hdc = GetDC( hWnd );
	HBITMAP hBit = LoadBitmap( hInst, MAKEINTRESOURCE( bitMapId ) );

	MemDC = CreateCompatibleDC( hdc );
	OldBitmap = ( HBITMAP ) SelectObject( MemDC, hBit );

	GetObject( hBit, sizeof( BITMAP ), &bit );
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt( hdc, x + spaceBetweenEdgeAndEntity, y + spaceBetweenEdgeAndEntity,
			cellSize, cellSize, MemDC, 0, 0, SRCCOPY );

	SelectObject( MemDC, OldBitmap );
	DeleteDC( MemDC );
}

void DrawCell()
{
	HDC hdc = GetDC( hWnd );

	for ( int x = 0; x < xCellCount; x++ )
	{
		for ( int y = 0; y < yCellCount; y++ )
		{
			Rectangle( hdc, mostLeftX + cellSize*x, mostLeftY + cellSize*y, mostLeftX + cellSize*x + cellSize, mostLeftY + cellSize*y + cellSize );
		}
	}
}

void DrawCell( INT x, INT y )
{
	HDC hdc = GetDC( hWnd );
	Rectangle( hdc, mostLeftX + cellSize*x, mostLeftY + cellSize*y, mostLeftX + cellSize*x + cellSize, mostLeftY + cellSize*y + cellSize );
}

void DrawEntity()
{
	const SSL::EntityManager::ENTITY_MAP& entityMap = SSL::EntityManager::GetInstance()->GetEntityMap();
	if ( entityMap.empty() )
	{
		return;
	}

	for ( const auto& it : entityMap )
	{
		SSL::ActionMove* am = SSL::GetEntityAction( it.second );
		SSL::ST_COORDINATE curLocation = am->GetCurLocation();

		if ( it.second->Type() == SSL::EN_ENTITY_TYPE::ENTITY_NPC_TYPE )
		{
			SSL::ActionAI* aa = SSL::GetEntityAction( it.second );
			SSL::ActionState* as = SSL::GetEntityAction( it.second );
			
			INT bitMapIndex = IDB_MONSTER;
			if ( as->IsDead() )
			{
				bitMapIndex = IDB_MONSTER_DEAD;
			}
			else if ( SSL::EN_STATE_ID::STATE_NPC_ATTACK == aa->GetCurrentStateID() )
			{
				bitMapIndex = IDB_MONSTER_ATTACK;
			}
			else if ( SSL::EN_STATE_ID::STATE_NPC_PATROL == aa->GetCurrentStateID() )
			{
				bitMapIndex = IDB_MONSTER_FREEWALK;
			}

			DrawBitmap( mostLeftX + curLocation.x * cellSize, mostLeftY + curLocation.y * cellSize, bitMapIndex );
		}
		else
		{			
			INT bitUserMapIndex = IDB_PC_UP;
			SSL::EN_ENTITY_DIRECTION playerCurDirection = am->GetDirection();

			switch ( playerCurDirection )
			{
				case SSL::EN_ENTITY_DIRECTION::DIRECTION_DOWN:
					bitUserMapIndex = IDB_PC_DOWN;
					break;
				case SSL::EN_ENTITY_DIRECTION::DIRECTION_UP:
					bitUserMapIndex = IDB_PC_UP;
					break;
				case SSL::EN_ENTITY_DIRECTION::DIRECTION_LEFT:
					bitUserMapIndex = IDB_PC_LEFT;
					break;
				case SSL::EN_ENTITY_DIRECTION::DIRECTION_RIGHT:
					bitUserMapIndex = IDB_PC_RIGHT;
					break;
				default:
					bitUserMapIndex = IDB_PC_UP;
			}

			DrawBitmap( mostLeftX + curLocation.x * cellSize, mostLeftY + curLocation.y * cellSize, bitUserMapIndex );
		}
	}	
}


void Attack( INT keyType )
{
	const SSL::EntityManager::ENTITY_MAP& entityMap = SSL::EntityManager::GetInstance()->GetEntityMap();
	if ( entityMap.empty() )
	{
		return;
	}

	SSL::Entity* player = nullptr;
	for ( const auto& it : entityMap )
	{
		if ( it.second->Type() == SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
		{
			player = static_cast<SSL::Entity*>( it.second );
			break;
		}
	}

	SSL::ActionMove* am = SSL::GetEntityAction( player );

	SSL::ST_COORDINATE playerCurLocation = am->GetCurLocation();
	SSL::EN_ENTITY_DIRECTION playerCurDirection = am->GetDirection();

	SSL::ST_COORDINATE attackPosition = playerCurLocation;

	switch ( playerCurDirection )
	{
		case SSL::EN_ENTITY_DIRECTION::DIRECTION_UP:
			if ( attackPosition.y != 0 )
			{
				attackPosition.y--;
			}
			break;
		case SSL::EN_ENTITY_DIRECTION::DIRECTION_DOWN:
			if ( attackPosition.y < yCellCount )
			{
				attackPosition.y++;
			}
			break;
		case SSL::EN_ENTITY_DIRECTION::DIRECTION_RIGHT:
			if ( attackPosition.x < xCellCount )
			{
				attackPosition.x++;
			}
			break;
		case SSL::EN_ENTITY_DIRECTION::DIRECTION_LEFT:
			if ( attackPosition.x != 0 )
			{
				attackPosition.x--;
			}
			break;
		default:
			break;
	}

	INT32 attackedMonsterId = am->IsEntityAt( attackPosition.x, attackPosition.y );
	if ( 0 != attackedMonsterId )
	{
		auto& it = entityMap.find( attackedMonsterId );

		for ( int i = 0; i < 5; i++ )
		{			
			//it->second->PushEvent( std::bind( &SSL::NPC::AddHP, static_cast< SSL::NPC* >( it->second ), -1 ) );			
			//auto sp = std::make_shared<SSL::EventQueue::EventStruct>( it->second, std::bind( &SSL::NPC::AddHP, static_cast< SSL::NPC* >( it->second ), -1 ) );
			auto sp = std::make_shared<SSL::ST_ADD_HP>( it->second->ID(), -1 );
			SSL::ThreadEventManager::GetInstance()->PushEvent( sp );

		}
		
		/*auto& it = entityMap.find( attackedMonsterId );
		if ( it != entityMap.end() )
		{
			SSL::ST_MESSAGE_INFO messageInfo;
			messageInfo.senderID = player->ID();
			messageInfo.receiverID = it->first;
			messageInfo.delayTime = 0;
			messageInfo.messageType = SSL::EN_MESSAGE_TYPE::MSG_SUBTRACTION_HP;
			messageInfo.extraInfo = reinterpret_cast<void*>( -1 );

			SSL::MessageManager::GetInstance()->Dispatch( messageInfo );
		}*/
	}	
}

void Move( INT keyType )
{
	const SSL::EntityManager::ENTITY_MAP& entityMap = SSL::EntityManager::GetInstance()->GetEntityMap();
	if ( entityMap.empty() )
	{
		return;
	}

	SSL::Entity* player = nullptr;
	for ( const auto& it : entityMap )
	{
		if ( it.second->Type() == SSL::EN_ENTITY_TYPE::ENTITY_PLAYER_TYPE )
		{
			player = static_cast<SSL::Entity*>( it.second );
			break;
		}
	}

	SSL::ActionMove* am = SSL::GetEntityAction( player );

	SSL::ST_COORDINATE playerCurLocation = am->GetCurLocation();
	DrawCell( playerCurLocation.x, playerCurLocation.y );
	SSL::EN_ENTITY_DIRECTION playerCurDirection = am->GetDirection();

	if ( keyType != playerCurDirection )
	{
		am->SetDirection( static_cast<SSL::EN_ENTITY_DIRECTION>( keyType ) );
		DrawEntity();
		return;
	}

	switch ( keyType )
	{
		case VK_LEFT:
			if ( playerCurLocation.x != 0 )
			{
				playerCurLocation.x--;
			}
			break;
		case VK_RIGHT:
			if ( playerCurLocation.x < xCellCount )
			{
				playerCurLocation.x++;
			}
			break;
		case VK_UP:
			if ( playerCurLocation.y != 0 )
			{
				playerCurLocation.y--;
			}
			break;
		case VK_DOWN:
			if ( playerCurLocation.y < yCellCount )
			{
				playerCurLocation.y++;
			}
			break;
		default:
			break;
	}	
	INT32 attackedMonsterId = am->IsEntityAt( playerCurLocation.x, playerCurLocation.y );
	if ( 0 != attackedMonsterId )
	{
		return;
	}

	am->SetCurLocation( playerCurLocation.x, playerCurLocation.y );

	DrawEntity();
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( message )
	{
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_LEFT:
				case VK_RIGHT:
				case VK_UP:
				case VK_DOWN:
					Move( wParam );
					break;
				case VK_SPACE:
					Attack( wParam );
					break;
				default:
					break;
			}
		case WM_COMMAND:
			wmId = LOWORD( wParam );
			wmEvent = HIWORD( wParam );
			// 메뉴 선택을 구문 분석합니다.
			switch ( wmId )
			{
				case IDM_ABOUT:
					DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
					break;
				case IDM_EXIT:
					DestroyWindow( hWnd );
					break;
				default:
					return DefWindowProc( hWnd, message, wParam, lParam );
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			// TODO: 여기에 그리기 코드를 추가합니다.
			DrawCell();
			DrawEntity();
			EndPaint( hWnd, &ps );
			break;		
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
		case WM_INITDIALOG:
			return ( INT_PTR ) TRUE;

		case WM_COMMAND:
			if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
			{
				EndDialog( hDlg, LOWORD( wParam ) );
				return ( INT_PTR ) TRUE;
			}
			break;
	}
	return ( INT_PTR ) FALSE;
}
