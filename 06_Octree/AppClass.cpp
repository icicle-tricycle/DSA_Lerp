#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Octree"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 150.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	m_nInstances = 3500;
	int nSquare = static_cast<int>(std::sqrt(m_nInstances));
	m_nInstances = nSquare * nSquare;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			String sInstance = "Cube_" + std::to_string(i) + "_" + std::to_string(j);
			matrix4 m4Positions = glm::translate(static_cast<float>(i - nSquare / 2.0f), static_cast<float>(j), 0.0f);
			m4Positions = glm::translate(vector3(glm::sphericalRand(35.0f)));
			m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", sInstance, false, m4Positions);
		}
	}

	//for (int n = 0; n < 25; n++)
	//	m_pMeshMngr->LoadModel("Minecraft\\Steve.ato", "Plane", false, glm::translate(vector3(-n, -n, 0)));

	SafeDelete(m_pRoot);
	m_pRoot = new OctantClass(3, 3);
	
	m_v3ClickedOn = ZERO_V3;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	//SafeDelete(m_pRoot);
	//m_pRoot = new OctantClass(3, 2);

	//m_pMeshMngr->SetVisibleBO(REBODISPLAY::BD_NONE, "ALL", true);
	//m_pMeshMngr->m_pModelMngr->CheckCollisions(BD_OB);
	m_pRoot->CheckCollisions();
	m_pRoot->DisplayLeafs();

	//m_pMeshMngr->SetVisibleAxis(true, "ALL", true);

	m_pMeshMngr->SetModelMatrix(m_m4Model * ToMatrix4(m_qArcBall), "Plane");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pRoot);
	super::Release(); //release the memory of the inherited fields
}