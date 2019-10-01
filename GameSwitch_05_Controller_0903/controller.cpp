
#include "main.h"
#include "controller.h"



// パッド変数
NpadJoyDualState g_OldPadState;
NpadJoyDualState g_PadState;


// 振動子変数
VibrationDeviceHandle g_VibrationDevice[2];
int g_VibrationLeftFrame;
int g_VibrationRightFrame;


// 6軸センサ変数
SixAxisSensorHandle g_SixAxisSensor[2];
SixAxisSensorState g_SixAxisSensorState[2] = {};

// タッチスクリーン変数
TouchScreenState<1> g_TouchScreenState;



void InitController()
{

	// パッド初期化

	InitializeNpad();
	NpadIdType npadIds[] = { NpadId::No1,
							NpadId::Handheld };
	SetSupportedNpadStyleSet(NpadStyleJoyDual::Mask | NpadStyleHandheld::Mask);
	SetSupportedNpadIdType(npadIds, 2);



	// 振動子初期化
	if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
		GetVibrationDeviceHandles(g_VibrationDevice, 2, NpadId::No1, NpadStyleJoyDual::Mask);
	else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
		GetVibrationDeviceHandles(g_VibrationDevice, 2, NpadId::Handheld, NpadStyleHandheld::Mask);

	for (int i = 0; i < 2; i++)
	{
		InitializeVibrationDevice(g_VibrationDevice[i]);
	}

	g_VibrationLeftFrame = 0;
	g_VibrationRightFrame = 0;

	

	// 6軸センサ初期化
	if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
		GetSixAxisSensorHandles(g_SixAxisSensor, 2, NpadId::No1, NpadStyleJoyDual::Mask);
	else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
		GetSixAxisSensorHandles(g_SixAxisSensor, 2, NpadId::Handheld, NpadStyleHandheld::Mask);

	for (int i = 0; i < 2; i++)
	{
		StartSixAxisSensor(g_SixAxisSensor[i]);
	}



	// タッチスクリーン初期化
	InitializeTouchScreen();

}



void UninitController()
{

}


void UpdateController()
{

	g_OldPadState = g_PadState;


	// パッド状態取得
	if (GetNpadStyleSet(NpadId::No1).Test<nn::hid::NpadStyleJoyDual>())
		GetNpadState(&g_PadState, NpadId::No1);
	else if (GetNpadStyleSet(NpadId::Handheld).Test<nn::hid::NpadStyleHandheld>())
		GetNpadState((NpadHandheldState*)&g_PadState, NpadId::Handheld);


	// 6軸センサ状態取得
	GetSixAxisSensorState(&g_SixAxisSensorState[0], g_SixAxisSensor[0]);

	if (g_SixAxisSensor[1]._storage)
		GetSixAxisSensorState(&g_SixAxisSensorState[1], g_SixAxisSensor[1]);
	else
		g_SixAxisSensorState[1] = g_SixAxisSensorState[0];


	// タッチスクリーン状態取得
	GetTouchScreenStates(&g_TouchScreenState, 1);



	// 振動子停止
	if (g_VibrationLeftFrame > 0)
	{
		g_VibrationLeftFrame--;

		if (g_VibrationLeftFrame == 0)
		{
			VibrationValue vibration = VibrationValue::Make();

			for (int i = 0; i < 2; i++)
			{
				VibrationDeviceInfo info;
				GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
				if (info.position == VibrationDevicePosition_Left)
				{
					SendVibrationValue(g_VibrationDevice[i], vibration);
				}
			}
		}
	}

	if (g_VibrationRightFrame > 0)
	{
		g_VibrationRightFrame--;

		if (g_VibrationRightFrame == 0)
		{
			VibrationValue vibration = VibrationValue::Make();

			for (int i = 0; i < 2; i++)
			{
				VibrationDeviceInfo info;
				GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
				if (info.position == VibrationDevicePosition_Right)
				{
					SendVibrationValue(g_VibrationDevice[i], vibration);
				}
			}
		}
	}


}




bool GetControllerPress(int button)
{
	return g_PadState.buttons.Test(button);
}

bool GetControllerTrigger(int button)
{
	return (g_PadState.buttons ^ g_OldPadState.buttons & g_PadState.buttons).Test(button);

}


Float2 GetControllerLeftStick()
{
	Float2 stick;
	stick.x = (float)g_PadState.analogStickL.x / AnalogStickMax;
	stick.y = (float)g_PadState.analogStickL.y / AnalogStickMax;
	return stick;
}

Float2 GetControllerRightStick()
{
	Float2 stick;
	stick.x = (float)g_PadState.analogStickR.x / AnalogStickMax;
	stick.y = (float)g_PadState.analogStickR.y / AnalogStickMax;
	return stick;
}




Float3 GetControllerLeftAcceleration()
{
	return g_SixAxisSensorState[0].acceleration;
}

Float3 GetControllerRightAcceleration()
{
	return g_SixAxisSensorState[1].acceleration;
}

Float3 GetControllerLeftAngle()
{
	return g_SixAxisSensorState[0].angle;
}

Float3 GetControllerRightAngle()
{
	return g_SixAxisSensorState[1].angle;
}



void SetControllerLeftVibration(int frame)
{

	for (int i = 0; i < 2; i++)
	{
		VibrationDeviceInfo info;
		GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
		if (info.position == VibrationDevicePosition_Left)
		{
			VibrationValue vibration = VibrationValue::Make(0.0f, 10.0f, 0.5f, 320.0f);
			SendVibrationValue(g_VibrationDevice[i], vibration);
		}
	}

	g_VibrationLeftFrame = frame;

}


void SetControllerRightVibration(int frame)
{

	for (int i = 0; i < 2; i++)
	{
		VibrationDeviceInfo info;
		GetVibrationDeviceInfo(&info, g_VibrationDevice[i]);
		if (info.position == VibrationDevicePosition_Right)
		{
			VibrationValue vibration = VibrationValue::Make(0.0f, 10.0f, 0.5f, 320.0f);
			SendVibrationValue(g_VibrationDevice[i], vibration);
		}
	}

	g_VibrationRightFrame = frame;

}



bool GetControllerTouchScreen()
{
	if (g_TouchScreenState.count > 0)
		return true;
	else
		return false;
}

Float2 GetControllerTouchScreenPosition()
{
	Float2 position;
	position.x = g_TouchScreenState.touches[0].x * SCREEN_WIDTH / 1280.0f - SCREEN_WIDTH / 2.0f;
	position.y = g_TouchScreenState.touches[0].y * SCREEN_HEIGHT / 720.0f - SCREEN_HEIGHT / 2.0f;

	return position;
}