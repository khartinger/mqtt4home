//_____D1_class_Screen1a.cpp__________________170412-210202_____
// The class Screen1 extends the class Screen_64x48 
// with screen methods to write text on the display.
//
// Hardware: (1) WeMos D1 mini
//           (2) OLED Shield: SSD1306, 64x48 pixel, I2C
// Created by Karl Hartinger, December 15, 2018,
// Updates: 
//        scroll screen 4|5|6
// 210202 change begin()
// Released into the public domain.
#include "D1_class_Screen1b.h"       //

//**************************************************************
//       constructor & co
//**************************************************************

//_______constructor 1__________________________________________
Screen1::Screen1() { setup(OLED_RESET); }

//_______constructor 2__________________________________________
Screen1::Screen1(int oled_resetpin) { setup(oled_resetpin); }

//_______setup properties_______________________________________
void Screen1::setup(int pin_num) {
 _Adafruit_SSD1306((int8_t)pin_num);
 i2cAddress=SSD1306_I2C_ADDRESS;            // default: 0x3C
 dotCounter=0;                              //
}

  
//_______init display___________________________________________
bool Screen1::begin() { return begin(true, i2cAddress); }

//_______init display___________________________________________
bool Screen1::begin(bool startI2C) { 
 return begin(startI2C, i2cAddress);
}

//_______init display___________________________________________
bool Screen1::begin(bool startI2C, int i2c_address) {
 int  iRet;                            // i2c return value
 setAddress(i2c_address);
 //------start I2C?---------------------------------------------
 if(startI2C) {
  Wire.begin();                        //call Wire.begin only once
 }
 //------check for display (address 0x3C or 0x3D)---------------
 Wire.beginTransmission(SSD1306_I2C_ADDRESS);    // start
 iRet = Wire.endTransmission();        // end transmission
 if(iRet!=0) { //-----ACK NOT received--------------------------
  Wire.beginTransmission(SSD1306_I2C_ADDRESS2);  // start
  iRet = Wire.endTransmission();       // end transmission
  if(iRet!=0) return false;            // ACK NOT received
 }
 //this->begin(SSD1306_SWITCHCAPVCC, OLED_I2C);    //I2C addr 0x3C
 _Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, i2cAddress);
 this->clearDisplay();                      //clear display-buff
 this->setTextColor(WHITE);                 //
 this->setFont(&fontKH_5x8);                //
 //this->setFont(NULL);
 this->setTextSize(1);                      //
 this->display();                           //show display-buff
 return true;
}

//_______set i2c address________________________________________
void Screen1::setAddress(int i2c_address) {
 if((i2c_address!=SSD1306_I2C_ADDRESS) &&
    (i2c_address!=SSD1306_I2C_ADDRESS2)) return;
 i2cAddress=i2c_address;
}

//_______return i2c address_____________________________________
int Screen1::getAddress() { return i2cAddress; }


//**************************************************************
// simple screens: one area, 4..6 single size lines
//**************************************************************

//_____display: 1 area, 6 lines, 10 chars/line, no border_______
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen6(int line_, String text_)
{ scr6(line_, text_, 'l', false, false); }

void Screen1::screen6(int line_, String text_, char align_)
{ scr6(line_, text_, align_, false, false); }

void Screen1::screen6Clear(int line_, String text_)
{ scr6(line_, text_, 'l', true, false); }

void Screen1::screen6Clear(int line_, String text_, char align_)
{ scr6(line_, text_, align_, true, false); }

//_____display: 1 area, 6 lines, 10 chars/line, no border_______
// first line inverted
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen6i(int line_, String text_)
{ scr6(line_, text_, 'c', false, true); }

void Screen1::screen6i(int line_, String text_, char align_)
{ scr6(line_, text_, align_, false, true); }

void Screen1::screen6iClear(int line_, String text_)
{ scr6(line_, text_, 'c', true, true); }

void Screen1::screen6iClear(int line_, String text_, char align_)
{ scr6(line_, text_, align_, true, true); }

//_____display: 1 area, 5 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen5(int line_, String text_)
{ scr5(line_, text_, 'l', false, false); }

void Screen1::screen5(int line_, String text_, char align_)
{ scr5(line_, text_, align_, false, false); }

void Screen1::screen5Clear(int line_, String text_)
{ scr5(line_, text_, 'l', true, false); }

void Screen1::screen5Clear(int line_, String text_, char align_)
{ scr5(line_, text_, align_, true, false); }

