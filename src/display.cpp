#include "display.hpp"



Display::Display ( ) {
    u8g2 = new U8G2_SH1106_128X64_NONAME_1_SW_I2C ( U8G2_R0, SCL, SDA );
    u8g2->begin ( );

    actualPage = 0;
}



void Display::drawHeader ( ) {
    u8g2->drawHLine ( 0, 16, 160 );
    u8g2->setFont ( u8g_font_profont17r );
    u8g2->drawStr ( 0, 15, "sensors" );
    u8g2->setFont ( u8g_font_profont15r );
    u8g2->drawStr ( 80, 15, title[actualPage] );
}



void Display::draw ( ) {
    u8g2->firstPage ( );
    do {
        drawHeader ( );
        //u8g2->setFont ( u8g_font_profont15r );
    } while ( u8g2->nextPage ( ) );
}



void Display::next ( ) {
  if (actualPage == 3) actualPage = 0;
  else actualPage++;
}
