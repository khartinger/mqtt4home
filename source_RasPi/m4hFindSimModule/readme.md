Last modified: 2022-02-11 <a name="up"></a>   
<table><tr><td><img src="./images/mqtt4home_96.png"></img></td><td>
<h1>RasPi: Searching the port name of a SIM module</h1>
<a href="../../README.md">==> Home page</a> &nbsp; &nbsp; &nbsp; 
<a href="./LIESMICH.md">==> German version</a> &nbsp; &nbsp; &nbsp; 
</td></tr></table>
<hr>

This simple program tries to open various serial ports in sequence, send the command `ATE0` to them and wait for an answer `OK`.   
The result of the sending is displayed on the console.   
The interfaces that are tried are listed in the main program in the line   
`std::string devices[]={"/dev/ttyS0", "/dev/ttyACM0", "/dev/ttyUSB0", "/dev/ttyUSB1"};`   

One possible program output is:   
```   
pi_@raspi:~/m4hFindSimModule $ ./m4hFindSimModule
Searching for modem...
/dev/ttyS0: NO modem found (error on reading OK)
/dev/ttyACM0: NO modem found (error on reading OK)
/dev/ttyUSB0: GSM modem found!
/dev/ttyUSB1: Could not open device (Error 2: No such file or directory)
```   
So in this example the device name is `/dev/ttyUSB0`.   

_Note_: The creation of the program `m4hFindSimModule` is done with   
`g++ m4hFindSimModule.cpp -o m4hFindSimModule`.   