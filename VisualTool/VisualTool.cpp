// VisualTool.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "VisualTool.h"

#include "..\Core\NPC.h"
#include "..\Core\Player.h"
#include "..\Core\StateNPC.h"
#include "..\Core\StatePlayer.h"
#include "..\Core\LuaManager.h"
#include "..\Core\WorldManager.h"
#include "..\Core\TimerWorker.h"
#include "..\Core\MessageManager.h"
#include "..\Core\EntityManager.h"
#include "..\Core\ThreadPool.h"
#include "..\Core\NPC.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

//==============================================================

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_VISUALTOOL, szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );
		
	// Thread initialize
	SSL::ThreadPool::GetInstance()->Init( 20 );
	
	// User�� NPC ����
	if ( !InitEntities() )
	{
		return FALSE;
	}

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}	

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_VISUALTOOL ) );

	// �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
	// Update �Լ��� �۾� ť�� ���� �ִ´�. �´� ����ΰ�?
	const SSL::EntityManager::ENTITY_MAP& entityMap = SSL::EntityManager::GetInstance()->GetEntityMap();
	for ( const auto& it : entityMap )
	{		
		if ( it.second->GetEntityState() == SSL::EN_ENTITY_STATE::STATE_ALIVE )
		{
			SSL::ThreadPool::GetInstance()->enqueue( std::bind( &SSL::BaseEntity::Update, it.second ) );
		}		
	}
	
	DrawCell();
	DrawEntity();
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
		SSL::NPC* monster = new SSL::NPC( SSL::ID_RANGE_NPC + i, SSL::NPCPatrol::GetInstance() );		
		SSL::EntityManager::GetInstance()->RegisterEntity( monster );
	}	
	
	SSL::Player* player = new SSL::Player( SSL::ID_RANGE_PLAYER, SSL::PlayerPatrol::GetInstance() );
	SSL::EntityManager::GetInstance()->RegisterEntity( player );

	player->SetCurLocation( 7, 10 );

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
		if ( it.second->ID() < SSL::EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
		{
			SSL::ST_COORDINATE monsterCurLocation = it.second->GetCurLocation();
			INT bitMapIndex = IDB_MONSTER;
			if ( SSL::EN_ENTITY_STATE::STATE_DEAD == it.second->GetEntityState() )
			{
				bitMapIndex = IDB_MONSTER_DEAD;
			}
			else if ( SSL::EN_STATE_ID::STATE_NPC_ATTACK == it.second->GetCurrentStateID() )
			{
				bitMapIndex = IDB_MONSTER_ATTACK;
			}
			else if ( SSL::EN_STATE_ID::STATE_NPC_PATROL == it.second->GetCurrentStateID() )
			{
				bitMapIndex = IDB_MONSTER_FREEWALK;
			}

			DrawBitmap( mostLeftX + monsterCurLocation.x * cellSize, mostLeftY + monsterCurLocation.y * cellSize, bitMapIndex );
		}
		else
		{
			SSL::ST_COORDINATE playerCurLocation = it.second->GetCurLocation();
			INT bitUserMapIndex = IDB_PC_UP;
			SSL::EN_ENTITY_DIRECTION playerCurDirection = it.second->GetDirection();

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

			DrawBitmap( mostLeftX + playerCurLocation.x * cellSize, mostLeftY + playerCurLocation.y * cellSize, bitUserMapIndex );
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

	SSL::Player* player = nullptr;
	for ( const auto& it : entityMap )
	{
		if ( it.second->ID() >= SSL::EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
		{
			player = static_cast<SSL::Player*>( it.second );
			break;
		}
	}

	SSL::ST_COORDINATE playerCurLocation = player->GetCurLocation();	
	SSL::EN_ENTITY_DIRECTION playerCurDirection = player->GetDirection();

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

	INT32 attackedMonsterId = SSL::WorldManager::GetInstance()->IsEntityAt( attackPosition.x, attackPosition.y );
	if ( 0 != attackedMonsterId )
	{
		auto& it = entityMap.find( attackedMonsterId );

		for ( int i = 0; i < 5; i++ )
		{			
			it->second->PushEvent( std::bind( &SSL::NPC::AddHP, static_cast< SSL::NPC* >( it->second ), -1 ) );			
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

	SSL::Player* player = nullptr;
	for ( const auto& it : entityMap )
	{
		if ( it.second->ID() >= SSL::EN_ENTITY_ID_RANGE::ID_RANGE_PLAYER )
		{
			player = static_cast<SSL::Player*>( it.second );
			break;
		}
	}

	SSL::ST_COORDINATE playerCurLocation = player->GetCurLocation();
	DrawCell( playerCurLocation.x, playerCurLocation.y );
	SSL::EN_ENTITY_DIRECTION playerCurDirection = player->GetDirection();

	if ( keyType != playerCurDirection )
	{
		player->SetDirection( static_cast<SSL::EN_ENTITY_DIRECTION>( keyType ) );
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
	INT32 attackedMonsterId = SSL::WorldManager::GetInstance()->IsEntityAt( playerCurLocation.x, playerCurLocation.y );
	if ( 0 != attackedMonsterId )
	{
		return;
	}

	player->SetCurLocation( playerCurLocation.x, playerCurLocation.y );

	DrawEntity();
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
			// �޴� ������ ���� �м��մϴ�.
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
			// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
