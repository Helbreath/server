#pragma once


namespace hbx
{

#define DEF_OWNERTYPE_PLAYER			1
#define DEF_OWNERTYPE_NPC				2
#define DEF_OWNERTYPE_PLAYER_INDIRECT	3

// for Client Data
#define DEF_MAPDATASIZEX		30
#define DEF_MAPDATASIZEY		25
#define DEF_MAXTELEPORTLOC		200
#define DEF_MAXWAYPOINTCFG		200
#define DEF_MAXMGAR				50
#define DEF_MAXNMR				50
#define DEF_MAXSPOTMOBGENERATOR	100
#define DEF_MAXFISHPOINT		200
#define DEF_MAXMINERALPOINT		200
#define DEF_MAXOCCUPYFLAG		20001 //3001
#define	DEF_MAXINITIALPOINT		20


#define DEF_MAPTYPE_NORMAL				0
#define DEF_MAPTYPE_NOPENALTY_NOREWARD	1
#define DEF_MAXENERGYSPHERES			10

#define DEF_MAXSTRATEGICPOINTS			200
#define DEF_MAXSECTORS					60
#define DEF_MAXSTRIKEPOINTS				20

#define DEF_MAXITEMEVENTS				50


#define DEF_CLIENTSOCKETBLOCKLIMIT	15

#define DEF_MSGBUFFERSIZE	30000
#define DEF_MAXITEMS		50
#define DEF_MAXBANKITEMS	120
#define DEF_MAXGUILDSMAN	128

#define	DEF_MAXMAGICTYPE	100
#define DEF_MAXSKILLTYPE	60

#define DEF_MAXTELEPORTLIST	20

#define DEF_MAXPARTYMEMBERS	8

#define DEF_SPECABLTYTIMESEC	1200

#define DEF_PARTYSTATUS_0		0
#define DEF_PARTYSTATUS_PROCESSING	1
#define DEF_PARTYSTATUS_CONFIRM		2


#define DEF_MAXMAGICEFFECTS			100

enum class magic_type {
    damage_spot = 1,
    hpup_spot,
    damage_area,
    spdown_spot,
    spdown_area,
    spup_spot,
    spup_area,
    teleport,
    summon,
    create,
    protect,
    hold_object,
    invisibility,
    create_dynamic,
    possession,
    confuse,
    poison,
    berserk,
    damage_linear,
    polymorph,
    damage_area_nospot,
    tremor,
    ice,
    damage_spot_spdown,
    damage_area_nospot_spdown,
    damage_area_armor_break,
    ice_linear,
    cancellation,
    inhibition,
    damage_linear_spdown,
    scan,
    resurrection,
    damage_area_move,
    sotg_linear, // ?
    cotg_linear, // ?
    fsw_linear, // ?
    mb_linear // ?
};


#define DEF_MAXWAYPOINTS			10

enum class move_type
{
    stop,
    sequential_waypoint,
    random_waypoint,
    follow,
    random_area,
    random,
    guard
};

enum class behavior
{
    stop,
    move,
    attack,
    flee,
    dead
};

enum class owner_type
{
    none,
    player,
    npc,
    player_indirect,
    npc_indirect
};

enum class weather
{
    sunny,
    light_rain,
    medium_rain,
    heavy_rain,
    light_snow,
    medium_snow,
    heavy_snow
};

#define DEF_ATTRIBUTE_EARTH			1
#define DEF_ATTRIBUTE_AIR			2
#define DEF_ATTRIBUTE_FIRE			3
#define DEF_ATTRIBUTE_WATER			4

enum class motion_id
{
    MSGIDTYPE_MOTION = 1 << 15,
    stop = MSGIDTYPE_MOTION,
    move,
    run,
    attrack,
    magic,
    get_item,
    damage,
    damage_move,
    attack_move,
    dying,
    null,
    event_confirm,
    event_reject,
    max
};

enum class log_message_id
{
    login = 25,
    create_new_account = 26,
    create_new_character = 28,
    enter_game = 29,
    delete_character = 31
};

enum class log_rsp_message_id
{
    log = 27,
    enter_game = 30
};

enum class message_id
{
    NOTIFY,
    REQUEST_INITPLAYER,
    RESPONSE_INITPLAYER,

    REQUEST_INITDATA,
    RESPONSE_INITDATA,

    RESPONSE_MOTION,

    EVENT_COMMON,
    COMMAND_COMMON,

