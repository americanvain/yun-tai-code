#ifndef RAMMER_CONTROL_H
#define RAMMER_CONTROL_H

#define NONE_SHOOT 1
#define SINGLE_SHOOT 3
#define SERIES_SHOOT 2



#define REVOL_SPEED_RATIO   2160       //�����һ��תһȦ,2160ת��ת��,60*36,����Ƶ�ٳ��Բ��̸����Ϳɵ���Ӧ��Ƶ�µ�ת��

/*       �����ͺ�Ԥ����       */
#define SEVEN_REVOLVER    0    	//7����
#define EIGHT_REVOLVER    1		//8����
#define TEN_REVOLVER      2		//10����
#define TWELVE_REVOLVER   3		//12����

#define REVOLVER_CHOOSE  EIGHT_REVOLVER	//ѡ�����ͺ�

#if REVOLVER_CHOOSE == EIGHT_REVOLVER			
	#define		REVOL_SPEED_GRID      8				//���̸���
	#define 	AN_BULLET         (36864.0f)		//�����ӵ����λ������ֵ

#elif REVOLVER_CHOOSE == SEVEN_REVOLVER
	#define 	REVOL_SPEED_GRID      7				//���̸���
	#define   	AN_BULLET     	  (42130.2857f)		//�����ӵ����λ������ֵ

#elif REVOLVER_CHOOSE == TEN_REVOLVER
	#define 	REVOL_SPEED_GRID  	  10			//���̸���
	#define    	AN_BULLET     	  (29491.2f)		//�����ӵ����λ������ֵ

#elif REVOLVER_CHOOSE == TWELVE_REVOLVER
	#define 	REVOL_SPEED_GRID      12			//���̸���
	#define    	AN_BULLET         (24576.0f)		//�����ӵ����λ������ֵ

#endif	


void RammerControlLoop(char shoot_state);

#endif
