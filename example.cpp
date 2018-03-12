#ifndef OUT
#define OUT out
#define TMP_OUT
#endif
{
OUT << u8"Hello world!\n";
 for (int i = 0; i < 5; ++i) { 
;OUT << u8"\n  Iteration ";
OUT << ( i );
OUT << u8"\n";
 } 
;OUT << u8"\n";
}
#ifdef TMP_OUT
#undef OUT
#undef TMP_OUT
#endif
