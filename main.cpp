// see tuto there : https://mikke89.github.io/RmlUiDoc/pages/cpp_manual/main_loop.html


#define RMLUI_STATIC_LIB // required for RmlUi library statically built


#include <RmlUi/Core.h>

//#include <RmlUi/Core/Debug.h>

//#include <RmlUi/Debugger.h>
#include <RmlUi_Backend.h>



//#include "MyRenderInterface.h"
//#include "MySystemInterface.h"



//#include<SDL.h>

#include<string>
#include<iostream>
using namespace std;

int main(int argc, char** argv) 
{
    cout << "Starting RmlUi tests" << endl;
	
	// New : now mandatory : path to a .RML document to open :
	std::cout << "nb args :" << argc << std::endl;
  
    if (argc != 2)
     {	   
	   std::cout << "Please provide one argument, being path and filename of a RML doc to open, for instance :" << std::endl;
	   // first arg is binary name itself
	   std::cout << argv[0] << " assets/demo.rml" << std::endl;
	   return -2;
     } 
   
    std::string rmlFile;
	rmlFile.assign(argv[1]);
	std::cout << "RML file to process : "<< rmlFile.c_str() << std::endl;	  
	 

///*

// inspired from :  https://github.com/mikke89/RmlUi/blob/master/Samples/basic/loaddocument/src/main.cpp

	const int window_width = 640;
	const int window_height = 480; 

	// Constructs the system and render interfaces, creates a window, and attaches the renderer.
	if (!Backend::Initialize("Load Document Sample", window_width, window_height, true))
	{
		cerr << "Fail to initialise Backend !" << endl;
		//Shell::Shutdown();		
		return -2;
	}

	// Install the custom interfaces constructed by the backend before initializing RmlUi.
	Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());

	// RmlUi initialisation.
	Rml::Initialise();

	// Create the main RmlUi context.
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
	if (!context)
	{
		cerr << "Fail to Create Context!" << endl;

		Rml::Shutdown();
		Backend::Shutdown();
		//Shell::Shutdown();
		return -2;
	}

	// The RmlUi debugger is optional but very useful. Try it by pressing 'F8' after starting this sample.	
	// Rml::Debugger::Initialise(context); // VA : lot of linking errors !	 	
	/*
	bool res = Rml::Debugger::Initialise(context);
	std::string resStr = res ? "yes" : "No";
    cout << "Initialized Debugger ? : " << resStr << endl;
	*/

	// Fonts should be loaded before any documents are loaded.
	//Shell::LoadFonts();  // VA : we do not use the proposed Shell, so init the font directly :
	// note : must call binary from location parenting assets folder
	if(!Rml::LoadFontFace("FONTS:_ttf/Arial.ttf")) // one of the default MorphOS fonts (Arial, Verdana, ...)
	//if(!Rml::LoadFontFace("FONTS:_ttf/Monospace Typewriter.ttf")) // OK if police installed using FTManager
	//if(!Rml::LoadFontFace("fonts/monospace-typewriter/Monospace Typewriter.ttf")  //Ubuntu ok  
	//   || !Rml::LoadFontFace("fonts/one-slot/OneSlot.ttf")
	//) 
	{
		cerr << "Fail to Load Font Face!" << endl;
		Rml::Shutdown();
		Backend::Shutdown();
		//Shell::Shutdown();
		return -2;
	}

	// Load and show the demo document.
	// must call binary from location parenting assets folder
	//if (Rml::ElementDocument* document = context->LoadDocument("assets/demo.rml"))	
	//if (Rml::ElementDocument* document = context->LoadDocument("assets/my_document.rml")) 
	//if (Rml::ElementDocument* document = context->LoadDocument("assets/my_document_Ubuntu.rml")) // must call binary from location parenting assets folder
	//if (Rml::ElementDocument* document = context->LoadDocument("Work:VA_devs/RmlUi_tests/assets/my_document.rml"))  // MorphOS
	//if (Rml::ElementDocument* document = context->LoadDocument("Work:VA_devs/RmlUi_tests/assets/my_document_3.rml"))  // MorphOS
	if (Rml::ElementDocument* document = context->LoadDocument(rmlFile.c_str()) )  // MorphOS
	  { document->Show();}
	else 
 	{
		cerr << "Fail to Load Document :" << rmlFile.c_str() <<  " !" << endl;
		Rml::Shutdown();
		Backend::Shutdown();
		//Shell::Shutdown();
		return -2;
	}

	bool running = true;
	while (running)
	{
		// Handle input and window events.
		//running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts);
		running = Backend::ProcessEvents(context); // VA, as we do not use Shell

		// This is a good place to update your game or application.

		// Always update the context before rendering.
		context->Update();

		// Prepare the backend for taking rendering commands from RmlUi and then render the context.
		Backend::BeginFrame();
		context->Render();
		Backend::PresentFrame();
	}

	// Shutdown RmlUi.
	Rml::Shutdown();

	Backend::Shutdown();
	//Shell::Shutdown();
    
