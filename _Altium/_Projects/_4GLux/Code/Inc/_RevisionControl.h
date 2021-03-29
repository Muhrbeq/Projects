/*
 * _RevisionControl.h
 *
 *  Created on: 25 mars 2021
 *      Author: Rasmus.Muhrbeck
 */

#ifndef REVISIONCONTROL_H_
#define REVISIONCONTROL_H_

#include "main.h"
#include "__ExegerGeneric.h"

typedef enum RevisionControl_State
{
	REVISIONSTATE_ENABLE,
	REVISIONSTATE_CALIBRATIONSTART,
	REVISIONSTATE_START,
	REVISIONSTATE_POLLCONVERSION,
	REVISIONSTATE_GETADCSTATE,
	REVISIONSTATE_CHECKREVISION,
	REVISIONSTATE_DISABLE,
	REVISIONSTATE_DONE,
	REVISIONSTATE_ERROR
}RevisionControl_State;

typedef enum RevisionControl_Revision
{
	REVISION_ERROR,
	REVISION_V1,
	REVISION_V2,
	REVISION_V3,
	REVISION_V4
} RevisionControl_Revision;

RevisionControl_Revision revisionBoard;

void RevisionControl_StateMachine(void);
RevisionControl_State RevisionControl_GetState(void);
#endif /* REVISIONCONTROL_H_ */
