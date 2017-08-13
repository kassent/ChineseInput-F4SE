#pragma once

#include "f4se_common/Utilities.h"
#include "f4se_common/Relocation.h"

#include "f4se/GameTypes.h"
#include "f4se/GameUtilities.h"
#include "f4se/ScaleformCallbacks.h"
#include "f4se/ScaleformValue.h"

// 70
class IMenu : public SWFToCodeFunctionHandler
{
public:
	virtual ~IMenu();

	virtual void	Unk_03(void);
	virtual void	Unk_04(void);     //ProcessMessage
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);

	void			* inputUser;	// 10	BSInputEventUser
	UInt32			unk18;			// 18
	UInt32			unk1C;			// 1C
	GFxValue		stage;			// 20
	GFxMovieView	* movie;		// 40
	BSFixedString	unk48;			// 48
	BSFixedString	menuName;		// 50
	UInt64			flags;			// 58
	UInt64			unk60;			// 60	init'd as DWord then Byte
	UInt32			unk68;			// 68	init'd in IMenu::IMenu
	UInt32			pad6C;			// 6C
};
STATIC_ASSERT(offsetof(IMenu, movie) == 0x40);

// E0
class GameMenuBase : public IMenu
{
public:
	virtual ~GameMenuBase();

	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);

	tArray<BSGFxDisplayObject*>		subcomponents;					// 70
	BSGFxShaderFXTarget				* shaderTarget;					// 88
	UInt64							unk90[(0xE0 - 0x90)>>3];		// 90
};
STATIC_ASSERT(offsetof(GameMenuBase, shaderTarget) == 0x88);

// 218
class LooksMenu : public GameMenuBase
{
public:
	BSTEventSink<ChargenCharacterUpdateEvent> eventSink; // E0
	UInt64	unkE8;	// E8
	void	* unkF0; // F0 - LooksInputRepeatHandler
	UInt64 unkF8[(0x150-0xF8)/8];
	UInt32	nextBoneID;			// 150
	UInt32	currentBoneID;		// 154
	UInt64	unk158[(0x218-0x158)/8];


	MEMBER_FN_PREFIX(LooksMenu);
	DEFINE_MEMBER_FN(LoadCharacterParameters, void, 0x00B3F0E0); // This function updates all the internals from the current character
																 // It's followed by a call to onCommitCharacterPresetChange
};
STATIC_ASSERT(offsetof(LooksMenu, nextBoneID) == 0x150);

// 20
template <class T>
class HUDContextArray
{
public:
	T			* entries;	// 00
	UInt32		count;		// 08
	UInt32		unk0C;		// 0C
	UInt32		flags;		// 10
	UInt32		unk14;		// 14
	UInt32		unk18;		// 18
	bool		unk1C;		// 1C
};

// F8
class HUDComponentBase : public BSGFxShaderFXTarget
{
public:
	HUDComponentBase(GFxValue * parent, const char * componentName, HUDContextArray<BSFixedString> * contextList);
	virtual ~HUDComponentBase();

	virtual bool Unk_02() { return false; }
	virtual void Unk_03() { }
	virtual void UpdateComponent() { CALL_MEMBER_FN(this, Impl_UpdateComponent)(); } // Does stuff
	virtual void UpdateVisibilityContext(void * unk1);
	virtual void ColorizeComponent();
	virtual bool IsVisible() { return CALL_MEMBER_FN(this, Impl_IsVisible)(); }
	virtual bool Unk_08() { return contexts.unk1C; }

	UInt64							unkB0;			// B0
	UInt64							unkB8;			// B8
	UInt64							unkC0;			// C0
	HUDContextArray<BSFixedString>	contexts;		// C8
	UInt32							unkE8;			// E8
	UInt32							unkEC;			// EC
	UInt8							unkF0;			// F0
	UInt8							unkF1;			// F1
	bool							isWarning;		// F2 - This chooses the warning color over the default color
	UInt8							padF3[5];		// F3

	MEMBER_FN_PREFIX(HUDComponentBase);
	DEFINE_MEMBER_FN(Impl_ctor, HUDComponentBase *, 0x00A207A0, GFxValue * parent, const char * componentName, HUDContextArray<BSFixedString> * contextList);
	DEFINE_MEMBER_FN(Impl_IsVisible, bool, 0x00A20AE0);
	DEFINE_MEMBER_FN(Impl_UpdateComponent, void, 0x00A20840);
	
};
STATIC_ASSERT(offsetof(HUDComponentBase, contexts) == 0xC8);
STATIC_ASSERT(offsetof(HUDComponentBase, unkE8) == 0xE8);
STATIC_ASSERT(sizeof(HUDComponentBase) == 0xF8);

