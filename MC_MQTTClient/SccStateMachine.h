
/*===================================================================
	SCC State Machine
	- ¡§Conveyor work station¡¨ State Machine
	- Transition State
===================================================================*/

//State Machine of Conveyor Work station
enum Conveyor_State{
	CState_D0U0,
	CState_D1U0,
	CState_D0U1,
	CState_D1U1,
	CState_DxU0,
	CState_DxU1
};

enum Conveyor_State_Rule0{
	CState_R0_D0U0,
	CState_R0_D1U0
};

enum Conveyor_State_RuleB{
	CState_RB_D0U0,
	CState_RB_D1U0,
	CState_RB_D0U1,
	CState_RB_DxU0
};

enum Conveyor_State_RuleA{
	CState_RA_D0U0,
	CState_RA_D1U0,
	CState_RA_D0U1,
	CState_RA_D1U1,
	CState_RA_DxU0,
	CState_RA_DxU1
};


//Transition State of Conveyor Work station
enum Transition_State{
	CT1_D0U0toD1U0,
	CT2_D1U0toD0U0,
	CT3_D1U0toD0U1,
	CT4_D0U1toD1U1,
	CT5_D1U1toD0U1,
	CT6_D0U1toDxU1,
	CT7_DxU1toD0U1,
	CT8_D0U1toDxU0,
	CT9_DxU0toD0U0
};

//Rule 0/B/Ai/Am/Ao/Aq of Conveyor Work station 
enum Conveyor_Rule{
	CR_0,
	CR_B,
	CR_Ai,
	CR_Am,
	CR_Ao,
	CR_Aq
};

//Rule R0/1/2/3/4 + R0x/1x/2x/3x/4x of Conveyor Reverse station

//Rule ... of Elevator Work station

