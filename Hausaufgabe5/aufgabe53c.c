void AnkunftWagen(){
	wait(wagen); //Mutex
	fahreAufPlattform();
	oeffneTuer();
	signal(sitze); //Zaehlsemaphore initialisiert mit 0
	signal(sitze);
	wait(besucher); //Zaehlsemaphore initialisiert mit 0
	wait(besucher);
	schliesseTueren();
	verlassePlattform();
	signal(wagen);

}

void AnkunftBesucher(){
	wait(sitze);
	signal(besucher);
	betreteWagen();
}