//_____display: 1 area, 5 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen5i(int line_, String text_) 
{ scr5(line_, text_, 'c', false, true); }

void Screen1::screen5i(int line_, String text_, char align_)
{ scr5(line_, text_, align_, false, true); }

void Screen1::screen5iClear(int line_, String text_)
{ scr5(line_, text_, 'c', true, true); }

void Screen1::screen5iClear(int line_, String text_, char align_)
{ scr5(line_, text_, align_, true, true); }

//_____display: 1 area, 4 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen4(int line_, String text_)
{ scr4(line_, text_, 'l', false, false); }

void Screen1::screen4(int line_, String text_, char align_)
{ scr4(line_, text_, align_, false, false); }

void Screen1::screen4Clear(int line_, String text_)
{ scr4(line_, text_, 'l', true, false); }

void Screen1::screen4Clear(int line_, String text_, char align_)
{ scr4(line_, text_, align_, true, false); }

//_____display: 1 area, 4 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen4i(int line_, String text_) 
{ scr4(line_, text_, 'c', false, true); }

void Screen1::screen4i(int line_, String text_, char align_)
{ scr4(line_, text_, align_, false, true); }

void Screen1::screen4iClear(int line_, String text_)
{ scr4(line_, text_, 'c', true, true); }

void Screen1::screen4iClear(int line_, String text_, char align_)
{ scr4(line_, text_, align_, true, true); }

//**************************************************************
// simple screens: one area, 1..2 double size lines
//**************************************************************

void Screen1::screen2(int line_, String text_)
{ screen2(line_, text_, 'c', false); }

void Screen1::screen2(int line_, String text_, char align_)
{ screen2(line_, text_, align_, false); }

void Screen1::screen2Clear(int line_, String text_, char align_)
{ screen2(line_, text_, align_, true); }

void Screen1::screen1(String text_)
{ screen1(text_, 'c', false); }

void Screen1::screen1(String text_, char align_)
{ screen1(text_, align_, false); }

void Screen1::screen1Clear(String text_, char align_)
{ screen1(text_, align_, true); }

//**************************************************************
// screens with more than one area, single size lines
//**************************************************************

//_____display: 3 areas, 5 lines, 10 chars/line, borders________
// border around area 1 and 2
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen221(int line_, String text_, 
  char align_, bool cls_)
{
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);   //clear area before writing
 align_=tolower(align_);
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,1,62,9,BLACK); //clear line 1
   this->setCursor(dx+2,2);                 //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
   case 2: //-----area 1, line 2--------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,10,62,8,BLACK);//clear line 2
   this->setCursor(dx+2,10);                //
   this->print(mytrim(max_, s_, align_));   //write text
   break;
  case 3: //-----area 2, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,19,62,9,BLACK);//clear line 3
   this->setCursor(dx+2,20);                //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
   case 4: //-----area 2, line 4--------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(1,28,62,10,BLACK);//clear line 4
   this->setCursor(dx+2,29);                //
   this->print(mytrim(max_, s_, align_));   //write text
   break;  
  case 5: //-----area 3, one line out of the rectangle----------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,39,64,9,BLACK);//clear line 5
   this->setCursor(dx,40);                  //dx only!
   this->print(mytrim(max_, s_, align_));   //write text
   break;
  default: break;
 }
 //-----draw lines----------------------------------------------
 this->drawRect(0,0,64,39,WHITE);      // border around line 1-4
 this->drawLine(1,18,62,18,WHITE);     // horizontal line
 this->display();                      // show buffer
}

void Screen1::screen221(int line_, String text_)
{ screen221(line_, text_, 'l', false); }

void Screen1::screen221(int line_, String text_, char align_)
{ screen221(line_, text_, align_, false); }

void Screen1::screen221Clear(int line_, String text_, char align_)
{ screen221(line_, text_, align_, true); }

//**************************************************************
// screens with single and double size lines
//**************************************************************

