#pragma once
#include "sourceinterface.h"
#include "color.h"
#include "cvar.h"
#include "concommand.h"

// taken directly from iconvar.h

// The default, no flags at all
#define FCVAR_NONE 0

// Command to ConVars and ConCommands
// ConVar Systems
#define FCVAR_UNREGISTERED    (1 << 0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY (1 << 1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL         (1 << 2)	// defined by the game DLL
#define FCVAR_CLIENTDLL       (1 << 3)	// defined by the client DLL
#define FCVAR_HIDDEN          (1 << 4)	// Hidden. Doesn't appear in find or auto complete. Like DEVELOPMENTONLY, but can't be compiled out.

// ConVar only
#define FCVAR_PROTECTED       (1 << 5)	// It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value.
#define FCVAR_SPONLY   (1 << 6)	// This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_ARCHIVE  (1 << 7)	// set to cause it to be saved to vars.rc
#define FCVAR_NOTIFY   (1 << 8)	// notifies players when changed
#define FCVAR_USERINFO (1 << 9)	// changes the client's info string

#define FCVAR_PRINTABLEONLY (1 << 10) // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_GAMEDLL_FOR_REMOTE_CLIENTS                                                                                            \
	(1 << 10) // When on concommands this allows remote clients to execute this cmd on the server.
			  // We are changing the default behavior of concommands to disallow execution by remote clients without
			  // this flag due to the number existing concommands that can lag or crash the server when clients abuse them.

#define FCVAR_UNLOGGED        (1 << 11) // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING (1 << 12) // never try to print that cvar

// It's a ConVar that's shared between the client and the server.
// At signon, the values of all such ConVars are sent from the server to the client (skipped for local client, of course )
// If a change is requested it must come from the console (i.e., no remote client changes)
// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED       (1 << 13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_CHEAT            (1 << 14)	// Only useable in singleplayer / debug / multiplayer & sv_cheats
#define FCVAR_SS               (1 << 15)	// causes varnameN where N == 2 through max splitscreen slots for mod to be autogenerated
#define FCVAR_DEMO             (1 << 16)	// record this cvar when starting a demo file
#define FCVAR_DONTRECORD       (1 << 17)	// don't record these command in demofiles
#define FCVAR_SS_ADDED         (1 << 18)	// This is one of the "added" FCVAR_SS variables for the splitscreen players
#define FCVAR_RELEASE          (1 << 19)	// Cvars tagged with this are the only cvars avaliable to customers
#define FCVAR_RELOAD_MATERIALS (1 << 20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES  (1 << 21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED          (1 << 22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1 << 23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_PLAYERPROFILE  (1 << 24)	// respawn-defined flag, same as FCVAR_ARCHIVE but writes to profile.cfg

#define FCVAR_SERVER_CAN_EXECUTE                                                                                                           \
	(1 << 28) // the server is allowed to execute this command on clients via
			  // ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY                                                                                                          \
	(1 << 29) // If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE                                                                                                        \
	(1 << 30) // IVEngineClient::ClientCmd is allowed to execute this command.
			  // Note: IVEngineClient::ClientCmd_Unrestricted can run any client command.

#define FCVAR_ACCESSIBLE_FROM_THREADS (1 << 25) // used as a debugging tool necessary to check material system thread convars
// #define FCVAR_AVAILABLE			(1<<26)
// #define FCVAR_AVAILABLE			(1<<27)
// #define FCVAR_AVAILABLE			(1<<31)

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class ConCommandBase;
class ConCommand;
class ConVar;

//-----------------------------------------------------------------------------
// Purpose: A console variable
//-----------------------------------------------------------------------------
class ConVar
{
public:
	ConVar(void){};
	ConVar(const char* pszName, const char* pszDefaultValue, int nFlags, const char* pszHelpString);
	ConVar(const char* pszName, const char* pszDefaultValue, int nFlags, const char*pszHelpString,
		bool bMin, float fMin, bool bMax, float fMax, void* pCallback);
	~ConVar(void);

	const char* GetBaseName(void) const;
	const char* GetHelpText(void) const;

	void AddFlags(int nFlags);
	void RemoveFlags(int nFlags);

	bool IsRegistered(void) const;

	bool GetBool(void) const;
	float GetFloat(void) const;
	int GetInt(void) const;
	Color GetColor(void) const;
	const char* GetString(void) const;

	bool GetMin(float& flMinValue) const;
	bool GetMax(float& flMaxValue) const;
	float GetMinValue(void) const;
	float GetMaxValue(void) const;
	bool HasMin(void) const;
	bool HasMax(void) const;

	void SetValue(int nValue);
	void SetValue(float flValue);
	void SetValue(const char* pszValue);
	void SetValue(Color clValue);

	void ChangeStringValue(const char* pszTempValue, float flOldValue);
	bool SetColorFromString(const char* pszValue);
	bool ClampValue(float& value);

	static bool IsFlagSet(ConVar* pConVar, int nFlags);

	ConCommandBase m_ConCommandBase {}; //0x0000
	ConVar*        m_pParent        {}; //0x0040
	const char*    m_pzsCurrentValue{}; //0x0048
	size_t         m_iStringLength  {}; //0x004C
	float          m_flValue        {}; //0x0054
	int            m_nValue         {}; //0x0058
	bool           m_bHasMin        {}; //0x005C
	float          m_flMinValue     {}; //0x0060
	bool           m_bHasMax        {}; //0x0064
	float          m_flMaxValue     {}; //0x0068
	void*          m_pMalloc        {}; //0x0070
	char           m_pPad80[10]     {}; //0x0080
}; //Size: 0x0080

void InitialiseConVars(HMODULE baseAddress);
extern std::unordered_map<std::string, ConVar*> g_CustomConvars;