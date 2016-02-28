#include "Input.h"
//#include "s3eDevice.h"

CDECLARE_SINGLETONS(Input)

void Input::Init()
{
	this->zTouched = false;
	this->zPrev_Touched = false;

	this->zBackKeyPressed = false;
	this->zHomeKeyPressed = false;

	// Set touch event callback handlers, single and multi-touch devices have different callbacks assigned
	if(s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) != 0)
	{
		s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB, 0);
		s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, 0);
	}
	else
	{
		s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)TouchButtonCB, 0);
		s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)TouchMotionCB, 0);
	}

	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback) KeyEventCallback, NULL);
}

void Input::Release()
{
	// unregister touch event callback handlers, single and multi-touch devices have different callbacks assigned
	if(s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) != 0)
	{
		s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB);
		s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB);
	}
	else
	{
		s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)TouchButtonCB);
		s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)TouchMotionCB);
	}

	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback) KeyEventCallback);
}

void Input::Update()
{
	s3ePointerUpdate();

	s3eKeyboardUpdate();

	s3eKey key = s3eKeyBack;
	//// Check the current platform 
	//const char* platform = s3eDeviceGetString(S3E_DEVICE_OS);
	//if(platform == "WINDOWS")
	//	key = s3eKeyP;

	//s3eKeyP s3eKeyBack
	if(s3eKeyboardGetState(key) & S3E_KEY_STATE_PRESSED)
	{
		INPUT_MANAGER->SetBackKeyStatus(true);
	}
	else
	{
		INPUT_MANAGER->SetBackKeyStatus(false);
	}
}

void Input::Reset()
{
	//this->zPrev_Touched = false;
	//this->zTouched = false;

	INPUT_MANAGER->SetBackKeyStatus(false);
}

/**
 * @fn    void Input::TouchButtonCB(s3ePointerEvent* event)
 *
 * @brief The system will call this callback when the user touches the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::TouchButtonCB( s3ePointerEvent* event )
{
	INPUT_MANAGER->zPrev_Touched = INPUT_MANAGER->zTouched;
	INPUT_MANAGER->zTouched = event->m_Pressed != 0;
	INPUT_MANAGER->zX_Pos = event->m_x;
	INPUT_MANAGER->zY_Pos = event->m_y;
}

/**
 * @fn    void Input::TouchMotionCB(s3ePointerMotionEvent* event)
 *
 * @brief The system will call this callback when the user moves their finger on the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::TouchMotionCB( s3ePointerMotionEvent* event )
{
	INPUT_MANAGER->zX_Pos = event->m_x;
	INPUT_MANAGER->zY_Pos = event->m_y;
}

/**
 * @fn    void Input::MultiTouchButtonCB(s3ePointerTouchEvent* event)
 *
 * @brief The system will call this callback when the user touches the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::MultiTouchButtonCB( s3ePointerTouchEvent* event )
{
	INPUT_MANAGER->zPrev_Touched = INPUT_MANAGER->zTouched;
	INPUT_MANAGER->zTouched = event->m_Pressed != 0;
	INPUT_MANAGER->zX_Pos = event->m_x;
	INPUT_MANAGER->zY_Pos = event->m_y;
}

void Input::MultiTouchMotionCB( s3ePointerTouchMotionEvent* event )
{
	INPUT_MANAGER->zX_Pos = event->m_x;
	INPUT_MANAGER->zX_Pos = event->m_y;
}

void Input::KeyEventCallback( s3eKeyboardEvent* event )
{
	int32 homeKeyState = s3eKeyboardGetState(s3eKeyHome);
	int32 escKeyState = s3eKeyboardGetState(s3eKeyEsc);
	if(homeKeyState & S3E_KEY_STATE_PRESSED || escKeyState & S3E_KEY_STATE_PRESSED)
	{
		INPUT_MANAGER->zHomeKeyPressed = true;
	}
	else if(homeKeyState & S3E_KEY_STATE_RELEASED || escKeyState & S3E_KEY_STATE_RELEASED)
	{
		INPUT_MANAGER->zHomeKeyPressed = false;
	}

	if(homeKeyState & S3E_KEY_STATE_DOWN || escKeyState & S3E_KEY_STATE_DOWN)
	{
		INPUT_MANAGER->zHomeKeyPressed = true;
	}
	else if(homeKeyState & S3E_KEY_STATE_UP || escKeyState & S3E_KEY_STATE_UP)
	{
		INPUT_MANAGER->zBackKeyPressed = false;
	}
}