#ifndef _LIBSNIFER_H_
#define _LIBSNIFER_H_
    

    /*Define estructuras de sockets origen y destino*/
     struct sockaddr_in source,dest;

    /*Varibles contadores de paquetes cachados*/
     int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;


    
    
    extern int inicia();
    extern void ProcessPacket(unsigned char* buffer, int size);
    extern void print_ethernet_header(unsigned char* Buffer, int Size);
    extern void print_ip_header(unsigned char* Buffer, int Size);
    extern void print_tcp_packet(unsigned char* Buffer, int Size);
    extern void print_udp_packet(unsigned char *Buffer , int Size);
    extern void print_icmp_packet(unsigned char* Buffer , int Size);

#endif