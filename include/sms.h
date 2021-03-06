#pragma once
#include "util.h"


struct Req
{
    std::string senduser;   //发送者邮箱
    std::string recvuser;   //接收者邮箱
    std::string sqm;        //qq邮箱授权码
    std::string Data;       //数据内容

    //全非空才返回true
    bool checking()
    {
        return !senduser.empty()&&!recvuser.empty()&&!sqm.empty()&&!Data.empty();
    }
    std::string GetStr()
    {
        return ("senduser: "+senduser+"\trecvuser: "+recvuser+"\tsqm: "+sqm+"\tData: "+Data);
    }
};

/**
 * @brief sms客户端
 *  实现json解析、保存所有json数据，转换为get url资源，发送到腾讯邮箱域
 */
class SMSClient
{
public:
    typedef std::queue<Req> MessageQueue;

    SMSClient(boost::asio::io_context&);
    ~SMSClient();
    
    //接收json数据
    std::string pushAJson(std::string& json);
    
private:
    bool json_handle(std::string& GetRes,Req&);   //解析json
    void close();

private:
    std::string base64(std::string&);   //base64加密
    static void WorkThread_main(SMSClient* th);
    bool workThread_login(Req&);    //登录
    bool workThread_relogin(Req&);  //重新登录
    //void workThread_loginoff(); //退出登录
    bool workThread_sendData(Req&);
    size_t workThread_sendline(std::string&);   //发送一行
    int workThread_recvive(std::string&);      //接受一行
private:
    boost::asio::io_context& ioc_;
    MessageQueue mq_;
    boost::mutex lock_;
    boost::asio::ip::tcp::socket sockClient_;
    boost::atomic_bool Connecting_;
    boost::atomic_bool timeout_;
    static const std::string host;
    static const std::string port;
};



/*
MAIL FROM: <979336542@qq.com>
RCPT TO:<979446542@qq.com>
DATA
SUBJECT:nihao
你好
.

*/