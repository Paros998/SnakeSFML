#include "Gracz.h"

enum opis_gracz {GLOWA, CIALO};

void Gracz::przejdzNaKoniecListy(Lista** wsk)
{
	while ((*wsk)->nast != NULL)
		*wsk = (*wsk)->nast;
}

void Gracz::dodajElement()
{
	if (wsk_listy != NULL)
	{
		Lista* nowy_element = new Lista;
		if (nowy_element != NULL)
		{
			Lista* wsk = wsk_listy;
			przejdzNaKoniecListy(&wsk);
			nowy_element->sprite = graczSprite[CIALO];
			nowy_element->sprite.setRotation(wsk->sprite.getRotation());
			nowy_element->y = wsk->y;
			nowy_element->x = wsk->x;
			wsk->nast = nowy_element;
			nowy_element->poprz = wsk;
		}
	}
}

Gracz::Gracz()
{

	graczGlowaTekstura.loadFromFile("data/Sprity do gry/Gracz i przedmioty/snake_glowa12.png");
	graczCialoTekstura.loadFromFile("data/Sprity do gry/Gracz i przedmioty/snake_cialo12.png");

	graczSprite = new Sprite[2];
	graczSprite[0].setTexture(graczGlowaTekstura);
	Rect<float> rozmiar = graczSprite[0].getGlobalBounds();
	graczSprite[0].setOrigin(Vector2f(rozmiar.width / 2.0f, rozmiar.height / 2.0f));

	graczSprite[1].setTexture(graczCialoTekstura);
	Rect<float> _rozmiar = graczSprite[1].getGlobalBounds();
	graczSprite[1].setOrigin(Vector2f(rozmiar.width / 2.0f, rozmiar.height / 2.0f));
	
	graczSprite[GLOWA].setScale(0.85f, 1.0f);
	graczSprite[CIALO].setScale(0.85f, 1.0f);

	czas = czasomierz = 0.0f;
	kierunek = 0;
	opoznienie = 0.1f;
	szybkosc = (float)graczGlowaTekstura.getSize().x;
	cout << szybkosc;

	// Tworzenie listy
	wsk_listy = new Lista;
	if (wsk_listy != NULL)
	{
		wsk_listy->sprite = graczSprite[GLOWA];
		wsk_listy->x = START_X;
		wsk_listy->y = START_Y;
		wsk_listy->nast = wsk_listy->poprz = NULL;
	}

	// Dodanie element�w listy
	for (int i = 0; i < 10; i++)
		dodajElement();
}


Gracz::~Gracz()
{
	while (wsk_listy != NULL)
	{
		Lista* nast = wsk_listy->nast;
		delete wsk_listy;
		wsk_listy = nast;
	}
	delete[] graczSprite;
}

void Gracz::przejdzPrzezSciane()
{
	if (wsk_listy->x > KONIEC_X) wsk_listy->x = START_X;
	if (wsk_listy->y > KONIEC_Y) wsk_listy->y = START_Y;
	if (wsk_listy->x < START_X) wsk_listy->x = KONIEC_X;
	if (wsk_listy->y < START_Y) wsk_listy->y = KONIEC_Y;
}

bool Gracz::walnijPrzeszkode(Sprite * przeszkodaSprite,int liczbaPrzeszkod)
{
	for (int i = 0; i < liczbaPrzeszkod; i++)
	{
		if (kolizja(wsk_listy->sprite, przeszkodaSprite[i]))
		{
			return true;
		}
	}
	return false;
}

