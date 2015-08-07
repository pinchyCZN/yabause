#define CHAR_BIT 8
unsigned short _byteswap_ushort(unsigned short value) 
{ 
    return (value >> CHAR_BIT) | (value << CHAR_BIT); 
}
unsigned long _byteswap_ulong(unsigned long value) 
{ 
    return _byteswap_ushort((unsigned short)value) << (2 * CHAR_BIT) | 
           _byteswap_ushort((unsigned short)(value >> (2 * CHAR_BIT))); 
} 