//_____display: 3 areas, 1+1+2 lines, 10/5/10 chars/line________
// area 1: inverted, area 2: double text size
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen112(int line_, String text_, 
  char align_, bool cls_)
{
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);   //clear area before writing
 align_=tolower(align_);
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 //-----write text----------------------------------------------
 switch(line_)
 {
   case 1: //-----area 1, line 1, inverted----------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,0,64,10,WHITE);//clear line 1
   this->setCursor(dx+2,1);                 //
   this->setTextColor(BLACK,WHITE);
   this->print(mytrim(max_, s_, align_));   //write text
   this->setTextColor(WHITE,BLACK);
   break; 
  case 2: //-----area 2, double text size-----------------------
   max_=5;
   if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=2*dc; }
   if(clr_)this->fillRect(1,10,62,19,BLACK);//clear area 2 
   this->setCursor(dx+3,12);
   this->setTextSize(2);                    //charsize 10x16 pix
   this->println(mytrim(max_, text_, align_));   //write line 2  
   this->setTextSize(1);                    //charsize 5x8 pixel
   break;
  case 3: //-----area 3, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,29,64,11,BLACK);//clear line 3
   this->setCursor(dx,31);                  //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
   case 4: //-----area 3, line 2--------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,40,64,8,BLACK);//clear line 4
   this->setCursor(dx,40);                  //
   this->print(mytrim(max_, s_, align_));   //write text
   break;  
  default: break;
 }
 //-----draw lines----------------------------------------------
 this->drawRect(0,0,64,30,WHITE);      // border around area 1
 this->display();                      // show buffer
}

void Screen1::screen112(int line_, String text_)
{ screen112(line_, text_, 'c', false); }

void Screen1::screen112(int line_, String text_, char align_)
{ screen112(line_, text_, align_, false); }

void Screen1::screen112Clear(int line_, String text_, char align_)
{ screen112(line_, text_, align_, true); }


//_____display: 2 areas: 1/3 lines, 5/10 chars/line_____________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen13(int line_, String text_, 
  char align_='c', bool cls_=false)
{
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);   // clear area before writing
 align_=tolower(align_);
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, double text size-----------------------
   max_=5;
   if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=2*dc; }
   if(clr_) this->fillRect(1,1,62,19,BLACK);     //clear area 1 
   this->setCursor(dx+3,3);
   this->setTextSize(2);                    //charsize 10x16 pix
   this->println(mytrim(max_, text_, align_));   //write line 1  
   this->setTextSize(1);                    //charsize 5x8 pixel
   break;
  case 2: //-----area 2, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,21,64,10,BLACK);//clear line 2
   this->setCursor(dx,22);                  //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();  
  case 3: //-----area 2, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,30,64,10,BLACK);//clear line 3
   this->setCursor(dx,31);                  //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();  
  case 4: //-----area 2, line 4---------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,40,64,8,BLACK);//clear line 4
   this->setCursor(dx,40);                  //
   this->print(mytrim(max_, s_, align_));   //write text
   break; 
  default: break;
 }
 //-----draw lines----------------------------------------------
 this->drawRect(0,0,64,21,WHITE);      // border around area 1
 this->display();                      // show buffer
}

void Screen1::screen13(int line_, String text_)
{ screen13(line_, text_, 'c', false); }

void Screen1::screen13(int line_, String text_, char align_)
{ screen13(line_, text_, align_, false); }

void Screen1::screen13Clear(int line_, String text_, char align_)
{ screen13(line_, text_, align_, true); }

//_____display: 2 areas, 1+2 lines, 5/10 chars/line, border_____
// area 1: double text size
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen12(int line_, String text_, 
  char align_='c', bool cls_=false)
{
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);   // clear area before writing
 align_=tolower(align_);
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, double text size-----------------------
   max_=5;
   if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=2*dc; }
   if(clr_) this->fillRect(1,1,62,27,BLACK);     //clear area 1 
   this->setCursor(dx+3,7);
   this->setTextSize(2);                    //charsize 10x16 pix
   this->println(mytrim(max_, text_, align_));   //write line 1  
   this->setTextSize(1);                    //charsize 5x8 pixel
   break;
  case 2: //-----area 2, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,29,64,19,BLACK);//clear line 2
   this->setCursor(dx,31);                   //
   this->print(mytrim(max_, s_, align_));    //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
   case 3: //-----area 2, line 3--------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,40,64,8,BLACK);//clear line 4
   this->setCursor(dx,40);                   //
   this->print(mytrim(max_, s_, align_));    //write text
   break;  
  default: break;
 }
 //-----draw lines----------------------------------------------
 this->drawRect(0,0,64,29,WHITE);      // border around area 1
 this->display();                      // show buffer
}

void Screen1::screen12(int line_, String text_)
{ screen12(line_, text_, 'c', false); }

void Screen1::screen12(int line_, String text_, char align_)
{ screen12(line_, text_, align_, false); }

void Screen1::screen12Clear(int line_, String text_, char align_)
{ screen12(line_, text_, align_, true); }

