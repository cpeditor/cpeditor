#if 1
int x; // variable shall not be grey
#endif
#if defined (A)
int y; // variable shall not be grey
#elif defined (B)
int z; // variable shall not be grey
#endif
