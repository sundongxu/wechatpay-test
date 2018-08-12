#include "src/include/entity.h"
#include "src/include/comm_def.h"

using namespace std;

int main(int argc, char **argv)
{
    string cert;
    string key;
    string root;
    string server{"localhost:50051"};

    util::read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/client.crt", cert);
    util::read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/client.key", key);
    util::read("/Users/sundongxu/Code/Git/Mine/Work/wechatpay/wechatpay/wechatpay-test/crt/ca.crt", root);

    WechatPayClient client(cert, key, root, server);

    // 命令行交互
    bool stop = false;
    bool first = true;
    bool error = false;
    while (!stop)
    {
        if (first)
        {
            first = false;
            cout << "欢迎来到测试系统，请输入操作所对应的数字：" << endl;
            cout << "1-登录，2-注册，3-交互，4-登出，5-退出：" << endl;
        }
        else if (error)
        {
            error = false;
            cout << "请重新输入：" << endl;
        }
        else
        {
            cout << "请继续输入：" << endl;
        }

        string strCmd;
        cin.clear();
        cin.sync();
        if (!getline(cin, strCmd))
        {
            error = true;
            cout << "读取用户输入时出错！" << endl;
            continue;
        }
        int cmd = atoi(strCmd.c_str()); // 命令字
        if (cmd < 1 || cmd > 5)
        {
            // 目前只支持5种客户端操作，cmd=1~5
            error = true;
            cout << "操作不存在！" << endl;
            continue;
        }
        else
        {
            int ret = 0;
            switch (cmd)
            {
            case CMD_LOGIN:
            {
                cout << "-----登录操作-----" << endl;
                string username = "";
                string password = "";
                cout << "请输入用户名：" << endl;
                getline(cin, username);
                cout << "请输入密码：" << endl;
                getline(cin, password);
                ret = client.Login(username, password);
                break;
            }
            case CMD_REGISTER:
            {
                cout << "-----注册操作-----" << endl;
                string username = "";
                string password = "";
                string passwordRepeat;
                cout << "请输入新用户名：" << endl;
                getline(cin, username);
                bool first = true;
                do
                {
                    if (first)
                        first = false;
                    else
                    {
                        cout << "两次密码输入不一致，请重新输入！" << endl;
                    }
                    cout << "请输入密码：" << endl;
                    getline(cin, password);
                    cout << "请再次输入密码：" << endl;
                    getline(cin, passwordRepeat);
                } while (passwordRepeat != password);
                ret = client.Register(username, password);
                break;
            }

            case CMD_INTERACT:
            {
                cout << "-----交互操作-----" << endl;
                string msg;
                cout << "请输入发送内容：" << endl;
                getline(cin, msg);
                ret = client.Interact(msg);
                break;
            }

            case CMD_LOGOUT:
            {
                cout << "-----登出操作-----" << endl;
                ret = client.Logout();
                break;
            }

            case CMD_QUIT:
            {
                cout << "-----关闭客户端-----" << endl;
                ret = client.Logout();  // 退出前发出下线请求
                stop = true;
                break;
            }
            }
        }
    }
    return 0;
}
