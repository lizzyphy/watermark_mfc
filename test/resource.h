//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by test.rc
//
#define IDD_TEST_DIALOG                 102
#define IDP_SOCKETS_INIT_FAILED         103
#define IDR_MAINFRAME                   128
#define IDC_EDIT_WM                     1000
#define IDC_EDIT_VIDEO                  1001
#define IDC_BUTTON_GENERATE_AUTO        1002
#define IDC_BUTTON_SCAN                 1003
#define IDC_EDIT_SCREEN                 1004
#define IDC_BUTTON_SAVE                 1005
#define IDC_EDIT_SAVE                   1006
#define IDC_PROGRESS1                   1007
#define IDC_PROGRESS                    1007

//嵌入水印部分
#define WMLENGTH    96		//水印长度：bits
#define WMSTRENGTH  0.6		//水印嵌入强度
#define WMDEPTH		1.5		//水印嵌入深度

#define NWIDTH      720		//视频一帧宽度
#define NHEIGHT		576		//视频一帧高度

#define MWIDTH		528		//水印嵌入区域宽度
#define MHEIGHT		320		//水印嵌入区域高度
#define MWSTART		(NWIDTH - MWIDTH) / 2		//水印嵌入区域起点
#define MHSTART		(NHEIGHT - MHEIGHT) / 2

#define UDoff		0			//提取时的上下偏移，上负下正
#define LRoff		0			//提取时的左右偏移，左负右正
#define Nbeg		0			//嵌入或提取水印起始帧号，默认为0
#define Nend		1499			//嵌入或提取水印终止帧号，默认为FRAME-1
#define FRAMES		(Nend - Nbeg + 1)		//处理的视频总帧数

#define DELFILE				//是否删除上次嵌入水印操作保存的视频


// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        129
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1008
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
