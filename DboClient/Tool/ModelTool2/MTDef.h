#pragma once
#include <tchar.h>

// WINDOW NAME
#define MT_WINDOW_NAME              _T("DBO Model Tool 9.8.3 by agebreak") 

typedef std::vector<HTREEITEM> TREEITEM_VECTOR;
typedef std::vector<WCHAR*> VWChar;
 
// File Open Filter
#define FILTER_TEXTURE              L"Texture File (*.png;*.dds)|*.png;*.dds||"
#define FILTER_CLUMP                L"Clump File (*.dff)|*.dff||"
#define FILTER_ANIM                 L"Animation File (*.anm)|*.anm||"
#define FILTER_UVANIM               L"UV Anim File (*.uva)|*.uva||"
#define FILTER_SOUND                L"Sound File(*.wav;*.ogg;*.mp3)|*.wav;*.ogg;*.mp3||"

// 애니메이션 폴더 정의
#define PATH_ANIM_HUMAN_FEMALE      L"\\character\\animation_humanfemale\\"
#define PATH_ANIM_HUMAN_MALE        L"\\character\\animation_humanmale\\"
#define PATH_ANIM_MAJIN_MALE        L"\\character\\animation_majinmale\\"        
#define PATH_ANIM_NAMEK             L"\\character\\animation_namek\\"
#define PATH_ANIM_MOB               L"\\character\\animation_mob\\"
#define PATH_ANIM_NPC               L"\\character\\animation_npc\\"

// Path
#define MT_PATH_CHARACTER			L"\\devdata\\Character\\"
#define MT_PATH_OBJECT				L"\\devdata\\Object\\"
#define MT_PATH_ITEM                L"\\devdata\\Item\\"
#define MT_PATH_TOOL                L"\\Tool\\ModelTool\\"
#define PROPERTY_LIST_VER           L"1.0"
#define PROPERTY_LIST_CHAR_PATH     L"./devdata/Character/"
#define PROPERTY_LIST_OBJECT_PATH   L"./devdata/Object/"
#define PROPERTY_LIST_ITEM          L"./devdata/Item/"
#define MT_PATH_NAVI_MESH           L"\\Tool\\ToolShareData\\NaviModel\\"

// File
#define MT_TREE_ITEM                L"TreeItem.XML" 
#define MT_TREE_VEHICLE             L"TreeVehicle.xml"
#define MT_TREE_PC                  L"TreePC.xml"

#define FRAME_FIX_FPS 30.0f             ///< Frame Fix 수치

/// 한 애니메이션의 최대 HitEvent 개수
#define MAX_HIT_EVENT_COUNT         16

/// 안전하게 싱글톤 객체를 가져오는 매크로 함수
#define GetSafeInstance(class) if(class::GetInstance()) class::GetInstance()

// Face camera Size
#define FACE_CAMERA_SIZE            128

enum EModelToolMode
{
    MT_MODE_CLUMP,
    MT_MODE_PC,
    MT_MODE_MOB,
    MT_MODE_ITEM,
    MT_MODE_OBJECT,
    MT_MODE_NPC,
    MT_MODE_VEHICLE,
    MT_MODE_NONE,
};

// 상대경로/절대 경로에서 파일이름만 추출하는 함수
static char* GetFileNameFromPath(const char* szFilePath)
{
    if(!szFilePath)
        return "";

    char* index = strstr((char*)szFilePath, "\\");
    char* ret = NULL;
    while(index)
    {
        ret = index + 1;
        index = strstr(ret, "\\");
    }

    return ret;
}

static CString GetFileNameFromPath(CString strFilePath)
{
    int nIndex = strFilePath.ReverseFind(L'\\');
    if(nIndex)
    {
        int nLength = strFilePath.GetLength() - nIndex - 1;
        strFilePath = strFilePath.Right(nLength);
    }

    return strFilePath;
}

// 사운드 파일의 하위 경로명만 추출하는 함수
static CString GetSoundFilePath(CString strFilePath)
{
    strFilePath.MakeUpper();
    int nIndex = strFilePath.Find(L"SOUND\\");
    if(nIndex)
    {
        int nLength = strFilePath.GetLength() - nIndex - 6;
        strFilePath = strFilePath.Right(nLength);
    }

    return strFilePath;    
}

// 절대경로로부터 상대경로를 받아온다.
static CString GetLinkPathFromPath(CString strFilePath, CString strAbsPath)
{
    strFilePath.MakeUpper();
    strAbsPath.MakeUpper();
    strFilePath.Replace(strAbsPath, L"");

    return strFilePath;
}

static CString FindFileFromPath(CString strFileName, CString strPath)
{
    strFileName.MakeUpper();

    CFileFind finder;
    CString strWildcard(strPath);   
    strWildcard += _T("\\*.*");   // start working for files       

    BOOL bWorking = finder.FindFile(strWildcard);   

    while (bWorking)   
    {    
        bWorking = finder.FindNextFile();      

        // skip . and .. files; otherwise, we'd      
        // recur infinitely!      
        if (finder.IsDots())         
            continue;      

        // if it's a directory, recursively search it      
        if (finder.IsDirectory())      
        {        
            CString str = finder.GetFilePath(); 
            CString strPathName = finder.GetFileName();            

            CString strRet = FindFileFromPath(strFileName, str);            
            if(strRet != "")
            {
                return strRet;
            }
        }  
        else
        {
            CString strSearchName = finder.GetFileName();
            strSearchName.MakeUpper();
            if(strFileName == strSearchName)
            {
                return finder.GetFilePath();
            }
        }
    }   

    finder.Close();

    return L"";
}

// 뒤의 문자열이 앞의 문자열에 포함되었는지를 반환한다. (대소문자 구별없이)
static BOOL FindNoCase(CString strDest, CString strFind)
{
    strDest.MakeUpper();
    strFind.MakeUpper();

    int nIndex = strDest.Find(strFind);

    return nIndex != -1?TRUE:FALSE;
}

// 오브젝트용 이펙트(Wrd, Obj)인지를 반환한다.
static BOOL MTLogic_IsObjectEffect(RwChar* szEffectName)
{
    USES_CONVERSION;
    CString strEffectName = A2W(szEffectName);

    return FindNoCase(strEffectName, L"Wrd") || FindNoCase(strEffectName, L"Obj");
}