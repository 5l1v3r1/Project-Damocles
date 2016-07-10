/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include <stdlib.h>
#include <time.h>
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����

#include "C:\\Python27\include\Python.h"
#pragma comment(lib,"C:\\Python27\\libs\\python27.lib")
using namespace std;

char *welcome[] = { "���ǵ���;���ǳ���󺣣�\n","������������֣�\n","���������ƥ��ʥѩ`�ƥ����ޤ��磡\n","��ڿ͹���Ҫ����\n","�������˴���\n","�o���Ҥ�����Ф館!\n","c������ֻ��c prime plus\n","//����������������������ˮ��\n","Just hack for fun\n","As we do, as you know \n" };
int lenWelcode = sizeof(welcome);

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
bool enabled = false;
PyObject *pModule;
void checkImage(int64_t fromGroup, int64_t fromQQ, const char *msg);
/* 
* ����Ӧ�õ�ApiVer��Appid������󽫲������
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}


/* 
* ����Ӧ��AuthCode����Q��ȡӦ����Ϣ��������ܸ�Ӧ�ã���������������������AuthCode��
* ��Ҫ�ڱ��������������κδ��룬���ⷢ���쳣���������ִ�г�ʼ����������Startup�¼���ִ�У�Type=1001����
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}


/*
* Type=1001 ��Q����
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q������ִ��һ�Σ���������ִ��Ӧ�ó�ʼ�����롣
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventStartup, 0)() {

	return 0;
}


/*
* Type=1002 ��Q�˳�
* ���۱�Ӧ���Ƿ����ã������������ڿ�Q�˳�ǰִ��һ�Σ���������ִ�в���رմ��롣
* ������������Ϻ󣬿�Q���ܿ�رգ��벻Ҫ��ͨ���̵߳ȷ�ʽִ���������롣
*/
CQEVENT(int32_t, __eventExit, 0)() {

	return 0;
}

/*
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;
	srand(clock());
	Py_SetPythonHome("C:\\python27");
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('C:\\module')");
	pModule = PyImport_ImportModule("CQTools");
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;
	Py_Finalize();
	return 0;
}


/*
* Type=21 ˽����Ϣ
* subType �����ͣ�11/���Ժ��� 1/��������״̬ 2/����Ⱥ 3/����������
*/
CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, int32_t font) {
	if (fromQQ == 87294982) {
		char *getImage = "[CQ:image,file=";
		char *get = strstr((char *)msg, getImage);
		if (get != NULL) {
			checkImage(0, fromQQ, msg);
		}
	}
	//���Ҫ�ظ���Ϣ������ÿ�Q�������ͣ��������� return EVENT_BLOCK - �ضϱ�����Ϣ�����ټ�������  ע�⣺Ӧ�����ȼ�����Ϊ"���"(10000)ʱ������ʹ�ñ�����ֵ
	//������ظ���Ϣ������֮���Ӧ��/�������������� return EVENT_IGNORE - ���Ա�����Ϣ
	return EVENT_IGNORE;
}


void adminCmd(int64_t fromGroup, const char * msg) {
	int64_t QQId;
	char *bp = (char *)malloc(0x1000);
	if (strncmp(msg, "ban:", 4) == 0) {
		QQId = atoll(&msg[4]);
		CQ_setGroupBan(ac, fromGroup, QQId, 600);
	}
	if (strncmp(msg, "at:", 3) == 0) {
		QQId = atoll(&msg[3]);
		sprintf(bp, "[CQ:at,qq=%lld] hello", QQId);
		CQ_sendGroupMsg(ac, fromGroup, bp);
	}
	free(bp);
}

int64_t times[200];

void requestAt(int64_t fromGroup, int64_t fromQQ, const char *msg) {
	char *bp = (char *)malloc(0x1000);
	for (int i = 0; i < 200; i++) {
		if (times[i * 2] == fromQQ) {
			times[i * 2 + 1]++;
			if (times[i * 2 + 1] == 9999) {
				times[i * 2 + 1] = 0;
				CQ_setGroupBan(ac, fromGroup, fromQQ, 600);
				sprintf(bp, "[CQ:at,qq=%lld] �������������ɶ��", fromQQ);
				CQ_sendGroupMsg(ac, fromGroup, bp);
				return;
			}
			break;
		}
		else if (times[i * 2] == 0) {
			times[i * 2] = fromQQ;
			break;
		}
	}
	//sprintf(bp, "[CQ:at,qq=%lld] ���Ҹ�ʲô", fromQQ);
	//CQ_sendGroupMsg(ac, fromGroup, bp);
	free(bp);
}

