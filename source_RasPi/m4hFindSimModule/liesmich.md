# RasPi: Suchen des Anschlussnamens eines SIM-Moduls
Programm: m4hFindSimModule.cpp, Version 2021-08-27   
[--> English Version](./README.md "English Version")   

Dieses einfache Programm versucht, der Reihe nach verschiedene serielle Schnittstellen zu &ouml;ffnen, an diese das Kommando `ATE0` zu senden und auf eine Antwort `OK` zu warten.   
Das Ergebnis der Sendevorg&auml;nge wird auf der Konsole angezeigt.   
Die Schnittstellen, die ausprobiert werden, stehen im Hauptprogramm in der Zeile   
`std::string devices[]={"/dev/ttyS0", "/dev/ttyACM0", "/dev/ttyUSB0", "/dev/ttyUSB1"};`   

Eine m&ouml;gliche Programmausgabe ist:   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM-Modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
In diesem Beispiel ist der Device-Name also `/dev/ttyUSB0`.   

_Anmerkung_: Das Erzeugen des Programmes `m4hFindSimModule` erfolgt mit   
`g++ m4hFindSimModule.cpp -o m4hFindSimModule`.   