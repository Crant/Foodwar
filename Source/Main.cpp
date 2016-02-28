/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
#include "s3e.h"
#include "IwDebug.h"
#include "Iw2DSceneGraph.h"
#include "IwResManager.h"

#include "Foodwar.h"

// Main entry point for the application
int main()
{
	//Initialise graphics system(s)
	Iw2DInit();
	IwResManagerInit();
    
	Foodwar* foodwar = new Foodwar();

	foodwar->Init();

	foodwar->Run();

	delete foodwar;

	//Terminate modules being used
	IwResManagerTerminate();
	Iw2DTerminate();

    // Return
    return 0;
}