void checkImage(int64_t fromGroup, int64_t fromQQ,const char *msg) {
	PyObject *pFunc,*pArg,*pRet;
	int time;
	char *bp = (char *)malloc(0x1000);
	pFunc = PyObject_GetAttrString(pModule, "aliCheck");
	pArg = Py_BuildValue("(s)", msg);
	pRet = PyObject_CallObject(pFunc, pArg);
	time = PyInt_AsLong(pRet);
	if (time == -1) {
		CQ_addLog(ac, 1, "test", "error");
	}
	else if (time != 0) {
		CQ_setGroupBan(ac, fromGroup, fromQQ, time);
		sprintf(bp, "[CQ:at,qq=%lld] ��˵�˶��ٴ��ˣ�����������Ⱥ��", fromQQ);
		CQ_sendGroupMsg(ac, fromGroup, bp);
	}
	free(bp);
}

/*
* Type=2 Ⱥ��Ϣ
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	if (fromGroup == 555091662 && fromQQ == 87294982 && *msg == '$') {
		adminCmd(fromGroup,&msg[1]);
	}
	if (fromGroup == 555091662) {
		char *atMe = "[CQ:at,qq=858325880]";
		char *at = strstr((char *)msg, atMe);
		if (at != NULL) {
			requestAt(fromGroup, fromQQ, msg);
		}
	}
	if (fromGroup == 555091662) {
		char *getImage = "[CQ:image,file=";
		char *get = strstr((char *)msg, getImage);
		if (get != NULL) {
			checkImage(fromGroup, fromQQ, msg);
		}
	}
	
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=4 ��������Ϣ
*/
CQEVENT(int32_t, __eventDiscussMsg, 32)(int32_t subType, int32_t sendTime, int64_t fromDiscuss, int64_t fromQQ, const char *msg, int32_t font) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=101 Ⱥ�¼�-����Ա�䶯
* subType �����ͣ�1/��ȡ������Ա 2/�����ù���Ա
*/
CQEVENT(int32_t, __eventSystem_GroupAdmin, 24)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=102 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/ȺԱ�뿪 2/ȺԱ���� 3/�Լ�(����¼��)����
* fromQQ ������QQ(��subTypeΪ2��3ʱ����)
* beingOperateQQ ������QQ
*/
CQEVENT(int32_t, __eventSystem_GroupMemberDecrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=103 Ⱥ�¼�-Ⱥ��Ա����
* subType �����ͣ�1/����Ա��ͬ�� 2/����Ա����
* fromQQ ������QQ(������ԱQQ)
* beingOperateQQ ������QQ(����Ⱥ��QQ)
*/
CQEVENT(int32_t, __eventSystem_GroupMemberIncrease, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, int64_t beingOperateQQ) {
	char *bp = (char *)malloc(0x1000);
	int index = rand() % lenWelcode;
	sprintf(bp, "[CQ:at,qq=%lld] ��ӭ������Ϣ��ȫЭ��2016������Ⱥ\n�����Ķ��������\n1��Э��ctfƽ̨: ��ûдOrz wiki��http://t.cn/R5BI2h5 ��drops��http://t.cn/R5BILcO \n2��Э�������Ʋ���http://t.cn/R5BIyba \n3�������κ����ʣ�����Ⱥ�ﰬ�ع���Ա���� \n PS:%s", beingOperateQQ, welcome[index]);
	CQ_sendGroupMsg(ac, fromGroup, bp);
	free(bp);
	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=201 �����¼�-���������
*/
CQEVENT(int32_t, __eventFriend_Add, 16)(int32_t subType, int32_t sendTime, int64_t fromQQ) {

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}


/*
* Type=302 ����-Ⱥ���
* subType �����ͣ�1/����������Ⱥ 2/�Լ�(����¼��)������Ⱥ
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddGroup, 32)(int32_t subType, int32_t sendTime, int64_t fromGroup, int64_t fromQQ, const char *msg, const char *responseFlag) {

	//if (subType == 1) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPADD, REQUEST_ALLOW, "");
	//} else if (subType == 2) {
	//	CQ_setGroupAddRequestV2(ac, responseFlag, REQUEST_GROUPINVITE, REQUEST_ALLOW, "");
	//}

	return EVENT_IGNORE; //���ڷ���ֵ˵��, ����_eventPrivateMsg������
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuA, 0)() {
	MessageBoxA(NULL, "����menuA�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}

CQEVENT(int32_t, __menuB, 0)() {
	MessageBoxA(NULL, "����menuB�����������봰�ڣ����߽�������������", "" ,0);
	return 0;
}