//**************************************************************
// screens with 2xbutton
//**************************************************************

void Screen1::screen4B(int line_, String text_)
{
 if((line_<5)||(line_==7)) 
      scr4B(line_, text_, 'l', false, false, false);
 else scr4B(line_, text_, 'c', false, false, false);
}
 
void Screen1::screen4B(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, false, false, false); }

void Screen1::screen4BClear(int line_, String text_)
{
 if((line_<5)||(line_==7))
      scr4B(line_, text_, 'l', true, false, false);
 else scr4B(line_, text_, 'c', true, false, false);
}

void Screen1::screen4BClear(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, true, false, false); }

void Screen1::screen4BClearAll(int line_, String text_)
{
 if((line_<5)||(line_==7))
      scr4B(line_, text_, 'l', true, false, true);
 else scr4B(line_, text_, 'c', true, false, true);
}

void Screen1::screen4BClearAll(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, true, false, true); }

void Screen1::screen4Bi(int line_, String text_)
{
 if((line_<5)||(line_==7))
      scr4B(line_, text_, 'l', false, true, false);
 else scr4B(line_, text_, 'c', false, true, false);
}
 
void Screen1::screen4Bi(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, false, true, false); }

void Screen1::screen4BiClear(int line_, String text_)
{
 if((line_<5)||(line_==7))
      scr4B(line_, text_, 'l', true, true, false);
 else scr4B(line_, text_, 'c', true, true, false);
}

void Screen1::screen4BiClear(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, true, true, false); }

void Screen1::screen4BiClearAll(int line_, String text_)
{
 if((line_<5)||(line_==7))
      scr4B(line_, text_, 'l', true, true, true);
 else scr4B(line_, text_, 'c', true, true, true);
}

void Screen1::screen4BiClearAll(int line_, String text_, char align_)
{ scr4B(line_, text_, align_, true, true, true); }

//**************************************************************
// special screens
//**************************************************************

//_____clear display and draw a border__________________________
void Screen1::dotClear()
{
 this->dotClear("");
}

//_____clear display, write line 6 and draw a border____________
void Screen1::dotClear(String line6)
{
 dotCounter=0;
 this->clearDisplay();
 this->fillRect(1,1,63,38,BLACK);      // clear rectangle
 this->drawRect(0,0,64,39,WHITE);      // border around
 this->dotLine(line6);                 // set line 6, display
}

//_____draw a dot_______________________________________________
void Screen1::dot()
{
 if(dotCounter<50)
 {
  if(dotCounter%10==0)
   this->setCursor(2,2+(dotCounter/10)*7);
  dotCounter++;
  this->print(DOT);                    // print dot
  this->display();                     // show buffer
 }  
 else
 {
  dotCounter=0;
  this->fillRect(1,1,63,38,BLACK);     // clear rectangle
  this->drawRect(0,0,64,39,WHITE);     // border around
  this->setCursor(0,0);                // set cursor start
 }
}

//_____dot screen: write line 6_________________________________
void Screen1::dotLine(String line6)
{
 this->fillRect(0,39,64,9,BLACK);      // clear text area
 this->setCursor(0,40);                // set cursor start
 this->print(line6);                   // print line 6
 int posx = 2 + 6*(dotCounter%10);     // old cursor pos x
 int posy = 2 + 7*(dotCounter/10);     // old cursor pos y
 this->setCursor(posx, posy);          // set cursor
 this->display();                      // show buffer
}

//**************************************************************
// helper functions
//**************************************************************

