// https://codeday.me/bug/20181101/348970.html

#include <stdio.h>
#include <string.h>
#include <unistd.h>          
#include <errno.h>           
#include <sys/types.h>       
#include <sys/socket.h>      
#include <sys/ioctl.h>  
#include <sys/resource.h>    
#include <sys/utsname.h>       
#include <netdb.h>           
#include <netinet/in.h>      
#include <netinet/in_systm.h>                 
#include <netinet/ip.h>      
#include <netinet/ip_icmp.h> 
#include <assert.h>

#include <net/if_dl.h>       
#include <ifaddrs.h>         
#include <net/if_types.h>  

#include <iostream>

using namespace std;

//---------------------------------get user name ------------------------------------
const char* getMachineName() 
{ 
   static struct utsname u;  

   if ( uname( &u ) < 0 )    
   {       
      assert(0);             
      return "unknown";      
   }       

   return u.nodename;        
}   

//---------------------------------get Volume and CPU ------------------------------------

unsigned short getVolumeHash()          
{ 
   // we don't have a 'volume serial number' like on windows. Lets hash the system name instead.    
   unsigned char* sysname = (unsigned char*)getMachineName();       
   unsigned short hash = 0;             

   for ( unsigned int i = 0; sysname[i]; i++ )         
      hash += ( sysname[i] << (( i & 1 ) * 8 ));       

   return hash;              
} 

#ifdef DARWIN                
 #include <mach-o/arch.h>    
 unsigned short getCpuHash()            
 {         
     const NXArchInfo* info = NXGetLocalArchInfo();    
     unsigned short val = 0;            
     val += (unsigned short)info->cputype;               
     val += (unsigned short)info->cpusubtype;            
     return val;             
 }         

#else // !DARWIN             

 static void getCpuid( unsigned int* p, unsigned int ax )       
 {         
    __asm __volatile         
    (   "movl %%ebx, %%esi\n\t"               
        "cpuid\n\t"          
        "xchgl %%ebx, %%esi" 
        : "=a" (p[0]), "=S" (p[1]),           
          "=c" (p[2]), "=d" (p[3])            
        : "0" (ax)           
    );  
    cout<<"p is "<<*p<<endl;
    cout<<"ax is "<<ax<<endl;
 }         

 unsigned short getCpuHash()            
 {         
    unsigned int cpuinfo[4] = { 0, 0, 0, 0 };          
    getCpuid( cpuinfo, 0 );  
    unsigned short hash = 0;            
    unsigned int* ptr = (&cpuinfo[0]);                 
    for ( unsigned int i = 0; i < 4; i++ )             
       hash += (ptr[i] & 0xFFFF) + ( ptr[i] >> 16 );   

    return hash;             
 }         
#endif // !DARWIN            