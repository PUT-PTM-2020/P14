# Weather Station

## Instrukcja użytkowania
#### Instrukcja obsługi urządzenia
1. Interfejs graficzny stacji pogodowej obsługuje się czterema przyciskami, które odpowiadają strzałkom (__góra__, __dół__, __lewo__, __prawo__).
1. W celu zrestartowania urządzenia można użyć znajdującego się z tyłu przycisku __reset__ lub odłączyć i podłączyć ponownie zasilanie.
1. Po uruchomieniu stacji znajdujemy się na ekranie głównym, z którego możemy przejść do ekranu opcji za pomocą przycisku __góra__, lub odświeżyć status połączenia z siecią za pomocą dowolnego innego.
1. Na ekranie opcji można się poruszać między wyborami używając __góra__ lub __dół__, wrócić do ekranu głównego za pomocą __lewo__, lub wybrać opcję używając __prawo__.
1. Po zatwierdzeniu każdej z opcji urządzenie wróci do ekranu głównego lub przeniesie nas na ekran dodatkowy w celu wprowadzenia wymaganych danych. Dzieje się tak w przypadku __Connect to WiFi__ oraz __Set time and date__. Opis obsługi tych ustawień widnieje w podpunkcie danego ustawienia poniżej.
1. Opcje dostępne dla użytkownika to:    
   
   1. __Force update__ 		- wymuś aktualizację pogody.
   1. __Connect to WiFi__ 	- wyszukaj dostępne sieci bezprzewodowe.
   
      * Za pomocą __góra__ oraz __dół__ można wybrać sieć do której chcemy się połączyć, jeśli jakakolwiek znajduje się w zasięgu urządzenia.
	  * Za pomocą __lewo__ wracamy do ekranu opcji, a __prawo__ przenosi nas do ekranu wprowadzania hasła, jeśli takowe jest wymagane.
	  
	     Na ekranie wprowadzania hasła wybieramy kolejny znak za pomocą __góra__, przesuwamy się po wprowadzonych znakach za pomocą __lewo__ oraz __prawo__, a wprowadzone dane akceptujemy za pomocą __dół__.
		 
   1. __WiFi Disconnect__   - odłącz się od aktualnej sieci bezprzewodowej.
   1. __Set time and date__ - ręcznie ustaw datę i czas urządzenia.
   
      Za pomocą __góra__ zmieniamy wartość pola, które wskazuje kursor, przesuwamy się po polach za pomocą __lewo__ oraz __prawo__, a dane akceptujemy z użyciem __dół__.
	  
	  Aby wyjść bez dokonywania zmian wprowadzamy błędne dane, lub zostawiamy pola w domyślnym ustawieniu.
	   
   1. __Clear SD data__ 	- wyczyść wszystkie logi pogody dostępne na urządzeniu.
   1. __Toggle RGB usage__ 	- przełącz używanie przez urządzenie diody led.
   
   1. __Display timeout__ - ustaw czas do wygaszenia ekranu.
   
   	     Na ekranie zmieniamy czas za pomocą __lewo__ oraz __prawo__, a wprowadzone dane akceptujemy za pomocą __dół__.
	     
	     
   1. __Update interval__ - ustaw czas automatycznego odświeżania pogody.
   
   	     Na ekranie zmieniamy czas za pomocą __lewo__ oraz __prawo__, a wprowadzone dane akceptujemy za pomocą __dół__.

#### Instrukcja obsługi strony internetowej
1. Aby poprawnie uzyskać dostęp do danych udostępnianych przez stację pogodową niezbędne jest posiadanie sprawnego połączenia internetowego. Bez niego strona internetowa będzie wyświetlana jako czysty html, bez dodatkowej szaty wizualnej.
1. Użytkownik po podłączeniu do sieci bezprzewodowej wchodząc pod adres IP nadany stacji w podsieci routera znajdzie się na stronie głównej.
1. Główna strona zawiera umieszone pod napisem `Weather Station` odnośniki do następujących podstron: `data`, `about`. Obecne poniżej pole `Current Weather` po pobraniu danych z urządzenia wyświetli aktualną pogodę oraz zsynchronizuje czas urządzenia z czasem systemu stacji.
1. Wciśnięcie przez użytkownika odnośnika `data` skutkować będzie przeniesieniem na podstronę, gdzie została udostępniona możliwość podglądu starszych odczytów pogody znajdujących się w pamięci urządzenia. Po pobraniu ilości dostępnych danych użytkownik zostanie poproszony o wpisanie zakresu danych do pobrania.
	* Udostępniona została także możliwość generowania wykresów na podstawie pobranych danych, wybór reprezentacji jest możliwy na ekranie głównym podstrony `data`. 
	* Po wygenerowaniu wykresu możliwe jest jego pobranie w formacie `png` za pomocą przycisku pod wykresem.
1. Wciśnięcie przez  użytkownika odnośnika `about` powoduję przejście na podstronę poświęconą autorom projektu oraz informacjom ogólnym na temat stacji pogodowej.
## O projekcie
### Celem projektu było własnoręczne utworzenie stacji pogodowej przy zastosowaniu następujących elementów:
* Moduł WiFi (mikrokontroler ESP8266)
* Moduł karty SD
* Czujnik temperatury i wilgotności powietrza DHT11
* Dioda RGB
* Wyświetlacz LCD 4x20 ze sterownikiem HD44780
* Przyciski Tact-Switch
* Powerbank
* Mikrokontroler STM32F407VG Discovery

### Technologia zastosowana w projekcie:
* Język C - oprogramowanie płytki STM
* Język JavaScript - skryptowa obsługa strony internetowej
* HTML 5 - struktura strony internetowej
* CSS 3 - wygląd strony internetowej
* RESTlike API - struktura działania serwera HTTP
* Fetch API - pobieranie informacji z serwera HTTP
* Bootstrap 4 - responsywność strony internetowej

### Biblioteki języka C użyte w projekcie:
* FatFs by elm-chan

### Biblioteki języka JavaScript użyte w projekcie:
* ChartJS

### Prezentacja [elementów](https://github.com/PUT-PTM-2020/P14/tree/master/stm/stmphotos) hardware projektu:

<p align="center">
  <img src="/stm/stmphotos/hardware.jpg" width="500" title="Aktualna pogoda">
</p>

### [Strona](https://github.com/PUT-PTM-2020/P14/tree/master/website/wwwphotos) internetowa została stworzona w celu odczytywania danych na temat pogody z serwera HTTP mikrokontrolera:

<p align="center">
  <img src="/website/wwwphotos/currentweather.png" width="600" title="Aktualna pogoda">
</p>
<p align="center">
  <img src="/website/wwwphotos/data.png" width="600" title="Dane">
</p>
<p align="center">
  <img src="/website/wwwphotos/phonecurrent.jpg" width="200" title="Wersja mobilna1">
  <img src="/website/wwwphotos/phonedata.jpg" width="200" title="Wersja mobilna2">
  <img src="/website/wwwphotos/phoneabout.jpg" width="200" title="Wersja mobilna3">
</p>

### Obudowa stacji pogodowej została wykonana przy użyciu programu Autodesk Fusion 360.  Fragmenty [obudowy](https://github.com/PUT-PTM-2020/P14/tree/master/model3d/Photo3D): 

<p align="center">
  <img src="/model3d/Photo3D/Obudowa.PNG" width="400" title="Obudowa">
</p>
<p align="center">
  <img src="/model3d/Photo3D/Czujniki.PNG" width="400" title="Czujniki">
</p>

## Autorzy
* Hubert Knioła
* Bartosz Kosmala
> 2020 PUT
