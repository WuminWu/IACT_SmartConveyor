#ifndef MC_CMD_FORMAT_H
#define MC_CMD_FORMAT_H

/*============================================Command Format============================================*/
/* ======Mode===== */
#define SHIFT_MODE 31
#define BIT_MASK_MODE (0x01<<SHIFT_MODE)
#define VALUE_MODE_DIAG 1
#define VALUE_MODE_NORMAL 0

/* ======Sequence ID===== */
#define SHIFT_SEQUENCE_ID 23
#define BIT_MASK_SEQUENCE_ID (0xFF << SHIFT_SEQUENCE_ID)

/* ======Control Type===== */
#define SHIFT_ControlType 20
#define BIT_MASK_ControlType (0x07 << SHIFT_ControlType)
#define VALUE_ELEV_STATION1_LOGIC_THREAD 0x03
#define VALUE_ELEV_KEY_SENSOR 0x00

/* ======Control Type & Action===== */
#define SHIFT_ControlType_Action 16
#define BIT_MASK_ControlType_Action (0x7F << SHIFT_ControlType_Action)
#define VALUE_CONVEYOR_MOVE_FORWARD 0x11
#define VALUE_CONVEYOR_MOVE_REVERSE 0x12
#define VALUE_CONVEYOR_MOVE_STOP 0x13
#define VALUE_CONVEYOR_MOVE_BOT_FORWARD 0x41
#define VALUE_CONVEYOR_MOVE_BOT_REVERSE 0x42
#define VALUE_CONVEYOR_MOVE_BOT_STOP 0x43
#define VALUE_CYLINDER_UP 0x21
#define VALUE_CYLINDER_DOWN 0x22
#define VALUE_CYLINDER_STOP 0x23
#define VALUE_READ_CONVEYOR_SENSOR 0x01

#define VALUE_ELEV_UP 0x37
#define VALUE_ELEV_DOWN 0x38
#define VALUE_ELEV_TOP_MOTOR_FORWARD 0x31
#define VALUE_ELEV_TOP_MOTOR_REVERSE 0x32
#define VALUE_ELEV_TOP_MOTOR_STOP 0x33
#define VALUE_ELEV_BOT_MOTOR_FORWARD 0x34
#define VALUE_ELEV_BOT_MOTOR_REVERSE 0x35
#define VALUE_ELEV_BOT_MOTOR_STOP 0x36
#define VALUE_READ_ELEVATOR_SENSOR 0x02

/* ======Pallet Position===== */
#define SHIFT_PALLET_POSITION 12
#define BIT_MASK_PALLET_POSITION (0x0F << SHIFT_PALLET_POSITION)
#define VALUE_POSITION1 8
#define VALUE_POSITION2 4
#define VALUE_POSITION3 2
#define VALUE_POSITION4 1
#define VALUE_POSITION5 0
#define VALUE_SENSOR1 1
#define VALUE_SENSOR2 2
#define VALUE_SENSOR3 3
#define VALUE_SENSOR4 4
#define VALUE_SENSOR5 5
#define VALUE_SENSOR6 6
#define VALUE_SENSOR7 7
#define VALUE_SENSOR8 8
#define VALUE_SENSOR9 9
#define VALUE_SENSOR10 10
#define VALUE_SENSOR11 11
#define VALUE_SENSOR12 12
#define VALUE_SENSOR13 13
#define VALUE_SENSOR14 14

/* ======Cross Flag===== */
#define SHIFT_CROSS_FLAG 9
#define BIT_MASK_CROSS_FLAG (0x01 << SHIFT_CROSS_FLAG)
#define VALUE_CROSS_FLAG_TRUE 1
#define VALUE_CROSS_FLAG_FALSE 0

/* ======State Machine Before===== */
#define SHIFT_STATE_MACHINE_BEFORE 5
#define BIT_MASK_STATE_MACHINE_BEFORE (0x0F << SHIFT_STATE_MACHINE_BEFORE)

/* ======State Machine After===== */
#define SHIFT_STATE_MACHINE_AFTER 1
#define BIT_MASK_STATE_MACHINE_AFTER (0x0F << SHIFT_STATE_MACHINE_AFTER)

/*============================================Event Format============================================*/

/* ======Module ID===== */
#ifdef CONVEYOR_MODULE
	#define MUDULE_ID 2
#endif
#ifdef ELEVATOR_MODULE
	#define MUDULE_ID 1
#endif

typedef enum
{
	SHIFT_NA = 0,
	SHIFT_POSITION = 3,
	SHIFT_ACTION_DONE = 7,
	SHIFT_EVENT_TYPE = 11,
	SHIFT_SID = 15,
	SHIFT_MODULE_ID = 23,
	SHIFT_MODULE_TYPE = 31,
}ConvEventPayload32Offset;

typedef enum
{
	VALUE_MODULE_CONVEYOR_TYPE = 0,
	VALUE_MODULE_ELEVATOR_TYPE = 1,
}EModuleType;

// Uncomplete
typedef enum
{//4 bit
	EVT_ACTION_DONE = 1,
	EVT_KEY = 2,
	EVT_BUZZER = 3,
	EVT_CYLINDER = 4,
	EVT_EVENTTYPE_ERROR = 15,
}EventType;

typedef enum
{//4 bits
	ACTION_MOVE_FORWARD_DONE = 1,
	ACTION_MOVE_REVERSE_DONE = 2,
	ACTION_CYLINDER_UP_DONE  = 3,
	ACTION_CYLINDER_DOWN_DONE = 4,
	ACTION_MOVE_BOT_STOP_DONE = 5,
	ACTION_ACTIONTYPE_ERROR = 15,
}EventConveyorActionType;

typedef enum
{//4 bits
	ACTION_TOP_MOVE_FORWARD_DONE = 1,
	ACTION_TOP_MOVE_REVERSE_DONE = 2,
	ACTION_BOT_MOVE_FORWARD_DONE = 3,
	ACTION_BOT_MOVE_REVERSE_DONE = 4,
	ACTION_ELEVATOR_UP_DONE  = 5,
	ACTION_ELEVATOR_DOWN_DONE = 6,
}EventElevatorActionType;

#endif