//_____replace special chars by hex code________________________
String Screen1::utf8ToOled(String s)
{
 String sUnknown=String((char)127);
 String s1="";
 char c1,c2,c3;
 int i, imax=s.length();
 //Serial.print("imax=");Serial.println(imax);
 for(i=0; i<imax; i++)
 {
  c1=s.charAt(i);
  if(c1<128) 
   s1=s1+String(c1);
  else
  {
   c2=s.charAt(++i);
   switch(c1)
   {
    case 0xc2: //-----first byte is C2--------------------------
     switch(c2) {
      case 0xb0: s1=s1+"\xF8"; break;  //degree sign
      case 0xb5: s1=s1+"\xE6"; break;  //micro sign
      case 0xaa: s1=s1+"\xA6"; break;  //feminine ordinal indicator
      case 0xb2: s1=s1+"\xFD"; break;  //superscript 2
      //default: s1=s1+sUnknown;
      default: s1=s1+"\xC2"; i--;
     }
     break;
    case 0xc3: //-----first byte is C3--------------------------
     switch(c2){
      case 0x84: s1=s1+"\x8E"; break;  //A with diaeresis
      case 0x96: s1=s1+"\x99"; break;  //O with diaeresis
      case 0x9c: s1=s1+"\x9A"; break;  //U with diaeresis
      case 0xa4: s1=s1+"\x84"; break;  //a with diaeresis
      case 0xb6: s1=s1+"\x94"; break;  //o with diaeresis
      case 0xbc: s1=s1+"\x81"; break;  //u with diaeresis
      case 0x9f: s1=s1+"\xE1"; break;  //sharp s, beta
      //default: s1=s1+sUnknown;
      default: s1=s1+"\xC3"; i--;
     }
     break;
    case 0xe2: //-----first byte is E2--------------------------
     switch(c2) {
      case 0x82: 
       c3=s.charAt(++i);
       if(c3==0xAC) { s1=s1+"\x9E"; }  // Euro
               else { s1=s1+sUnknown; }
       break;
      //default: s1=s1+sUnknown;
      default: s1=s1+"\xE2"; i--;
     }
     break;
    default: //-----first byte is no special--------------------
     //s1=s1+sUnknown; break;
     s1=s1+String(c1); i--; break;
   }
  }
 }
 return s1;
}

//_____clear scroll buffer______________________________________
void Screen1::clearLines()
{
 for(int i=0; i<6; i++)
 {
  stLines[i].txt="";
  stLines[i].ali='l';
  stLines[i].inv=false;
 }
}

//_____clear scroll buffer______________________________________
void Screen1::clearLines4B()
{
 for(int i=0; i<4; i++)
 {
  stLines[i].txt="";
  stLines[i].ali='l';
  stLines[i].inv=false;
 }
}

//_____make string with max chars (add blank before/after)______
String Screen1::mytrim(int max_, String txt_, char align_='l')
{
 String s=txt_;
 //-----add leading blanks, if align_ needs them----------------
 int blank_=0;
 int len_=s.length();
 if(align_=='c') blank_=(max_-len_)/2;
 if(align_=='r') { if(len_<max_) blank_=max_-len_; }
 //-----add blanks before text----------------------------------
 for(int i=0; i<blank_; i++) s=" "+s;
 if(len_>max_) 
 { //-----text too long, truncate text--------------------------
  s=s.substring(0,max_);
 }
 else //-----text too short, add blank--------------------------
 {
  for(int i=len_; i<max_; i++) s+=" ";
 }
 return s;
}

//_____display: 1 area, 6 lines, 10 chars/line, no border_______
// first line may be inverted
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::scr6(int line_, String text_, 
  char align_, bool cls_, bool invert_)
{
 String sX;                                 // helper for scroll
 String s_=text_;
 int dx=2, dc=6;                            // x-Offset,c=center
 int max_=10;
 int len_=s_.length();
 int fore_=WHITE;                           //color foreground
 int back_=BLACK;                           //color background
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 if(invert_) { fore_=BLACK; back_=WHITE; }
 this->setTextColor(fore_, back_);
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,0,64,8,back_); //clear line 1
   this->setCursor(dx,0);                   //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;                    //txt <=10 chars: OK
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 2: //-----area 1, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,8,64,8,back_); //clear line 2
   this->setCursor(dx,8);                   //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[0].ali=align_;                   // remember...
   stLines[0].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[0].txt=s_;break;} // line finished
   stLines[0].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 3: //-----area 1, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,16,64,8,back_);//clear line 3
   this->setCursor(dx,16);                  //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[1].ali=align_;                   // remember...
   stLines[1].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[1].txt=s_;break;} // line finished
   stLines[1].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
   len_=s_.length();                        //text for next line
  case 4: //-----area 1, line 4 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,24,64,8,back_);//clear line 4
   this->setCursor(dx,24);                  //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[2].ali=align_;                   // remember...
   stLines[2].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[2].txt=s_;break;} // line finished
   stLines[2].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 5: //-----area 1, line 5 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,32,64,8,back_);//clear line 5
   this->setCursor(dx,32);                  //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[3].ali=align_;                   // remember...
   stLines[3].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[3].txt=s_;break;} // line finished
   stLines[3].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 6: //-----area 1, line 6---------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(0,40,64,8,back_);//clear line 6
   this->setCursor(dx,40);                  //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[4].ali=align_;                   // remember...
   stLines[4].inv=invert_;                  // ...for scrolling
   if(len_<=max_) { stLines[4].txt=s_; stLines[5].txt=""; }
   else { stLines[4].txt=sX; stLines[5].txt=s_.substring(max_);}
   stLines[5].ali=align_;                  // ...also the
   stLines[5].inv=invert_;                 // ...next line
   break;
  case 7: //-----scroll up and write to line 6------------------
   if(cls_){// clear screen: print last line
    scr6(6, s_, stLines[5].ali, true, stLines[5].inv);
   }
   else  //.....no clear screen.................................
   {
    if(len_>0) {                            // text given...
     stLines[5].txt=s_;                     // remember text,
     stLines[5].ali=align_;                 // ..align, invert
     stLines[5].inv=invert_;                // ..for scrolling
    }    
    for(int i=0; i<6; i++) { //.....scroll text.................
     scr6(i+1,stLines[i].txt,stLines[i].ali,false,stLines[i].inv);
    }
   }
   break;
  default: break;
 }
 this->setTextColor(WHITE,BLACK);           //default textcolor
 this->display();                           //show buffer
}

