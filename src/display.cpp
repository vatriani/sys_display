#include "display.hpp"
#include "displaydata.hpp"



Display::Display ( ) {
  u8g2 = new U8G2_SH1106_128X64_NONAME_1_SW_I2C ( U8G2_R0, SCL, SDA );
  u8g2->begin ( );

  actualPage = 0;
}



void Display::drawHeader ( ) {
  u8g2->drawHLine ( 0, 16, 160 );
  u8g2->setFont ( u8g_font_profont17r );
  u8g2->drawStr ( 0, 15, (const char*) FPSTR ( "sensors" ) );
  u8g2->setFont ( u8g_font_profont15r );
  u8g2->drawStr ( 80, 14, (const char*) FPSTR ( title[actualPage] ) );
}



void Display::drawContend ( ) {
  std::string lineText[3];

  switch ( actualPage ) {
  case 0: // CPU
    lineText[0].append ( displayString.t );
    sReplace ( &lineText[0], data->cpu_t );
    lineText[1].append ( displayString.c );
    sReplace ( &lineText[1], data->cpu_c );
    lineText[2].append ( displayString.u );
    sReplace ( &lineText[2], data->cpu_u );
    break;;

  case 1: // LIQUID
    lineText[0].append ( displayString.s );
    sReplace ( &lineText[0], data->liquid_f );
    lineText[1].append ( displayString.s );
    sReplace ( &lineText[1], data->liquid_p );
    lineText[2].append ( displayString.t );
    sReplace ( &lineText[2], data->liquid_t );
    break;;

  case 2: // GPU
    lineText[0].append ( displayString.t );
    sReplace ( &lineText[0], data->gpu_t );
    lineText[1].append ( displayString.u );
    sReplace ( &lineText[1], data->gpu_u );
    lineText[2].append ( displayString.p );
    sReplace ( &lineText[2], data->gpu_p );
    break;;

  case 3: // SYS
    lineText[0].append ( displayString.s );
    sReplace ( &lineText[0], data->fan1_s );
    lineText[1].append ( displayString.s );
    sReplace ( &lineText[1], data->fan2_s );
    lineText[2].append ( displayString.s );
    sReplace ( &lineText[2], data->fan3_s );
    break;;
  }

  u8g2->setFont ( u8g_font_profont15r );

  for (unsigned int counter = 0; counter < 3; counter++ )
    u8g2->drawStr ( 0, line[counter], lineText[counter].c_str ( ) );
}



void Display::draw ( ) {
  u8g2->firstPage ( );

  do {
    drawHeader ( );
    drawContend ( );
  } while ( u8g2->nextPage ( ) );
}



void Display::next ( ) {
  if (actualPage == 3) actualPage = 0;
  else actualPage++;
}



void Display::drawErr ( std::string err ) {
  drawHeader ( );
  u8g2->setFont ( u8g_font_profont15r );
  u8g2->drawStr ( 0, line[0], err.c_str( ) );
}



void Display::setDisplayData ( displayData* dataN ) {
  this->data = dataN;
}


void Display::sReplace ( std::string* input, std::string replace ) {
  std::size_t pos = input->find ( "$" );
  input->replace ( pos, 1, replace );
}
