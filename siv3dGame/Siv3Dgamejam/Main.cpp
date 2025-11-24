# include <Siv3D.hpp>
# include "Common.h"
# include "Title.h"
# include "Game.h"
# include "Stage1.h"
# include "Stage2.h"
# include "StageFinal.h"
# include "Clear.h"


void Main()
{
	Window::Resize(1280, 720);
	
	
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	FontAsset::Register(U"CreditBig", 32, Typeface::Bold);
	FontAsset::Register(U"CreditSmall", 28, Typeface::Regular);

	App manager;
	
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<Stage1>(State::Stage1);
	manager.add<Stage2>(State::Stage2);
	manager.add<StageFinal>(State::StageFinal);
	manager.add<Clear>(State::Clear);
	//manager.add<Ranking>(State::Ranking);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