//_____display: 1 area, 5 lines, 10 chars/line, border__________
// first line may be inverted
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::scr5(int line_, String text_, 
  char align_, bool cls_, bool invert_)
{
 String sX;                                 // helper for scroll
 String s_=text_;
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 int len_=s_.length();
 int fore_=WHITE;                           //color foreground
 int back_=BLACK;                           //color background
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 if(invert_) { fore_=BLACK; back_=WHITE; }
 this->setTextColor(fore_, back_);
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,1,62,10,back_); //clear line 1
   this->setCursor(dx+3,2);                 //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
  case 2: //-----area 1, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,11,62,9,back_);//clear line 2
   this->setCursor(dx+3,11);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[0].ali=align_;                   // remember...
   stLines[0].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[0].txt=s_;break;} // line finished
   stLines[0].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 3: //-----area 1, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,20,62,9,back_);//clear line 3
   this->setCursor(dx+3,20);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[1].ali=align_;                   // remember...
   stLines[1].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[1].txt=s_;break;} // line finished
   stLines[1].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 4: //-----area 1, line 4 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,29,62,9,back_);//clear line 4
   this->setCursor(dx+3,29);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[2].ali=align_;                   // remember...
   stLines[2].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[2].txt=s_;break;} // line finished
   stLines[2].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 5: //-----area 1, line 5---------------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_) this->fillRect(1,38,62,9,back_);//clear line 5
   this->setCursor(dx+3,38);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[3].ali=align_;                   // remember...
   stLines[3].inv=invert_;                  // ...for scrolling
   if(len_<=max_) { stLines[3].txt=s_; stLines[4].txt=""; }
   else { stLines[3].txt=sX; stLines[4].txt=s_.substring(max_);}
   stLines[4].ali=align_;                  // ...also the
   stLines[4].inv=invert_;                 // ...next line
   break;
  case 6: //-----scroll up and write to line 6------------------
   if(cls_){// clear screen: print last line
    scr5(5, s_, stLines[4].ali, true, stLines[4].inv);
   }
   else  //.....no clear screen.................................
   {
    if(len_>0) {                            // text given...
     stLines[4].txt=s_;                     // remember text,
     stLines[4].ali=align_;                 // ..align, invert
     stLines[4].inv=invert_;                // ..for scrolling
    }    
    for(int i=0; i<5; i++) { //.....scroll text.................
     scr5(i+1,stLines[i].txt,stLines[i].ali,false,stLines[i].inv);
    }
   }
   break;
  default: break;
 }
 this->drawRect(0,0,64,48,WHITE);           //border
 this->setTextColor(WHITE,BLACK);           //default textcolor
 this->display();                           //show buffer
}

