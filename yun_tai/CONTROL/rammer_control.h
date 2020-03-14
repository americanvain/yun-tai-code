#ifndef RAMMER_CONTROL_H
#define RAMMER_CONTROL_H

#define NONE_SHOOT 1
#define SINGLE_SHOOT 3
#define SERIES_SHOOT 2



#define REVOL_SPEED_RATIO   2160       //电机轴一秒转一圈,2160转子转速,60*36,乘射频再除以拨盘格数就可得相应射频下的转速

/*       拨盘型号预编译       */
#define SEVEN_REVOLVER    0    	//7格拨盘
#define EIGHT_REVOLVER    1		//8格拨盘
#define TEN_REVOLVER      2		//10格拨盘
#define TWELVE_REVOLVER   3		//12格拨盘

#define REVOLVER_CHOOSE  EIGHT_REVOLVER	//选择拨盘型号

#if REVOLVER_CHOOSE == EIGHT_REVOLVER			
	#define		REVOL_SPEED_GRID      8				//拨盘格数
	#define 	AN_BULLET         (36864.0f)		//单个子弹电机位置增加值

#elif REVOLVER_CHOOSE == SEVEN_REVOLVER
	#define 	REVOL_SPEED_GRID      7				//拨盘格数
	#define   	AN_BULLET     	  (42130.2857f)		//单个子弹电机位置增加值

#elif REVOLVER_CHOOSE == TEN_REVOLVER
	#define 	REVOL_SPEED_GRID  	  10			//拨盘格数
	#define    	AN_BULLET     	  (29491.2f)		//单个子弹电机位置增加值

#elif REVOLVER_CHOOSE == TWELVE_REVOLVER
	#define 	REVOL_SPEED_GRID      12			//拨盘格数
	#define    	AN_BULLET         (24576.0f)		//单个子弹电机位置增加值

#endif	


void RammerControlLoop(char shoot_state);

#endif
