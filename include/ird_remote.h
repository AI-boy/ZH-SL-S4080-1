 #ifndef _IRD_REMOTE_H
 #define _IRD_REMOTE_H

//-----------------------------------------------------------------
//��������
extern float charge_r_value;	   //������������������ֵ **��ŷķ

extern float discharge_r_value;		//�ŵ������������

//��������
extern float p_v;    //��ǰ�����ѹ

extern float b_v;	//��ǰ��ص�ѹ

extern float load_v;	//��ǰ���ص�ѹ

extern float charge_I ; //��ǰ������

extern float load_I ;    //��ǰ���ص���

extern unsigned char  sys_mode;     //��ǰ�豸����ģʽ

extern unsigned char  today_index; //EEPROM�� �����ص�ѹ��¼��������λ��

extern float bt_00_hv;		//0��ǰ��ص�ѹ���ֵ (������)
extern float bt_00_lv;		//0��ǰ��ص�ѹ���ֵ (������)

extern float bt_01_hv;		//1��ǰ��ص�ѹ���ֵ
extern float bt_01_lv;		//1��ǰ��ص�ѹ���ֵ

extern float bt_02_hv;		//2��ǰ��ص�ѹ���ֵ
extern float bt_02_lv;		//2��ǰ��ص�ѹ���ֵ

extern float bt_03_hv;		//3��ǰ��ص�ѹ���ֵ
extern float bt_03_lv;		//3��ǰ��ص�ѹ���ֵ

extern float bt_04_hv;		//4��ǰ��ص�ѹ���ֵ
extern float bt_04_lv;		//4��ǰ��ص�ѹ���ֵ

extern float bt_05_hv;		//5��ǰ��ص�ѹ���ֵ
extern float bt_05_lv;		//5��ǰ��ص�ѹ���ֵ

extern float bt_06_hv;		//6��ǰ��ص�ѹ���ֵ
extern float bt_06_lv;		//6��ǰ��ص�ѹ���ֵ

extern float bt_07_hv;		//7��ǰ��ص�ѹ���ֵ
extern float bt_07_lv;		//7��ǰ��ص�ѹ���ֵ

//��������

extern float light_pv;			//��ص�ѹ   �趨��Χ:5--10V(500--1000)

extern float load_cc_I;			//LED�ƹ�������  �����趨ֵ:0.33--1.2A(33--1200)

extern float load_percent;  	//��������ٷֱ� 50%

extern float step_01_load_percent;  		//��������ٷֱ� 100%

extern float step_02_load_percent;  		//��������ٷֱ� 80%

extern float step_03_load_percent;  		//��������ٷֱ� 60%

extern float step_04_load_percent;  		//��������ٷֱ� 40%

extern float step_05_load_percent;  		//��������ٷֱ� 20%

extern unsigned int  step_01_time_limit;		//���ʱ�� 1 h

extern unsigned int  step_02_time_limit;		//���ʱ�� 3 h

extern unsigned int  step_03_time_limit;		//���ʱ�� 4 h

extern unsigned int  step_04_time_limit;		//���ʱ�� 3 h

extern unsigned int  step_05_time_limit;		//���ʱ�� 1 h

extern unsigned char volt_id;				//��ص�ѹ����

extern unsigned char bt_id; 				//�������

extern unsigned int light_time;			//���ʱ�� (��λ��min)

extern float bt_volt_100 ; 			//100%����ʱ

extern float bt_volt_80  ; 			//80%����

extern float bt_volt_60  ; 			//60%����

extern float bt_volt_40  ; 			//40%����

extern float bt_volt_20  ; 			//20%����

extern float bt_volt_00  ; 			//0%����


float dianchi_guoya_baohu;   	//100%  ��ع�ѹ������

float dianchi_guoya_huifu;		//95%   ��ع�ѹ�ָ���

float dianchi_qianya_baohu;		//0%	���Ƿѹ������

float dianchi_qianya_huifu;		//20%	���Ƿѹ�ָ���

float dianchi_cv_level;			//>12.40ʱ �����ѹ��緽ʽ

float charge_i_limit;			//�������޶�ֵ

float shuchu_shortout_voltage; 	//�����·��ѹ

float shuchu_open_voltage;		//�����·��ѹ

float discharge_i_baohu;		//�ŵ����������  ��������  2.4A

float discharge_i_limit;		//�ŵ�����޶�ֵ 2.0A

float light_on_pv_level;		//���ƹ����ѹ����ֵ Ĭ��6V

float light_off_pv_level;		//�صƹ����ѹ����ֵ Ĭ��8V


float Load_Pmax;
float Load2_Pmax;	



//----------------------------------------------
extern unsigned char  ird_status;

extern  unsigned char   receive_bit_count;

extern unsigned char   ird_receive_flag;

extern unsigned char   ird_receive_time_count;

extern  unsigned long   ird_data;

extern unsigned char light_mode;

extern unsigned char light2_mode;

extern unsigned char lr_cnt;

extern unsigned char led_out_ctrl;







#define  ZH_MAC                         0x10ED

#define  HAN_MODE                       0x00
        
#define  STA_MODE                       0x40        
        
#define  IRD_MODE                      	0x10
        
#define  AUT_MODE                       0x50

//#define  ONF_MODE						0x08
#define  ONF_MODE						0x88


//------------------------------------------------
extern void auto_system(void);

extern void read_param_eeprom(void);

extern void Init_Ird(void);

extern void Key_Menu(unsigned long key_value);



#endif