    ITEMCONFIGURATIONCONTENTS,
    NPCCONFIGURATIONCONTENTS,
    MAGICCONFIGURATIONCONTENTS,
    SKILLCONFIGURATIONCONTENTS,
    PLAYERITEMLISTCONTENTS,
    PORTIONCONFIGURATIONCONTENTS,
    PLAYERCHARACTERCONTENTS,
    QUESTCONFIGURATIONCONTENTS,
    BUILDITEMCONFIGURATIONCONTENTS,
    DUPITEMIDFILECONTENTS,
    NOTICEMENTFILECONTENTS,

    COMMAND_CHECKCONNECTION,
    COMMAND_CHATMSG,

    REQUEST_REGISTERGAMESERVER,
    RESPONSE_REGISTERGAMESERVER,
    REQUEST_REGISTERDBSERVER,
    RESPONSE_REGISTERDBSERVER,

//     REQUEST_LOGIN,
//     REQUEST_CREATENEWACCOUNT,
//     RESPONSE_LOG,
//     REQUEST_CREATENEWCHARACTER,
//     REQUEST_ENTERGAME,
//     RESPONSE_ENTERGAME,
//     REQUEST_DELETECHARACTER,
    REQUEST_CREATENEWGUILD = 32,
    RESPONSE_CREATENEWGUILD,
    REQUEST_DISBANDGUILD,
    RESPONSE_DISBANDGUILD,

    REQUEST_UPDATEGUILDINFO_NEWGUILDSMAN,
    REQUEST_UPDATEGUILDINFO_DELGUILDSMAN,

    REQUEST_CIVILRIGHT,
    RESPONSE_CIVILRIGHT,

    REQUEST_CHANGEPASSWORD,
    RESPONSE_CHANGEPASSWORD,

    REQUEST_INPUTKEYCODE,
    RESPONSE_INPUTKEYCODE,

    REQUEST_ANGEL,
    REQUEST_DKITEM,

    REQUEST_UPDATE_FRIENDS,

    REQUEST_LGNPTS,
    RESPONSE_LGNPTS,
    REQUEST_LGNSVC,
    RESPONSE_LGNSVC,
    SUBCASH,
    ADDCASH,

    REQUEST_SETRECALLPNT,

    CONFIRMBUMP,

    REQUEST_PLAYERDATA,
    RESPONSE_PLAYERDATA,
    RESPONSE_SAVEPLAYERDATA_REPLY,
    REQUEST_SAVEPLAYERDATA,
    REQUEST_SAVEPLAYERDATA_REPLY,
    REQUEST_SAVEPLAYERDATALOGOUT,
    REQUEST_NOSAVELOGOUT,

    REQUEST_RETRIEVEITEM,
    RESPONSE_RETRIEVEITEM,

    REQUEST_RETRIEVEITEM_GUILDBANK,
    RESPONSE_RETRIEVEITEM_GUILDBANK,

    REQUEST_FULLOBJECTDATA,

    GUILDNOTIFY,
    REQUEST_TELEPORT,

    LEVELUPSETTINGS,
    DYNAMICOBJECT,

    GAMESERVERALIVE,

    GAMESERVERDOWN,
    TOTALGAMESERVERCLIENTS,

    ENTERGAMECONFIRM,

    REQUEST_FIGHTZONE_RESERVE,
    RESPONSE_FIGHTZONE_RESERVE,

    CONFIRMEDLOGIN,

    GAMESERVERINITALIZED,

    ANNOUNCEACCOUNT,

    ACCOUNTINFOCHANGE,
    IPINFOCHANGE,

    GAMESERVERSHUTDOWNED,
    ANNOUNCEACCOUNTNEWPASSWORD,

    REQUEST_IPIDSTATUS,
    RESPONSE_IPIDSTATUS,
    REQUEST_ACCOUNTCONNSTATUS,
    RESPONSE_ACCOUNTCONNSTATUS,
    REQUEST_CLEARACCOUNTCONNSTATUS,
    RESPONSE_CLEARACCOUNTCONNSTATUS,

    REQUEST_FORCEDISCONECTACCOUNT,
    REQUEST_NOCOUNTINGSAVELOGOUT,

    OCCUPYFLAGDATA,
    REQUEST_SAVEARESDENOCCUPYFLAGDATA,
    REQUEST_SAVEELVINEOCCUPYFLAGDATA,

    ARESDENOCCUPYFLAGSAVEFILECONTENTS,
    ELVINEOCCUPYFLAGSAVEFILECONTENTS,

    REQUEST_SETITEMPOS,

    SENDSERVERSHUTDOWNMSG,
    ITEMLOG,

