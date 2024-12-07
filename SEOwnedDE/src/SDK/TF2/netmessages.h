#pragma once
#include "inetmessage.h"
#include "inetchannel.h"

class SendTable;
class KeyValue;
class KeyValues;
class INetMessageHandler;
class IServerMessageHandler;
class IClientMessageHandler;

#define	MAX_OSPATH 260 // max length of a filesystem pathname

// shared commands used by all streams, handled by stream layer
#define	net_NOP 				0	// nop command used for padding
#define net_Disconnect			1	// disconnect, last message in connection
#define net_File				2	// file transmission message request/deny
#define net_Tick				3	// send last world tick
#define net_StringCmd			4	// a string command
#define net_SetConVar			5	// sends one/multiple convar settings
#define	net_SignonState			6	// signals current signon state
// server to client
#define	svc_Print				7	// print text to console
#define	svc_ServerInfo			8	// first message from server about game, map etc
#define svc_SendTable			9	// sends a sendtable description for a game class
#define svc_ClassInfo			10	// Info about classes (first byte is a CLASSINFO_ define).							
#define	svc_SetPause			11	// tells client if server paused or unpaused
#define	svc_CreateStringTable	12	// inits shared string tables
#define	svc_UpdateStringTable	13	// updates a string table
#define svc_VoiceInit			14	// inits used voice codecs & quality
#define svc_VoiceData			15	// Voicestream data from the server
//#define svc_HLTV				16	// HLTV control messages
#define	svc_Sounds				17	// starts playing sound
#define	svc_SetView				18	// sets entity as point of view
#define	svc_FixAngle			19	// sets/corrects players viewangle
#define	svc_CrosshairAngle		20	// adjusts crosshair in auto aim mode to lock on traget
#define	svc_BSPDecal			21	// add a static decal to the worl BSP
//#define	svc_TerrainMod		22	// modification to the terrain/displacement. NOTE: This is now unused!
// Message from server side to client side entity
#define svc_UserMessage			23	// a game specific message 
#define svc_EntityMessage		24	// a message for an entity
#define	svc_GameEvent			25	// global game event fired
#define	svc_PacketEntities		26  // non-delta compressed entities
#define	svc_TempEntities		27	// non-reliable event object
#define svc_Prefetch			28	// only sound indices for now
#define svc_Menu				29	// display a menu from a plugin
#define svc_GameEventList		30	// list of known games events and fields
#define svc_GetCvarValue		31	// Server wants to know the value of a cvar on the client
#define svc_CmdKeyValues		32	// Server submits KeyValues command for the client
#define svc_SetPauseTimed		33	// Timed pause - to avoid breaking demos
#define SVC_LASTMSG				33	// last known server messages
// client to server
#define clc_ClientInfo			8	// client info (table CRC etc)
#define	clc_Move				9	// [CUserCmd]
#define clc_VoiceData			10	// Voicestream data from a client
#define clc_BaselineAck			11	// client acknowledges a new baseline seqnr
#define clc_ListenEvents		12	// client acknowledges a new baseline seqnr
#define clc_RespondCvarValue	13	// client is responding to a svc_GetCvarValue message.
#define clc_FileCRCCheck		14	// client is sending a file's CRC to the server to be verified.
#define clc_SaveReplay			15	// client is sending a save replay request to the server.
#define clc_CmdKeyValues		16
#define clc_FileMD5Check		17	// client is sending a file's MD5 to the server to be verified.
#define CLC_LASTMSG				17	//	last known client message
#define RES_FATALIFMISSING		(1<<0) // Disconnect if we can't get this file.
#define RES_PRELOAD				(1<<1) // Load on client rather than just reserving name
#define SIGNONSTATE_NONE		0	// no state yet, about to connect
#define SIGNONSTATE_CHALLENGE	1	// client challenging server, all OOB packets
#define SIGNONSTATE_CONNECTED	2	// client is connected to server, netchans ready
#define SIGNONSTATE_NEW			3	// just got serverinfo and string tables
#define SIGNONSTATE_PRESPAWN	4	// received signon buffers
#define SIGNONSTATE_SPAWN		5	// ready to receive entity packets
#define SIGNONSTATE_FULL		6	// we are fully connected, first non-delta packet received
#define SIGNONSTATE_CHANGELEVEL	7	// server is changing level, please wait
// matchmaking
#define mm_Heartbeat			16	// send a mm_Heartbeat
#define mm_ClientInfo			17	// information about a player
#define mm_JoinResponse			18	// response to a matchmaking join request
#define mm_RegisterResponse		19	// response to a matchmaking join request
#define mm_Migrate				20	// tell a client to migrate
#define mm_Mutelist				21	// send mutelist info to other clients
#define mm_Checkpoint			22	// game state checkpoints (start, connect, etc)
#define MM_LASTMSG				22	// last known matchmaking message

#define NETMSG_TYPE_BITS 6
#define NUM_NEW_COMMAND_BITS 4
#define NUM_BACKUP_COMMAND_BITS 3

class CNetMessage : public INetMessage
{
public:
	CNetMessage()
	{
		m_bReliable = true;
		m_NetChannel = nullptr;
	}

	virtual ~CNetMessage() {};

	virtual int GetGroup() const { return GENERIC; }
	INetChannel* GetNetChannel() const { return m_NetChannel; }

	virtual void SetReliable(bool state) { m_bReliable = state; };
	virtual bool IsReliable() const { return m_bReliable; };
	virtual void SetNetChannel(INetChannel* netchan) { m_NetChannel = netchan; }
	virtual bool Process() { return false; }; // no handler set

protected:
	bool m_bReliable;          // true if message should be send reliable
	INetChannel* m_NetChannel; // netchannel this message is from/for
	byte pad0[8];
};

class CLC_Move : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return clc_Move; }
	const char* GetName() const { return "clc_Move"; }
	void* m_pMessageHandler;
	int GetGroup() const { return MOVE; }

	CLC_Move() { m_bReliable = false; }

public:
	int m_nBackupCommands;
	int m_nNewCommands;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
};

class NET_SetConVar : public CNetMessage
{
public:
	bool ReadFromBuffer(bf_read& buffer);
	bool WriteToBuffer(bf_write& buffer);
	const char* ToString() const;
	int GetType() const { return net_SetConVar; }
	const char* GetName() const { return "net_SetConVar"; }
	void* m_pMessageHandler;
	int GetGroup() const { return STRINGCMD; }

	NET_SetConVar() {}
	NET_SetConVar(const char* name, const char* value)
	{
		CVar_t cvar;
		strncpy_s(cvar.Name, name, MAX_OSPATH);
		strncpy_s(cvar.Value, value, MAX_OSPATH);
		ConVar = cvar;
	}

public:
	typedef struct CVar_s
	{
		char Name[MAX_OSPATH];
		char Value[MAX_OSPATH];
	} CVar_t;
	CVar_t ConVar;
};