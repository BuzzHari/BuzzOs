//Note: Inline C assembly, uses GCM assembly syntax.
//Here the source and destiantion are switched.
//and the registers are denoeted by '%', hence the use 
//of "%%".

unsigned char port_byte_in(unsigned short port){
    unsigned char result;
    asm("in %%dx, %%al" : "=a"(result): "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data){
    asm("out %%al, %%dx": : "a"(data), "d"(port));
}

