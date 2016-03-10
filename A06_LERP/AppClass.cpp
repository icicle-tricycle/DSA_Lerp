#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	
	currentLocation = 0;
	//locations;
	location_count = 10;//with 0 counting.
	locations.push_back(vector3(-4.0f, -2.0f, 5.0f));
	locations.push_back(vector3(1.0f, -2.0f, 5.0f));
	locations.push_back(vector3(-3.0f, -1.0f, 3.0f));
	locations.push_back(vector3(2.0f, -1.0f, 3.0f));
	locations.push_back(vector3(-2.0f, 0.0f, 0.0f));
	locations.push_back(vector3(3.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 1.0f, -3.0f));
	locations.push_back(vector3(4.0f, 1.0f, -3.0f));
	locations.push_back(vector3(0.0f, 2.0f, -5.0f));
	locations.push_back(vector3(5.0f, 2.0f, -5.0f));
	locations.push_back(vector3(1.0f, 3.0f, -5.0f));

	sphereMatrixes = new matrix4[location_count];
	spheres = new PrimitiveClass[location_count];

	dudePos = locations.at(0);

	for (uint i = 0; i < location_count; i++)
	{
		//spheres.push_back(GenerateSphere(0.5f, 0.0f, RERED));
		spheres[i].GenerateSphere(0.1f, 2.0f, RERED);
		sphereMatrixes[i] = glm::translate(locations.at(i));
	}

	start = locations.at(0);
	end = locations.at(1);

	fDuration = glm::distance(start, end)/5;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
	timeJourney += fTimeSpan;
#pragma endregion

#pragma region Your Code goes here
	//std::cout << "location: " << currentLocation << std::endl;
	float journeyDist = glm::distance(start, end);

	lerpPercent += fTimeSpan / fDuration;

	if (lerpPercent > 1)
	{
		lerpPercent = 0;
		currentLocation += 1;
		if (currentLocation < (location_count - 1))
		{
			start = locations.at(currentLocation);
			end = locations.at(currentLocation + 1);
		}
		else if (currentLocation == location_count-1)
		{
			start = locations.at(currentLocation);
			end = locations.at(0);
		}
		else
		{
			currentLocation = 0;
			start = locations.at(currentLocation);
			end = locations.at(currentLocation + 1);
		}
	}
	vector3 dir = Lerp(start, end, lerpPercent);

	dudePos = dir;
	//std::cout << "Dudex: " << dudePos.x << std::endl << "Dudey: " << dudePos.y << std::endl << "Dudez: " << dudePos.z << std::endl;
	matrix4 m4_movement = glm::translate(dudePos);

	m_pMeshMngr->SetModelMatrix(m4_movement, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
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

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	for (uint i = 0; i < location_count; i++)
	{
		spheres[i].Render(m4Projection, m4View, sphereMatrixes[i]);
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	if (spheres != nullptr)
	{
		delete[] spheres;
		spheres = nullptr;
	}
	if (sphereMatrixes != nullptr)
	{
		delete[] sphereMatrixes;
		sphereMatrixes = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}

vector3 AppClass::Lerp(vector3 start, vector3 end, float percent)
{
	return(start + percent*(end - start));

}