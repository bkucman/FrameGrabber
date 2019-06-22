# FrameGrabber

Napisać uproszczony ‘frame grabber’. Program, ten będzie przetwarzał plik filmowy a także obraz
rejestrowany z kamery na określoną liczbę klatek w postaci plików graficznych oraz pozwalał na łączenie
plików graficznych w film. Należy umożliwić zdefiniowanie liczby klatek dla zadanego przedziału
czasowego. Przedział czasowy (początek, koniec) jest ustalany przez użytkownika. Dodatkowo, obraz z
kamery będzie przetwarzany do pliku wideo po naciśnięciu określonego klawisza (np. 's' start) do chwili
naciśnięcia innego klawisza (np. 'e' koniec), zaś plik wideo będzie przetwarzany do plików graficznych po
naciśnięciu odpowiednio klawiszy ‘s’ (start) oraz ‘e’ (koniec).

a) Umożliwić zapisanie do pliku wideo obrazu z kamery z podaną przez użytkownika liczbą klatek na
   sekundę (FPS - frames per second), np. 5 fps, 10 fps, 40 fps itd. W przypadku załadowanego z pliku
   filmu wideo należy również umożliwić zapisanie go z ustaloną liczbą klatek na sekundę np. 5 fps, 10 fps, 40 fps itd.

b) Zrealizować obsługę następujących opcji umożliwiających odtwarzanie plików wideo, z obsługą określonych klawiszy:
	Opcja: Skrót klawiszowy:
	a) Beginning Ctrl+Left
	b) End Ctrl+Right
	c) Prev frame Left 
	d) Next frame Right
	e) Prev keyframe Shift+Left
	f) Next keyframe Shift+Right
	g) Back 50 frames Alt+Left
	h) Forward 50 frames Page Down
	i) Prev Scene Ctrl+Shift+Left
	j) Next Scene Ctrl+Shift+Right

	Opis:
	a) Początkowa klatka.
	b) Końcowa klatka.
	c) Poprzednia klatka.
	d) Następna klatka.
	e) Przesunięcie wstecz o 15 klatek.
	f) Przesunięcie do przodu o 15 klatek.
	g) Przesunięcie wstecz o 50 klatek.
	h) Przesunięcie do przodu o 50 klatek.
	i) Cofnięcie klatka po klatce z bieżącej pozycji klatki do pierwszej klatki.
	j) Przesunięcie do przodu klatka po klatce z bieżącej pozycji do końcowej klatki.

c) Zrealizować otwarcie dwóch sąsiednich okien z odtwarzanymi plikami wideo, wykonujących opcje
   wymienione w punkcie b). W przypadku opcji i), j):
   	- w lewym oknie będzie odtwarzanie pliku wideo klatka po klatce
 	- w prawym oknie wyświetlona będzie klatka pierwsza dla opcji i) albo końcowa klatka, jeśli wybrano opcję j)

d) Dodać wyskalowany suwak, którego podziałka będzie się automatycznie przesuwała po wykonaniu
   określonych opcji skrótami klawiszowymi, wymienionych w punkcie b). Podziałka ma mieć wyskalowane numery wszystkich klatek.

e) Dodać obsługę myszy, umożliwiającej przemieszczanie suwaka, którego przesunięcie (na określony numer klatki) 
   spowoduje wyświetlenie wskazanej klatki w dwóch sąsiednich oknach.

f) Umożliwić dodanie logo, którego położeniem można sterować za pomocą klawiszy strzałek w oknie z
   załadowanym dowolnym plikiem graficznym. Umożliwić zapisanie obrazka z umieszczonym logo do pliku.
   Oprócz tego zrealizować pozycjonowanie logo (za pomocą strzałek) w :
	1) W oknie z załadowanym plikiem filmowym
	2) W oknie z odtwarzanym obrazem z kamery

g) Umożliwić zapis do pliku wideo: 
   (1) pliku filmowego 
   (2) obrazu z kamery, z umieszczonym i wypozycjonowanym logo, odpowiednio w punktach: 1) oraz 2).