//*/

  
    cout << "End of RmlUi tests" << endl;
    //

    return 0;
}

//
// sample (high level view of what  a RmlUi program is like :
//
/*
{
	    // Initialize the window and graphics API being used, along with your game or application.
	
	// ... //

	// Instantiate the interfaces to RmlUi.
	MyRenderInterface render_interface;
	MySystemInterface system_interface;

	// Begin by installing the custom interfaces.
	Rml::SetRenderInterface(&render_interface);
	Rml::SetSystemInterface(&system_interface);

	// Now we can initialize RmlUi.
	Rml::Initialise();
	
	// Create a context next.
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
	if (!context)
	{
		Rml::Shutdown();
		return -1;
	}

	// If you want to use the debugger, initialize it now.
	Rml::Debugger::Initialise(context);

	// Fonts should be loaded before any documents are loaded.
	//Rml::LoadFontFace("my_font_file.otf");
    Rml::LoadFontFace("fonts/monospace-typewriter/MonospaceTypewriter.ttf");

	// Now we are ready to load our document.
	//Rml::ElementDocument* document = context->LoadDocument("my_document.rml");
	Rml::ElementDocument* document = context->LoadDocument("data/my_document.rml");
	if (!document)
	{
		Rml::Shutdown();
		return -1;
	}

	document->Show();

	bool exit_application = false;
	while (!exit_application)
	{
		// We assume here that we have some way of updating and retrieving inputs internally.
		my_input->Update();

		if (my_input->KeyPressed(KEY_ESC))
			exit_application = true;

		// Submit input events before the call to Context::Update().
		if (my_input->MouseMoved())
			context->ProcessMouseMove(mouse_pos.x, mouse_pos.y, 0);

		// Toggle the debugger with a key binding.
		if (my_input->KeyPressed(KEY_F8))
			Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());

		// This is a good place to update your game or application.
		my_application->Update();

		// Update any elements to reflect changed data.
		if (Rml::Element* el = document->GetElementById("score"))
			el->SetInnerRML("Current score: " + my_application->GetScoreAsString());

		// Update the context to reflect any changes resulting from input events, animations, modified and
		// added elements, or changed data in data bindings.
		context->Update();

		// After the context update, the properties and layout of all elements are properly resolved.
		// At this point, we should no longer change any elements, or submit input or other events until
		// after the call to Context::Render().

		// Render your game or application.
		my_application->Render();

		// Set up any rendering states necessary before the render.
		my_renderer->PrepareRenderBuffer();

		// Render the user interface on top of the application.
		context->Render();

		// Present the rendered frame.
		my_renderer->PresentRenderBuffer();
	}

	// Shutting down RmlUi releases all its resources, including elements, documents, and contexts.
	Rml::Shutdown();

	// It is now safe to destroy the custom interfaces previously passed to RmlUi.
}
*/

// testing SDL only code from : https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/ :
/*
{
SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow(
    "SDL2Test",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    640,
    480,
    0
  );

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();
}
*/