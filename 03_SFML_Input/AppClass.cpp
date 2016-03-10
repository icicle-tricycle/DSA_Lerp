#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("SFML CONTROLS (Esc to terminate)");
}

void AppClass::InitVariables(void)
{
	//Load a model into memory
	m_pMeshMngr->LoadModel("Minecraft\\Steve.bto", "Steve");
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();
	//Update the information on the Mesh manager I will not check for collision detection so the argument is false
	m_pMeshMngr->Update();

	ArcBall();
	
	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Modify model's ModelToWorld matrix
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), "Steve");

	//Add a model to the render list
	m_pMeshMngr->AddInstanceToRenderList("Steve");

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
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

	m_pMeshMngr->Render(); //Renders everything set up in the render queue

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}