void Gracz::przejdzPrzezDziure(Sprite* dziuraSprite, int liczbaDziur)
{
	for (int i = 0; i < liczbaDziur; i++)
	{
		if (kolizja(wsk_listy->sprite, dziuraSprite[i]))
		{
			if (i == 0)
			{	
				if (kierunek == 0)
				{
					wsk_listy->x = dziuraSprite[1].getPosition().x;
					wsk_listy->y = dziuraSprite[1].getPosition().y + 64.0f;
				}
				if (kierunek == 1)
				{
					wsk_listy->x = dziuraSprite[1].getPosition().x - 64.0f;
					wsk_listy->y = dziuraSprite[1].getPosition().y;
				}
				if (kierunek == 2)
				{
					wsk_listy->x = dziuraSprite[1].getPosition().x + 64.0f;
					wsk_listy->y = dziuraSprite[1].getPosition().y;
				}
				if (kierunek == 3)
				{
					wsk_listy->x = dziuraSprite[1].getPosition().x;
					wsk_listy->y = dziuraSprite[1].getPosition().y - 64.0f;
				}
			}
			if (i == 1)
			{
				if (kierunek == 0)
				{
					wsk_listy->x = dziuraSprite[0].getPosition().x;
					wsk_listy->y = dziuraSprite[0].getPosition().y + 64.0f;
				}
				if (kierunek == 1)
				{
					wsk_listy->x = dziuraSprite[0].getPosition().x - 64.0f;
					wsk_listy->y = dziuraSprite[0].getPosition().y;
				}
				if (kierunek == 2)
				{
					wsk_listy->x = dziuraSprite[0].getPosition().x + 64.0f;
					wsk_listy->y = dziuraSprite[0].getPosition().y;
				}
				if (kierunek == 3)
				{
					wsk_listy->x = dziuraSprite[0].getPosition().x;
					wsk_listy->y = dziuraSprite[0].getPosition().y - 64.0f;
				}
			}
		}
	}
}

void Gracz::ruchGracza(Sprite* dziuraSprite, int liczbaDziur, Sprite* przeszkodaSprite, int liczbaPrzeszkod)
{
	Lista* koniec = wsk_listy;
	przejdzNaKoniecListy(&koniec);
	while (koniec != NULL)
	{
		if (koniec->poprz != NULL)
		{
			koniec->x = koniec->poprz->x;
			koniec->y = koniec->poprz->y;

			if (koniec->sprite.getRotation() != koniec->poprz->sprite.getRotation())
				koniec->sprite.setRotation(koniec->poprz->sprite.getRotation());
		}
		koniec = koniec->poprz;
	}

	switch (kierunek)
	{
	case 0:
		wsk_listy->y += szybkosc;
		break;

	case 1:
		wsk_listy->x -= szybkosc;
		break;

	case 2:
		wsk_listy->x += szybkosc;
		break;

	case 3:
		wsk_listy->y -= szybkosc;
		break;
	}
	
	przejdzPrzezSciane();
	przejdzPrzezDziure(dziuraSprite, liczbaDziur);
}

void Gracz::sterowanie()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{	
		if (kierunek == 2)return;
		kierunek = 1;
		wsk_listy->sprite.setRotation(90.0f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Right))
	{	
		if (kierunek == 1)return;
		kierunek = 2;
		wsk_listy->sprite.setRotation(-90.0f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{	
		if (kierunek == 0)return;
		kierunek = 3;
		wsk_listy->sprite.setRotation(180.0f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
	{	
		if (kierunek == 3)return;
		kierunek = 0;
		wsk_listy->sprite.setRotation(0.0f);
	}
}

void Gracz::obsluguj(Sprite* dziuraSprite, int liczbaDziur, Sprite* przeszkodaSprite, int liczbaPrzeszkod)
{	
	czas = zegar.getElapsedTime().asSeconds();
	zegar.restart();
	czasomierz += czas;
	if (czasomierz >= opoznienie)
	{
		czasomierz -= opoznienie;
		ruchGracza(dziuraSprite,liczbaDziur,przeszkodaSprite,liczbaPrzeszkod);
	}
	sterowanie();
}

void Gracz::rysuj(RenderWindow& okno)
{
	Lista* wsk = wsk_listy;
	while (wsk != NULL)
	{
		wsk->sprite.setPosition((float)wsk->x, (float)wsk->y);
		okno.draw(wsk->sprite);
		wsk = wsk->nast;
	}
}
