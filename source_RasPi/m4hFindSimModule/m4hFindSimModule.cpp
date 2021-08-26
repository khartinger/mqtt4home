//_____m4hFindSimModule.cpp______________________khartinger_____
// g++ m4hFindSimModule.cpp -o m4hFindSimModule
// Search for GSM Modem e.g. SIM808 @ /dev/ttyS0, /dev/ttyUSB0, ...
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
// Hardware: (1) Raspberry Pi
//           (2) GSM Modem e.g. SIM808
// Updates:
// 2021-08-26 First release
// Released into the public domain.
#include <string>       // std::string, find
#include <stdio.h>      // printf
#include <string.h>     // strerror
#include <fcntl.h>      // file controls like O_RDWR
#include <errno.h>      // Error integer and strerror()
#include <termios.h>    // POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

//_______close serial interface_________________________________
void ttyClose(int fd) { close(fd); }

//_______open serial interface__________________________________
// return: file descriptor or -1|-2|-3 on error
int ttyOpen(std::string device_)
{
 int fd=open(device_.c_str(), O_RDWR);
 if(fd==-1) return -1;
 //-----------device open---------------------------------------
 //-----------set settings--------------------------------------
 struct termios tty;
 if(tcgetattr(fd, &tty) != 0) return -2;
 tty.c_cflag &= ~PARENB;     // Clear parity bit
 tty.c_cflag &= ~CSTOPB;     // Clear stop field, only one stop bit
 tty.c_cflag |= CS8;         // 8 bits per byte (most common)
 tty.c_cflag &= ~CRTSCTS;    // Disable RTS/CTS hardware flow control
 tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines
 tty.c_lflag &= ~ICANON;     // Disable canonical mode
 tty.c_lflag &= ~ECHO;       // Disable echo
 tty.c_lflag &= ~ECHOE;      // Disable erasure
 tty.c_lflag &= ~ECHONL;     // Disable new-line echo
 tty.c_lflag &= ~ISIG;       // Disable interpretation INTR, QUIT, SUSP
 tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
 tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
 tty.c_oflag &= ~OPOST;      // Prevent special interpretation of output bytes (e.g. newline chars)
 tty.c_oflag &= ~ONLCR;      // Prevent conversion of newline to carriage return/line feed
 tty.c_cc[VTIME] = 10;       // Wait for up to 1s (10 deciseconds), ...
 tty.c_cc[VMIN] = 0;         // ...returning as soon as any data is received.
 cfsetispeed(&tty, B9600);   // input baudrate
 cfsetospeed(&tty, B9600);   // output baudrate
 // Save tty settings, also checking for error
 if (tcsetattr(fd, TCSANOW, &tty) != 0) return -3;
 return fd;
}

//_______write AT to serial_____________________________________
// return: 0=OK, -1 on error
int ttyWriteAT(int fd)
{
 int iRet=0;
 unsigned char cmd[] = { 'A', 'T', 'E','0', '\r', '\n'};
 int cmdLen=sizeof(cmd);
 for(int j=0; j<cmdLen; j++)
 {//--------write char by char--------------------------------
  char c1=cmd[j];
  int  n=write(fd, &c1, 1);
  if(n<1) 
  {//-------char not written, try again-----------------------
   usleep(2000);
   n = write(fd, &c1, 1);
   if(n<1) return -1;
  }
 } // end write char by char
 return 0;
}

//_______(try to) read OK from modem____________________________
// return: 0|1 = OK, -1 on error
int ttyReadOK(int fd)
{
 char cmdIn[256];
 int n=read(fd, &cmdIn, sizeof(cmdIn));// OK\r\n received?
 if(n<4) {
  usleep(500000);                      // wait 500ms
  n+=read(fd, &cmdIn, sizeof(cmdIn)); 
 }
 cmdIn[n]=0;
 //------search for "OK"----------------------------------------
 std::string s1=cmdIn;
 int pos1=s1.find("OK");
 if(pos1==std::string::npos) return -1;
 return 0;
}

//_______main program___________________________________________
int main(int argc, char *argv[])
{ 
 std::string devices[]={"/dev/ttyS0", "/dev/ttyACM0", "/dev/ttyUSB0", "/dev/ttyUSB1"};
 int devicesNum=4;
 std::string device;
 int    fd;
 int    iRet=0;
 printf("Searching for modem...\n");
 //-----------for all devices-----------------------------------
 for(int i=0; i<devicesNum; i++)
 {
  //----------try to open serial interface----------------------
  device=devices[i];
  printf("%s: ",device.c_str()); fflush(stdout);
  fd=ttyOpen(device);
  if(fd<0) {
   if(fd==-1) printf("Could not open device (Error %i: %s)\n", errno, strerror(errno));
   else if(fd==-2) printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
   else if(fd==-3) printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
   else printf("Unknown error %i: %s\n", errno, strerror(errno));
   continue;
  }
  //----------send AT to serial---------------------------------
  iRet=ttyWriteAT(fd);
  if(iRet!=0) {
   ttyClose(fd);
   printf("Error writing AT command!\n");
   continue;
  }
  //----------(try to) read OK from module----------------------
  iRet=ttyReadOK(fd);
  ttyClose(fd);
  if(iRet>=0) 
  {//---------read OK success-----------------------------------
   printf("GSM-Modem found!\n");
  } else 
  {//---------error on reading OK-------------------------------
   printf("NO modem found (error on reading OK)\n");
  }
 }// end for all devices
 return 0;
}