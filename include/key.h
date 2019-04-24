  
#ifndef _ZH_KEY_H
#define _ZH_KEY_H


//**--------------宏定义-----------------------------------------------------

#define   K1_TRIS   (TRISBbits.TRISB6)
#define   K2_TRIS   (TRISCbits.TRISC6)
#define   K3_TRIS   (TRISCbits.TRISC5)
#define   K4_TRIS   (TRISCbits.TRISC4)


#define	  K1_GET	(PORTBbits.RB6)      
#define	  K2_GET	(PORTCbits.RC6)
#define	  K3_GET	(PORTCbits.RC5)
#define	  K4_GET	(PORTCbits.RC4)


//**--------------数据结构-------------------------------------------------------


typedef enum KEY_CURRENT_STATE
       {
         KEY_UP  =0x00,
         KEY_DOWN=0x01
       } KEY_STATE_NU ;

typedef enum KEY_CURRENT_VALUE
       {
         KNOP =0x10,
         K1   =0x01,
         K2   =0x02,
         K3   =0x03,
         K4   =0x04,
       } KEY_VALUE_NU ;


//**--------------全局声明-------------------------------------------------------



extern KEY_STATE_NU  KEY_STATE;

extern KEY_VALUE_NU  KEY_VALUE;


extern unsigned char scan_key_time_count;

extern unsigned char handle_scan_key_flag;

extern unsigned char have_person1_flag;

extern unsigned char have_person1_time_count;

extern unsigned char have_person2_flag;

extern unsigned char have_person2_time_count;

extern unsigned char device_status;

extern unsigned char key1_time_count;

extern unsigned char key1_index;





//---------------函数全局声明------------------

extern void init_key(void);

extern void scan_key_value(void);




	


#endif 
