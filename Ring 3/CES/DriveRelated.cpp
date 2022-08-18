#include"Ceydbg.h"

DWORD64 Ntoskrnl_Base_bf = { 0 };//�ں˻�ַ����Ҫͨ��KdDebuggerEnabled��ַ���
WCHAR Drive_Name[50] = L"CEYDBG";
WCHAR Drive_File_Name[50] = L"Ceydbg_drive.sys";
int Answer_Num = 0;

// ��װ����
BOOL Install_Drive() {
    WCHAR Drive_Path[MAX_PATH] = { 0 };
    GetCurrentDirectoryW(MAX_PATH, Drive_Path);
    lstrcatW(Drive_Path, L"\\");
    lstrcatW(Drive_Path, Drive_File_Name);
    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManagerW(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        DWORD64 e = GetLastError();
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ����������������������ƹ��������ݿ�
    SC_HANDLE schService = CreateServiceW(
        schSCManager,               // ����ؼ����������ݿ�ľ��
        Drive_Name,                // Ҫ��װ�ķ��������
        Drive_Name,                // �û��������������ʶ�������ʾ����
        SERVICE_ALL_ACCESS,         // �Է���ķ���Ȩ�ޣ�����ȫȨ��
        SERVICE_KERNEL_DRIVER,      // �������ͣ���������
        SERVICE_DEMAND_START,       // ��������ѡ����̵��� StartService ʱ����
        SERVICE_ERROR_IGNORE,       // ����޷����������Դ����������
        Drive_Path,                    // �����ļ�����·������������ո���Ҫ���˫����
        NULL,                       // ���������ĸ��ض����飺��������ĳ����
        NULL,                       // ���ն�����Ψһ���ֵ��������
        NULL,                       // �������˳�����飺����û��������
        NULL,                       // ���з�����˻�����ʹ�� LocalSystem �˻�
        NULL                        // LocalSystem �˻�����
    );
    if (schService == NULL) {
        DWORD64 E = GetLastError();
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        if (E == 1073)
            return TRUE;
        return FALSE;
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

//����
BOOL Start_Drive() {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        Drive_Name,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    if (StartService(hs, 0, 0) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }


    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ֹͣ
BOOL Stop_Drive() {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        Drive_Name,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ���������������
    SERVICE_STATUS status;
    if (QueryServiceStatus(hs, &status) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }
    if (status.dwCurrentState != SERVICE_STOPPED &&
        status.dwCurrentState != SERVICE_STOP_PENDING
        ) {
        // ���͹رշ�������
        if (ControlService(
            hs,                         // ������
            SERVICE_CONTROL_STOP,       // �����룺֪ͨ����Ӧ��ֹͣ
            &status                     // �������µķ���״̬��Ϣ
        ) == 0) {
            CloseServiceHandle(hs);
            CloseServiceHandle(schSCManager);
            return FALSE;
        }

        // �жϳ�ʱ
        INT timeOut = 0;
        while (status.dwCurrentState != SERVICE_STOPPED) {
            timeOut++;
            QueryServiceStatus(hs, &status);
            Sleep(50);
        }
        if (timeOut > 80) {
            CloseServiceHandle(hs);
            CloseServiceHandle(schSCManager);
            return FALSE;
        }
    }

    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ж������
BOOL Unload_Drive() {

    // �򿪷�����ƹ��������ݿ�
    SC_HANDLE schSCManager = OpenSCManager(
        NULL,                   // Ŀ������������,NULL�����ӱ��ؼ�����ϵķ�����ƹ�����
        NULL,                   // ������ƹ��������ݿ�����ƣ�NULL���� SERVICES_ACTIVE_DATABASE ���ݿ�
        SC_MANAGER_ALL_ACCESS   // ����Ȩ��
    );
    if (schSCManager == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // �򿪷���
    SC_HANDLE hs = OpenService(
        schSCManager,           // ����ؼ����������ݿ�ľ��
        Drive_Name,            // Ҫ�򿪵ķ�����
        SERVICE_ALL_ACCESS      // �������Ȩ�ޣ�����Ȩ��
    );
    if (hs == NULL) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    // ɾ������
    if (DeleteService(hs) == 0) {
        CloseServiceHandle(hs);
        CloseServiceHandle(schSCManager);
        return FALSE;
    }

    CloseServiceHandle(hs);
    CloseServiceHandle(schSCManager);
    return TRUE;
}

// ��������
void Load_Drive() {
    DWORD lent = 0;
    Pdbfile.My_Drive = CreateFile(DEVICE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
    if (Pdbfile.My_Drive == INVALID_HANDLE_VALUE)
    {
        Error_Out(1);
    }
    int test = 1000;
    if (!DeviceIoControl(Pdbfile.My_Drive, GET_BASE, &Ntoskrnl_Base_bf, sizeof(DWORD64), &test, 4, &lent, NULL))
        Error_Out(1);
    CheckPdb_X("KdDebuggerEnabled");
    if (Answer_Num == 0)
        Error_Out(1);
}