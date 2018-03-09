#ifndef OUT
#define OUT out
#endif
{
OUT << u8"Hello world!\n";
 for (int i = 0; i < 5; ++i) { ;
OUT << u8"\n  Iteration ";
OUT << ( i );
OUT << u8"\n";
 } ;
OUT << u8"\n";
}
#undef OUT
