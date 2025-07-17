#include "display.hpp"
#include "displaydata.hpp"







Display::Display ( ) {
  u8g2 = new U8G2_SH1106_128X64_NONAME_1_SW_I2C ( U8G2_R0, SCL, SDA );
  u8g2->begin ( );

  data = (displayData*) malloc ( 20*sizeof(std::string("testdata")));
  updatePage.setDelay ( 2000 );
  state = WELCOME;
  actualPage = SYS;
}



void Display::drawWelcome ( ) {
  u8g2->setFont ( u8g_font_profont17r );
  u8g2->drawUTF8 ( 50, 14+16, "sys_mon" );
  u8g2->setFont ( u8g_font_profont15r );
  u8g2->drawUTF8 ( 50, 14+16+14, "beta 0.1" );
  u8g2->drawXBMP( 0, 16, 50, 48, arch_bits );
}



void Display::drawWaiting ( std::string message ) {
  u8g2->drawHLine ( 0, 16, 160 );
  u8g2->setFont ( u8g_font_profont17r );
  u8g2->drawStr ( 0, 15, (const char*) FPSTR ( "Waiting:" ) );
  u8g2->setFont ( u8g_font_profont15r );
  if ( !message.empty ( ) )
    u8g2->drawStr ( 0, line[0], message.c_str( ) );
  else
    u8g2->drawStr ( 0, line[0], "no Message" );
}



void Display::drawErr ( std::string err ) {
  state = ERROR;

  u8g2->drawHLine ( 0, 16, 160 );
  u8g2->setFont ( u8g_font_profont17r );
  u8g2->drawStr ( 0, 15, (const char*) FPSTR ( "Error:" ) );
  u8g2->setFont ( u8g_font_profont15r );
  u8g2->drawStr ( 0, line[0], err.c_str( ) );

  while ( 1 ) ;
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
  case CPU:
    lineText[0].append ( displayString.t );
    sReplace ( &lineText[0], data->cpu_t );
    lineText[1].append ( displayString.c );
    sReplace ( &lineText[1], data->cpu_c );
    lineText[2].append ( displayString.u );
    sReplace ( &lineText[2], data->cpu_u );
    break;;

  case LIQUID:
    lineText[0].append ( displayString.s );
    sReplace ( &lineText[0], data->liquid_f );
    lineText[1].append ( displayString.s );
    sReplace ( &lineText[1], data->liquid_p );
    lineText[2].append ( displayString.t );
    sReplace ( &lineText[2], data->liquid_t );
    break;;

  case GPU:
    lineText[0].append ( displayString.t );
    sReplace ( &lineText[0], data->gpu_t );
    lineText[1].append ( displayString.u );
    sReplace ( &lineText[1], data->gpu_u );
    lineText[2].append ( displayString.p );
    sReplace ( &lineText[2], data->gpu_p );
    break;;

  case SYS:
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
  if ( updatePage.isUpdate ( ) ) {
    u8g2->firstPage ( );
    do {
      switch ( state ) {
        case DRAW:
          drawHeader ( );
          drawContend ( );
          break;;
        case WELCOME:
          drawWelcome ( );
          break;;
        case ERROR:
          break;;
        case WAITING:
          drawWaiting ( ) ;
          break;;
      }
    } while ( u8g2->nextPage ( ) );
    if ( state == WELCOME ) state = DRAW;
    if ( state == DRAW ) next ( );
    if ( state == WAITING ) state = DRAW;
  }
  updatePage.loop ( );
}



void Display::next ( ) {
  if ( actualPage == SYS ) actualPage = CPU;
  else actualPage = static_cast<Page> ( static_cast<int> ( actualPage ) + 1 );
}



void Display::setDisplayData ( displayData* dataN ) {
  this->data = dataN;
}



void Display::sReplace ( std::string* input, std::string replace ) {
  std::size_t pos = input->find ( "$" );
  input->reserve ( input->size ( ) + replace.size ( ) );
  input->replace ( pos, 1, replace );
}
