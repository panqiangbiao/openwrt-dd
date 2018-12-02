#include <msdCopyright.h>

/********************************************************************************
* Pearl_msdBrgFdb.c
*
* DESCRIPTION:
*       API definitions for Multiple Forwarding Databases 
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*******************************************************************************/

#include <pearl/include/api/Pearl_msdBrgFdb.h>
#include <pearl/include/api/Pearl_msdApiInternal.h>

/*******************************************************************************
* gfdbAddMacEntry
*
* DESCRIPTION:
*       Creates the new entry in MAC address table.
*
* INPUTS:
*       macEntry    - mac address entry to insert to the ATU.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK   - on success
*       MSD_FAIL - on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       DBNum in macEntry -
*            ATU MAC Address Database number. If multiple address 
*            databases are not being used, DBNum should be zero.
*            If multiple address databases are being used, this value
*            should be set to the desired address database number.
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbAddMacEntryIntf
(
    IN MSD_QD_DEV    *dev,
    IN MSD_ATU_ENTRY *macEntry
)
{
    PEARL_MSD_ATU_ENTRY entry;

	if (NULL == macEntry)
	{
		MSD_DBG_ERROR(("Input param MSD_ATU_ENTRY in Pearl_gfdbAddMacEntryIntf is NULL. \n"));
		return MSD_BAD_PARAM;
	}

    entry.DBNum = macEntry->fid;
    entry.portVec = macEntry->portVec;
    entry.entryState = macEntry->entryState;
    entry.exPrio.macFPri = macEntry->exPrio.macFPri;
    entry.trunkMember = macEntry->trunkMemberOrLAG;

    msdMemCpy(entry.macAddr.arEther, macEntry->macAddr.arEther, 6);

    return Pearl_gfdbAddMacEntry(dev, &entry);
}

/*******************************************************************************
* Pearl_gfdbFlush
*
* DESCRIPTION:
*       This routine flush all or unblocked addresses from the MAC Address
*       Table.
*
* INPUTS:
*       flushCmd - the flush operation type.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL- on error
*
* COMMENTS:
*		none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbFlushIntf
(
IN MSD_QD_DEV *dev,
IN MSD_FLUSH_CMD flushCmd
)
{
	return Pearl_gfdbFlush(dev, (PEARL_MSD_FLUSH_CMD)flushCmd);
}

/*******************************************************************************
* Pearl_gfdbFlushInDB
*
* DESCRIPTION:
*       This routine flush all or unblocked addresses from the particular
*       ATU Database (DBNum). If multiple address databases are being used, this
*       API can be used to flush entries in a particular DBNum database.
*
* INPUTS:
*       flushCmd  - the flush operation type.
*       DBNum     - ATU MAC Address Database Number.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL- on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbFlushInDBIntf
(
IN MSD_QD_DEV *dev,
IN MSD_FLUSH_CMD flushCmd,
IN MSD_U32 DBNum
)
{
	return Pearl_gfdbFlushInDB(dev, (PEARL_MSD_FLUSH_CMD)flushCmd, DBNum);
}
/*******************************************************************************
* Pearl_gfdbMove
*
* DESCRIPTION:
*        This routine moves all or unblocked addresses from a port to another.
*
* INPUTS:
*        moveCmd  - the move operation type.
*        moveFrom - port where moving from
*        moveTo   - port where moving to
*
* OUTPUTS:
*        None
*
* RETURNS:
*        MSD_OK  - on success
*        MSD_FAIL- on error
*        MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbMoveIntf
(
IN MSD_QD_DEV     *dev,
IN MSD_MOVE_CMD    moveCmd,
IN MSD_U32        moveFrom,
IN MSD_U32        moveTo
)
{
	return Pearl_gfdbMove(dev, (PEARL_MSD_MOVE_CMD)moveCmd, moveFrom, moveTo);
}

/*******************************************************************************
* Pearl_gfdbMoveInDB
*
* DESCRIPTION:
*        This routine move all or unblocked addresses which are in the particular
*        ATU Database (DBNum) from a port to another.
*
* INPUTS:
*        moveCmd  - the move operation type.
*        DBNum         - ATU MAC Address Database Number.
*        moveFrom - port where moving from
*        moveTo   - port where moving to
*
* OUTPUTS:
*     None
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL- on error
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*		 none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbMoveInDBIntf
(
IN MSD_QD_DEV	*dev,
IN MSD_MOVE_CMD	moveCmd,
IN MSD_U32       DBNum,
IN MSD_U32       moveFrom,
IN MSD_U32       moveTo
)
{
	return Pearl_gfdbMoveInDB(dev, (PEARL_MSD_MOVE_CMD)moveCmd, DBNum, moveFrom, moveTo);
}

/*******************************************************************************
* Pearl_gfdbPortRemoveIntf
*
* DESCRIPTION:
*        This routine remove all or all non-static addresses from a port.
*
* INPUTS:
*        moveCmd  - the move operation type.
*        portNum  - logical port number
*
* OUTPUTS:
*        None
*
* RETURNS:
*        MSD_OK  - on success
*        MSD_FAIL- on error
*        MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbPortRemoveIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_MOVE_CMD  moveCmd,
IN  MSD_LPORT portNum
)
{
    PEARL_MSD_MOVE_CMD pmoveCmd;

    switch (moveCmd)
    {
    case MSD_MOVE_ALL:
        pmoveCmd = PEARL_MSD_MOVE_ALL;
        break;
    case MSD_MOVE_ALL_NONSTATIC:
        pmoveCmd = PEARL_MSD_MOVE_ALL_NONSTATIC;
        break;
    default:
        return MSD_BAD_PARAM;
    }

    return Pearl_gfdbMove(dev, pmoveCmd, portNum, 0xF);
}

/*******************************************************************************
* Pearl_gfdbPortRemoveInDBIntf
*
* DESCRIPTION:
*        This routine remove all or all non-static addresses from a port in the
*        specified ATU DBNum.
*
* INPUTS:
*        moveCmd  - the move operation type.
*        portNum  - logical port number
*        DBNum    - fid
*
* OUTPUTS:
*        None
*
* RETURNS:
*        MSD_OK  - on success
*        MSD_FAIL- on error
*        MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*        none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbPortRemoveInDBIntf
(
IN  MSD_QD_DEV  *dev,
IN  MSD_MOVE_CMD  moveCmd,
IN  MSD_U32  DBNum,
IN  MSD_LPORT  portNum
)
{
    PEARL_MSD_MOVE_CMD pmoveCmd;

    switch (moveCmd)
    {
    case MSD_MOVE_ALL:
        pmoveCmd = PEARL_MSD_MOVE_ALL;
        break;
    case MSD_MOVE_ALL_NONSTATIC:
        pmoveCmd = PEARL_MSD_MOVE_ALL_NONSTATIC;
        break;
    default:
        return MSD_BAD_PARAM;
    }

    return Pearl_gfdbMoveInDB(dev, pmoveCmd, DBNum, portNum, 0xF);
}

/*******************************************************************************
* Pearl_gfdbGetAtuEntryNext
*
* DESCRIPTION:
*       Gets next lexicographic MAC address from the specified Mac Addr.
*
* INPUTS:
*       atuEntry - the Mac Address to start the search.
*
* OUTPUTS:
*       atuEntry - match Address translate unit entry.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_NO_SUCH - no more entries.
*       MSD_BAD_PARAM - if invalid parameter is given
*
* COMMENTS:
*       Search starts from atu.macAddr[xx:xx:xx:xx:xx:xx] specified by the
*       user.
*
*       DBNum in atuEntry -
*            ATU MAC Address Database number. If multiple address
*            databases are not being used, DBNum should be zero.
*            If multiple address databases are being used, this value
*            should be set to the desired address database number.
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbGetAtuEntryNextIntf
(
IN MSD_QD_DEV *dev,
INOUT MSD_ATU_ENTRY  *atuEntry
)
{
	PEARL_MSD_ATU_ENTRY entry;
	MSD_STATUS       retVal;

	if (NULL == atuEntry)
	{
		MSD_DBG_ERROR(("Input param MSD_ATU_ENTRY in Pearl_gfdbGetAtuEntryNextIntf is NULL. \n"));
		return MSD_BAD_PARAM;
	}

	entry.DBNum = atuEntry->fid;
	entry.portVec = atuEntry->portVec;
	entry.entryState = atuEntry->entryState;
	entry.exPrio.macFPri = atuEntry->exPrio.macFPri;
	entry.trunkMember = atuEntry->trunkMemberOrLAG;

	msdMemCpy(entry.macAddr.arEther, atuEntry->macAddr.arEther, 6);

	retVal = Pearl_gfdbGetAtuEntryNext(dev, &entry);
	if (MSD_OK != retVal)
		return retVal;

	msdMemSet((void*)atuEntry, 0, sizeof(MSD_ATU_ENTRY));
	atuEntry->fid = entry.DBNum;
	atuEntry->portVec = entry.portVec;
	atuEntry->entryState = entry.entryState;
	atuEntry->exPrio.macFPri = entry.exPrio.macFPri;
	atuEntry->trunkMemberOrLAG = entry.trunkMember;
	msdMemCpy(atuEntry->macAddr.arEther, entry.macAddr.arEther, 6);
	return retVal;
}

/*******************************************************************************
* Pearl_gfdbGetViolation
*
* DESCRIPTION:
*       Get ATU Violation data
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       atuIntStatus - interrupt cause, source portID, and vid.
*
* RETURNS:
*       MSD_OK  - on success
*       MSD_FAIL- on error
*
* COMMENTS:
*       none
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbGetViolationIntf
(
IN  MSD_QD_DEV         *dev,
OUT MSD_ATU_INT_STATUS *atuIntStatus
)
{
	PEARL_MSD_ATU_INT_STATUS atuInt;
	MSD_STATUS       retVal;

	msdMemSet((void*)(&atuInt), 0, sizeof(PEARL_MSD_ATU_INT_STATUS));
	retVal = Pearl_gfdbGetViolation(dev, &atuInt);
	if (MSD_OK != retVal)
		return retVal;

	if (NULL == atuIntStatus)
	{
		MSD_DBG_ERROR(("Input param MSD_ATU_INT_STATUS in Pearl_gfdbGetViolationIntf is NULL. \n"));
		return MSD_BAD_PARAM;
	}
	msdMemSet((void*)atuIntStatus, 0, sizeof(MSD_ATU_INT_STATUS));
    atuIntStatus->atuIntCause.ageOutVio = ((atuInt.atuIntCause & PEARL_MSD_ATU_AGE_OUT_VIOLATION) == 0) ? MSD_FALSE : MSD_TRUE;
    atuIntStatus->atuIntCause.memberVio = ((atuInt.atuIntCause & PEARL_MSD_ATU_MEMBER_VIOLATION) == 0) ? MSD_FALSE : MSD_TRUE;
    atuIntStatus->atuIntCause.missVio = ((atuInt.atuIntCause & PEARL_MSD_ATU_MISS_VIOLATION) == 0) ? MSD_FALSE : MSD_TRUE;
    atuIntStatus->atuIntCause.fullVio = ((atuInt.atuIntCause & PEARL_MSD_ATU_FULL_VIOLATION) == 0) ? MSD_FALSE : MSD_TRUE;

	atuIntStatus->fid = atuInt.dbNum;
	atuIntStatus->spid = atuInt.spid;
	msdMemCpy(atuIntStatus->macAddr.arEther, atuInt.macAddr.arEther, 6);
	return retVal;
}

/*******************************************************************************
* Pearl_gfdbFindAtuMacEntry
*
* DESCRIPTION:
*       Find FDB entry for specific MAC address from the ATU.
*
* INPUTS:
*       macAddr - the Mac address to search.
*       fid     - DBNum used to search
*
* OUTPUTS:
*       found    - MSD_TRUE, if the appropriate entry exists.
*       atuEntry - the entry parameters.
*
* RETURNS:
*       MSD_OK      - on success.
*       MSD_FAIL    - on error.
*       MSD_BAD_PARAM  - on bad parameter
*
* COMMENTS:
*        DBNum in atuEntry -
*            ATU MAC Address Database number. If multiple address
*            databases are not being used, DBNum should be zero.
*            If multiple address databases are being used, this value
*            should be set to the desired address database number.
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbFindAtuMacEntryIntf
(
    IN  MSD_QD_DEV    *dev,
    IN  MSD_ETHERADDR *macAddr,
    IN  MSD_U32       fid,
    OUT MSD_ATU_ENTRY *atuEntry,
    OUT MSD_BOOL      *found
)
{
	PEARL_MSD_ATU_ENTRY entry;
	MSD_STATUS       retVal;

    if (NULL == macAddr)
    {
        MSD_DBG_ERROR(("Input param MSD_ETHERADDR in Pearl_gfdbFindAtuMacEntryIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

    msdMemSet((void*)(&entry), 0, sizeof(PEARL_MSD_ATU_ENTRY));
    entry.DBNum = (MSD_U16)fid;
    msdMemCpy(entry.macAddr.arEther, macAddr, 6);

	retVal = Pearl_gfdbFindAtuMacEntry(dev, &entry, found);
    if (MSD_OK != retVal || MSD_FALSE == *found)
        return retVal;

    if (NULL == atuEntry)
    {
        MSD_DBG_ERROR(("Input param MSD_ATU_ENTRY in Pearl_gfdbFindAtuMacEntryIntf is NULL. \n"));
        return MSD_BAD_PARAM;
    }

	msdMemSet((void*)atuEntry, 0, sizeof(MSD_ATU_ENTRY));
	atuEntry->fid = entry.DBNum;
	atuEntry->portVec = entry.portVec;
	atuEntry->entryState = entry.entryState;
	atuEntry->exPrio.macFPri = entry.exPrio.macFPri;
	atuEntry->trunkMemberOrLAG = entry.trunkMember;
	msdMemCpy(atuEntry->macAddr.arEther, entry.macAddr.arEther, 6);
	return retVal;
}

/*******************************************************************************
* gfdbDump
*
* DESCRIPTION:
*       Find all MAC address in the specified fid and print it out.
*
* INPUTS:
*       fid    - ATU MAC Address Database Number.
*
* OUTPUTS:
*       None
*
* RETURNS:
*       MSD_OK      - on success
*       MSD_FAIL    - on error
*       MSD_NOT_SUPPORTED - if current device does not support this feature.
*
* COMMENTS:
*       None
*
*******************************************************************************/
MSD_STATUS Pearl_gfdbDump
(
IN MSD_QD_DEV    *dev,
IN MSD_U32       fid
)
{
    MSD_STATUS status;
    MSD_ATU_ENTRY entry;
    int temp;

    msdMemSet(&entry, 0, sizeof(MSD_ATU_ENTRY));

    entry.macAddr.arEther[0] = 0xff;
    entry.macAddr.arEther[1] = 0xff;
    entry.macAddr.arEther[2] = 0xff;
    entry.macAddr.arEther[3] = 0xff;
    entry.macAddr.arEther[4] = 0xff;
    entry.macAddr.arEther[5] = 0xff;
    entry.fid = (MSD_U16)fid;

    /* Print out ATU entry field */
    MSG(("\n----------------------------------------------------\n"));
    MSG(("FID  MAC           LAG  PORTVEC  STATE  FPRI  QPRI\n"));
    MSG(("----------------------------------------------------\n"));

    temp = 1;
    while (1 == temp)
    {
        status = Pearl_gfdbGetAtuEntryNextIntf(dev, &entry);
        if (MSD_NO_SUCH == status)
            break;

        if (MSD_OK != status)
        {
            return status;
        }

        MSG(("%-5x%02x%02x%02x%02x%02x%02x  %-5x%x%x%x%x%x%x%x  %-7x%-6x%-4x\n", entry.fid, entry.macAddr.arEther[0], entry.macAddr.arEther[1], entry.macAddr.arEther[2], 
                                                        entry.macAddr.arEther[3], entry.macAddr.arEther[4], entry.macAddr.arEther[5], entry.trunkMemberOrLAG, 
                                                       ((entry.portVec & 0x40) ? 1 : 0), ((entry.portVec & 0x20) ? 1 : 0),((entry.portVec & 0x10) ? 1 : 0), 
                                                       ((entry.portVec & 0x8) ? 1 : 0), ((entry.portVec & 0x4) ? 1 : 0),
                                                        ((entry.portVec & 0x2) ? 1 : 0), ((entry.portVec & 0x1) ? 1 : 0), 
                                                        entry.entryState, entry.exPrio.macFPri, entry.exPrio.macQPri));

        if (PEARL_IS_BROADCAST_MAC(entry.macAddr))
        {
            break;
        }

    }

    MSG(("\n"));
    return MSD_OK;
}