    REQUEST_NOTICEMENT,
    RESPONSE_NOTICEMENT,

    REGISTER_WORLDSERVER,
    REGISTER_WORLDSERVERSOCKET,
    REGISTER_WORLDSERVER_GAMESERVER,

    REQUEST_CHARINFOLIST,
    RESPONSE_CHARINFOLIST,

    REQUEST_REMOVEGAMESERVER,
    REQUEST_CLEARACCOUNTSTATUS,

    REQUEST_SETACCOUNTINITSTATUS,
    REQUEST_SETACCOUNTWAITSTATUS,

    REQUEST_CHECKACCOUNTPASSWORD,
    WORLDSERVERACTIVATED,

    REQUEST_PANNING,
    RESPONSE_PANNING,

    REQUEST_RESTART,
    RESPONSE_REGISTER_WORLDSERVERSOCKET,

    REQUEST_BLOCKACCOUNT,
    IPTIMECHANGE,
    ACCOUNTTIMECHANGE,
    REQUEST_IPTIME,
    RESPONSE_IPTIME,

    REQUEST_SELLITEMLIST,

    REQUEST_ALL_SERVER_REBOOT,
    REQUEST_EXEC_1DOTBAT,
    REQUEST_EXEC_2DOTBAT,
    MONITORALIVE,

    COLLECTEDMANA,
    METEORSTRIKE,

    SERVERSTOCKMSG,
    GATEWAY_CURRENTCONN,
    GETMINIMUMLOADGATEWAY,

    CONFIRMFORCELOGOUT,

    REQUEST_TELEPORT_LIST,
    RESPONSE_TELEPORT_LIST,
    REQUEST_CHARGED_TELEPORT,
    RESPONSE_CHARGED_TELEPORT,

    //====HG CONFIG==============================================================================
    REQUEST_REGISTERGAMESERVERSOCKET,

    GAMEMASTERLOG,
    GAMEITEMLOG,
    GAMECRUSADELOG,
    PLAYERLOG,

    MANAGER_INIT,
    MANAGER_SHUTDOWN,

    TELEPORTLISTCONTENTS,
    UPDATECONFIGFILES,

    RESPONSE_CHECKCONNECTION,

    CRAFTINGCONFIGURATIONCONTENTS,

    REQUEST_HELDENIAN_WINNER,
    REQUEST_HELDENIAN_SCROLL,

    // Ressurection
    REQUEST_RESURRECTPLAYER_YES,
    REQUEST_RESURRECTPLAYER_NO,

    // MJ Stats Change
    STATECHANGEPOINT,
    GUILDINFO,
    REQUEST_GUILDBANK,
    RESPONSE_GUILDBANK,
    GUILDSMANSTATUS,
    REQGUILDSUMMONS,
    RSPGUILDSUMMONS,
    GUILDCONTRIBUTE,
    GUILDUPGRADE,
    GUILDSMANCHANGE,
    REQ_GUILDBOARD,
    RSP_GUILDBOARD,
    REQ_GUILDPOSTDATA,
    RSP_GUILDBOARDPOST,
    REQ_DELETEGUILDPOST,
    REQ_POSTGUILDBOARD,
    RSP_POSTGUILDBOARD,

    REQ_MAILBOX,
    RSP_MAILBOX,
    REQ_SENDMAIL,
    RSP_SENDMAIL,
    REQ_MAILDATA,
    RSP_MAILDATA,
    REQ_DELETEMAIL,
    REQ_RETRIEVEMAILITEM,
    RSP_RETRIEVEMAILITEM,

    REQUEST_LOAD_GUILDINFO,
    RESPONSE_LOAD_GUILDINFO,
    SAVE_GUILDINFO,
    REQUEST_SAVE_GUILDBANK,

    REQUEST_QUEST_LIST,
    RESPONSE_QUEST_LIST,
    REQUEST_ACCEPT_QUEST,

    PINGMAP,

    REQUEST_TITLES,
    SETTITLE,

    REQUEST_CRITICALS,

    SETITEMTO_AUCTIONHOUSE_WORLD,
    SETITEMTO_AUCTIONHOUSE_WORLD2,
    AUCTIONHOUSE_SENDITEMLIST,

