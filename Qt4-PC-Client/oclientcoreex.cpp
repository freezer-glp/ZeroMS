#include <QByteArray>
#include <QMessageBox>
#include "const.h"
#include "oclientcoreex.h"
#include "login.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

//public:
OClientCoreEx::OClientCoreEx():login(0),mainwidget(0)
{

}

OClientCoreEx::~OClientCoreEx()
{

}

void OClientCoreEx::init()
{
    OClientCore::init();
    connect(this,SIGNAL(onSMsg(QString,QString,QString)),this,SLOT(onMsg(QString,QString,QString)));
    showLogin();
}

void OClientCoreEx::showLogin()
{
    //调用该函数之前应该检查登陆窗口是否已经显示
    DELETE(login);
    login=new Login;
    connect(this,SIGNAL(onLoginError()),login,SLOT(LoginError()));
    login->show();
}

void OClientCoreEx::showMainWidget()
{
    //调用该函数之前应该检查主窗口是否已经显示
    DELETE(login);
    DELETE(mainwidget);
    mainwidget=new MainWidget;
    connect(this,SIGNAL(onGroupMsg(QString,QString)),mainwidget,SLOT(onMsg(QString,QString)));
    connect(this,SIGNAL(onUList(QStringList&)),mainwidget,SLOT(onUList(QStringList&)));
    mainwidget->show();
}

void OClientCoreEx::showChatWidget(QString uname)
{
    if(widgets.contains(uname))
    {
        widgets[uname]->activateWindow();
    }
    else
    {
        ChatWidget *cp=new ChatWidget(uname);
        widgets.insert(uname,cp);
        cp->show();
    }
}

void OClientCoreEx::msgLoginOk(QByteArray *data,unsigned int time)
{
    showMainWidget();
    msgAskUList();
}

void OClientCoreEx::Error(OClientCore::ErrorType e,QString msg,QAbstractSocket::SocketError s)
{
    OClientCore::Error(e,msg,s);
    QString msgStr;
    switch(e)
    {
        case Unknown:
        case NoError:
            msgStr=tr("未知错误");
            break;
        case CantUnderstand:
            msgStr=tr("无法理解服务器发来的命令，可能是您的客户端版本过旧");
            break;
        case MsgError:
            msgStr=msg;
            break;
        case SocketCantConnect:
            msgStr=tr("无法连接到服务器，有可能是服务器挂掉了...");
            break;
        case SocketConnectionAbort:
            msgStr=tr("连接被服务器中断");
            break;
        case SocketOthers:
            msgStr=errorString(e);
    }
    QMessageBox::critical(0,tr("错误 %1").arg(e),msgStr);
    if(e>3 && !login)
    {
        //如果是Socket错误，而且当前不在登陆窗体
        //则销毁其他所有窗体，并显示登陆窗体
        DELETE(mainwidget);

        for(QMap<QString,ChatWidget*>::Iterator i=widgets.begin();i!=widgets.end();++i)
        {
            delete i.value();
            widgets.erase(i);
        }

        showLogin();
    }
}

void OClientCoreEx::onMsg(QString user,QString view,QString msg)
{
    if(view==MAIN_GROUP)
    {
        emit onGroupMsg(user,msg);
    }
    else
    {
        showChatWidget(user);
        widgets[user]->onMsg(msg);
    }
}