//_____display: 1 area, 4 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::scr4(int line_, String text_, 
  char align_, bool cls_, bool invert_)
{
 String sX;                                 // helper for scroll
 String s_=text_;
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 int len_=s_.length();
 int fore_=WHITE;                           //color foreground
 int back_=BLACK;                           //color background
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 if(invert_) { fore_=BLACK; back_=WHITE; }
 this->setTextColor(fore_, back_);
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(1,1,62,13,back_);//clear line 1
   this->setCursor(dx+3,4);                //
   this->print(mytrim(max_, s_, align_));  //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
  case 2: //-----area 1, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(1,13,62,12,back_);//clear line 2
   this->setCursor(dx+3,15);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[0].ali=align_;                   // remember...
   stLines[0].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[0].txt=s_;break;} // line finished
   stLines[0].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 3: //-----area 1, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(1,24,62,12,back_);//clear line 3
   this->setCursor(dx+3,26);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[1].ali=align_;                   // remember...
   stLines[1].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[1].txt=s_;break;} // line finished
   stLines[1].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 4: //-----area 1, line 4 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(1,35,62,12,back_);//clear line 4
   this->setCursor(dx+3,37);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[2].ali=align_;                   // remember...
   stLines[2].inv=invert_;                  // ...for scrolling
   if(len_<=max_) { stLines[2].txt=s_; stLines[3].txt=""; }
   else { stLines[2].txt=sX; stLines[3].txt=s_.substring(max_);}
   stLines[3].ali=align_;                  // ...also the
   stLines[3].inv=invert_;                 // ...next line
   break;
  case 5: //-----scroll up and write to line 4------------------
   if(cls_){// clear screen: print last line
    scr4(4, s_, stLines[3].ali, true, stLines[3].inv);
   }
   else  //.....no clear screen.................................
   {
    if(len_>0) {                            // text given...
     stLines[3].txt=s_;                     // remember text,
     stLines[3].ali=align_;                 // ..align, invert
     stLines[3].inv=invert_;                // ..for scrolling
    }    
    for(int i=0; i<4; i++) { //.....scroll text.................
     scr4(i+1,stLines[i].txt,stLines[i].ali,false,stLines[i].inv);
    }
   }
   break;
  default: break;
 }
 this->drawRect(0,0,64,48,WHITE);           //border
 this->setTextColor(WHITE,BLACK);           //default textcolor
 this->display();                           //show buffer
}

//_____display: 1 area, 4 lines, 10 chars/line, border__________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::scr4B(int line_, String text_, 
  char align_, bool cls_, bool invert_, bool all_)
{
 String sX;                                 // helper for scroll
 String s_=text_;
 int dx=0, dc=3;                            // x-Offset,c=center
 int max_=10;
 int maxB_=4;
 int len_=s_.length();
 int fore_=WHITE;                           //color foreground
 int back_=BLACK;                           //color background
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { 
  if(all_) { this->clearDisplay(); clearLines(); }
      else { this->fillRect(0,0,64,36,BLACK); clearLines4B(); }
 }
 if(invert_) { fore_=BLACK; back_=WHITE; }
 this->setTextColor(fore_, back_);
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,0,64,8,back_);//clear line 1
   this->setCursor(dx+2,0);                //
   this->print(mytrim(max_, s_, align_));  //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
  case 2: //-----area 1, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,8,64,9,back_);  //clear line 2
   this->setCursor(dx+2,9);                 //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[0].ali=align_;                   // remember...
   stLines[0].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[0].txt=s_;break;} // line finished
   stLines[0].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 3: //-----area 1, line 3 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,17,64,9,back_); //clear line 3
   this->setCursor(dx+2,18);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[1].ali=align_;                   // remember...
   stLines[1].inv=invert_;                  // ...for scrolling
   if(len_<=max_){stLines[1].txt=s_;break;} // line finished
   stLines[1].txt=sX;                       //remember full line
   s_=s_.substring(max_);                   //prepair remaining
   len_=s_.length();                        //text for next line
  case 4: //-----area 1, line 4 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)>0)) dx=dc; }
   if(clr_)this->fillRect(0,26,64,9,back_); //clear line 4
   this->setCursor(dx+2,27);                //
   sX=mytrim(max_, s_, align_);             // text max 10 chars
   this->print(sX);                         // write text
   stLines[2].ali=align_;                   // remember...
   stLines[2].inv=invert_;                  // ...for scrolling
   if(len_<=max_) { stLines[2].txt=s_; stLines[3].txt=""; }
   else { stLines[2].txt=sX; stLines[3].txt=s_.substring(max_);}
   stLines[2].ali=align_;                  // ...also the
   stLines[2].inv=invert_;                 // ...next line
   break;
  case 5: //-----left button------------------------------------
   if(text_=="") {
    this->fillRect(0,36,31,12,BLACK);       // remove button
   }
   else {
    this->drawRect(4,36,27,12,WHITE);       // border
    this->drawFastVLine(0, 41, 2, WHITE);   // arrow
    this->drawFastVLine(1, 40, 4, WHITE);   // arrow
    this->drawFastVLine(2, 39, 6, WHITE);   // arrow
    if(align_=='c'){ if((len_<maxB_) &&((len_&1)>0)) dx=dc; }
    if(clr_)this->fillRect(5,37,25,10,back_); //clear left butt
    this->setCursor(dx+6,38);               //    
    sX=mytrim(maxB_, s_, align_);           // text max 4 chars
    stLines[4].txt=s_;                      // remember text,
    stLines[4].ali=align_;                  // ..align, invert
    stLines[4].inv=invert_;                 // ..for scrolling
    this->print(sX);                        // write text
   }
   break;
  case 6: //-----right button-----------------------------------
   if(text_=="") {
    this->fillRect(33,36,31,12,BLACK);       // remove button
   }
   else {
    this->drawRect(33,36,27,12,WHITE);      // border
    this->drawFastVLine(63, 41, 2, WHITE);   // arrow
    this->drawFastVLine(62, 40, 4, WHITE);   // arrow
    this->drawFastVLine(61, 39, 6, WHITE);   // arrow
    if(align_=='c'){ if((len_<maxB_) &&((len_&1)>0)) dx=dc; }
    if(clr_)this->fillRect(34,37,25,10,back_); //clear left butt
    this->setCursor(dx+35,38);              //    
    sX=mytrim(maxB_, s_, align_);           // text max 10 chars
    stLines[5].txt=s_;                      // remember text,
    stLines[5].ali=align_;                  // ..align, invert
    stLines[5].inv=invert_;                 // ..for scrolling
    this->print(sX);                        // write text
   }
   break;
  case 7: //-----scroll up and write to line 4------------------
   if(cls_){// clear screen: print last line
    scr4B(4, s_, stLines[3].ali, true, stLines[3].inv, all_);
   }
   else  //.....no clear screen.................................
   {
    if(len_>0) {                            // text given...
     stLines[3].txt=s_;                     // remember text,
     stLines[3].ali=align_;                 // ..align, invert
     stLines[3].inv=invert_;                // ..for scrolling
    }    
    for(int i=0; i<4; i++) { //.....scroll text.................
     scr4B(i+1,stLines[i].txt,stLines[i].ali,false,stLines[i].inv, false);
    }
   }
   break;
  default: break;
 }
 this->setTextColor(WHITE,BLACK);           //default textcolor
 this->display();                           //show buffer
}