typedef bool (* _HasHUDContext)(HUDContextArray<BSFixedString> * contexts, void * unk1);
extern RelocAddr <_HasHUDContext> HasHUDContext;


// 110
class HUDComponents
{
public:
	UInt64								unk00;					// 00
	HUDComponentBase					* components[0x1E];		// 08
	UInt64								unk98;					// 98
	UInt64								unk100;					// 100
	UInt32								numComponents;			// 108 - 0x1E

	MEMBER_FN_PREFIX(HUDComponents);
	DEFINE_MEMBER_FN(Impl_Destroy, void, 0x01268F20);	// 3DD133AB9DDB89D138FB8958EB3A68CBF2F15DD9+FE
};

// 220
class HUDMenu : public GameMenuBase
{
public:
	BSTEventSink<UserEventEnabledEvent> inputEnabledSink;		// E0
	BSTEventSink<RequestHUDModesEvent>	requestHudModesSink;	// E8
	HUDComponents						children;				// F0
	UInt64								unk200;					// 200
	UInt64								unk208;					// 208
	UInt64								unk210;					// 210
	UInt64								unk218;					// 218
};
STATIC_ASSERT(offsetof(HUDMenu, unk200) == 0x200);

// 00C
class MenuTableItem
{
public:
	BSFixedString	name;				// 000
	IMenu			* menuInstance;		// 008	0 if the menu is not currently open
	void			* menuConstructor;	// 010
	void			* unk18;			// 018

	bool operator==(const MenuTableItem & rhs) const	{ return name == rhs.name; }
	bool operator==(const BSFixedString a_name) const	{ return name == a_name; }
	operator UInt64() const								{ return (UInt64)name.data->Get<char>(); }

	static inline UInt32 GetHash(BSFixedString * key)
	{
		UInt32 hash;
		CalculateCRC32_64(&hash, (UInt64)key->data, 0);
		return hash;
	}

	void Dump(void)
	{
		_MESSAGE("\t\tname: %s", name.data->Get<char>());
		_MESSAGE("\t\tinstance: %08X", menuInstance);
	}
};

// 250 ?
class UI
{
public:
	virtual ~UI();

	virtual void	Unk_01(void);

	typedef IMenu*	(*CreateFunc)(void);
	typedef tHashSet<MenuTableItem,BSFixedString> MenuTable;

	bool	IsMenuOpen(BSFixedString * menuName);
	IMenu * GetMenu(BSFixedString * menuName);
	IMenu * GetMenuByMovie(GFxMovieView * movie);
	void	Register(const char* name, CreateFunc creator)
	{
		CALL_MEMBER_FN(this, RegisterMenu)(name, creator, 0);
	}

	template<typename T>
	void ForEachMenu(T & menuFunc)
	{
		menuTable.ForEach(menuFunc);
	}

protected:
	MEMBER_FN_PREFIX(UI);
	DEFINE_MEMBER_FN(RegisterMenu, void, 0x0201B9F0, const char * name, CreateFunc creator, UInt64 unk1);
	DEFINE_MEMBER_FN(IsMenuOpen, bool, 0x02019E60, BSFixedString * name);

	UInt64	unk08[(0x190-0x08)/8];	// 458
	tArray<IMenu*>	menuStack;		// 190
	MenuTable		menuTable;		// 1A8
	// ...
};

extern RelocPtr <UI*> g_ui;

