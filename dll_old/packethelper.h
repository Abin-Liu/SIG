// packethelper.h ackmed@gotwalls.com

// functions
BOOL TakePortal(DWORD wpEntityID);
BOOL TakeWayPoint(DWORD wpEntityID, DWORD Destination);
BOOL InteractWithEntity(DWORD type,DWORD EntityID);
BOOL StopInteractWithEntity(DWORD type,DWORD EntityID);
BOOL TakeStep(WORD x, WORD y);
BOOL OpenStore(DWORD npcID);
BOOL BuyItem(DWORD itemID, DWORD npcID);
BOOL CloseStore(DWORD npcID);
BOOL ForceClientViewLocation(DWORD dwPlayerID, WORD x, WORD y, BYTE type);
BOOL SendSoundToServer(WORD sndID);
BOOL SendSoundToClient(DWORD pID, WORD sndID);

// Added by Abin (2002-7-10)

void PrintPacket(BYTE* aPacket, int aLen, int SR = 0);
BOOL WritePacket(const BYTE* aPacket, DWORD aLen, int SR, LPCTSTR lpFile = NULL);

BOOL StepToEntity(DWORD type, DWORD EntityID);
BOOL ForceClientViewWalk(DWORD dwPlayerID, WORD x, WORD y);

BOOL SelectSkill(WORD SkillID, BOOL bRight);
BOOL CastOnMap(WORD x, WORD y, WORD SkillID, BOOL bRight);
BOOL CastOnEntity(DWORD dwPlayerID, DWORD EntityID, WORD SkillID, BOOL bRight);

BOOL RepairAll(DWORD NpcID);
BOOL OverHeadMsg(LPCTSTR lpMsg);
BOOL OverHeadMsgFake(DWORD dwPlayerID, LPCSTR lpMsg);
BOOL AntiIdle(int nAntiIdleMode);
BOOL PickCorpose();
BOOL IsInGame();
BOOL ResurrectMerc(DWORD npcID);

BOOL ForceClientCastOnMap(DWORD dwPlayerID, WORD wSkillID, WORD x, WORD y);
BOOL ForceClientCastOnEntity(DWORD dwPlayerID, WORD wSkillID, DWORD dwEntityID);

// check if character position is within a predefined range
BOOL CheckPosition(WORD x, WORD y, WORD offset = 0);

// move based on current position, used for anti-detect
void DepositGold(DWORD dwAmount);

BOOL Say(WORD wPlayerID, LPCSTR lpMsg);


