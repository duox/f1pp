#define shuffle1( a, b, ... )  b,__VA_ARGS__##,a 
#define shuffle2( a, b, ... )  b,## __VA_ARGS__,a 
#define shuffle3( a, b, ... )  b,## __VA_ARGS__##,a 
#define showlist( ... )        #__VA_ARGS__ 
#define args( f, ... )         __VA_ARGS__ 

shuffle1(x,y,z)          //y,z,x 
shuffle1(x,y)            //y,x      // second ',' dissappears 
shuffle1(a,b,c,d,e)      //b,c,d,e,a 
---
shuffle2(x,y,z)          //y,z,x 
shuffle2(x,y)            //y,x      // second ',' dissappears 
shuffle2(a,b,c,d,e)      //b,c,d,e,a 
---
shuffle3(x,y,z)          //y,z,x 
shuffle3(x,y)            //yx      // all ',' dissappear
shuffle3(a,b,c,d,e)      //b,c,d,e,a 
=======
showlist(x,y,z)         //"x,y,z" 
args("%s",charptr)      //charptr 
args("%d+%d=%d",a,b,c)  //a,b,c 
args("none")			//--none--

