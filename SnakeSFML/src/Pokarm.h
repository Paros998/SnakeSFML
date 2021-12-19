#pragma once
#include "Biblioteki.h"

class Gracz;

class Pokarm : public Animacja
{
	float x, y;

public:
	Pokarm(string sciezkaPliku);
	bool sprawdzCzyZjedzony(Gracz& gracz);

private:
	void ustawPokarm(Gracz& gracz);
};

