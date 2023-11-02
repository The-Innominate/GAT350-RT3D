#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Use this to test the StringUtils class
	auto stringy = make_unique<nc::StringUtils>();
	string str = "Hello World";
	string str2 = "hello world";
	cout << stringy->ToLower(str) << endl;
	cout << stringy->ToUpper(str) << endl;
	cout << stringy->IsEqualIgnoreCase(str, str2) << endl;
	cout << stringy->CreateUnique(str) << endl;
	cout << stringy->CreateUnique(str2) << endl;


	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World05>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
