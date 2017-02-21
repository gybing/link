//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("formlink.cpp", MainForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 HANDLE hMutex=CreateMutex(NULL,true,"_run_flag");//���������ź���
                 if(GetLastError()==ERROR_ALREADY_EXISTS)     //�˻������Ѿ�����
                 {
                   ShowMessage("CchLink �Ѿ���������!");
                   ReleaseMutex(hMutex);    //�ͷŻ�����
                    return 0;     //�˳�
                 }
                 Application->Initialize();
                 Application->Title = "CchLink 0.01��";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