    MODIFYTILE,
    MAX,
};

enum class common_type_id
{
    ITEMDROP = 0x0a01,
    EQUIPITEM,
    REQ_LISTCONTENTS,
    REQ_PURCHASEITEM,
    GIVEITEMTOCHAR,
    JOINGUILDAPPROVE,
    JOINGUILDREJECT,
    DISMISSGUILDAPPROVE,
    DISMISSGUILDREJECT,
    RELEASEITEM,
    TOGGLECOMBATMODE,
    SETITEM,
    MAGIC,
    REQ_STUDYMAGIC,
    REQ_TRAINSKILL,
    REQ_GETREWARDMONEY,
    REQ_USEITEM,
    REQ_USESKILL,
    REQ_SELLITEM,
    REQ_REPAIRITEM,
    REQ_SELLITEMCONFIRM,
    REQ_REPAIRITEMCONFIRM,
    REQ_GETFISHTHISTIME,
    TOGGLESAFEATTACKMODE,
    REQ_CREATEPOTION,
    TALKTONPC,
    REQ_SETDOWNSKILLINDEX,
    REQ_GETOCCUPYFLAG,
    REQ_GETHEROMANTLE,
    EXCHANGEITEMTOCHAR,
    SETEXCHANGEITEM,
    CONFIRMEXCHANGEITEM,
    CANCELEXCHANGEITEM,
    QUESTACCEPTED,
    BUILDITEM,
    CRAFTITEM,
    GETMAGICABILITY,
    CLEARGUILDNAME,

    REQ_GETOCCUPYFIGHTZONETICKET,
    BANGUILD,

    REQUEST_ACCEPTJOINPARTY,
    REQUEST_JOINPARTY,
    RESPONSE_JOINPARTY,
    REQUEST_ACTIVATESPECABLTY,
    REQUEST_CANCELQUEST,
    REQUEST_SELECTCRUSADEDUTY,
    REQUEST_MAPSTATUS,
    REQUEST_HUNTMODE,

    SETGUILDTELEPORTLOC,
    GUILDTELEPORT,
    SUMMONWARUNIT,
    SETGUILDCONSTRUCTLOC,
    UPGRADEITEM,
    REQGUILDNAME,
    REQ_REPAIRALL,
    REQ_REPAIRALLCONFIRM,

    REQ_CREATESLATE,
    REQTITLE,
    REQARENASTATUS,
    REQ_GETTRADEEKITEM,
    REQ_GETTRADEBALLITEM,
    HACK,
    DLLHACK,
    MAX,
};

enum class msg_type
{
    CONFIRM = 0x0F14,
    REJECT = 0x0F15
};

enum class log_res_msg
{
    CONFIRM = 0x0f14,
    REJECT = 0x0f15,
    PASSWORDMISMATCH = 0x0f16,
    NOTEXISTINGACCOUNT = 0x0f17,
    NEWACCOUNTCREATED = 0x0f18,
    NEWACCOUNTFAILED = 0x0f19,
    ALREADYEXISTINGACCOUNT = 0x0f1a,
    NOTEXISTINGCHARACTER = 0x0f1b,
    NEWCHARACTERCREATED = 0x0f1c,
    NEWCHARACTERFAILED = 0x0f1d,
    ALREADYEXISTINGCHARACTER = 0x0f1e,
    CHARACTERDELETED = 0x0f1f,
    NOTENOUGHPOINT = 0x0f30,
    ACCOUNTLOCKED = 0x0f31,
    SERVICENOTAVAILABLE = 0x0f32,
    PASSWORDCHANGESUCCESS = 0x0a00,
    PASSWORDCHANGEFAIL = 0x0a01,
    NOTEXISTINGWORLDSERVER = 0x0a02,
    INPUTKEYCODE = 0x0a03,
    REALACCOUNT = 0x0a04,
    FORCECHANGEPASSWORD = 0x0a05,
    INVALIDKOREANSSN = 0x0a06,
    LESSTHENFIFTEEN = 0x0a07,
};

enum class enter_game_msg
{
    NEW = 0x0f1c,
    NOENTER_FORCEDISCONN = 0x0f1d,
    CHANGINGSERVER = 0x0f1e,
    PLAYING = 0x0f20,
    REJECT = 0x0f21,
    CONFIRM = 0x0f22,
    FORCEDISCONN = 0x0f23,
};

enum class reject_type
{
    CHARABOVETRIALLEVEL = 0x01,
    MAXREGISTEREDIPREACHED = 0x02,
    GAMESERVERNOTONLINE = 0x03,
    DATADIFFERENCE = 0x04,
    GAMESERVEROFFLINE = 0x05,
    MAXSERVERUSERLIMITREACHED = 0x06,
    WORLDSERVERISFULL = 0x07,
    LOGINERROR = 0x08,
};

enum class mail_type
{
    guild,
    player,
    system
};


}