/*
int __fastcall sub_201A130(__int64 a1)
{
__int64 v1; // r13@1
signed __int64 v2; // r15@1
int *v3; // rax@1
int v4; // er12@1
int v5; // er14@1
signed __int64 v6; // rsi@1
__int64 v7; // rdx@1
unsigned __int64 v8; // rbx@1
unsigned __int64 v9; // rdi@1
void (*v10)(void); // rax@9
__int64 *v11; // rcx@14
__int64 v12; // rdx@14
__int64 v13; // rbx@14
int v14; // eax@14
signed int v15; // er15@14
int v16; // er14@15
signed __int64 v17; // rsi@15
__int64 v18; // r10@17
unsigned int v19; // er8@17
__int64 *v20; // r9@18
__int64 *v21; // rax@22
__int64 v22; // rdi@27
_QWORD *v23; // rax@27
__int64 *v24; // r12@27
__int64 v25; // rdx@29
signed __int64 v26; // rdi@30
__int64 v27; // rcx@37
int v28; // ecx@46
int v29; // edx@48
char v30; // r15@52
__int64 v31; // rdx@54
__int64 v32; // rcx@54
_DWORD *v33; // rdi@55
__int64 v34; // rax@58
void *v35; // rax@67
int v36; // eax@71
int v37; // eax@80
__int64 v38; // rdx@82
_QWORD *v39; // rcx@89
signed __int64 v40; // r8@89
unsigned int v41; // er11@89
__int64 v42; // r9@89
__int64 v43; // r10@90
char v44; // di@100
int v45; // edi@105
__int64 v46; // r8@105
char v47; // r15@105
_QWORD *v48; // rax@105
signed __int64 v49; // rcx@110
bool v50; // cl@113
char v51; // r12@121
signed __int32 v52; // ebx@124
signed __int64 v53; // rax@124
bool v54; // al@129
__int64 v55; // rdx@145
signed __int64 v56; // rdi@146
__int64 v57; // rcx@153
__int64 v58; // r15@159
__int64 v59; // rax@161
__int64 v60; // rbx@161
__int64 i; // rdx@166
__int64 v62; // rdx@167
__int64 v63; // r8@171
char v64; // di@171
_QWORD *v65; // rax@171
signed __int64 v66; // rcx@176
bool v67; // cl@179
int v68; // edx@189
__int64 v69; // rdx@193
__int64 v70; // rcx@193
__int64 v71; // rax@193
__int64 v72; // rax@196
__int64 v73; // rdx@196
void *v74; // rax@206
__int64 v75; // rdx@219
char v76; // bl@223
_QWORD *v77; // rcx@227
signed __int64 v78; // rdx@227
__int64 v79; // r9@227
unsigned int v80; // er11@227
__int64 v81; // r10@228
unsigned int v82; // er8@253
__int64 *v83; // rbx@255
__int64 v84; // rdi@258
__int64 v85; // rdx@266
__int64 v86; // rcx@266
__int64 v87; // r8@266
__int64 v88; // rax@266
int (__fastcall *v89)(__int64); // rdi@266
__int64 v90; // r8@267
char v91; // r15@267
_QWORD *v92; // rax@267
_QWORD *v93; // rbx@269
signed __int64 v94; // rcx@273
bool v95; // cl@276
int v96; // edx@285
signed __int64 v97; // rax@287
bool v98; // al@291
__int64 *v99; // rbx@310
__int64 v100; // rdi@313
int result; // eax@318
int *v102; // rcx@319
__int64 v103; // [sp+30h] [bp-98h]@27
__int64 v104; // [sp+38h] [bp-90h]@27
__int64 v105; // [sp+40h] [bp-88h]@145
int (__fastcall *v106)(__int64); // [sp+48h] [bp-80h]@145
__int64 v107; // [sp+50h] [bp-78h]@67
int *v108; // [sp+58h] [bp-70h]@1
int v109; // [sp+60h] [bp-68h]@14
int v110; // [sp+64h] [bp-64h]@206
_QWORD *v111; // [sp+68h] [bp-60h]@166
__int64 *v112; // [sp+70h] [bp-58h]@21
__int64 v113; // [sp+78h] [bp-50h]@29
char v114; // [sp+80h] [bp-48h]@194
char v115; // [sp+88h] [bp-40h]@194
_QWORD *v116; // [sp+90h] [bp-38h]@173
char v117; // [sp+98h] [bp-30h]@226
char v118; // [sp+A0h] [bp-28h]@226
_QWORD *v119; // [sp+A8h] [bp-20h]@270
char v120; // [sp+B0h] [bp-18h]@221
char v121; // [sp+B8h] [bp-10h]@221
char v122; // [sp+C0h] [bp-8h]@103
char v123; // [sp+C8h] [bp+0h]@103
_QWORD *v124; // [sp+D0h] [bp+8h]@107
char v125; // [sp+E0h] [bp+18h]@98
char v126; // [sp+E8h] [bp+20h]@98
char v127; // [sp+F0h] [bp+28h]@56
char v128; // [sp+F8h] [bp+30h]@56
char v129; // [sp+100h] [bp+38h]@266
__int64 v130; // [sp+118h] [bp+50h]@29
unsigned int v131; // [sp+138h] [bp+70h]@14
__int64 *v132; // [sp+140h] [bp+78h]@17
unsigned int v133; // [sp+340h] [bp+278h]@14
int vars10; // [sp+398h] [bp+2D0h]@27
int vars20; // [sp+3A8h] [bp+2E0h]@1
int vars28; // [sp+3B0h] [bp+2E8h]@1

v1 = a1;
v2 = 0i64;
v3 = (int *)(*(_QWORD *)(*MK_FP(__GS__, 88i64) + 8i64 * (unsigned int)TlsIndex) + 2496i64);
v4 = 0;
v5 = *v3;
vars20 = 0;
v108 = v3;
vars28 = v5;
*v3 = 66;
sub_1AF2DB0(&unk_64F8430);
sub_201CE80(v1 + 400);
sub_1AF3000(&unk_64F8430);
v6 = v1 + 472;
sub_244D0(v1 + 472, 0i64);
sub_1AF2D30(&unk_64F8438);
v7 = *(_QWORD *)(v1 + 464);
v8 = 0i64;
v9 = 0i64;
if ( v7 )
{
v8 = *(_QWORD *)(v1 + 464);
v9 = v7 + 40i64 * *(_DWORD *)(v1 + 436);
while ( v8 < v9 && !*(_QWORD *)(v8 + 32) )
v8 += 40i64;
if ( v7 )
v2 = v7 + 40i64 * *(_DWORD *)(v1 + 436);
}
while ( v8 != v2 )
{
v10 = *(void (**)(void))(v8 + 24);
if ( v10 )
v10();
do
v8 += 40i64;
while ( v8 < v9 && !*(_QWORD *)(v8 + 32) );
}
sub_1AF2FF0(&unk_64F8438);
v131 = v131 & 0x80000000 | 0x80000000;
sub_1AF3100(&v133);
sub_2024A70(UIMessageManager_585BD48, &v131);
v12 = v133;
v13 = 0i64;
v14 = 0;
v109 = 0;
v15 = 2;
if ( v133 )
{
v16 = 0;
v17 = v1 + 400;
while ( 1 )
{
if ( (unsigned int)v14 >= 0x10 )
{
LABEL_265:
v5 = vars28;
v4 = vars20;
v6 = v1 + 472;
goto LABEL_266;
}
v18 = (__int64)v132;
v19 = v131;
if ( (_DWORD)v12 )
{
v20 = (__int64 *)&v132;
if ( !_bittest((const signed int *)&v19, 0x1Fu) )
v20 = v132;
}
else
{
v20 = 0i64;
}
v112 = v20;
if ( (_DWORD)v12 )
{
v11 = (__int64 *)&v132;
if ( !_bittest((const signed int *)&v19, 0x1Fu) )
v11 = v132;
v21 = &v11[(unsigned int)v12];
}
else
{
v21 = 0i64;
}
v112 = v21;
if ( v20 != v21 )
break;
LABEL_253:
v82 = v19 >> 31;
if ( v82 || v18 )
{
v83 = (__int64 *)&v132;
if ( !v82 )
v83 = (__int64 *)v18;
if ( (_DWORD)v12 )
{
v84 = (unsigned int)v12;
do
{
if ( *v83 )
(**(void (__fastcall ***)(_QWORD, _QWORD))*v83)(*v83, 1i64);
++v83;
--v84;
}
while ( v84 );
v16 = vars20;
}
v13 = 0i64;
v133 = 0;
}
sub_2024A70(UIMessageManager_585BD48, &v131);
v12 = v133;
v14 = v109++ + 1;
if ( !v133 )
goto LABEL_265;
}
v104 = 0i64;
v22 = *v112;
vars10 = *(_DWORD *)(v1 + 480);
v103 = v22;
LODWORD(v23) = sub_20243F0(v11, v12, v131);
v24 = (__int64 *)(v22 + 8);
if ( *(_QWORD *)(v22 + 8) == *v23 )
{
sub_201A040(v1, &v104, 21);
v13 = v104;
}
else
{
v130 = 0i64;
sub_1AF2D30(&unk_64F8438);
sub_1AF2C30(&v113, *v24, 0i64);
v25 = *(_QWORD *)(v1 + 464);
if ( v25 )
{
v26 = v25 + 40i64 * ((unsigned int)v113 & (*(_DWORD *)(v1 + 436) - 1));
if ( v26 && *(_QWORD *)(v26 + 32) )
{
while ( *(_QWORD *)v26 != *v24 )
{
v26 = *(_QWORD *)(v26 + 32);
if ( !v26 || v26 == *(_QWORD *)(v1 + 448) )
goto LABEL_41;
}
if ( v26 )
{
v27 = *(_QWORD *)(v26 + 8);
if ( v27 )
sub_20E6890(v27);
v13 = *(_QWORD *)(v26 + 8);
v130 = v13;
if ( v13 )
sub_20E6890(v13);
}
}
LABEL_41:
v22 = v103;
}
v104 = v13;
sub_1AF2FF0(&unk_64F8438);
if ( !v13 )
{
LABEL_45:
if ( *(_DWORD *)(v22 + 16) != 1 )
{
LABEL_252:
v13 = 0i64;
++v112;
LODWORD(v12) = v133;
v18 = (__int64)v132;
v19 = v131;
goto LABEL_253;
}
LABEL_46:
v28 = *(_DWORD *)(v22 + 16);
if ( v28 != 1 )
{
if ( (unsigned int)(v28 - 3) > 1 )
{
v45 = v15;
LOBYTE(vars10) = ((v28 - 7) & 0xFFFFFFFB) == 0;
sub_1AF2D30(&unk_64F8430);
v46 = *(_DWORD *)(v17 + 16);
v47 = 0;
v48 = 0i64;
if ( (_DWORD)v46 )
v48 = *(_QWORD **)v17;
while ( 1 )
{
v124 = v48;
v50 = 0;
if ( !v47 && *(_DWORD *)(v1 + 416) )
{
v16 |= 8u;
v49 = (_DWORD)v46 ? *(_QWORD *)v17 + 8 * v46 : 0i64;
if ( v48 != (_QWORD *)v49 )
v50 = 1;
}
if ( v16 & 8 )
v16 &= 0xFFFFFFF7;
if ( !v50 )
break;
if ( *v48 == v13 )
{
v108 = (int *)v48;
v47 = 1;
}
++v48;
}
vars20 = v16;
sub_1AF2FF0(&unk_64F8430);
v51 = vars10;
if ( v47 )
{
sub_1AF2D30(&unk_64F8430);
while ( 1 )
{
v16 |= 1u;
if ( *(_DWORD *)(v17 + 16) )
{
v53 = *(_QWORD *)v17 - 8i64;
v52 = 0;
}
else
{
v52 = 0;
v53 = 0i64;
}
v54 = v108 != (int *)v53 && (v45 == 2 || v51);
if ( v16 & 1 )
v16 &= 0xFFFFFFFE;
if ( !v54 )
break;
sub_201B870(v1);
v45 = (*(int (__fastcall **)(_QWORD, __int64))(**(_QWORD **)v108 + 24i64))(*(_QWORD *)v108, v103);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = v52;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, v52, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
if ( *(_BYTE *)(*(_QWORD *)v108 + 88i64) & 0x10 && !v51 )
v45 = 1;
v108 -= 2;
}
vars20 = v16;
sub_1AF2FF0(&unk_64F8430);
v13 = v104;
goto LABEL_249;
}
sub_201B870(v1);
(*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)v13 + 24i64))(v13, v103);
v15 = 2;
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
v13 = v104;
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
v13 = v104;
}
}
else
{
sub_201B870(v1);
v29 = (*(int (__fastcall **)(__int64, __int64))(*(_QWORD *)v13 + 24i64))(v13, v22);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
v13 = v104;
if ( v29 != 1 )
{
sub_20E6890(v104);
v30 = sub_201CD10(v1, v13, &v113);
if ( v30 )
{
sub_1AF2DB0(&unk_64F8430);
sub_2021BE0(v17, (v113 - *(_QWORD *)v17) >> 3, 1i64);
sub_1AF3000(&unk_64F8430);
(*(void (__fastcall **)(__int64))(*(_QWORD *)v13 + 96i64))(v13);
sub_201C4E0(v1);
}
sub_20E6930(v13);
if ( v30 )
{
v33 = (_DWORD *)(v13 + 88);
if ( *(_BYTE *)(v13 + 88) & 1 )
{
--*(_DWORD *)(v1 + 480);
sub_1B19A80(&v127, v24);
v128 = 0;
sub_20213F0(v1 + 200, &v127);
sub_1B19B70((__int64)&v127);
}
if ( *v33 & 0x20000 )
{
LODWORD(v34) = sub_14637E0(v32, v31);
sub_28DDF20(v34, 0i64);
}
if ( *v33 & 0x2000 )
{
_InterlockedDecrement((volatile signed __int32 *)(v1 + 484));
v13 = v104;
}
if ( *v33 & 0x80 && *(_DWORD *)(v1 + 488) )
{
_InterlockedDecrement((volatile signed __int32 *)(v1 + 488));
v13 = v104;
}
if ( *(_BYTE *)v33 & 0x20 )
--*(_DWORD *)(v1 + 492);
if ( *v33 & 0x400000 )
{
LODWORD(v107) = 1;
v35 = off_5A67950;
if ( !off_5A67950 )
{
LODWORD(v35) = sub_129AC70(&unk_5A67960, (char *)off_5859E98 + 16);
off_5A67950 = v35;
}
sub_2020AF0((char *)v35 + 16, &v107);
}
if ( *v33 & 0x4000000 )
{
v36 = *(_DWORD *)(v1 + 508);
if ( v36 )
*(_DWORD *)(v1 + 508) = v36 - 1;
}
if ( !(*v33 & 0x800) )
--*(_DWORD *)(v1 + 500);
if ( *v33 & 0x1000000 )
--*(_DWORD *)(v1 + 496);
if ( !(*v33 & 0x800000) )
--*(_DWORD *)(v1 + 504);
if ( *v33 & 0x8000000 )
{
v37 = *(_DWORD *)(v1 + 512);
if ( v37 )
*(_DWORD *)(v1 + 512) = v37 - 1;
}
v38 = *(_DWORD *)(v13 + 104);
if ( (_DWORD)v38 != 33 )
sub_1B0AA90(off_5965BF0, v38);
if ( *(_BYTE *)v33 & 8 )
{
sub_1B0AA90(off_5965BF0, 1i64);
sub_1B0AA90(off_5965BF0, 2i64);
}
v22 = v103;
}
if ( !(*(_BYTE *)(v13 + 88) & 2) || *(_DWORD *)(v22 + 16) == 4 )
{
v39 = (_QWORD *)(v1 + 376);
v40 = 0xFFFFFFFFi64;
v41 = *(_DWORD *)(v1 + 392);
v42 = 0i64;
if ( v41 )
{
v43 = 0i64;
while ( (_DWORD)v40 == -1 )
{
if ( *(_QWORD *)(v43 + *v39) == *v24 )
v40 = (unsigned int)v42;
v42 = (unsigned int)(v42 + 1);
v43 += 8i64;
if ( (unsigned int)v42 >= v41 )
{
if ( (_DWORD)v40 != -1 )
break;
goto LABEL_96;
}
}
}
else
{
LABEL_96:
sub_46A20(v39, v24, v40, v42);
}
}
sub_201C0B0(v1);
if ( !v30 )
goto LABEL_249;
sub_1B19A80(&v125, v24);
v126 = 0;
sub_20216F0(v1 + 24, &v125);
if ( vars10 )
{
if ( !*(_DWORD *)(v1 + 480) )
{
v44 = 0;
goto LABEL_103;
}
}
else if ( *(_DWORD *)(v1 + 480) )
{
v44 = 1;
LABEL_103:
sub_1B19A80(&v122, v24);
v123 = v44;
sub_20210F0(v1 + 112, &v122);
sub_1B19B70((__int64)&v122);
goto LABEL_104;
}
LABEL_104:
sub_1B19B70((__int64)&v125);
LABEL_249:
v15 = 2;
goto LABEL_250;
}
}
LABEL_250:
if ( v13 )
sub_20E6930(v13);
goto LABEL_252;
}
v105 = 0i64;
v106 = 0i64;
v107 = 0i64;
sub_1AF2D30(&unk_64F8438);
sub_1AF2C30(&v109, *v24, 0i64);
v55 = *(_QWORD *)(v1 + 464);
if ( v55 )
{
v56 = v55 + 40i64 * (v109 & (unsigned int)(*(_DWORD *)(v1 + 436) - 1));
if ( v56 && *(_QWORD *)(v56 + 32) )
{
while ( *(_QWORD *)v56 != *v24 )
{
v56 = *(_QWORD *)(v56 + 32);
if ( !v56 || v56 == *(_QWORD *)(v1 + 448) )
goto LABEL_158;
}
if ( v56 )
{
v57 = *(_QWORD *)(v56 + 8);
if ( v57 )
sub_20E6890(v57);
if ( v105 )
sub_20E6930(v105);
v105 = *(_QWORD *)(v56 + 8);
v106 = *(int (__fastcall **)(__int64))(v56 + 16);
v107 = *(_QWORD *)(v56 + 24);
}
}
LABEL_158:
v22 = v103;
}
sub_1AF2FF0(&unk_64F8438);
v58 = v105;
if ( v105 )
{
sub_20E6F50(v105);
}
else
{
if ( v106 == (int (__fastcall *)(__int64))v105 )
goto LABEL_171;
sub_201B870(v1);
LODWORD(v59) = v106(v22);
v60 = v59;
if ( v105 )
sub_20E6930(v105);
v105 = v60;
sub_1B19BE0((__int64 *)(v60 + 80), v24);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
sub_1AF2DB0(&unk_64F8438);
sub_1AF2C30(&v111, *v24, 0i64);
for ( i = *(_QWORD *)(v1 + 464); !(unsigned __int8)sub_201E780(v1 + 432); v62 = *(_QWORD *)(v1 + 464) )
sub_2021AD0(v1 + 432);
sub_1AF3000(&unk_64F8438);
v13 = v104;
}
v58 = v105;
LABEL_171:
sub_1AF2D30(&unk_64F8430);
v63 = *(_DWORD *)(v1 + 416);
v64 = 0;
v65 = 0i64;
if ( (_DWORD)v63 )
v65 = *(_QWORD **)(v1 + 400);
while ( 1 )
{
v116 = v65;
v67 = 0;
if ( !v64 && *(_DWORD *)(v1 + 416) )
{
v16 |= 4u;
v66 = (_DWORD)v63 ? *(_QWORD *)(v1 + 400) + 8 * v63 : 0i64;
if ( v65 != (_QWORD *)v66 )
v67 = 1;
}
if ( v16 & 4 )
v16 &= 0xFFFFFFFB;
if ( !v67 )
break;
if ( *v65 == v58 )
v64 = 1;
++v65;
}
vars20 = v16;
sub_1AF2FF0(&unk_64F8430);
if ( !v105 )
goto LABEL_248;
if ( v64 )
{
sub_2027F20(v103, (__int64)v24, 2);
sub_201B870(v1);
(*(void (__fastcall **)(__int64, __int64))(*(_QWORD *)v105 + 24i64))(v105, v103);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
}
else
{
sub_201B870(v1);
v68 = (*(int (__fastcall **)(__int64, __int64))(*(_QWORD *)v105 + 24i64))(v105, v103);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
if ( v68 != 1 )
{
sub_201C340(v1, &v105);
v71 = v105;
if ( *(_BYTE *)(v105 + 88) & 1 )
{
++*(_DWORD *)(v1 + 480);
sub_1B19A80(&v114, v24);
v115 = 1;
sub_20213F0(v1 + 200, &v114);
sub_1B19B70((__int64)&v114);
v71 = v105;
}
if ( *(_DWORD *)(v71 + 88) & 0x20000 )
{
LODWORD(v72) = sub_14637E0(v70, v69);
LOBYTE(v73) = 1;
sub_28DDF20(v72, v73);
v71 = v105;
}
if ( *(_DWORD *)(v71 + 88) & 0x4000000 )
++*(_DWORD *)(v1 + 508);
if ( *(_DWORD *)(v71 + 88) & 0x2000 )
{
_InterlockedIncrement((volatile signed __int32 *)(v1 + 484));
v71 = v105;
}
if ( *(_DWORD *)(v71 + 88) & 0x80 )
{
_InterlockedIncrement((volatile signed __int32 *)(v1 + 488));
v71 = v105;
}
if ( *(_BYTE *)(v71 + 88) & 0x20 )
++*(_DWORD *)(v1 + 492);
if ( *(_DWORD *)(v71 + 88) & 0x400000 )
{
v74 = off_5A67950;
v110 = 0;
if ( !off_5A67950 )
{
LODWORD(v74) = sub_129AC70(&unk_5A67960, (char *)off_5859E98 + 16);
off_5A67950 = v74;
}
sub_2020AF0((char *)v74 + 16, &v110);
v71 = v105;
}
if ( !(*(_DWORD *)(v71 + 88) & 0x800) )
++*(_DWORD *)(v1 + 500);
if ( *(_DWORD *)(v71 + 88) & 0x1000000 )
++*(_DWORD *)(v1 + 496);
if ( !(*(_DWORD *)(v71 + 88) & 0x800000) )
++*(_DWORD *)(v1 + 504);
if ( *(_DWORD *)(v71 + 88) & 0x8000000 )
++*(_DWORD *)(v1 + 512);
if ( *(_BYTE *)(v71 + 88) & 8 )
{
sub_1B0A9A0(off_5965BF0, 1i64);
sub_1B0A9A0(off_5965BF0, 2i64);
v71 = v105;
}
v75 = *(_DWORD *)(v71 + 104);
if ( (_DWORD)v75 != 33 )
sub_1B0A9A0(off_5965BF0, v75);
sub_201C0B0(v1);
sub_1B19A80(&v120, v24);
v121 = 1;
sub_20216F0(v1 + 24, &v120);
if ( vars10 )
{
if ( !*(_DWORD *)(v1 + 480) )
{
v76 = 0;
goto LABEL_226;
}
}
else if ( *(_DWORD *)(v1 + 480) )
{
v76 = 1;
LABEL_226:
sub_1B19A80(&v117, v24);
v118 = v76;
sub_20210F0(v1 + 112, &v117);
sub_1B19B70((__int64)&v117);
goto LABEL_227;
}
LABEL_227:
v77 = (_QWORD *)(v1 + 376);
v78 = 0xFFFFFFFFi64;
v79 = 0i64;
v80 = *(_DWORD *)(v1 + 392);
if ( v80 )
{
v81 = 0i64;
while ( (_DWORD)v78 == -1 )
{
if ( *(_QWORD *)(v81 + *v77) == *v24 )
v78 = (unsigned int)v79;
v79 = (unsigned int)(v79 + 1);
v81 += 8i64;
if ( (unsigned int)v79 >= v80 )
{
if ( (_DWORD)v78 == -1 )
goto LABEL_235;
break;
}
}
sub_2021EB0(v77, v78, 1i64, v79);
}
LABEL_235:
sub_1B19B70((__int64)&v120);
goto LABEL_245;
}
if ( !(*(_BYTE *)(v105 + 88) & 2) )
{
sub_201C7F0(v1, v24);
if ( v105 )
sub_20E6930(v105);
v105 = 0i64;
if ( v104 )
sub_20E6930(v104);
v13 = 0i64;
v104 = 0i64;
goto LABEL_246;
}
}
LABEL_245:
v13 = v104;
LABEL_246:
if ( v105 )
sub_20E6930(v105);
LABEL_248:
v17 = v1 + 400;
goto LABEL_249;
}
sub_20E6930(v13);
}
if ( v13 )
goto LABEL_46;
goto LABEL_45;
}
LABEL_266:
sub_20F4C20(off_5A2FC28);
*(_BYTE *)(v1 + 585) = 0;
v106 = 0i64;
LODWORD(v88) = sub_20243F0(v86, v85, v87);
CreateUIMessage_2027EC0(&v129, v88, 0);
sub_201A040(v1, &v106, 15);
v89 = v106;
if ( v106 )
{
sub_1AF2D30(&unk_64F8430);
v90 = *(_DWORD *)(v1 + 416);
v91 = 0;
v92 = 0i64;
if ( (_DWORD)v90 )
v92 = *(_QWORD **)(v1 + 400);
v93 = v111;
while ( 1 )
{
v119 = v92;
v95 = 0;
if ( !v91 && *(_DWORD *)(v1 + 416) )
{
v4 |= 0x10u;
v94 = (_DWORD)v90 ? *(_QWORD *)(v1 + 400) + 8 * v90 : 0i64;
if ( v92 != (_QWORD *)v94 )
v95 = 1;
}
if ( v4 & 0x10 )
v4 &= 0xFFFFFFEF;
if ( !v95 )
break;
if ( (int (__fastcall *)(__int64))*v92 == v89 )
{
v93 = v92;
v91 = 1;
}
++v92;
}
v111 = v93;
sub_1AF2FF0(&unk_64F8430);
v5 = vars28;
if ( v91 )
{
v96 = 2;
while ( 1 )
{
v4 |= 2u;
if ( *(_DWORD *)(v1 + 416) )
v97 = *(_QWORD *)(v1 + 400) - 8i64;
else
v97 = 0i64;
v98 = v93 != (_QWORD *)v97 && v96 == 2;
if ( v4 & 2 )
v4 &= 0xFFFFFFFD;
if ( !v98 )
break;
if ( *(_DWORD *)(*v93 + 88i64) & 0x100 )
{
sub_201B870(v1);
v96 = (*(int (__fastcall **)(_QWORD, char *))(*(_QWORD *)*v93 + 24i64))(*v93, &v129);
if ( dword_64F8444 == 1 )
{
dword_64F8440 = 0;
_mm_mfence();
_InterlockedCompareExchange((volatile signed __int32 *)&dword_64F8444, 0, dword_64F8444);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)&dword_64F8444);
}
v93 = v111;
}
if ( *(_BYTE *)(*v93 + 88i64) & 0x10 )
v96 = 1;
--v93;
v111 = v93;
}
v89 = v106;
v5 = vars28;
}
LODWORD(v13) = 0;
}
sub_2027F00(&v129);
if ( v89 )
sub_20E6930(v89);
if ( v131 >> 31 || v132 )
{
v99 = (__int64 *)&v132;
if ( !(v131 >> 31) )
v99 = v132;
if ( v133 )
{
v100 = v133;
do
{
if ( *v99 )
(**(void (__fastcall ***)(_QWORD, _QWORD))*v99)(*v99, 1i64);
++v99;
--v100;
}
while ( v100 );
}
sub_1AF3FE0(&v131);
LODWORD(v13) = 0;
v133 = 0;
}
result = nullsub_680(&v133);
if ( v6 )
{
result = *(_DWORD *)(v6 + 4);
v102 = v108;
if ( result == 1 )
{
*(_DWORD *)v6 = v13;
_mm_mfence();
result = _InterlockedCompareExchange((volatile signed __int32 *)(v6 + 4), v13, 1);
}
else
{
_InterlockedDecrement((volatile signed __int32 *)(v6 + 4));
}
}
else
{
v102 = v108;
}
*v102 = v5;
return result;
}

*/