//_____display: 1 area, 2 lines, 5 chars/line, border___________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen2(int line_, String text_, 
  char align_='c', bool cls_=false)
{
 int dx=0, dc=6;                            // x-Offset,c=center
 int max_=5;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 this->setTextSize(2);                      //char size 10x16px
 //-----write text----------------------------------------------
 switch(line_)
 {
  case 1: //-----area 1, line 1, double text size---------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=dc; }
   if(clr_) this->fillRect(1,1,62,25,BLACK);//clear line 1
   this->setCursor(dx+3,7);                 //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
   s_=s_.substring(max_);
   len_=s_.length();
  case 2: //-----area 1, line 2 or more-------------------------
   if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=dc; }
   if(clr_)this->fillRect(1,26,62,22,BLACK);//clear line 2
   this->setCursor(dx+3,26);                //
   this->print(mytrim(max_, s_, align_));   //write text
   if(len_<=max_) break;
  default: break;
 }
 this->setTextSize(1);                      //charsize 5x8 pixel
 this->drawRect(0,0,64,48,WHITE);           // border
 this->display();                           //show buffer
}

//_____display: 1 area, 2 lines, 5 chars/line, border___________
// align: l=left, c=center, r=right, L=left+overwrite, C, R...
void Screen1::screen1(String text_, 
  char align_='c', bool cls_=false)
{
 int dx=0, dc=6;                            // x-Offset,c=center
 int max_=5;
 String s_=text_;
 int len_=s_.length();
 bool clr_=isLowerCase(align_);             //clear area before
 align_=tolower(align_);                    //only lower chars
 if(cls_) { this->clearDisplay(); clearLines(); } //clear buffer
 this->setTextSize(2);                      //char size 10x16px
 //-----write text----------------------------------------------
 if(align_=='c'){ if((len_<max_) &&((len_&1)==0)) dx=dc; }
 if(clr_) this->fillRect(1,1,62,46,BLACK);  //clear
 this->setCursor(dx+3,16);                  //
 this->print(mytrim(max_, s_, align_));     //write text
 this->setTextSize(1);                      //charsize 5x8 pixel
 this->drawRect(0,0,64,48,WHITE);           // border
 this->display();                           //show buffer
}

